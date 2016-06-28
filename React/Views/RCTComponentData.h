/**
 * Copyright (c) 2015-present, Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */

#import <Foundation/Foundation.h>

#import "RCTComponent.h"
#import "RCTDefines.h"
#import "RCTViewManager.h"

@class RCTBridge;
@class RCTShadowView;
@class UIView;

//组件包装类，一个组件对应一个componentData
@interface RCTComponentData : NSObject


//指向本组件的管理类，用于实例化manager对象，也用于接收类方法
@property (nonatomic, readonly) Class managerClass;

//组件名称
@property (nonatomic, copy, readonly) NSString *name;

//js调用native设置属性、实例方法时，知晓调用哪个viewManager类实例（mapManager、alertManager）
@property (nonatomic, weak, readonly) RCTViewManager *manager;

- (instancetype)initWithManagerClass:(Class)managerClass
                              bridge:(RCTBridge *)bridge NS_DESIGNATED_INITIALIZER;

//创建view和shadowView，本类只是管理类，不是具体view
- (UIView *)createViewWithTag:(NSNumber *)tag;
- (RCTShadowView *)createShadowViewWithTag:(NSNumber *)tag;

//设置view和shadowView的属性
- (void)setProps:(NSDictionary<NSString *, id> *)props forView:(id<RCTComponent>)view;
- (void)setProps:(NSDictionary<NSString *, id> *)props forShadowView:(RCTShadowView *)shadowView;

- (NSDictionary<NSString *, id> *)viewConfig;

- (RCTViewManagerUIBlock)uiBlockToAmendWithShadowViewRegistry:(NSDictionary<NSNumber *, RCTShadowView *> *)registry;

@end
