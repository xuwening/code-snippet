/**
 * Copyright (c) 2015-present, Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */

#import "RCTRootView.h"
#import "RCTRootViewDelegate.h"
#import "RCTRootViewInternal.h"

#import <objc/runtime.h>

#import "RCTAssert.h"
#import "RCTBridge+Private.h"
#import "RCTEventDispatcher.h"
#import "RCTKeyCommands.h"
#import "RCTLog.h"
#import "RCTPerformanceLogger.h"
#import "RCTSourceCode.h"
#import "RCTTouchHandler.h"
#import "RCTUIManager.h"
#import "RCTUtils.h"
#import "RCTView.h"
#import "UIView+React.h"
#import "RCTProfile.h"

NSString *const RCTContentDidAppearNotification = @"RCTContentDidAppearNotification";

@interface RCTUIManager (RCTRootView)

- (NSNumber *)allocateRootTag;

@end

@interface RCTRootContentView : RCTView <RCTInvalidating>

@property (nonatomic, readonly) BOOL contentHasAppeared;

//主view上的手势识别
@property (nonatomic, readonly, strong) RCTTouchHandler *touchHandler;

- (instancetype)initWithFrame:(CGRect)frame
                       bridge:(RCTBridge *)bridge
                     reactTag:(NSNumber *)reactTag
               sizeFlexiblity:(RCTRootViewSizeFlexibility)sizeFlexibility NS_DESIGNATED_INITIALIZER;
@end

@implementation RCTRootView
{
  RCTBridge *_bridge;
  NSString *_moduleName;
  NSDictionary *_launchOptions;
  RCTRootContentView *_contentView;
}

- (instancetype)initWithBridge:(RCTBridge *)bridge
                    moduleName:(NSString *)moduleName
             initialProperties:(NSDictionary *)initialProperties
{
  RCTAssertMainThread();
  RCTAssert(bridge, @"A bridge instance is required to create an RCTRootView");
  RCTAssert(moduleName, @"A moduleName is required to create an RCTRootView");

  RCT_PROFILE_BEGIN_EVENT(0, @"-[RCTRootView init]", nil);

  if ((self = [super initWithFrame:CGRectZero])) {

    self.backgroundColor = [UIColor whiteColor];

    _bridge = bridge;
    _moduleName = moduleName;
    _appProperties = [initialProperties copy];
    _loadingViewFadeDelay = 0.25;
    _loadingViewFadeDuration = 0.25;
    _sizeFlexibility = RCTRootViewSizeFlexibilityNone;
    self.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;

    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(bridgeDidReload)
                                                 name:RCTJavaScriptWillStartLoadingNotification
                                               object:_bridge];

    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(javaScriptDidLoad:)
                                                 name:RCTJavaScriptDidLoadNotification
                                               object:_bridge];

    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(hideLoadingView)
                                                 name:RCTContentDidAppearNotification
                                               object:self];

    if (!_bridge.loading) {
      [self bundleFinishedLoading:_bridge.batchedBridge];
    }

    [self showLoadingView];
  }

  RCT_PROFILE_END_EVENT(0, @"", nil);

  return self;
}

- (instancetype)initWithBundleURL:(NSURL *)bundleURL
                       moduleName:(NSString *)moduleName
                initialProperties:(NSDictionary *)initialProperties
                    launchOptions:(NSDictionary *)launchOptions
{
  RCTBridge *bridge = [[RCTBridge alloc] initWithBundleURL:bundleURL
                                            moduleProvider:nil
                                             launchOptions:launchOptions];

  return [self initWithBridge:bridge moduleName:moduleName initialProperties:initialProperties];
}

RCT_NOT_IMPLEMENTED(- (instancetype)initWithFrame:(CGRect)frame)
RCT_NOT_IMPLEMENTED(- (instancetype)initWithCoder:(NSCoder *)aDecoder)

- (void)setBackgroundColor:(UIColor *)backgroundColor
{
  super.backgroundColor = backgroundColor;
  _contentView.backgroundColor = backgroundColor;
}

- (UIViewController *)reactViewController
{
  return _reactViewController ?: [super reactViewController];
}

