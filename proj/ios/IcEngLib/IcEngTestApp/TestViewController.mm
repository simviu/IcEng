//
//  TestViewController.mm
//  DevEng
//
//  Created by Sherman Chen on 12/2/16.
//  Copyright (c) 2016 Simviu Technology Inc.
//  All rights reserved.
//  http://www.simviu.com/dev
//
//

#import "TestViewController.h"
#include "MyTestApp.h"

using namespace std;
using namespace ctl;
using namespace Ic3d;


@interface TestViewController ()
{
    MyTestApp   m_testApp;
}
//---- These to be override
-(void)Ic3d_onInit;
-(void)Ic3d_onViewRect:(CGRect)viewRect;
-(void)Ic3d_onDrawUpdate:(double)deltaT;

@end

@implementation TestViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    [self initWithIcApp:&m_testApp];
    // Do any additional setup after loading the view from its nib.
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

-(void) Ic3d_onInit
{
    [super Ic3d_onInit];
}
-(void) Ic3d_onViewRect:(CGRect)viewRect
{
    [super Ic3d_onViewRect:viewRect];
}
-(void)Ic3d_onDrawUpdate:(double)deltaT;
{
    [super Ic3d_onDrawUpdate:deltaT];

}

@end
