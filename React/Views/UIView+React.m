/**
 * Copyright (c) 2015-present, Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */

#import "UIView+React.h"

#import <objc/runtime.h>

#import "RCTAssert.h"
#import "RCTLog.h"
#import "RCTShadowView.h"

@implementation UIView (React)

//view 的标识
- (NSNumber *)reactTag
{
  return objc_getAssociatedObject(self, _cmd);
}

- (void)setReactTag:(NSNumber *)reactTag
{
  objc_setAssociatedObject(self, @selector(reactTag), reactTag, OBJC_ASSOCIATION_RETAIN_NONATOMIC);
}

#if RCT_DEV

- (RCTShadowView *)_DEBUG_reactShadowView
{
  return objc_getAssociatedObject(self, _cmd);
}

- (void)_DEBUG_setReactShadowView:(RCTShadowView *)shadowView
{
  // Use assign to avoid keeping the shadowView alive it if no longer exists
  objc_setAssociatedObject(self, @selector(_DEBUG_reactShadowView), shadowView, OBJC_ASSOCIATION_ASSIGN);
}

#endif

//用来区分是否主view
- (BOOL)isReactRootView
{
  return RCTIsReactRootView(self.reactTag);
}

//判断触摸事件发生在哪个view
- (NSNumber *)reactTagAtPoint:(CGPoint)point
{
  UIView *view = [self hitTest:point withEvent:nil];
  while (view && !view.reactTag) {
    view = view.superview;
  }
  return view.reactTag;
}

//插入子view
- (void)insertReactSubview:(UIView *)subview atIndex:(NSInteger)atIndex
{
  [self insertSubview:subview atIndex:atIndex];
}

//移除子view
- (void)removeReactSubview:(UIView *)subview
{
  RCTAssert(subview.superview == self, @"%@ is a not a subview of %@", subview, self);
  [subview removeFromSuperview];
}

//返回view的所有子view
- (NSArray<UIView *> *)reactSubviews
{
  return self.subviews;
}

//父view
- (UIView *)reactSuperview
{
  return self.superview;
}

- (void)reactSetFrame:(CGRect)frame
{
  // These frames are in terms of anchorPoint = topLeft, but internally the
  // views are anchorPoint = center for easier scale and rotation animations.
  // Convert the frame so it works with anchorPoint = center.
  CGPoint position = {CGRectGetMidX(frame), CGRectGetMidY(frame)};
  CGRect bounds = {CGPointZero, frame.size};

  // Avoid crashes due to nan coords
  if (isnan(position.x) || isnan(position.y) ||
      isnan(bounds.origin.x) || isnan(bounds.origin.y) ||
      isnan(bounds.size.width) || isnan(bounds.size.height)) {
    RCTLogError(@"Invalid layout for (%@)%@. position: %@. bounds: %@",
                self.reactTag, self, NSStringFromCGPoint(position), NSStringFromCGRect(bounds));
    return;
  }

  self.center = position;
  self.bounds = bounds;
}

- (void)reactSetInheritedBackgroundColor:(__unused UIColor *)inheritedBackgroundColor
{
  // Does nothing by default
}

//获取view所在的viewcontroller
- (UIViewController *)reactViewController
{
  id responder = [self nextResponder];
  while (responder) {
    if ([responder isKindOfClass:[UIViewController class]]) {
      return responder;
    }
    responder = [responder nextResponder];
  }
  return nil;
}

//添加viewcontroller，保持viewcontroller的生命周期
- (void)reactAddControllerToClosestParent:(UIViewController *)controller
{
  //如果该viewcontroller不存在父viewcontroller，才添加，存在则不用添加
  if (!controller.parentViewController) {
    
    //获取父view
    UIView *parentView = (UIView *)self.reactSuperview;
    while (parentView) {
      
      //如果父view有viewcontroller，则把controller添加到父view的控制器上
      if (parentView.reactViewController) {
        
        //双向添加
        [parentView.reactViewController addChildViewController:controller];
        [controller didMoveToParentViewController:parentView.reactViewController];
        break;
      }
      
      //遍历父view，找到有viewcontroller的
      parentView = (UIView *)parentView.reactSuperview;
    }
    return;
  }
}

/**
 * Responder overrides - to be deprecated.
 */
- (void)reactWillMakeFirstResponder {};
- (void)reactDidMakeFirstResponder {};
- (BOOL)reactRespondsToTouch:(__unused UITouch *)touch
{
  return YES;
}

@end
