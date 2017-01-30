//
//  ViewController.m
//  DevEng
//
//  Created by Sherman Chen on 11/3/16.
//  Copyright (c) 2016 Simviu Technology Inc.
//  All rights reserved.
//  http://www.simviu.com/dev
//

#import "ViewController.h"
#import "IcDemoTableVCon.h"

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
}

-(void)viewWillTransitionToSize:(CGSize)size withTransitionCoordinator:(id<UIViewControllerTransitionCoordinator>)coordinator
{
    [super viewWillTransitionToSize:size withTransitionCoordinator:coordinator];
    NSLog(@"Device rotated.\n");
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


- (IBAction)onButton_Demo:(id)sender {
    IcDemoTableVCon* vcon = [[IcDemoTableVCon alloc] init];
    [self presentViewController:vcon animated:NO completion:nil];
}
@end
