/**
 * Copyright (c) 2015-present, Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */

#import "RCTTouchHandler.h"

#import <UIKit/UIGestureRecognizerSubclass.h>

#import "RCTAssert.h"
#import "RCTBridge.h"
#import "RCTEventDispatcher.h"
#import "RCTLog.h"
#import "RCTTouchEvent.h"
#import "RCTUIManager.h"
#import "RCTUtils.h"
#import "UIView+React.h"

// TODO: this class behaves a lot like a module, and could be implemented as a
// module if we were to assume that modules and RootViews had a 1:1 relationship
//此类是手势分发器，用于通知js
@implementation RCTTouchHandler
{
  //使用事件分发器发送事件
  __weak RCTEventDispatcher *_eventDispatcher;

  /**
   * Arrays managed in parallel tracking native touch object along with the
   * native view that was touched, and the React touch data dictionary.
   * These must be kept track of because `UIKit` destroys the touch targets
   * if touches are canceled, and we have no other way to recover this info.
   */
  
  //每个react侧的touch对应一个本地touch
  NSMutableOrderedSet<UITouch *> *_nativeTouches;
  //保存react侧的touch
  NSMutableArray<NSMutableDictionary *> *_reactTouches;
  //保存每个touch的目标view
  NSMutableArray<UIView *> *_touchViews;

  BOOL _dispatchedInitialTouches;
  BOOL _recordingInteractionTiming;
  CFTimeInterval _mostRecentEnqueueJS;
  uint16_t _coalescingKey;
}

- (instancetype)initWithBridge:(RCTBridge *)bridge
{
  RCTAssertParam(bridge);

  if ((self = [super initWithTarget:self action:@selector(handleGestureUpdate:)])) {

    //主view上的手势监控，是和js事件交互的基础
    _eventDispatcher = [bridge moduleForClass:[RCTEventDispatcher class]];
    _dispatchedInitialTouches = NO;
    _nativeTouches = [NSMutableOrderedSet new];
    _reactTouches = [NSMutableArray new];
    _touchViews = [NSMutableArray new];

    // `cancelsTouchesInView` is needed in order to be used as a top level
    // event delegated recognizer. Otherwise, lower-level components not built
    // using RCT, will fail to recognize gestures.
    
    //是手势操作与touch同时起作用，不发送touch cancel，为的是react低阶组件能处理手势
    //默认为yes，意思是当识别到手势操作后，取消touch识别，会发送touch cancel操作
    self.cancelsTouchesInView = NO;
  }
  return self;
}

RCT_NOT_IMPLEMENTED(- (instancetype)initWithTarget:(id)target action:(SEL)action)

typedef NS_ENUM(NSInteger, RCTTouchEventType) {
  RCTTouchEventTypeStart,
  RCTTouchEventTypeMove,
  RCTTouchEventTypeEnd,
  RCTTouchEventTypeCancel
};

#pragma mark - Bookkeeping for touch indices

- (void)_recordNewTouches:(NSSet<UITouch *> *)touches
{
  for (UITouch *touch in touches) {

    RCTAssert(![_nativeTouches containsObject:touch],
              @"Touch is already recorded. This is a critical bug.");

    // Find closest React-managed touchable view
    //查找js控制的可触摸view（查找最上层的view，后面还要根据触摸点继续往下查找）
    UIView *targetView = touch.view;
    while (targetView) {
      
      //reactTag不为0表示受React管控，userInteractionEnabled代表view可触摸
      if (targetView.reactTag && targetView.userInteractionEnabled &&
          [targetView reactRespondsToTouch:touch]) {
        break;
      }
      targetView = targetView.superview;
    }

    //查找可处理触摸事件的view，并返回该view的tag
    NSNumber *reactTag = [targetView reactTagAtPoint:[touch locationInView:targetView]];
    
    //这个判断语句写在这里，而不是reactTagAtPoint里，会不会漏掉某些事件处理？
    if (!reactTag || !targetView.userInteractionEnabled) {
      return;
    }

    // Get new, unique touch identifier for the react touch
    
    //touchID，是自定义的，保证唯一性
    const NSUInteger RCTMaxTouches = 11; // This is the maximum supported by iDevices
    NSInteger touchID = ([_reactTouches.lastObject[@"identifier"] integerValue] + 1) % RCTMaxTouches;
    for (NSDictionary *reactTouch in _reactTouches) {
      NSInteger usedID = [reactTouch[@"identifier"] integerValue];
      if (usedID == touchID) {
        // ID has already been used, try next value
        touchID ++;
      } else if (usedID > touchID) {
        // If usedID > touchID, touchID must be unique, so we can stop looking
        break;
      }
    }

    // Create touch
    //表示一个touch事件，保存touch的view tag，和touchID
    //一个reactTouch对应一个nativeTouch
    //targetView保存真实的view，reactTouch保存view tag
    NSMutableDictionary *reactTouch = [[NSMutableDictionary alloc] initWithCapacity:RCTMaxTouches];
    reactTouch[@"target"] = reactTag;
    reactTouch[@"identifier"] = @(touchID);

    // Add to arrays
    [_touchViews addObject:targetView];
    [_nativeTouches addObject:touch];
    [_reactTouches addObject:reactTouch];
  }
}

