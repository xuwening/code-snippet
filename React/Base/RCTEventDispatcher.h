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

typedef NS_ENUM(NSInteger, RCTTextEventType)
{
  RCTTextEventTypeFocus,
  RCTTextEventTypeBlur,
  RCTTextEventTypeChange,
  RCTTextEventTypeSubmit,
  RCTTextEventTypeEnd,
  RCTTextEventTypeKeyPress
};

/**
 * The threshold at which text inputs will start warning that the JS thread
 * has fallen behind (resulting in poor input performance, missed keys, etc.)
 */
RCT_EXTERN const NSInteger RCTTextUpdateLagWarningThreshold;

/**
 * Takes an input event name and normalizes it to the form that is required
 * by the events system (currently that means starting with the "top" prefix,
 * but that's an implementation detail that may change in future).
 */
RCT_EXTERN NSString *RCTNormalizeInputEventName(NSString *eventName);


//touch事件必须符合该协议
@protocol RCTEvent <NSObject>

@required

//协议中定义属性，实现类中必须使用@synthesize
//事件来源于哪个view
@property (nonatomic, strong, readonly) NSNumber *viewTag;
//事件名
@property (nonatomic, copy, readonly) NSString *eventName;
@property (nonatomic, assign, readonly) uint16_t coalescingKey;

- (BOOL)canCoalesce;
- (id<RCTEvent>)coalesceWithEvent:(id<RCTEvent>)newEvent;

// used directly for doing a JS call
+ (NSString *)moduleDotMethod;
// must contain only JSON compatible values
- (NSArray *)arguments;

@end


/**
 * This class wraps the -[RCTBridge enqueueJSCall:args:] method, and
 * provides some convenience methods for generating event calls.
 */

//native事件分发器，事件发送接口还是使用了enqueueJSCall:args 函数发送（js端对应__fbBatchedBridge对象）
//事件分发器也是一个单独组件（导出类）

//事件分类： app事件、设备事件、输入事件
@interface RCTEventDispatcher : NSObject <RCTBridgeModule>

/**
 * Send an application-specific event that does not relate to a specific
 * view, e.g. a navigation or data update notification.
 */

//app事件：即app中和view无关的事件，如导航事件、数据更新通知等
- (void)sendAppEventWithName:(NSString *)name body:(id)body;

/**
 * Send a device or iOS event that does not relate to a specific view,
 * e.g.rotation, location, keyboard show/hide, background/awake, etc.
 */

//设备事件：设备旋转，定位，键盘的显示隐藏、app后台、唤醒
- (void)sendDeviceEventWithName:(NSString *)name body:(id)body;

/**
 * Send a user input event. The body dictionary must contain a "target"
 * parameter, representing the React tag of the view sending the event
 */
//输入事件
- (void)sendInputEventWithName:(NSString *)name body:(NSDictionary *)body;

/**
 * Send a text input/focus event.
 */
//焦点事件
- (void)sendTextEventWithType:(RCTTextEventType)type
                     reactTag:(NSNumber *)reactTag
                         text:(NSString *)text
                          key:(NSString *)key
                   eventCount:(NSInteger)eventCount;

/**
 * Send a pre-prepared event object.
 *
 * Events are sent to JS as soon as the thread is free to process them.
 * If an event can be coalesced and there is another compatible event waiting, the coalescing will happen immediately.
 */
- (void)sendEvent:(id<RCTEvent>)event;

@end

@interface RCTBridge (RCTEventDispatcher)

- (RCTEventDispatcher *)eventDispatcher;

@end
