//
//  ViewController.m
//  ViewControllerAnimationDemo
//
//  Created by aqua on 12/10/14.
//  Copyright (c) 2014 aqua. All rights reserved.
//

#import "ViewController.h"
#import "PresentViewcontroller.h"

@interface ViewController () {
    UIModalPresentationStyle _presentStyle;
    UIModalTransitionStyle _transitionStyle;
}

@property (weak, nonatomic) IBOutlet UISegmentedControl *ui_segment;

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

- (IBAction)segmentAction:(id)sender {
    _presentStyle = self.ui_segment.selectedSegmentIndex;
}

-(void)presentAction:(id)sender {
    
    PresentViewcontroller *present = [self.storyboard instantiateViewControllerWithIdentifier:@"PresentViewcontroller"];
    present.modalPresentationStyle = _presentStyle;
    present.modalTransitionStyle = _transitionStyle;
    
    [self presentViewController:present animated:YES completion:^{
        
    }];
}

- (IBAction)coverVerticalAction:(id)sender {
    _transitionStyle = UIModalTransitionStyleCoverVertical;
    [self presentAction: sender];
}
- (IBAction)flipHorizontalAction:(id)sender {
    _transitionStyle = UIModalTransitionStyleFlipHorizontal;
    [self presentAction: sender];
}
- (IBAction)crossDissolveAction:(id)sender {
    _transitionStyle = UIModalTransitionStyleCrossDissolve;
    [self presentAction: sender];
}
- (IBAction)partialCurlAction:(id)sender {
    _transitionStyle = UIModalTransitionStylePartialCurl;
    [self presentAction: sender];
}


@end