- (BOOL)canBecomeFirstResponder
{
  return YES;
}

- (void)setLoadingView:(UIView *)loadingView
{
  _loadingView = loadingView;
  if (!_contentView.contentHasAppeared) {
    [self showLoadingView];
  }
}

- (void)showLoadingView
{
  if (_loadingView && !_contentView.contentHasAppeared) {
    _loadingView.hidden = NO;
    [self addSubview:_loadingView];
  }
}

- (void)hideLoadingView
{
  if (_loadingView.superview == self && _contentView.contentHasAppeared) {
    if (_loadingViewFadeDuration > 0) {
      dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(_loadingViewFadeDelay * NSEC_PER_SEC)),
                     dispatch_get_main_queue(), ^{

                       [UIView transitionWithView:self
                                         duration:_loadingViewFadeDuration
                                          options:UIViewAnimationOptionTransitionCrossDissolve
                                       animations:^{
                                         _loadingView.hidden = YES;
                                       } completion:^(__unused BOOL finished) {
                                         [_loadingView removeFromSuperview];
                                       }];
                     });
    } else {
      _loadingView.hidden = YES;
      [_loadingView removeFromSuperview];
    }
  }
}

- (NSNumber *)reactTag
{
  RCTAssertMainThread();
  if (!super.reactTag) {
    /**
     * Every root view that is created must have a unique react tag.
     * Numbering of these tags goes from 1, 11, 21, 31, etc
     *
     * NOTE: Since the bridge persists, the RootViews might be reused, so the
     * react tag must be re-assigned every time a new UIManager is created.
     */
    self.reactTag = [_bridge.uiManager allocateRootTag];
  }
  return super.reactTag;
}

- (void)bridgeDidReload
{
  RCTAssertMainThread();
  // Clear the reactTag so it can be re-assigned
  self.reactTag = nil;
}

//main.js加载完毕后发送通知，开始执行此方法
- (void)javaScriptDidLoad:(NSNotification *)notification
{
  RCTAssertMainThread();
  RCTBridge *bridge = notification.userInfo[@"bridge"];
  [self bundleFinishedLoading:bridge];
}

- (void)bundleFinishedLoading:(RCTBridge *)bridge
{
  if (!bridge.valid) {
    return;
  }

  [_contentView removeFromSuperview];
  
//  创建content view，用于所有js创建的view的管理
  _contentView = [[RCTRootContentView alloc] initWithFrame:self.bounds
                                                    bridge:bridge
                                                  reactTag:self.reactTag
                                            sizeFlexiblity:self.sizeFlexibility];
  
  //开始调用js运行
  [self runApplication:bridge];

  _contentView.backgroundColor = self.backgroundColor;
  [self insertSubview:_contentView atIndex:0];
}

//运行js的程序
- (void)runApplication:(RCTBridge *)bridge
{
  NSString *moduleName = _moduleName ?: @"";
  NSDictionary *appParameters = @{
    @"rootTag": _contentView.reactTag,
    @"initialProps": _appProperties ?: @{},
  };

  //调用AppRegistry.runApplicationjs运行，并传递native参数
  [bridge enqueueJSCall:@"AppRegistry.runApplication"
                   args:@[moduleName, appParameters]];
}

- (void)setSizeFlexibility:(RCTRootViewSizeFlexibility)sizeFlexibility
{
  _sizeFlexibility = sizeFlexibility;
  [self setNeedsLayout];
}

- (void)layoutSubviews
{
  [super layoutSubviews];
  _contentView.frame = self.bounds;
  _loadingView.center = (CGPoint){
    CGRectGetMidX(self.bounds),
    CGRectGetMidY(self.bounds)
  };
}

- (void)setAppProperties:(NSDictionary *)appProperties
{
  RCTAssertMainThread();

  if ([_appProperties isEqualToDictionary:appProperties]) {
    return;
  }

  _appProperties = [appProperties copy];

  if (_contentView && _bridge.valid && !_bridge.loading) {
    [self runApplication:_bridge.batchedBridge];
  }
}

