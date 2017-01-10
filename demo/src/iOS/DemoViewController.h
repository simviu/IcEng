//
//  VCon_IcDemo.h
//  DevEng
//
//  Created by Sherman Chen on 3/3/16.
//  Copyright (c) 2016 Sherman Chen. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "IcViewController.h"

@interface DemoViewController : IcViewController{
}

@property (nonatomic, assign) int m_demoSel;
-(NSArray*) getDemoList;
-(void)createApp;
@end
