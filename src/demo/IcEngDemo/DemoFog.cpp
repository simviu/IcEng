//
//  DemoFog.cpp
//  IcEngDemo
//
//  Created by Sherman Chen on 3/22/17.
//  Copyright (c) 2017 Simviu Technology Inc.
//  All rights reserved.
//  http://www.simviu.com/dev
//

#include "DemoScene.h"

using namespace std;
using namespace ctl;
using namespace Ic3d;
static const string K_sModel = "IcDemo/MixShapes/MixShapes.obj";
static const int K_objAry_N = 10;   // NxNxN of objs
static const int K_objAry_W = 10.0;  // Width of objs
static const int K_objAry_H = 20.0;  // Height of objs

//----------------------------------------------
//  onInit
//----------------------------------------------
void DemoFog::onInit()
{
    //--- Always call parent class onInit()
    IcScene::onInit();
    
    //--- Init Fog
    
    //--- Load Scene
    loadScene();
    
}
//----------------------------------------------
//  loadScene
//----------------------------------------------
void DemoFog::loadScene()
{
    //--- Load Object
    const auto& cfg = IcApp::getInstance()->m_cfg;
    string sPathRes = cfg.m_sPathRes;
    string sFile = sPathRes + K_sModel;
    auto pModel = ctl::makeSp<IcModel>(sFile);
    // Add array of objs to Scene
    int N = K_objAry_N;
    float w = K_objAry_W;
    float h = K_objAry_H;
    for(int y=0;y<N;y++)
        for(int x=0;x<N;x++)
            for(int z=0;z<N;z++)
            {
                auto pObj   = ctl::makeSp<IcObject>(pModel);
                TVec3 pos((x-N/2)*w, (y-N/2)*h, (z-N/2)*w);
                pObj->setPos(pos);
                addObj(pObj);
            }
 
}
