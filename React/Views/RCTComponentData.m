/**
 * Copyright (c) 2015-present, Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */

#import "RCTComponentData.h"

#import <objc/message.h>

#import "RCTBridge.h"
#import "RCTConvert.h"
#import "RCTShadowView.h"
#import "RCTUtils.h"
#import "UIView+React.h"

typedef void (^RCTPropBlock)(id<RCTComponent> view, id json);

@interface RCTComponentProp : NSObject

@property (nonatomic, copy, readonly) NSString *type;
@property (nonatomic, copy) RCTPropBlock propBlock;

@end

@implementation RCTComponentProp

- (instancetype)initWithType:(NSString *)type
{
  if ((self = [super init])) {
    _type = [type copy];
  }
  return self;
}

@end

@implementation RCTComponentData
{
  //这个用于导出自定义属性，给出默认view参数
  id<RCTComponent> _defaultView; // Only needed for RCT_CUSTOM_VIEW_PROPERTY
  
  //
  NSMutableDictionary<NSString *, RCTPropBlock> *_viewPropBlocks;
  NSMutableDictionary<NSString *, RCTPropBlock> *_shadowPropBlocks;
  BOOL _implementsUIBlockToAmendWithShadowViewRegistry;
  __weak RCTBridge *_bridge;
}

@synthesize manager = _manager;

- (instancetype)initWithManagerClass:(Class)managerClass
                              bridge:(RCTBridge *)bridge
{
  if ((self = [super init])) {
    _bridge = bridge;
    _managerClass = managerClass;
    
    _viewPropBlocks = [NSMutableDictionary new];
    _shadowPropBlocks = [NSMutableDictionary new];

    _name = RCTBridgeModuleNameForClass(_managerClass);
    RCTAssert(_name.length, @"Invalid moduleName '%@'", _name);
    if ([_name hasSuffix:@"Manager"]) {
      _name = [_name substringToIndex:_name.length - @"Manager".length];
    }

    _implementsUIBlockToAmendWithShadowViewRegistry = NO;
    Class cls = _managerClass;
    while (cls != [RCTViewManager class]) {
      
      //非RCTViewManager类，可能有一个uiBlockToAmendWithShadowViewRegistry方法，需提前调用
      _implementsUIBlockToAmendWithShadowViewRegistry = _implementsUIBlockToAmendWithShadowViewRegistry ||
      RCTClassOverridesInstanceMethod(cls, @selector(uiBlockToAmendWithShadowViewRegistry:));
      cls = [cls superclass];
    }
  }
  return self;
}

//组件的manager，组件不关心具体manager
- (RCTViewManager *)manager
{
  if (!_manager) {
    
    //这里不创建多个实例，而是由BatchedBridge保存公共manager实例
    _manager = [_bridge moduleForClass:_managerClass];
  }
  return _manager;
}

RCT_NOT_IMPLEMENTED(- (instancetype)init)

//通过js调用创建view，
- (UIView *)createViewWithTag:(NSNumber *)tag
{
  RCTAssertMainThread();

  //根据组件manager去创建具体view，每个组件基本都有自己的manager，manager知道创建什么样的view
  UIView *view = [self.manager view];
  view.reactTag = tag;
  view.multipleTouchEnabled = YES;
  view.userInteractionEnabled = YES; // required for touch handling
  view.layer.allowsGroupOpacity = YES; // required for touch handling
  return view;
}

//创建shadowView，也是由manager创建
- (RCTShadowView *)createShadowViewWithTag:(NSNumber *)tag
{
  RCTShadowView *shadowView = [self.manager shadowView];
  shadowView.reactTag = tag;
  shadowView.viewName = _name;
  return shadowView;
}


