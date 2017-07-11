//
//  CalendarManager.m
//  reactDemo
//
//  Created by hong on 2017/7/5.
//  Copyright © 2017年 Facebook. All rights reserved.
//

#import "CalendarManager.h"
#import <React/RCTConvert.h>

@implementation CalendarManager

RCT_EXPORT_MODULE();

//导出常量
- (NSDictionary *)constantsToExport {
  
  return @{ @"firstDayOfTheWeek": @"Monday" };
}

//RCT_EXPORT_METHOD(addEvent:(NSString *)name location:(NSString *)location date:(nonnull NSNumber *)secondsSinceUnixEpoch) {
//  
//  NSDate *date = [RCTConvert NSDate:secondsSinceUnixEpoch];
//}

RCT_EXPORT_METHOD(addEvent:(NSString *)name location:(NSString *)location)
{
  RCTLogInfo(@"Pretending to create an event %@ at %@", name, location);
}

- (NSArray<NSString *> *)supportedEvents
{
  return @[@"EventReminder"];
}

- (void)calendarEventReminderReceived:(NSNotification *)notification
{
  NSString *eventName = notification.userInfo[@"name"];
  [self sendEventWithName:@"EventReminder" body:@{@"name": eventName}];
}


@end