//清理缓存touch事件
- (void)_recordRemovedTouches:(NSSet<UITouch *> *)touches
{
  for (UITouch *touch in touches) {
    NSUInteger index = [_nativeTouches indexOfObject:touch];
    if(index == NSNotFound) {
      continue;
    }

    [_touchViews removeObjectAtIndex:index];
    [_nativeTouches removeObjectAtIndex:index];
    [_reactTouches removeObjectAtIndex:index];
  }
}

//更新touch记录
- (void)_updateReactTouchAtIndex:(NSInteger)touchIndex
{
  //touch事件以native touch记录为准
  UITouch *nativeTouch = _nativeTouches[touchIndex];
  
  //组装touch信息，已备给js发送
  CGPoint windowLocation = [nativeTouch locationInView:nativeTouch.window];
  CGPoint rootViewLocation = [nativeTouch.window convertPoint:windowLocation toView:self.view];

  UIView *touchView = _touchViews[touchIndex];
  CGPoint touchViewLocation = [nativeTouch.window convertPoint:windowLocation toView:touchView];

  //更新_reactTouches中的记录
  NSMutableDictionary *reactTouch = _reactTouches[touchIndex];
  reactTouch[@"pageX"] = @(rootViewLocation.x);
  reactTouch[@"pageY"] = @(rootViewLocation.y);
  reactTouch[@"locationX"] = @(touchViewLocation.x);
  reactTouch[@"locationY"] = @(touchViewLocation.y);
  reactTouch[@"timestamp"] =  @(nativeTouch.timestamp * 1000); // in ms, for JS

  // TODO: force for a 'normal' touch is usually 1.0;
  // should we expose a `normalTouchForce` constant somewhere (which would
  // have a value of `1.0 / nativeTouch.maximumPossibleForce`)?
  if (RCTForceTouchAvailable()) {
    reactTouch[@"force"] = @(RCTZeroIfNaN(nativeTouch.force / nativeTouch.maximumPossibleForce));
  }
}

/**
 * Constructs information about touch events to send across the serialized
 * boundary. This data should be compliant with W3C `Touch` objects. This data
 * alone isn't sufficient to construct W3C `Event` objects. To construct that,
 * there must be a simple receiver on the other side of the bridge that
 * organizes the touch objects into `Event`s.
 *
 * We send the data as an array of `Touch`es, the type of action
 * (start/end/move/cancel) and the indices that represent "changed" `Touch`es
 * from that array.
 */
- (void)_updateAndDispatchTouches:(NSSet<UITouch *> *)touches
                        eventName:(NSString *)eventName
                  originatingTime:(__unused CFTimeInterval)originatingTime
{
  // Update touches
  NSMutableArray<NSNumber *> *changedIndexes = [NSMutableArray new];
  
  for (UITouch *touch in touches) {
    NSInteger index = [_nativeTouches indexOfObject:touch];
    if (index == NSNotFound) {
      continue;
    }

    //更新_reactTouches中的所有记录
    //注意_reactTouches更新记录后，里面的事件信息不同，change的事件信息更全
    //而且后面不是发送变更记录，而是将所有记录发送，并且发送后未见清理缓存touch（累计发送？）
    [self _updateReactTouchAtIndex:index];
    
    //将所有变化的下标序列提供给js
    [changedIndexes addObject:@(index)];
  }

  if (changedIndexes.count == 0) {
    return;
  }

  // Deep copy the touches because they will be accessed from another thread
  // TODO: would it be safer to do this in the bridge or executor, rather than trusting caller?
  NSMutableArray<NSDictionary *> *reactTouches =
    [[NSMutableArray alloc] initWithCapacity:_reactTouches.count];
  
  //深拷贝_reactTouches中的touch记录
  for (NSDictionary *touch in _reactTouches) {
    [reactTouches addObject:[touch copy]];
  }

  //创建一个触摸事件，发送到js
  RCTTouchEvent *event = [[RCTTouchEvent alloc] initWithEventName:eventName
                                                     reactTouches:reactTouches
                                                   changedIndexes:changedIndexes
                                                    coalescingKey:_coalescingKey];
  [_eventDispatcher sendEvent:event];
}

#pragma mark - Gesture Recognizer Delegate Callbacks

