/**
 * Copyright (c) 2015-present, Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */

#import "RCTBridge+Private.h"

#import <objc/runtime.h>

#import "RCTConvert.h"
#import "RCTEventDispatcher.h"
#import "RCTKeyCommands.h"
#import "RCTLog.h"
#import "RCTModuleData.h"
#import "RCTPerformanceLogger.h"
#import "RCTUtils.h"

NSString *const RCTReloadNotification = @"RCTReloadNotification";
NSString *const RCTJavaScriptWillStartLoadingNotification = @"RCTJavaScriptWillStartLoadingNotification";
NSString *const RCTJavaScriptDidLoadNotification = @"RCTJavaScriptDidLoadNotification";
NSString *const RCTJavaScriptDidFailToLoadNotification = @"RCTJavaScriptDidFailToLoadNotification";
NSString *const RCTDidInitializeModuleNotification = @"RCTDidInitializeModuleNotification";

static NSMutableArray<Class> *RCTModuleClasses;
NSArray<Class> *RCTGetModuleClasses(void);
NSArray<Class> *RCTGetModuleClasses(void)
{
  return RCTModuleClasses;
}

/**
 * Register the given class as a bridge module. All modules must be registered
 * prior to the first bridge initialization.
 */
//注册native模块，用于导出到js侧调用，导出模块必须实现RCTBridgeModule协议，该协议就是为了在load时自动注册类
void RCTRegisterModule(Class);
void RCTRegisterModule(Class moduleClass)
{
  static dispatch_once_t onceToken;
  dispatch_once(&onceToken, ^{
    RCTModuleClasses = [NSMutableArray new];
  });

  RCTAssert([moduleClass conformsToProtocol:@protocol(RCTBridgeModule)],
            @"%@ does not conform to the RCTBridgeModule protocol",
            moduleClass);

  // Register module
  [RCTModuleClasses addObject:moduleClass];
}

/**
 * This function returns the module name for a given class.
 */
NSString *RCTBridgeModuleNameForClass(Class cls)
{
#if RCT_DEV
  RCTAssert([cls conformsToProtocol:@protocol(RCTBridgeModule)],
            @"Bridge module `%@` does not conform to RCTBridgeModule", cls);
#endif

  //moduleName应该是自定义名称
  NSString *name = [cls moduleName];
  if (name.length == 0) {
    name = NSStringFromClass(cls);
  }
  
  //RK根据文档说明，应该是旧协议，这里对旧的模块做兼容处理，将RK替换为RCT
  if ([name hasPrefix:@"RK"]) {
    name = [name stringByReplacingCharactersInRange:(NSRange){0,@"RK".length} withString:@"RCT"];
  }
  return name;
}

@implementation RCTBridge
{
  NSURL *_delegateBundleURL;
}

dispatch_queue_t RCTJSThread;

+ (void)initialize
{
  static dispatch_once_t onceToken;
  dispatch_once(&onceToken, ^{

    // Set up JS thread
    RCTJSThread = (id)kCFNull;
  });
}

static RCTBridge *RCTCurrentBridgeInstance = nil;

/**
 * The last current active bridge instance. This is set automatically whenever
 * the bridge is accessed. It can be useful for static functions or singletons
 * that need to access the bridge for purposes such as logging, but should not
 * be relied upon to return any particular instance, due to race conditions.
 */
+ (instancetype)currentBridge
{
  return RCTCurrentBridgeInstance;
}

+ (void)setCurrentBridge:(RCTBridge *)currentBridge
{
  RCTCurrentBridgeInstance = currentBridge;
}

- (instancetype)initWithDelegate:(id<RCTBridgeDelegate>)delegate
                   launchOptions:(NSDictionary *)launchOptions
{
  if ((self = [super init])) {
    RCTPerformanceLoggerStart(RCTPLBridgeStartup);
    RCTPerformanceLoggerStart(RCTPLTTI);

    _delegate = delegate;
    _launchOptions = [launchOptions copy];
    
    //启动BatchedBridge，加载main.js
    [self setUp];
    
    //绑定key用于调试
    RCTExecuteOnMainThread(^{ [self bindKeys]; }, NO);
  }
  return self;
}

- (instancetype)initWithBundleURL:(NSURL *)bundleURL
                   moduleProvider:(RCTBridgeModuleProviderBlock)block
                    launchOptions:(NSDictionary *)launchOptions
{
  if ((self = [super init])) {
    RCTPerformanceLoggerStart(RCTPLBridgeStartup);
    RCTPerformanceLoggerStart(RCTPLTTI);

    _bundleURL = bundleURL;
    _moduleProvider = block;
    _launchOptions = [launchOptions copy];
    [self setUp];
    RCTExecuteOnMainThread(^{ [self bindKeys]; }, NO);
  }
  return self;
}

