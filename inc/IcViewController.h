//
//  IcViewController.h
//  DevEng
//
//  Created by Sherman Chen on 3/2/16.
//  Copyright (c) 2016 Simviu Technology Inc.
//  All rights reserved.
//  http://www.simviu.com/dev
//
//

#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>

@interface IcViewController : GLKViewController
{
	
}
-(void)initIcApp:(void*)pAppIn;

//---- These can be override
-(void)Ic3d_onInitWindow;
-(void)Ic3d_onViewRect:(CGRect)viewRect;
-(void)Ic3d_onDrawUpdate:(double)deltaT;
@end
