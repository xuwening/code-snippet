//
//  ModalAnimated.m
//  UIViewControllerTransition_demo
//
//  Created by aqua on 12/17/14.
//  Copyright (c) 2014 aqua. All rights reserved.
//

#import "ModalAnimated.h"


@interface ModalAnimated ()

@end

@implementation ModalAnimated

- (NSTimeInterval)transitionDuration:(id <UIViewControllerContextTransitioning>)transitionContext {
    return 0.8;
}

- (void)animateTransition:(id <UIViewControllerContextTransitioning>)transitionContext {
    
    UIView *containerView = [transitionContext containerView];
    
    if (_type == PresentAnimateType) {
        UIView *modalView = [transitionContext viewControllerForKey:UITransitionContextToViewControllerKey].view;
        
        //先设置初始显示位置，屏幕上方
        modalView.frame = CGRectOffset(containerView.frame, 0, -containerView.frame.size.height);
        modalView.alpha = 0;
        
        [containerView addSubview:modalView];
        
        [UIView animateWithDuration:0.8 delay:0.0 usingSpringWithDamping:0.8 initialSpringVelocity:0.2 options:UIViewAnimationOptionCurveEaseInOut animations:^{
            
            modalView.frame = containerView.frame;
            modalView.alpha = 1;
        } completion:^(BOOL finished) {
            [transitionContext completeTransition:YES];
        }];
        
    } else if (_type == DismissAnimationType) {
        
        UIView *modalView = [transitionContext viewControllerForKey:UITransitionContextFromViewControllerKey].view;
        UIView *snapshot = [modalView snapshotViewAfterScreenUpdates:NO];
        snapshot.frame = modalView.frame;
        [containerView addSubview:snapshot];
        [containerView bringSubviewToFront:snapshot];
        [modalView removeFromSuperview];
        
        [UIView animateWithDuration:0.8 delay:0.0 usingSpringWithDamping:0.8 initialSpringVelocity:0.2 options:UIViewAnimationOptionCurveEaseInOut animations:^{
            
            //设置结束显示位置，屏幕上方
            snapshot.frame = CGRectOffset(containerView.frame, 0, -containerView.frame.size.height);
            snapshot.alpha = 0;
        } completion:^(BOOL finished) {
            [snapshot removeFromSuperview];
            [transitionContext completeTransition:YES];
        }];
    }
}

@end