//设置属性值：js调用过来的，该函数返回block，外层直接执行
- (RCTPropBlock)propBlockForKey:(NSString *)name
                   inDictionary:(NSMutableDictionary<NSString *, RCTPropBlock> *)propBlocks
{
  //propBlocks可能是_shadowPropBlocks 也可能是 _viewPropBlocks
  BOOL shadowView = (propBlocks == _shadowPropBlocks);
  
  //
  RCTPropBlock propBlock = propBlocks[name];
  if (!propBlock) {

    __weak RCTComponentData *weakSelf = self;

    // Get type
    SEL type = NULL;
    NSString *keyPath = nil;
    
    
    //判断是否有此方法：propConfigShadow_##name 或 propConfig_##name
    SEL selector = NSSelectorFromString([NSString stringWithFormat:@"propConfig%@_%@", shadowView ? @"Shadow" : @"", name]);
    if ([_managerClass respondsToSelector:selector]) {
      
      //调用类方法如：propConfigShadow_flex（属性方法），返回类型：属性类型
      NSArray<NSString *> *typeAndKeyPath =
        ((NSArray<NSString *> *(*)(id, SEL))objc_msgSend)(_managerClass, selector);
      
      //0是返回值类型，1是第一个参数，依次类推
      //这里获取返回值类型就是属性类型，需要将json转换
      type = RCTConvertSelectorForType(typeAndKeyPath[0]);
      
      //处理keyPath
      keyPath = typeAndKeyPath.count > 1 ? typeAndKeyPath[1] : nil;
      
    } else {
      
      //空block（因为找不到设置属性的方法），这个更新的是_shadowPropBlocks或_viewPropBlocks
      //返回propBlock后，外层立即执行，即什么都不操作（空block）
      propBlock = ^(__unused id view, __unused id json) {};
      propBlocks[name] = propBlock;
      return propBlock;
    }

    // Check for custom setter
    //处理RCT_CUSTOM_VIEW_PROPERTY 自定义属性
    if ([keyPath isEqualToString:@"__custom__"]) {

      // Get custom setter
      SEL customSetter = NSSelectorFromString([NSString stringWithFormat:@"set_%@:for%@View:withDefaultView:", name, shadowView ? @"Shadow" : @""]);

      propBlock = ^(id<RCTComponent> view, id json) {
        RCTComponentData *strongSelf = weakSelf;
        if (!strongSelf) {
          return;
        }
        json = RCTNilIfNull(json);
        if (!json && !strongSelf->_defaultView) {
          // Only create default view if json is null
          strongSelf->_defaultView = [strongSelf createViewWithTag:nil];
        }
        ((void (*)(id, SEL, id, id, id))objc_msgSend)(
          strongSelf.manager, customSetter, json, view, strongSelf->_defaultView
        );
      };

    } else {

      // Disect keypath
      NSString *key = name;
      NSArray<NSString *> *parts = [keyPath componentsSeparatedByString:@"."];
      if (parts) {
        key = parts.lastObject;
        parts = [parts subarrayWithRange:(NSRange){0, parts.count - 1}];
      }

      //获取该属性的setter、getter方法
      // Get property getter
      SEL getter = NSSelectorFromString(key);

      // Get property setter
      SEL setter = NSSelectorFromString([NSString stringWithFormat:@"set%@%@:",
                                         [key substringToIndex:1].uppercaseString,
                                         [key substringFromIndex:1]]);

      // Build setter block
      //如果设置view的属性是block，和普通属性处理不同
      //这里的block主要针对view中的各种事件函数，如用户滑动view。manager中代理所有同类view事件，捕捉到事件后，看js时候设置了js端的事件函数
      //如果js设置了onXXX事件函数，则在设置时，并非直接将js回调和native block对接，而是给block设置native方法，方法给js发送事件通知
      void (^setterBlock)(id target, id json) = nil;
      if (type == NSSelectorFromString(@"RCTBubblingEventBlock:") ||
          type == NSSelectorFromString(@"RCTDirectEventBlock:")) {

        // Special case for event handlers
        __weak RCTViewManager *weakManager = self.manager;
        
        
        setterBlock = ^(id target, id json) {
          
          //给view添加属性，调用setter方法，设置block
          __weak id<RCTComponent> weakTarget = target;
          ((void (*)(id, SEL, id))objc_msgSend)(target, setter, [RCTConvert BOOL:json] ? ^(NSDictionary *body) {
            
            //这个blcok主要是添加一个目标view标识 发送给js
            body = [NSMutableDictionary dictionaryWithDictionary:body];
            ((NSMutableDictionary *)body)[@"target"] = weakTarget.reactTag;
            
            //发送输入事件到js
            [weakManager.bridge.eventDispatcher sendInputEventWithName:RCTNormalizeInputEventName(name) body:body];
          } : nil);
        };

      } else {

        // Ordinary property handlers
        // 调用RCTConvert的类型转换函数，这里先获取类型转换函数，稍后在RCT_CASE中调用
        // convert的作用： js传递过来的属性值是按照js写法，如颜色值使用数值或字符串，但属性是UIColor类型，需要转换
        NSMethodSignature *typeSignature = [[RCTConvert class] methodSignatureForSelector:type];
        if (!typeSignature) {
          RCTLogError(@"No +[RCTConvert %@] function found.", NSStringFromSelector(type));
          return ^(__unused id<RCTComponent> view, __unused id json){};
        }
        
        switch (typeSignature.methodReturnType[0]) {

            // 'c' char
            //其中一个set 一个get  一个convert
            //convert用于数据类型转换，如 [RCTConvert UIColor: json];
            //先调用view的get，把旧属性值保存到defaultValue中，然后调set设置新属性（target, value）
            //注意这里定义了block setterBlock，函数调用者执行该block，此处不执行
  #define RCT_CASE(_value, _type) \
          case _value: { \
            __block BOOL setDefaultValue = NO; \
            __block _type defaultValue; \
            _type (*convert)(id, SEL, id) = (typeof(convert))objc_msgSend; \
            _type (*get)(id, SEL) = (typeof(get))objc_msgSend; \
            void (*set)(id, SEL, _type) = (typeof(set))objc_msgSend; \
            setterBlock = ^(id target, id json) { \
              if (json) { \
                if (!setDefaultValue && target) { \
                  if ([target respondsToSelector:getter]) { \
                    defaultValue = get(target, getter); \
                  } \
                  setDefaultValue = YES; \
                } \
                set(target, setter, convert([RCTConvert class], type, json)); \
              } else if (setDefaultValue) { \
                set(target, setter, defaultValue); \
              } \
            }; \
            break; \
          }

            RCT_CASE(_C_SEL, SEL)
            RCT_CASE(_C_CHARPTR, const char *)
            RCT_CASE(_C_CHR, char)
            RCT_CASE(_C_UCHR, unsigned char)
            RCT_CASE(_C_SHT, short)
            RCT_CASE(_C_USHT, unsigned short)
            RCT_CASE(_C_INT, int)
            RCT_CASE(_C_UINT, unsigned int)
            RCT_CASE(_C_LNG, long)
            RCT_CASE(_C_ULNG, unsigned long)
            RCT_CASE(_C_LNG_LNG, long long)
            RCT_CASE(_C_ULNG_LNG, unsigned long long)
            RCT_CASE(_C_FLT, float)
            RCT_CASE(_C_DBL, double)
            RCT_CASE(_C_BOOL, BOOL)
            RCT_CASE(_C_PTR, void *)
            RCT_CASE(_C_ID, id)

          case _C_STRUCT_B:
          default: {

            NSInvocation *typeInvocation = [NSInvocation invocationWithMethodSignature:typeSignature];
            typeInvocation.selector = type;
            typeInvocation.target = [RCTConvert class];

            __block NSInvocation *targetInvocation = nil;
            __block NSMutableData *defaultValue = nil;

            setterBlock = ^(id target, id json) { \

              if (!target) {
                return;
              }

              // Get default value
              if (!defaultValue) {
                if (!json) {
                  // We only set the defaultValue when we first pass a non-null
                  // value, so if the first value sent for a prop is null, it's
                  // a no-op (we'd be resetting it to its default when its
                  // value is already the default).
                  return;
                }
                // Use NSMutableData to store defaultValue instead of malloc, so
                // it will be freed automatically when setterBlock is released.
                defaultValue = [[NSMutableData alloc] initWithLength:typeSignature.methodReturnLength];
                if ([target respondsToSelector:getter]) {
                  NSMethodSignature *signature = [target methodSignatureForSelector:getter];
                  NSInvocation *sourceInvocation = [NSInvocation invocationWithMethodSignature:signature];
                  sourceInvocation.selector = getter;
                  [sourceInvocation invokeWithTarget:target];
                  [sourceInvocation getReturnValue:defaultValue.mutableBytes];
                }
              }

              // Get value
              BOOL freeValueOnCompletion = NO;
              void *value = defaultValue.mutableBytes;
              if (json) {
                freeValueOnCompletion = YES;
                value = malloc(typeSignature.methodReturnLength);
                [typeInvocation setArgument:&json atIndex:2];
                [typeInvocation invoke];
                [typeInvocation getReturnValue:value];
              }

              // Set value
              if (!targetInvocation) {
                NSMethodSignature *signature = [target methodSignatureForSelector:setter];
                targetInvocation = [NSInvocation invocationWithMethodSignature:signature];
                targetInvocation.selector = setter;
              }
              [targetInvocation setArgument:value atIndex:2];
              [targetInvocation invokeWithTarget:target];
              if (freeValueOnCompletion) {
                // Only free the value if we `malloc`d it locally, otherwise it
                // points to `defaultValue.mutableBytes`, which is managed by ARC.
                free(value);
              }
            };
            break;
          }
        }
      }

      propBlock = ^(__unused id view, __unused id json) {

        // Follow keypath
        id target = view;
        
        //keypath的情况下，需要找到最内层的对象，才能调用方法
        for (NSString *part in parts) {
          target = [target valueForKey:part];
        }

        // Set property with json
        //给view设置属性值，有上面的 RCT_CASE(_value, _type) 生成 setterBlock方法
        setterBlock(target, RCTNilIfNull(json));
      };
    }

    if (RCT_DEBUG) {

      // Provide more useful log feedback if there's an error
      RCTPropBlock unwrappedBlock = propBlock;
      propBlock = ^(id<RCTComponent> view, id json) {
        NSString *logPrefix = [NSString stringWithFormat:
                               @"Error setting property '%@' of %@ with tag #%@: ",
                               name, weakSelf.name, view.reactTag];

        RCTPerformBlockWithLogPrefix(^{ unwrappedBlock(view, json); }, logPrefix);
      };
    }

    propBlocks[name] = [propBlock copy];
  }
  return propBlock;
}