- (void)setIntrinsicSize:(CGSize)intrinsicSize
{
  BOOL oldSizeHasAZeroDimension = _intrinsicSize.height == 0 || _intrinsicSize.width == 0;
  BOOL newSizeHasAZeroDimension = intrinsicSize.height == 0 || intrinsicSize.width == 0;
  BOOL bothSizesHaveAZeroDimension = oldSizeHasAZeroDimension && newSizeHasAZeroDimension;

  BOOL sizesAreEqual = CGSizeEqualToSize(_intrinsicSize, intrinsicSize);

  _intrinsicSize = intrinsicSize;

  // Don't notify the delegate if the content remains invisible or its size has not changed
  if (bothSizesHaveAZeroDimension || sizesAreEqual) {
    return;
  }

  [_delegate rootViewDidChangeIntrinsicSize:self];
}

- (void)contentViewInvalidated
{
  [_contentView removeFromSuperview];
  _contentView = nil;
  [self showLoadingView];
}

- (void)dealloc
{
  [[NSNotificationCenter defaultCenter] removeObserver:self];
  [_contentView invalidate];
}

- (void)cancelTouches
{
  [[_contentView touchHandler] cancel];
}

@end

@implementation RCTUIManager (RCTRootView)

- (NSNumber *)allocateRootTag
{
  NSNumber *rootTag = objc_getAssociatedObject(self, _cmd) ?: @1;
  objc_setAssociatedObject(self, _cmd, @(rootTag.integerValue + 10), OBJC_ASSOCIATION_RETAIN_NONATOMIC);
  return rootTag;
}

@end

@implementation RCTRootContentView
{
  __weak RCTBridge *_bridge;
  UIColor *_backgroundColor;
}

- (instancetype)initWithFrame:(CGRect)frame
                       bridge:(RCTBridge *)bridge
                     reactTag:(NSNumber *)reactTag
               sizeFlexiblity:(RCTRootViewSizeFlexibility)sizeFlexibility
{
  if ((self = [super initWithFrame:frame])) {
    _bridge = bridge;
    self.reactTag = reactTag;
    
    //创建手势添加到content view上
    _touchHandler = [[RCTTouchHandler alloc] initWithBridge:_bridge];
    [self addGestureRecognizer:_touchHandler];
    
    //UIManger管理content view，content view 应该是用来呈现js view的
    [_bridge.uiManager registerRootView:self withSizeFlexibility:sizeFlexibility];
    self.layer.backgroundColor = NULL;
  }
  return self;
}

RCT_NOT_IMPLEMENTED(-(instancetype)initWithFrame:(CGRect)frame)
RCT_NOT_IMPLEMENTED(-(instancetype)initWithCoder:(nonnull NSCoder *)aDecoder)

- (void)insertReactSubview:(id<RCTComponent>)subview atIndex:(NSInteger)atIndex
{
  [super insertReactSubview:subview atIndex:atIndex];
  RCTPerformanceLoggerEnd(RCTPLTTI);
  dispatch_async(dispatch_get_main_queue(), ^{
    if (!_contentHasAppeared) {
      _contentHasAppeared = YES;
      [[NSNotificationCenter defaultCenter] postNotificationName:RCTContentDidAppearNotification
                                                          object:self.superview];
    }
  });
}

- (void)setFrame:(CGRect)frame
{
  super.frame = frame;
  if (self.reactTag && _bridge.isValid) {
    [_bridge.uiManager setFrame:frame forView:self];
  }
}

- (void)setBackgroundColor:(UIColor *)backgroundColor
{
  _backgroundColor = backgroundColor;
  if (self.reactTag && _bridge.isValid) {
    [_bridge.uiManager setBackgroundColor:backgroundColor forView:self];
  }
}

- (UIColor *)backgroundColor
{
  return _backgroundColor;
}

- (void)invalidate
{
  if (self.userInteractionEnabled) {
    self.userInteractionEnabled = NO;
    [(RCTRootView *)self.superview contentViewInvalidated];
    [_bridge enqueueJSCall:@"AppRegistry.unmountApplicationComponentAtRootTag"
                      args:@[self.reactTag]];
  }
}

@end
