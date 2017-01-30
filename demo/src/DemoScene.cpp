//
//  DemoScene.cpp
//  IcEngDemo
//
//  Created by Sherman Chen on 12/6/16.
//  Copyright (c) 2016 Simviu Technology Inc.
//  All rights reserved.
//  http://www.simviu.com/dev
//

#include "DemoScene.h"
#include "DemoBasic.hpp"
#include "DemoTransform.hpp"
#include "DemoNestedTrans.hpp"
#include "DemoAnimation.hpp"
#include "DemoModels.h"
#include "DemoModelCreate.hpp"
#include "DemoLights.hpp"

using namespace std;
using namespace ctl;
using namespace Ic3d;

DemoScene::TCfg DemoScene::m_cfg;

//------------------------------------
//  DemeScnMng
//------------------------------------
DemeScnMng::DemeScnMng()
{
    auto pItem = ctl::makeSp<TItem>();
    
}
