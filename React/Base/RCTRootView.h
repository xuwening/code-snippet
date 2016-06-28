/**
 * Copyright (c) 2015-present, Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */

#import <UIKit/UIKit.h>

#import "RCTBridge.h"

@protocol RCTRootViewDelegate;

/**
 * This enum is used to define size flexibility type of the root view.
 * If a dimension is flexible, the view will recalculate that dimension
 * so the content fits. Recalculations are performed when the root's frame,
 * size flexibility mode or content size changes. After a recalculation,
 * rootViewDidChangeIntrinsicSize method of the RCTRootViewDelegate will be called.
 */

//root view的弹性布局，默认无，针对js的动态宽高
typedef NS_ENUM(NSInteger, RCTRootViewSizeFlexibility) {
  RCTRootViewSizeFlexibilityNone = 0,
  RCTRootViewSizeFlexibilityWidth,
  RCTRootViewSizeFlexibilityHeight,
  RCTRootViewSizeFlexibilityWidthAndHeight,
};

/**
 * This notification is sent when the first subviews are added to the root view
 * after the application has loaded. This is used to hide the `loadingView`, and
 * is a good indicator that the application is ready to use.
 */
//当root view首次添加子view时通知，意思app状态已经可用
//可用于隐藏loadingView
extern NSString *const RCTContentDidAppearNotification;

/**
 * Native view used to host React-managed views within the app. Can be used just
 * like any ordinary UIView. You can have multiple RCTRootViews on screen at
 * once, all controlled by the same JavaScript application.
 */

//主view，可以一个屏幕有多个主view，被同一js控制
@interface RCTRootView : UIView

/**
 * - Designated initializer -
 */
- (instancetype)initWithBridge:(RCTBridge *)bridge
                    moduleName:(NSString *)moduleName
             initialProperties:(NSDictionary *)initialProperties NS_DESIGNATED_INITIALIZER;

/**
 * - Convenience initializer -
 * A bridge will be created internally.
 * This initializer is intended to be used when the app has a single RCTRootView,
 * otherwise create an `RCTBridge` and pass it in via `initWithBridge:moduleName:`
 * to all the instances.
 */
- (instancetype)initWithBundleURL:(NSURL *)bundleURL
                       moduleName:(NSString *)moduleName
                initialProperties:(NSDictionary *)initialProperties
                    launchOptions:(NSDictionary *)launchOptions;

/**
 * The name of the JavaScript module to execute within the
 * specified scriptURL (required). Setting this will not have
 * any immediate effect, but it must be done prior to loading
 * the script.
 */
@property (nonatomic, copy, readonly) NSString *moduleName;

/**
 * The bridge used by the root view. Bridges can be shared between multiple
 * root views, so you can use this property to initialize another RCTRootView.
 */
@property (nonatomic, strong, readonly) RCTBridge *bridge;

/**
 * The properties to apply to the view. Use this property to update
 * application properties and rerender the view. Initialized with
 * initialProperties argument of the initializer.
 *
 * Set this property only on the main thread.
 */
@property (nonatomic, copy, readwrite) NSDictionary *appProperties;

/**
 * The size flexibility mode of the root view.
 */
@property (nonatomic, assign) RCTRootViewSizeFlexibility sizeFlexibility;

/**
 * The size of the root view's content. This is set right before the
 * rootViewDidChangeIntrinsicSize method of RCTRootViewDelegate is called.
 */
@property (readonly, nonatomic, assign) CGSize intrinsicSize;

/**
 * The delegate that handles intrinsic size updates.
 */
@property (nonatomic, weak) id<RCTRootViewDelegate> delegate;

/**
 * The backing view controller of the root view.
 */
@property (nonatomic, weak) UIViewController *reactViewController;

/**
 * The React-managed contents view of the root view.
 */
@property (nonatomic, strong, readonly) UIView *contentView;

/**
 * A view to display while the JavaScript is loading, so users aren't presented
 * with a blank screen. By default this is nil, but you can override it with
 * (for example) a UIActivityIndicatorView or a placeholder image.
 */
@property (nonatomic, strong) UIView *loadingView;

/**
 * Calling this will result in emitting a "touches cancelled" event to js,
 * which effectively cancels all js "gesture recognizers" such as as touchable
 * (unless they explicitely ignore cancellation events, but noone should do that).
 *
 * This API is exposed for integration purposes where you embed RN rootView
 * in a native view with a native gesture recognizer,
 * whose activation should prevent any in-flight js "gesture recognizer" from activating.
 *
 * An example would be RN rootView embedded in an UIScrollView.
 * When you touch down on a touchable component and drag your finger up,
 * you don't want any touch to be registered as soon as the UIScrollView starts scrolling.
 *
 * Note that this doesn't help with tapping on a touchable element that is being scrolled,
 * unless you can call cancelTouches exactly between "touches began" and "touches ended" events.
 * This is a reason why this API may be soon removed in favor of a better solution.
 */
//调用该接口，会给js发送一个“touches cancelled”事件，打断js侧的所有手势识别
//场景举例：当root view放在本地scrollview上时，滑动scrollview时不想在js侧接收手势
- (void)cancelTouches;

/**
 * Timings for hiding the loading view after the content has loaded. Both of
 * these values default to 0.25 seconds.
 */
//loading view的时长
@property (nonatomic, assign) NSTimeInterval loadingViewFadeDelay;
@property (nonatomic, assign) NSTimeInterval loadingViewFadeDuration;

@end
