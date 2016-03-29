//
//  ViewController.m
//  UIViewKeyframeAnimation_demo
//
//  Created by aqua on 12/16/14.
//  Copyright (c) 2014 aqua. All rights reserved.
//

#import "ViewController.h"

@interface ViewController ()

@property (weak, nonatomic) IBOutlet UIButton *ui_Button;

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

#pragma mark - user action
-(IBAction)buttonAction:(id)sender {
    
    [UIView animateKeyframesWithDuration:4.0 delay:0.0 options:UIViewKeyframeAnimationOptionCalculationModeLinear animations:^{
        
        //StartTime和RelativeDuration都是百分比时间
        [UIView addKeyframeWithRelativeStartTime:0.0 relativeDuration:0.25 animations:^{
            _ui_Button.center = CGPointMake(_ui_Button.center.x+100, _ui_Button.center.y);
        }];
        
        [UIView addKeyframeWithRelativeStartTime:0.25 relativeDuration:0.25 animations:^{
            _ui_Button.center = CGPointMake(_ui_Button.center.x, _ui_Button.center.y+100);
        }];
        
        [UIView addKeyframeWithRelativeStartTime:0.5 relativeDuration:0.25 animations:^{
            _ui_Button.center = CGPointMake(_ui_Button.center.x-100, _ui_Button.center.y);
        }];
        
        [UIView addKeyframeWithRelativeStartTime:0.75 relativeDuration:0.25 animations:^{
            _ui_Button.center = CGPointMake(_ui_Button.center.x, _ui_Button.center.y-100);
        }];
    } completion:^(BOOL finished) {
        
        [UIView animateWithDuration:0.3 animations:^{
            self.view.transform = CGAffineTransformMakeRotation(M_PI_4);
        }];
    }];
}

@end