//设置view的属性，一般在创建view时设置
- (void)setProps:(NSDictionary<NSString *, id> *)props forView:(id<RCTComponent>)view
{
  if (!view) {
    return;
  }

  [props enumerateKeysAndObjectsUsingBlock:^(NSString *key, id json, __unused BOOL *stop) {
    [self propBlockForKey:key inDictionary:_viewPropBlocks](view, json);
  }];

  if ([view respondsToSelector:@selector(didSetProps:)]) {
    [view didSetProps:[props allKeys]];
  }
}

//设置shadowView的属性
- (void)setProps:(NSDictionary<NSString *, id> *)props forShadowView:(RCTShadowView *)shadowView
{
  if (!shadowView) {
    return;
  }

  [props enumerateKeysAndObjectsUsingBlock:^(NSString *key, id json, __unused BOOL *stop) {
    [self propBlockForKey:key inDictionary:_shadowPropBlocks](shadowView, json);
  }];

  if ([shadowView respondsToSelector:@selector(didSetProps:)]) {
    [shadowView didSetProps:[props allKeys]];
  }
}

//返回可配置属性（模块使用宏来导出属性，其实宏定义了一个方法，所以这里从方法里遍历）
- (NSDictionary<NSString *, id> *)viewConfig
{
  NSMutableArray<NSString *> *directEvents = [NSMutableArray new];
  
  //判断类中是否有customDirectEventTypes方法
  if (RCTClassOverridesInstanceMethod(_managerClass, @selector(customDirectEventTypes))) {
    
    NSArray<NSString *> *events = [self.manager customDirectEventTypes];
    if (RCT_DEBUG) {
      RCTAssert(!events || [events isKindOfClass:[NSArray class]],
        @"customDirectEventTypes must return an array, but %@ returned %@",
        _managerClass, [events class]);
    }
    for (NSString *event in events) {
      [directEvents addObject:RCTNormalizeInputEventName(event)];
    }
  }

  NSMutableArray<NSString *> *bubblingEvents = [NSMutableArray new];
  
  //判断类中是否有customBubblingEventTypes方法
  if (RCTClassOverridesInstanceMethod(_managerClass, @selector(customBubblingEventTypes))) {
    NSArray<NSString *> *events = [self.manager customBubblingEventTypes];
    if (RCT_DEBUG) {
      RCTAssert(!events || [events isKindOfClass:[NSArray class]],
        @"customBubblingEventTypes must return an array, but %@ returned %@",
        _managerClass, [events class]);
    }
    for (NSString *event in events) {
      [bubblingEvents addObject:RCTNormalizeInputEventName(event)];
    }
  }

  unsigned int count = 0;
  NSMutableDictionary *propTypes = [NSMutableDictionary new];
  
  //遍历类中所有方法
  Method *methods = class_copyMethodList(object_getClass(_managerClass), &count);
  for (unsigned int i = 0; i < count; i++) {
    Method method = methods[i];
    SEL selector = method_getName(method);
    NSString *methodName = NSStringFromSelector(selector);
    
    //可配置属性
    if ([methodName hasPrefix:@"propConfig"]) {
      
      NSRange nameRange = [methodName rangeOfString:@"_"];
      if (nameRange.length) {
        
        //解析属性名称
        NSString *name = [methodName substringFromIndex:nameRange.location + 1];
        
        //获取属性类型
        NSString *type = ((NSArray<NSString *> *(*)(id, SEL))objc_msgSend)(_managerClass, selector)[0];
        if (RCT_DEBUG && propTypes[name] && ![propTypes[name] isEqualToString:type]) {
          RCTLogError(@"Property '%@' of component '%@' redefined from '%@' "
                      "to '%@'", name, _name, propTypes[name], type);
        }

        //保存到propTypes中：color = UIColor;
        if ([type isEqualToString:@"RCTBubblingEventBlock"]) {
          [bubblingEvents addObject:RCTNormalizeInputEventName(name)];
          propTypes[name] = @"BOOL";
        } else if ([type isEqualToString:@"RCTDirectEventBlock"]) {
          [directEvents addObject:RCTNormalizeInputEventName(name)];
          propTypes[name] = @"BOOL";
        } else {
          propTypes[name] = type;
        }
      }
    }
  }
  free(methods);

  if (RCT_DEBUG) {
    for (NSString *event in directEvents) {
      if ([bubblingEvents containsObject:event]) {
        RCTLogError(@"Component '%@' registered '%@' as both a bubbling event "
                    "and a direct event", _name, event);
      }
    }
    for (NSString *event in bubblingEvents) {
      if ([directEvents containsObject:event]) {
        RCTLogError(@"Component '%@' registered '%@' as both a bubbling event "
                    "and a direct event", _name, event);
      }
    }
  }

  return @{
    @"propTypes" : propTypes,
    
    //debug信息
    @"directEvents" : directEvents,
    @"bubblingEvents" : bubblingEvents,
  };
}

- (RCTViewManagerUIBlock)uiBlockToAmendWithShadowViewRegistry:(NSDictionary<NSNumber *, RCTShadowView *> *)registry
{
  if (_implementsUIBlockToAmendWithShadowViewRegistry) {
    return [[self manager] uiBlockToAmendWithShadowViewRegistry:registry];
  }
  return nil;
}

@end
