//
//  VConIc3d.h
//  DevEng
//
//  Created by Sherman Chen on 3/2/16.
//  Copyright (c) 2016 Sherman Chen. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>

@interface IcViewController : GLKViewController
{
	
}

//---- These to be override
-(void)Ic3d_onInit;
-(void)Ic3d_onViewRect:(CGRect)viewRect;
-(void)Ic3d_onDrawUpdate:(double)deltaT;


@end
