/**
 * Copyright (c) 2015-present, Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */

#import <UIKit/UIKit.h>

#import "RCTBridgeDelegate.h"
#import "RCTBridgeModule.h"
#import "RCTDefines.h"
#import "RCTFrameUpdate.h"
#import "RCTInvalidating.h"
#import "RCTJavaScriptExecutor.h"

@class RCTBridge;
@class RCTEventDispatcher;

/**
 * This notification triggers a reload of all bridges currently running.
 */
//重新加载通知
RCT_EXTERN NSString *const RCTReloadNotification;

/**
 * This notification fires when the bridge starts loading the JS bundle.
 */
//加载main.js时通知
RCT_EXTERN NSString *const RCTJavaScriptWillStartLoadingNotification;

/**
 * This notification fires when the bridge has finished loading the JS bundle.
 */
//加载完毕main.js时通知
RCT_EXTERN NSString *const RCTJavaScriptDidLoadNotification;

/**
 * This notification fires when the bridge failed to load the JS bundle. The
 * `error` key can be used to determine the error that occured.
 */
//加载main.js失败时通知：key：error
RCT_EXTERN NSString *const RCTJavaScriptDidFailToLoadNotification;

/**
 * This notification fires each time a native module is instantiated. The
 * `module` key will contain a reference to the newly-created module instance.
 * Note that this notification may be fired before the module is available via
 * the `[bridge moduleForClass:]` method.
 */
//每当有native模块被实例化时通知：key：module
RCT_EXTERN NSString *const RCTDidInitializeModuleNotification;

/**
 * This block can be used to instantiate modules that require additional
 * init parameters, or additional configuration prior to being used.
 * The bridge will call this block to instatiate the modules, and will
 * be responsible for invalidating/releasing them when the bridge is destroyed.
 * For this reason, the block should always return new module instances, and
 * module instances should not be shared between bridges.
 */
typedef NSArray<id<RCTBridgeModule>> *(^RCTBridgeModuleProviderBlock)(void);

/**
 * This function returns the module name for a given class.
 */
//根据moduleClass返回class名称
RCT_EXTERN NSString *RCTBridgeModuleNameForClass(Class bridgeModuleClass);

/**
 * Async batched bridge used to communicate with the JavaScript application.
 */
//BatchedBridge是与javascript通信的桥梁，异步执行
@interface RCTBridge : NSObject <RCTInvalidating>

/**
 * Creates a new bridge with a custom RCTBridgeDelegate.
 *
 * All the interaction with the JavaScript context should be done using the bridge
 * instance of the RCTBridgeModules. Modules will be automatically instantiated
 * using the default contructor, but you can optionally pass in an array of
 * pre-initialized module instances if they require additional init parameters
 * or configuration.
 */
- (instancetype)initWithDelegate:(id<RCTBridgeDelegate>)delegate
                   launchOptions:(NSDictionary *)launchOptions NS_DESIGNATED_INITIALIZER;

/**
 * DEPRECATED: Use initWithDelegate:launchOptions: instead
 *
 * The designated initializer. This creates a new bridge on top of the specified
 * executor. The bridge should then be used for all subsequent communication
 * with the JavaScript code running in the executor. Modules will be automatically
 * instantiated using the default contructor, but you can optionally pass in an
 * array of pre-initialized module instances if they require additional init
 * parameters or configuration.
 */
//废弃
- (instancetype)initWithBundleURL:(NSURL *)bundleURL
                   moduleProvider:(RCTBridgeModuleProviderBlock)block
                    launchOptions:(NSDictionary *)launchOptions NS_DESIGNATED_INITIALIZER;

/**
 * This method is used to call functions in the JavaScript application context.
 * It is primarily intended for use by modules that require two-way communication
 * with the JavaScript code. Safe to call from any thread.
 */
//调用js模块方法，也可以看做是发送事件，线程安全
//一般用于和js代码的二步交互的模块
- (void)enqueueJSCall:(NSString *)moduleDotMethod args:(NSArray *)args;

/**
 * Retrieve a bridge module instance by name or class. Note that modules are
 * lazily instantiated, so calling these methods for the first time with a given
 * module name/class may cause the class to be sychronously instantiated,
 * potentially blocking both the calling thread and main thread for a short time.
 */

//访问BatchedBridge中的模块：通过模块名称或模块类
- (id)moduleForName:(NSString *)moduleName;
- (id)moduleForClass:(Class)moduleClass;

/**
 * Convenience method for retrieving all modules conforming to a given protocol.
 * Modules will be sychronously instantiated if they haven't already been,
 * potentially blocking both the calling thread and main thread for a short time.
 */
//检索所有模块，看哪些模块符合提供的协议，如果模块还未创建，则同步实例化该模块
- (NSArray *)modulesConformingToProtocol:(Protocol *)protocol;

/**
 * Test if a module has been initialized. Use this prior to calling
 * `moduleForClass:` or `moduleForName:` if you do not want to cause the module
 * to be instantiated if it hasn't been already.
 */
//校验该模块是否已经被初始化（调用moduleForClass、moduleForName前校验）
//这样能阻止一些模块还未准备好，就被提前实例化
- (BOOL)moduleIsInitialized:(Class)moduleClass;

/**
 * All registered bridge module classes.
 */
//返回所有已经被注册的模块
@property (nonatomic, copy, readonly) NSArray<Class> *moduleClasses;

/**
 * URL of the script that was loaded into the bridge.
 */
//加载的url
@property (nonatomic, strong, readonly) NSURL *bundleURL;

/**
 * The class of the executor currently being used. Changes to this value will
 * take effect after the bridge is reloaded.
 */
@property (nonatomic, strong) Class executorClass;

/**
 * The delegate provided during the bridge initialization
 */

//提供main.js路径的代理，只读，如何通过修改delegate重新加载新main.js？
@property (nonatomic, weak, readonly) id<RCTBridgeDelegate> delegate;

/**
 * The launch options that were used to initialize the bridge.
 */
//启动参数，用于传递给js
@property (nonatomic, copy, readonly) NSDictionary *launchOptions;

/**
 * Use this to check if the bridge is currently loading.
 */
//校验是否处于加载中
@property (nonatomic, readonly, getter=isLoading) BOOL loading;

/**
 * Use this to check if the bridge has been invalidated.
 */
//校验是否已经被不可用
@property (nonatomic, readonly, getter=isValid) BOOL valid;

/**
 * Reload the bundle and reset executor & modules. Safe to call from any thread.
 */
//重新加载js环境，线程安全
- (void)reload;

/**
 * Says whether bridge has started recieving calls from javascript.
 */
- (BOOL)isBatchActive;

@end
