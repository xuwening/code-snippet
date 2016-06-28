/**
 * Copyright (c) 2015-present, Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */

#import <objc/runtime.h>

#import <JavaScriptCore/JavaScriptCore.h>

#import "RCTBridgeModule.h"
#import "RCTInvalidating.h"

typedef void (^RCTJavaScriptCompleteBlock)(NSError *error);
typedef void (^RCTJavaScriptCallback)(id json, NSError *error);

/**
 * Abstracts away a JavaScript execution context - we may be running code in a
 * web view (for debugging purposes), or may be running code in a `JSContext`.
 */

//抽象出该协议，不区分是webview还是jscontext（RCTJSCExecutor、RCTWebSocketExecutor是协议的实现者）
@protocol RCTJavaScriptExecutor <RCTInvalidating, RCTBridgeModule>

/**
 * Used to set up the executor after the bridge has been fully initialized.
 * Do any expensive setup in this method instead of `-init`.
 */

//bridge初始化完毕后调用
- (void)setUp;

/**
 * Whether the executor has been invalidated
 */
@property (nonatomic, readonly, getter=isValid) BOOL valid;

/**
 * Executes BatchedBridge.flushedQueue on JS thread and calls the given callback
 * with JSValue, containing the next queue, and JSContext.
 */
- (void)flushedQueue:(RCTJavaScriptCallback)onComplete;

/**
 * Executes BatchedBridge.callFunctionReturnFlushedQueue with the module name,
 * method name and optional additional arguments on the JS thread and calls the
 * given callback with JSValue, containing the next queue, and JSContext.
 */
- (void)callFunctionOnModule:(NSString *)module
                      method:(NSString *)method
                   arguments:(NSArray *)args
                    callback:(RCTJavaScriptCallback)onComplete;

/**
 * Executes BatchedBridge.invokeCallbackAndReturnFlushedQueue with the cbID,
 * and optional additional arguments on the JS thread and calls the
 * given callback with JSValue, containing the next queue, and JSContext.
 */
- (void)invokeCallbackID:(NSNumber *)cbID
               arguments:(NSArray *)args
                callback:(RCTJavaScriptCallback)onComplete;

/**
 * Runs an application script, and notifies of the script load being complete via `onComplete`.
 */

//加载执行main.jsbundle
- (void)executeApplicationScript:(NSData *)script
                       sourceURL:(NSURL *)sourceURL
                      onComplete:(RCTJavaScriptCompleteBlock)onComplete;

//将json字符串转换为js对象注入到jscontext中
- (void)injectJSONText:(NSString *)script
   asGlobalObjectNamed:(NSString *)objectName
              callback:(RCTJavaScriptCompleteBlock)onComplete;

/**
 * Enqueue a block to run in the executors JS thread. Fallback to `dispatch_async`
 * on the main queue if the executor doesn't own a thread.
 */
//js thread上执行block，如果是webview，则在主线程上执行block
- (void)executeBlockOnJavaScriptQueue:(dispatch_block_t)block;

@optional

/**
 * Special case for Timers + ContextExecutor - instead of the default
 *   if jsthread then call else dispatch call on jsthread
 * ensure the call is made async on the jsthread
 */
//在js线程上执行block
- (void)executeAsyncBlockOnJavaScriptQueue:(dispatch_block_t)block;

/**
 * For executors that support it, this method can be used to add a synchronous
 * callback function for communicating with the javascript context.
 */
//给jscontext中注入接口（在js线程上执行，所以此方法是线程安全的，开发者可以调用此接口添加自定义js接口）
- (void)addSynchronousHookWithName:(NSString *)name usingBlock:(id)block;

@end
