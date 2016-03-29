//
//  ViewController.m
//  UIViewControllerTransition_demo
//
//  Created by aqua on 12/17/14.
//  Copyright (c) 2014 aqua. All rights reserved.
//

#import "ViewController.h"
#import "ModalAnimated.h"
#import "ModalViewController.h"

@interface ViewController () <UIViewControllerTransitioningDelegate> {
    
    ModalAnimated *_modalAnimated;
    
}

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    
    _modalAnimated = [[ModalAnimated alloc] init];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

#pragma mark - user action

-(IBAction)presentAction:(id)sender {
    ModalViewController *modal = [self.storyboard instantiateViewControllerWithIdentifier:@"ModalViewController"];
    modal.transitioningDelegate = self;
    modal.modalPresentationStyle = UIModalPresentationCustom;
    [self presentViewController:modal animated:YES completion:nil];
}


#pragma mark - modal view delegate
- (id <UIViewControllerAnimatedTransitioning>)animationControllerForPresentedController:(UIViewController *)presented presentingController:(UIViewController *)presenting sourceController:(UIViewController *)source {
    
    _modalAnimated.type = PresentAnimateType;
    return _modalAnimated;
}

- (id <UIViewControllerAnimatedTransitioning>)animationControllerForDismissedController:(UIViewController *)dismissed {
    _modalAnimated.type = DismissAnimationType;
    return _modalAnimated;
}



@end
