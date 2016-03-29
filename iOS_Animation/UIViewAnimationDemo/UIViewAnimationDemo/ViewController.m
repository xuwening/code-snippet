//
//  ViewController.m
//  UIViewAnimationDemo
//
//  Created by aqua on 12/9/14.
//  Copyright (c) 2014 aqua. All rights reserved.
//

#import "ViewController.h"

@interface ViewController () {
    UIColor *_oldColor;
}
@property (weak, nonatomic) IBOutlet UIButton *rotateButton;
@property (weak, nonatomic) IBOutlet UIButton *moveButton;
@property (weak, nonatomic) IBOutlet UIButton *scaleButton;
@property (weak, nonatomic) IBOutlet UIButton *colorButton;
@property (weak, nonatomic) IBOutlet UIButton *alphaButton;


@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.

    [self rotationAction:self.rotateButton];
    [self moveAction:self.moveButton];
    [self scaleAction:self.scaleButton];
    [self changeColorAction:self.colorButton];
    [self alphaAction:self.alphaButton];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (IBAction)rotationAction:(id)sender {
    UIButton *btn = (UIButton *)sender;
    //旋转
    [UIView animateWithDuration:1.0 delay:0.0 options:UIViewAnimationOptionCurveLinear|UIViewAnimationOptionRepeat animations:^{
        btn.transform = CGAffineTransformMakeRotation(M_PI);
    } completion: nil];
}

- (IBAction)moveAction:(id)sender {
    UIButton *btn = sender;
    //移动
    [UIView animateWithDuration:1.0 delay:0.0 options:UIViewAnimationOptionCurveLinear|UIViewAnimationOptionAutoreverse|UIViewAnimationOptionRepeat animations:^{
        btn.transform = CGAffineTransformMakeTranslation(-100, 0);
    } completion:^(BOOL finished) {

    }];
}

- (IBAction)scaleAction:(id)sender {
    UIButton *btn = sender;
    //缩放
    [UIView animateWithDuration:1.0 delay:0.0 options:UIViewAnimationOptionCurveLinear|UIViewAnimationOptionAutoreverse|UIViewAnimationOptionRepeat animations:^{
        btn.transform = CGAffineTransformMakeScale(0.5, 0.5);
    } completion:^(BOOL finished) {

    }];
}

- (IBAction)changeColorAction:(id)sender {
    UIButton *btn = sender;
    _oldColor = btn.backgroundColor;
    
    //颜色渐变
    [UIView animateWithDuration:1.0 delay:0.0 options:UIViewAnimationOptionCurveLinear|UIViewAnimationOptionAutoreverse|UIViewAnimationOptionRepeat animations:^{
        btn.backgroundColor = [UIColor redColor];
    } completion:^(BOOL finished) {

    }];
}

- (IBAction)alphaAction:(id)sender {
    UIButton *btn = sender;
    
    //透明色渐变
    [UIView animateWithDuration:1.0 delay:0.0 options:UIViewAnimationOptionCurveLinear|UIViewAnimationOptionAutoreverse|UIViewAnimationOptionRepeat animations:^{
        btn.alpha = 0;
    } completion:^(BOOL finished) {

    }];
}

//- (IBAction)curlUpAction:(id)sender {
//    UIButton *btn = sender;
//    [UIView transitionWithView:btn duration:1.0 options:UIViewAnimationOptionTransitionCurlUp|UIViewAnimationOptionRepeat animations:^{
//    } completion:^(BOOL finished) {
//    }];
//}




@end
