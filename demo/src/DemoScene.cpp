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

DemoScene::TCfg DemoScene::m_cfg;
const static float K_camRotSpd = 1;
const static float K_camDistH = 20;
const static float K_camHeight = 8;
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
