//
//  IcDemoViewCon.m
//  IcEngDemo
//
//  Created by Sherman Chen on 2/15/17.
//  Copyright © 2017 Simviu Technology Inc. All rights reserved.
//

#import "IcDemoViewCon.h"
#import "IcViewController.h"
#include "DemoApp.hpp"
using namespace std;
@interface IcDemoViewCon ()
{
    IcViewController*    m_IcViewCon;
    DemoApp                 m_app;
}
@end

@implementation IcDemoViewCon
-(void*)getAppInstance
{
    return reinterpret_cast<void*>(&m_app);
}

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
    
    //---- Init IcViewController, and tell it IcApp instance.
    m_IcViewCon = [[IcViewController alloc] init];
    [m_IcViewCon initIcApp:&m_app];
    
    //---- Add IcViewController as subview,
    // send to bottom of view stack, so your other UI elements show.
    UIView* icView = m_IcViewCon.view;
    [self.view addSubview:m_IcViewCon.view];
    icView.frame = self.view.frame;
    [self.view sendSubviewToBack:icView];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

- (IBAction)onBtn_Close:(id)sender {
    [self dismissViewControllerAnimated:NO completion:nil];
}
@end