RCT_NOT_IMPLEMENTED(- (instancetype)init)

- (void)dealloc
{
  /**
   * This runs only on the main thread, but crashes the subclass
   * RCTAssertMainThread();
   */
  [[NSNotificationCenter defaultCenter] removeObserver:self];
  [self invalidate];
}


//绑定键盘监听：调试用，重新加载、远程调试等。
- (void)bindKeys
{
  RCTAssertMainThread();

  [[NSNotificationCenter defaultCenter] addObserver:self
                                           selector:@selector(reload)
                                               name:RCTReloadNotification
                                             object:nil];

#if TARGET_IPHONE_SIMULATOR
  RCTKeyCommands *commands = [RCTKeyCommands sharedInstance];

  // reload in current mode
  [commands registerKeyCommandWithInput:@"r"
                          modifierFlags:UIKeyModifierCommand
                                 action:^(__unused UIKeyCommand *command) {
    [[NSNotificationCenter defaultCenter] postNotificationName:RCTReloadNotification
                                                        object:nil
                                                      userInfo:nil];
  }];

#endif
}

- (NSArray<Class> *)moduleClasses
{
  return self.batchedBridge.moduleClasses;
}

//返回模块名称对应的实例
- (id)moduleForName:(NSString *)moduleName
{
  return [self.batchedBridge moduleForName:moduleName];
}

- (id)moduleForClass:(Class)moduleClass
{
  return [self moduleForName:RCTBridgeModuleNameForClass(moduleClass)];
}

- (NSArray *)modulesConformingToProtocol:(Protocol *)protocol
{
  NSMutableArray *modules = [NSMutableArray new];
  for (Class moduleClass in self.moduleClasses) {
    if ([moduleClass conformsToProtocol:protocol]) {
      id module = [self moduleForClass:moduleClass];
      if (module) {
        [modules addObject:module];
      }
    }
  }
  return [modules copy];
}

- (BOOL)moduleIsInitialized:(Class)moduleClass
{
  return [self.batchedBridge moduleIsInitialized:moduleClass];
}

//对外暴露的重新加载接口，可重新指定bridge的delegate，然后调用reload，动态更换main.js
- (void)reload
{
  /**
   * Any thread
   */
  dispatch_async(dispatch_get_main_queue(), ^{
    
    //主线程中reload，先让旧环境失效，释放BatchedBridge
    [self invalidate];
    [self setUp];
  });
}

//组装接口，可热启动
- (void)setUp
{
  // Only update bundleURL from delegate if delegate bundleURL has changed
  
  //这里表明，重新加载js时，可更换url，通过delegate
  NSURL *previousDelegateURL = _delegateBundleURL;
  _delegateBundleURL = [self.delegate sourceURLForBridge:self];
  if (_delegateBundleURL && ![_delegateBundleURL isEqual:previousDelegateURL]) {
    _bundleURL = _delegateBundleURL;
  }

  // Sanitize the bundle URL
  _bundleURL = [RCTConvert NSURL:_bundleURL.absoluteString];

  //重新创建BatchedBridge
  [self createBatchedBridge];
}

- (void)createBatchedBridge
{
  self.batchedBridge = [[RCTBatchedBridge alloc] initWithParentBridge:self];
}

- (BOOL)isLoading
{
  return self.batchedBridge.loading;
}

- (BOOL)isValid
{
  return self.batchedBridge.valid;
}

- (BOOL)isBatchActive
{
  return [_batchedBridge isBatchActive];
}

//重新加载main.js时，先释放旧的BatchedBridge
- (void)invalidate
{
  RCTBridge *batchedBridge = self.batchedBridge;
  self.batchedBridge = nil;

  if (batchedBridge) {
    
    //需要主线程执行
    RCTExecuteOnMainThread(^{
      [batchedBridge invalidate];
    }, NO);
  }
}

- (void)enqueueJSCall:(NSString *)moduleDotMethod args:(NSArray *)args
{
  [self.batchedBridge enqueueJSCall:moduleDotMethod args:args];
}

- (void)enqueueCallback:(NSNumber *)cbID args:(NSArray *)args
{
  [self.batchedBridge enqueueCallback:cbID args:args];
}

@end
