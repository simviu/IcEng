//
//  ViewController.m
//  DevEng
//
//  Created by Sherman Chen on 12/2/16.
//  Copyright © 2016 Simviu Technology Inc. All rights reserved.
//

#import "ViewController.h"
#import "IcViewController.h"
#import "MyTestApp.h"
@interface ViewController ()
{
    MyTestApp   m_myApp;
}
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
    IcViewController* vcon = [[IcViewController alloc] init];
    [vcon initIcApp:&m_myApp];
    [self presentViewController:vcon animated:NO completion:nil];
}
@end
