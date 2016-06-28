/**
 * Copyright (c) 2015-present, Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */

#import "RCTRootShadowView.h"

@implementation RCTRootShadowView

- (void)applySizeConstraints
{
  switch (_sizeFlexibility) {
    case RCTRootViewSizeFlexibilityNone:
      break;
    case RCTRootViewSizeFlexibilityWidth:
      self.cssNode->style.dimensions[CSS_WIDTH] = CSS_UNDEFINED;
      break;
    case RCTRootViewSizeFlexibilityHeight:
      self.cssNode->style.dimensions[CSS_HEIGHT] = CSS_UNDEFINED;
      break;
    case RCTRootViewSizeFlexibilityWidthAndHeight:
      self.cssNode->style.dimensions[CSS_WIDTH] = CSS_UNDEFINED;
      self.cssNode->style.dimensions[CSS_HEIGHT] = CSS_UNDEFINED;
      break;
  }
}

- (NSSet<RCTShadowView *> *)collectViewsWithUpdatedFrames
{
  //设置css属性
  [self applySizeConstraints];

  //更新css节点数
  [self fillCSSNode:self.cssNode];
  
  //重置css节点值
  resetNodeLayout(self.cssNode);
  
  //计算节点布局
  layoutNode(self.cssNode, CSS_UNDEFINED, CSS_UNDEFINED, CSS_DIRECTION_INHERIT);

  //获取节点frame信息
  NSMutableSet<RCTShadowView *> *viewsWithNewFrame = [NSMutableSet set];
  
  //根据css重新调整UI
  [self applyLayoutNode:self.cssNode viewsWithNewFrame:viewsWithNewFrame absolutePosition:CGPointZero];
  
  return viewsWithNewFrame;
}

@end
