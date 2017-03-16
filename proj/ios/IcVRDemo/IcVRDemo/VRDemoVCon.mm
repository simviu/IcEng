//
//  VRDemoVCon.m
//  IcVRDemo
//
//  Created by Sherman Chen on 3/16/17.
//  Copyright © 2017 Simviu Technology Inc. All rights reserved.
//

#import "VRDemoVCon.h"
#include "Ic3d.h"
#include "IcViewController.h"
#include "VRDemoApp.hpp"

using namespace ctl;
using namespace std;
using namespace Ic3d;

@interface VRDemoVCon ()
{
    IcViewController* m_IcVCon;
    VRDemoApp m_app;
}

@end

@implementation VRDemoVCon

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
    
    //---- IcViewController
    m_IcVCon = [[IcViewController alloc] init];
    
    //---- Init with App Instance
    [m_IcVCon  initIcApp:&m_app];
    
    //---- Add IcView
    UIView* icvw = m_IcVCon.view;
    [self.view addSubview:icvw];
    icvw.frame = self.view.frame;
    

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

@end
