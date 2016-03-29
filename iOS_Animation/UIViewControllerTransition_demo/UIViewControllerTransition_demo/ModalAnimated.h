//
//  ModalAnimated.h
//  UIViewControllerTransition_demo
//
//  Created by aqua on 12/17/14.
//  Copyright (c) 2014 aqua. All rights reserved.
//

#import <UIKit/UIKit.h>

typedef enum {
    PresentAnimateType,
    DismissAnimationType
}AnimateType;

@interface ModalAnimated : NSObject <UIViewControllerAnimatedTransitioning>

@property (assign, nonatomic) AnimateType type;

@end