static BOOL RCTAllTouchesAreCancelledOrEnded(NSSet<UITouch *> *touches)
{
  for (UITouch *touch in touches) {
    if (touch.phase == UITouchPhaseBegan ||
        touch.phase == UITouchPhaseMoved ||
        touch.phase == UITouchPhaseStationary) {
      return NO;
    }
  }
  return YES;
}

static BOOL RCTAnyTouchesChanged(NSSet<UITouch *> *touches)
{
  for (UITouch *touch in touches) {
    if (touch.phase == UITouchPhaseBegan ||
        touch.phase == UITouchPhaseMoved) {
      return YES;
    }
  }
  return NO;
}

//识别到某个手势后回调此函数
- (void)handleGestureUpdate:(__unused UIGestureRecognizer *)gesture
{
  // If gesture just recognized, send all touches to JS as if they just began.
  

  //识别到了某手势，改变状态，并将touch事件发送给js（注意：识别到手势才发送touch记录，并且并未使用gesture）
  //一旦识别一次，后续不再操作，即只发送一次top start
  if (self.state == UIGestureRecognizerStateBegan) {
    
    //手势识别前的touch记录，是top start，后续的touch才是start move end
    [self _updateAndDispatchTouches:_nativeTouches.set eventName:@"topTouchStart" originatingTime:0];

    // We store this flag separately from `state` because after a gesture is
    // recognized, its `state` changes immediately but its action (this
    // method) isn't fired until dependent gesture recognizers have failed. We
    // only want to send move/end/cancel touches if we've sent the touchStart.
    
    //更新touch事件分发状态，后续touch回调才可以更新发送touch事件
    _dispatchedInitialTouches = YES;
  }

  // For the other states, we could dispatch the updates here but since we
  // specifically send info about which touches changed, it's simpler to
  // dispatch the updates from the raw touch methods below.
}

- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
  [super touchesBegan:touches withEvent:event];

  _coalescingKey++;
  // "start" has to record new touches before extracting the event.
  // "end"/"cancel" needs to remove the touch *after* extracting the event.
  //开始记录所有触摸事件
  [self _recordNewTouches:touches];
  
  //未识别出手势时，一直处于touch began状态，缓存touch事件，直到touch end
  if (_dispatchedInitialTouches) {
    [self _updateAndDispatchTouches:touches eventName:@"touchStart" originatingTime:event.timestamp];
    self.state = UIGestureRecognizerStateChanged;
  } else {
    self.state = UIGestureRecognizerStateBegan;
  }
}

- (void)touchesMoved:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
  [super touchesMoved:touches withEvent:event];

  //这里没有记录touch事件，而是从缓存中找？
  if (_dispatchedInitialTouches) {
    [self _updateAndDispatchTouches:touches eventName:@"touchMove" originatingTime:event.timestamp];
    self.state = UIGestureRecognizerStateChanged;
  }
}

- (void)touchesEnded:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
  [super touchesEnded:touches withEvent:event];

  _coalescingKey++;
  if (_dispatchedInitialTouches) {
    [self _updateAndDispatchTouches:touches eventName:@"touchEnd" originatingTime:event.timestamp];

    if (RCTAllTouchesAreCancelledOrEnded(event.allTouches)) {
      self.state = UIGestureRecognizerStateEnded;
    } else if (RCTAnyTouchesChanged(event.allTouches)) {
      self.state = UIGestureRecognizerStateChanged;
    }
  }
  
  //每次触摸结束，清除touch记录（如果还是未识别手势，则touch事件一直未发送给js）
  [self _recordRemovedTouches:touches];
}

//因为关闭了cancelsTouchesInView，所以不会调用此回调
- (void)touchesCancelled:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
  [super touchesCancelled:touches withEvent:event];

  if (_dispatchedInitialTouches) {
    [self _updateAndDispatchTouches:touches eventName:@"touchCancel" originatingTime:event.timestamp];

    if (RCTAllTouchesAreCancelledOrEnded(event.allTouches)) {
      self.state = UIGestureRecognizerStateCancelled;
    } else if (RCTAnyTouchesChanged(event.allTouches)) {
      self.state = UIGestureRecognizerStateChanged;
    }
  }
  [self _recordRemovedTouches:touches];
}

- (BOOL)canPreventGestureRecognizer:(__unused UIGestureRecognizer *)preventedGestureRecognizer
{
  return NO;
}

- (BOOL)canBePreventedByGestureRecognizer:(__unused UIGestureRecognizer *)preventingGestureRecognizer
{
  return NO;
}

- (void)reset
{
  _dispatchedInitialTouches = NO;
}

- (void)cancel
{
  self.enabled = NO;
  self.enabled = YES;
}

@end
