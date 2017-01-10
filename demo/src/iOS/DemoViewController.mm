//
//  VCon_IcDemo.m
//  DevEng
//
//  Created by Sherman Chen on 3/3/16.
//  Copyright (c) 2016 Sherman Chen. All rights reserved.
//

#import "DemoViewController.h"
#include "DemoWindow.hpp"


@interface DemoViewController ()
{
    ctl::Sp<DemoWindow> m_pWin;
}
@end

@implementation DemoViewController
-(void)createApp
{
    //---- Init Res dir
    NSString* ns = [[NSBundle mainBundle] resourcePath];
    std::string sPathRes = std::string([ns UTF8String]) +"/";

    m_pWin = ctl::makeSp<DemoWindow>(sPathRes);
  
}

- (void)viewDidLoad {
    [super viewDidLoad];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

-(void)viewDidAppear:(BOOL)animated
{
	[super viewDidAppear:animated];

}
-(NSArray*) getDemoList
{
    if(m_pWin==nullptr)
        return [[NSArray alloc] init];
    
    NSMutableArray* ary = [[NSMutableArray alloc] init];
    size_t N = m_pWin->getDemoNum();
    for(int i=0;i<N;i++)
    {
        const auto& item = m_pWin->getDemoItem(i);
        NSString* ns = [NSString stringWithUTF8String:item.m_sTitle.c_str()];
        [ary addObject:ns];
    }
    return ary;
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
    m_pWin->setDemoSel(self.m_demoSel);
    m_pWin->onInit();

}
-(void) Ic3d_onViewRect:(CGRect)viewRect
{
    [super Ic3d_onViewRect:viewRect];
    
    if(m_pWin==nullptr) return;

    float w = viewRect.size.width;
    float h = viewRect.size.height;
    m_pWin->onScreenSize(ctl::TSize(w, h));
    
  
}
-(void)Ic3d_onDrawUpdate:(double)deltaT;
{
    [super Ic3d_onDrawUpdate:deltaT];
    
    if(m_pWin==nullptr) return;
    m_pWin->onDrawUpdate(deltaT);
   
}
@end
