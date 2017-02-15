//
//  IcDemoViewCon.m
//  IcEngDemo
//
//  Created by Sherman Chen on 2/15/17.
//  Copyright © 2017 Simviu Technology Inc. All rights reserved.
//

#import "IcDemoViewCon.h"
#import "IcViewController.h"

@interface IcDemoViewCon ()
{
    IcViewController*    m_IcViewCon;
}
@end

@implementation IcDemoViewCon
-(void)initWithApp:(void*)pApp
{
    m_IcViewCon = [[IcViewController alloc] init];
   [m_IcViewCon setIcAppInstance:pApp];
}


- (void)viewDidLoad {
    [super viewDidLoad];
    UIView* icView = m_IcViewCon.view;
    [self.view addSubview:m_IcViewCon.view];
    icView.frame = self.view.frame;
    [self.view sendSubviewToBack:icView];
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

- (IBAction)onBtn_Close:(id)sender {
    [self dismissViewControllerAnimated:NO completion:nil];
}
@end
