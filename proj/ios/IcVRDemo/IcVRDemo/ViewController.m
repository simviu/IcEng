//
//  ViewController.m
//  IcVRDemo
//
//  Created by Sherman Chen on 3/16/17.
//  Copyright © 2017 Simviu Technology Inc. All rights reserved.
//

#import "ViewController.h"
#import "VRDemoVCon.h"

@interface ViewController ()

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


- (IBAction)onBtn_VRDemo:(id)sender {
    VRDemoVCon* vcon = [[VRDemoVCon alloc] init];
    [self presentViewController:vcon animated:NO completion:nil];
}
@end
