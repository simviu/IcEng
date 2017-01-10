//
//  ViewController.m
//  IcEngTestApp
//
//  Created by Sherman Chen on 12/2/16.
//  Copyright © 2016 Simviu Technology Inc. All rights reserved.
//

#import "ViewController.h"
#import "TestViewController.h"

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


- (IBAction)onButton_Test:(id)sender {
    TestViewController* vcon = [[TestViewController alloc] init];
    [self presentViewController:vcon animated:NO completion:nil];
}
@end
