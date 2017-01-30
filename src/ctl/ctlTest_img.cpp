//
//  ctlTest_img.cpp
//  DevEng
//
//  Created by Sherman Chen on 2/24/16.
//  Copyright (c) 2016 Simviu Technology Inc.
//  All rights reserved.
//  http://www.simviu.com/dev
//
//



#include "ctl.h"
using namespace std;

const static string K_sPathTest = "/UTest/";
using namespace ctl;
//------------------------------------------
//	runTestImg
//------------------------------------------
bool UnitTest::runTestImg()
{
	string sPath ;
	string sPathDoc;
	IcImg img;
    bool isPass = true;
    isPass &= img.loadFile(sPath +"img1.png");
	isPass &= img.saveFile(sPathDoc +"img1_w.png");
	TSize sz = img.size();
	img.scaleTo(TSize(sz.w*1.2,sz.h*0.8));
	isPass &= img.saveFile(sPathDoc +"img1_w_s.png");
	return isPass;
}
