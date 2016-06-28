/**
 * Copyright (c) 2015-present, Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */

#import <JavaScriptCore/JavaScriptCore.h>

#import "RCTJavaScriptExecutor.h"

/**
 * Default name for the JS thread
 */
RCT_EXTERN NSString *const RCTJSCThreadName;

/**
 * This notification fires on the JS thread immediately after a `JSContext`
 * is fully initialized, but before the JS bundle has been loaded. The object
 * of this notification is the `JSContext`. Native modules should listen for
 * notification only if they need to install custom functionality into the
 * context. Note that this notification won't fire when debugging in Chrome.
 */

//本通知在创建jscontext后，在加载js bundle之前发送，用于native开发订制自己的js接口
//注意：在调试模式下，本通知不发送，因为调试模式下使用chrome浏览器自带context，没有单独创建一说
RCT_EXTERN NSString *const RCTJavaScriptContextCreatedNotification;

/**
 * Create a NSError from a JSError object.
 *
 * If available, the error's userInfo property will contain the JS stacktrace under
 * the RCTJSStackTraceKey key.
 */
//用于处理js error，通常是堆栈信息，native调用js时发生的错误信息
RCT_EXTERN NSError *RCTNSErrorFromJSError(JSContextRef context, JSValueRef jsError);

/**
 * Uses a JavaScriptCore context as the execution engine.
 */

//executor是RCTJavaScriptExecutor协议的实现者，主要功能都在协议中制订
//executor是独立线程，runloop实现
@interface RCTJSCExecutor : NSObject <RCTJavaScriptExecutor>

@end
