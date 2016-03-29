//
//  ViewController.m
//  CATransitionDemo
//
//  Created by aqua on 12/9/14.
//  Copyright (c) 2014 aqua. All rights reserved.
//

#import "ViewController.h"

@interface ViewController ()

@property (weak, nonatomic) IBOutlet UIButton *rotateButton;

@property (weak, nonatomic) IBOutlet UIButton *moveButton;
@property (weak, nonatomic) IBOutlet UIButton *scaleButton;
@property (weak, nonatomic) IBOutlet UIButton *colorButton;
@property (weak, nonatomic) IBOutlet UIButton *alphaButton;

@property (weak, nonatomic) IBOutlet UIButton *yRotateButton;
@property (weak, nonatomic) IBOutlet UIButton *xRotateButton;
@property (weak, nonatomic) IBOutlet UIButton *curlUpButton;

@property (weak, nonatomic) IBOutlet UIButton *rippleBUtton;
@property (weak, nonatomic) IBOutlet UIButton *suckButton;
@property (weak, nonatomic) IBOutlet UIButton *fadeButton;
@property (weak, nonatomic) IBOutlet UIButton *revealButton;

@property (weak, nonatomic) IBOutlet UIButton *moveInButton;

@property (weak, nonatomic) IBOutlet UIButton *cubeButton;
@property (weak, nonatomic) IBOutlet UIButton *oglFlipButton;

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    
    [self transBasicWithView: self.rotateButton keyPath:@"transform.rotation.z"];
    [self transBasicWithView: self.yRotateButton keyPath:@"transform.rotation.y"];
    [self transBasicWithView: self.xRotateButton keyPath:@"transform.rotation.x"];
    [self moveAction: self.moveButton];
    [self scaleAction: self.scaleButton];
    [self colorAction: self.colorButton];
    [self alphaAction: self.alphaButton];
    
    //@"cube" @"moveIn" @"reveal" @"fade"(default) @"pageCurl" @"pageUnCurl" @"suckEffect" @"rippleEffect" @"≈"
//    [self transWithName:@"pageCurl" view: self.curlUpButton];
//    [self transWithName:@"rippleEffect" view: self.rippleBUtton];
//    [self transWithName:@"suckEffect" view: self.suckButton];
//    [self transWithName:@"cube" view: self.cubeButton];
    [self transWithName:@"moveIn" view: self.moveInButton];
    [self transWithName:@"reveal" view: self.revealButton];
//    [self transWithName:@"fade" view: self.fadeButton];
    [self transWithName:@"push" view: self.oglFlipButton];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


#pragma mark - basic animation
- (IBAction)rotateAction:(id)sender {
}

-(void) transBasicWithView: (UIView *)view keyPath: (NSString *)keyPath {
    
    CABasicAnimation *animation = [CABasicAnimation animationWithKeyPath:keyPath];
    animation.toValue = [NSNumber numberWithFloat:2*M_PI];
    animation.duration = 1.0;
    animation.repeatCount = MAXFLOAT;
    animation.timingFunction = [CAMediaTimingFunction functionWithName:kCAMediaTimingFunctionLinear];
    [view.layer addAnimation:animation forKey:keyPath];
}

- (IBAction)moveAction:(id)sender {
    UIView *view = sender;
    
    CABasicAnimation *animation = [CABasicAnimation animationWithKeyPath:@"transform.translation.x"];
    animation.toValue = @(-100);
    animation.duration = 1.0;
    animation.repeatCount = MAXFLOAT;
    animation.autoreverses = YES;
    animation.timingFunction = [CAMediaTimingFunction functionWithName:kCAMediaTimingFunctionLinear];
    [view.layer addAnimation:animation forKey:@"transform.translation.x"];
}

- (IBAction)scaleAction:(id)sender {
    UIView *view = sender;
    
    CABasicAnimation *animation = [CABasicAnimation animationWithKeyPath:@"transform.scale.x"];
    animation.toValue = @(0.5);
    animation.duration = 1.0;
    animation.repeatCount = MAXFLOAT;
    animation.autoreverses = YES;
    animation.timingFunction = [CAMediaTimingFunction functionWithName:kCAMediaTimingFunctionLinear];
    [view.layer addAnimation:animation forKey:@"transform.scale.x"];

    CABasicAnimation *animationy = [CABasicAnimation animationWithKeyPath:@"transform.scale.y"];
    animationy.toValue = @(0.5);
    animationy.duration = 1.0;
    animationy.repeatCount = MAXFLOAT;
    animationy.autoreverses = YES;
    animationy.timingFunction = [CAMediaTimingFunction functionWithName:kCAMediaTimingFunctionLinear];
    [view.layer addAnimation:animationy forKey:@"transform.scale.y"];
}

- (IBAction)colorAction:(id)sender {
    UIView *view = sender;
    CABasicAnimation *animation = [CABasicAnimation animationWithKeyPath:@"backgroundColor"];
    animation.toValue = (id)[UIColor blueColor].CGColor;
    animation.duration = 1.0;
    animation.repeatCount = MAXFLOAT;
    animation.autoreverses = YES;
    animation.timingFunction = [CAMediaTimingFunction functionWithName:kCAMediaTimingFunctionLinear];
    [view.layer addAnimation:animation forKey:@"opacity"];
    
}

- (IBAction)alphaAction:(id)sender {
    UIView *view = sender;
    CABasicAnimation *animation = [CABasicAnimation animationWithKeyPath:@"opacity"];
    animation.toValue = @(0.0);
    animation.duration = 1.0;
    animation.repeatCount = MAXFLOAT;
    animation.autoreverses = YES;
    animation.timingFunction = [CAMediaTimingFunction functionWithName:kCAMediaTimingFunctionLinear];
    [view.layer addAnimation:animation forKey:@"opacity"];
}

#pragma mark - transition animation

//@"cube" @"moveIn" @"reveal" @"fade"(default) @"pageCurl" @"pageUnCurl" @"suckEffect" @"rippleEffect" @"≈"
- (IBAction)curlUpButton:(id)sender {
    UIView *view = sender;
    
    CATransition *trans = [CATransition animation];
    trans.delegate = self;
    trans.type = @"pageCurl";
//    trans.subtype = kCATransitionFromLeft;
    trans.duration = 1.0;
    trans.repeatCount = MAXFLOAT;
    view.layer.backgroundColor = [UIColor yellowColor].CGColor;
    [view.layer addAnimation:trans forKey:@"reveal"];
}

-(void) transWithName: (NSString *)name view: (UIView *) view {
    
    CATransition *trans = [CATransition animation];
    trans.delegate = self;
    trans.type = name;
    trans.subtype = kCATransitionFromLeft;
    trans.duration = 1.0;
    trans.repeatCount = MAXFLOAT;
    [view.layer addAnimation:trans forKey:name];
}

@end
