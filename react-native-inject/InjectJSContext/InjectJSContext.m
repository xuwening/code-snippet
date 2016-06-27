//
//  InjectJSContext.m
//  RNDemo
//
//  Created by 洪敬军 on 16/6/27.
//  Copyright © 2016年 Facebook. All rights reserved.
//

#import "InjectJSContext.h"
#import "RCTJSCExecutor.h"
#import <UIKit/UIKit.h>


//接收来自JSContext创建成功的通知，这里是添加接口，实际上应使用扩展模块的方法添加新功能，而不是注入JSContext来实现
@implementation InjectJSContext


+(void)load {
  
  [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(recieveNotification:) name:RCTJavaScriptContextCreatedNotification object:nil];
}


+(void) recieveNotification: (NSNotification *) notification {
  
  JSContext *context = notification.object;
  __weak typeof(self) weakSelf = self;
  
  //改造点： context属于单独线程，这里不能在UI线程操作context
  //这里无法获取jscontext所在线程
  context[@"alert"] = ^(NSString *message) {
    
    dispatch_async(dispatch_get_main_queue(), ^{
      
      UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"" message:@"native alert" delegate:weakSelf cancelButtonTitle:@"cancel" otherButtonTitles:nil, nil];
      
      [alert show];
    });
    
  };
}


@end
