//
//  CalendarManager.h
//  reactDemo
//
//  Created by hong on 2017/7/5.
//  Copyright © 2017年 Facebook. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <React/RCTBridgeModule.h>
#import <React/RCTLog.h>
#import <React/RCTEventEmitter.h>

//RCTEventEmitter用来给 js 发送事件
@interface CalendarManager : RCTEventEmitter <RCTBridgeModule>

@end
