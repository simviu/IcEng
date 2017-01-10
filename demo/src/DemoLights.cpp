//
//  DemoLights.cpp
//  DevEng
//
//  Created by Sherman Chen on 10/27/16.
//  Copyright © 2016 Sherman Chen. All rights reserved.
//

#include "DemoLights.hpp"

using namespace std;
using namespace ctl;
using namespace Ic3d;

//----------------------------------------------
//  DemoLights::onInit
//----------------------------------------------
void DemoLights::onInit()
{
    //--- Always call parent class onInit()
    IcScene::onInit();
    
    //--- Bottom plate
    {
        IcMeshData mshd;
        mshd.createPlaneXZ(TRect(-10,-10, 20,20), TRect(0,0,1,1));
        auto pModel = makeSp<IcModel>(mshd);
        auto pObj = makeSp<IcObject>(pModel);
        addObj(pObj);
    }
    
    //--- 3 Cubes
    struct TObjCfg{ TVec3 pos; TVec3 size; TVec3 angle; TColor color; };
    vector<TObjCfg> objCfgs = {
        { TVec3(0,0,0), TVec3(1,1,1),   TVec3(0,0,0),   TColor(1,0,0,1) },   // Red
        { TVec3(4,0,1), TVec3(1,1,0.5), TVec3(0,20,0),  TColor(0,1,0,1) },   // Green
        { TVec3(0,1,-2),TVec3(2,2,1),   TVec3(5,0,10),  TColor(0,0,1,1) },    // Blue
    //    { TVec3(0,0,0), TVec3(10,10,0.1),   TVec3(90,0,0),  TColor(0.5,0.5,0.5,1) }    // Gray plate
    };
    for(const auto& cfg : objCfgs)
    {
        auto pModel = IcModel::createCube(cfg.size, TVec3(0,0,0), cfg.color);
        auto pObj   = ctl::makeSp<IcObject>(pModel);
        const auto& a = cfg.angle;
        pObj->setQuat(TQuat(TVec3(deg2rad(a.x), deg2rad(a.y), deg2rad(a.z))));
        pObj->setPos(cfg.pos);
        addObj(pObj);
    }
    
    //---- Lights
    // IcScene has one default sun light
    auto pL0 = m_lights[0];
    auto& para0 = pL0->getPara();
    para0.m_mat.setColor(TColor(0,0,0,1), 1, 1);   // dark
    
    //---- Spot Light 1
    auto pL1 = makeSp<IcLight>(1);
    pL1->setLightColor(TColor(0.5,0.5,1,1)); // Blue Light
    pL1->setPos(TVec3(0, 10, 10));
    pL1->setQuat(TQuat(TVec3(deg2rad(-40), 0, 0)));
    pL1->setSpotPara(0.01, 20, 22);
    
    //---- Spot Light 2
    auto pL2 = makeSp<IcLight>(2);
    pL2->setLightColor(TColor(0.5,1,0.5,1)); // Green Light
    pL2->setPos(TVec3(2, 6, -3));
    pL2->setQuat(TQuat(TVec3(deg2rad(-140), 0, 0)));
    pL2->setSpotPara(0.01, 40, 45);
    
    addLight(pL1);
    addLight(pL2);
    //---- Set Camera
    auto& cam = *getCamera();
    
    // Put Camera somewhere apart.
    cam.setPos(TVec3(4, 10, 20));
    
    // Set camera look at obj, up vector is +y.
    // Our obj is at (0,0,0) position by default.
    cam.lookAt(TVec3(0,0,0), TVec3(0,1,0));
}
