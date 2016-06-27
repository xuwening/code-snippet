//
//  RCTWebView+inject.m
//  RNDemo
//
//  Created by 洪敬军 on 16/6/27.
//  Copyright © 2016年 Facebook. All rights reserved.
//

#import "RCTWebView+inject.h"
#import "RCTUtils.h"
#import <JavascriptCore/JavaScriptCore.h>


//给webview注入native对象，供js调用
@implementation RCTWebView (inject)

+(void)load {
  
  RCTSwapInstanceMethods([RCTWebView class], @selector(initWithFrame:), @selector(newInitWithFrame:));
  RCTSwapInstanceMethods([RCTWebView class], @selector(webViewDidFinishLoad:), @selector(newWebViewDidFinishLoad:));
}

- (instancetype)newInitWithFrame:(CGRect)frame {
  RCTWebView *slf = [self newInitWithFrame: frame];
  
  UIWebView *webView = [slf valueForKey:@"_webView"];
  [self injectWebView: webView];
  return slf;
}

- (void)newWebViewDidFinishLoad:(UIWebView *)webView {
  
  [self injectWebView: webView];
  [self newWebViewDidFinishLoad: webView];
}

-(void) injectWebView: (UIWebView *) webView {
  
  JSContext *context = [webView valueForKeyPath:@"documentView.webView.mainFrame.javaScriptContext"];
  if (context == nil) {
    return;
  }
  
  //自定义注入对象
  __weak typeof(self) weakSelf = self;
  context[@"alert"] = ^(NSString *message) {
    
    dispatch_async(dispatch_get_main_queue(), ^{
      
      UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"" message:@"native alert" delegate:weakSelf cancelButtonTitle:@"cancel" otherButtonTitles:nil, nil];
      
      [alert show];
    });
    
  };
}

@end
