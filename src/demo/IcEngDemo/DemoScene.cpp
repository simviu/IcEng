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

using namespace std;
using namespace ctl;
using namespace Ic3d;

const static float K_camRotSpd = 1;
const static float K_camDistH = 20;
const static float K_camHeight = 8;


//-----------------------------------
//  Demo list
//-----------------------------------
static std::vector<DemoScene::DemoItem> l_demoAry {
    {"Basic"},              // 0
    {"Nested Transform"},   // 1
    {"Model Create"},       // 2
    {"Lights"},             // 3
    {"Fog"},                // 4
    {"Render To Texture"},  // 5
};
size_t DemoScene::getDemoNum()
{
    return l_demoAry.size();
};
const DemoScene::DemoItem& DemoScene::getDemoItem(int idx)
{
    return l_demoAry[idx];
};
//-----------------------------------
//  createDemoScn
//-----------------------------------
ctl::Sp<IcScene> DemoScene::createDemoScn(int sel)
{
    logInfo("DemoScene::createDemoScn() [" +
        v2s(sel) + "]");
    ctl::Sp<IcScene> pScn = nullptr;
    switch (sel) {
        case 0: pScn = ctl::makeSp<DemoBasic>();        break;
        case 1: pScn = ctl::makeSp<DemoNestedTrans>();  break;
        case 2: pScn = ctl::makeSp<DemoModelCreate>();  break;
        case 3: pScn = ctl::makeSp<DemoLights>();       break;
        case 4: pScn = ctl::makeSp<DemoFog>();          break;
        case 5: pScn = ctl::makeSp<DemoRenderToTex>();  break;
        default:
            break;
    }
    return pScn;
}


//------------------------------------
//  DemoScene
//------------------------------------
void DemoScene::onUpdate(double deltaT)
{
    IcScene::onUpdate(deltaT);
    m_camMng.onUpdate(deltaT, *getCamera());
}

//------------------------------------
//  DemoScene
//------------------------------------
void DemoScene::CCamMng::onUpdate(double deltaT, IcCamera& cam)
{
    float dAng = K_camRotSpd * deltaT;
    m_angle += dAng;
    if(m_angle > 2*M_PI)
        m_angle -= 2*M_PI;
    
    const float r = K_camDistH;
    const float h = K_camHeight;
    TVec3 pos( r*cos(m_angle), h, r*sin(m_angle));
    cam.setPos(pos);
    //---- Look at origin, Up vector is +Y
    cam.lookAt(TVec3(0,0,0), TVec3(0,1,0));
}
