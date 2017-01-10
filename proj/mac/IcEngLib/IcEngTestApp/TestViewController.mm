//
//  VConIc3dTestScn.m
//  IcEngLib
//
//  Created by Sherman Chen on 12/2/16.
//  Copyright © 2016 Simviu Technology Inc. All rights reserved.
//

#import "TestViewController.h"
#include "Ic3d.h"
#include "IcTestWindow.h"

using namespace std;
using namespace ctl;
using namespace Ic3d;


@interface TestViewController ()
{
    IcTestWindow    m_testWin;
}
//---- These to be override
-(void)Ic3d_onInit;
-(void)Ic3d_onViewRect:(CGRect)viewRect;
-(void)Ic3d_onDrawUpdate:(double)deltaT;

@end

@implementation TestViewController

- (void)viewDidLoad {
    [super viewDidLoad];
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
    m_testWin.onInit();
}
-(void) Ic3d_onViewRect:(CGRect)viewRect
{
    [super Ic3d_onViewRect:viewRect];
    float w = viewRect.size.width;
    float h = viewRect.size.height;
    m_testWin.onScreenSize(ctl::TSize(w, h));
}
-(void)Ic3d_onDrawUpdate:(double)deltaT;
{
    [super Ic3d_onDrawUpdate:deltaT];
    m_testWin.onDrawUpdate(deltaT);

}

@end
