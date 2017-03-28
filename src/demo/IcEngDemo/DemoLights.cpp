//
//  DemoLights.cpp
//  DevEng
//
//  Created by Sherman Chen on 10/27/16.
//  Copyright (c) 2016 Simviu Technology Inc.
//  All rights reserved.
//  http://www.simviu.com/dev
//

#include "DemoScene.h"

using namespace std;
using namespace ctl;
using namespace Ic3d;
static const string K_sModel = "IcDemo/MixShapes/MixShapes.obj";
static const string K_sFile_dbgTex = "IcDemo/plane/dbgGrid.png";

//----------------------------------------------
//  DemoLights::onInit
//----------------------------------------------
void DemoLights::onInit()
{
    //--- Always call parent class onInit()
    IcScene::onInit();
    
    
    const auto& cfg = IcApp::getInstance()->m_cfg;
    string sPathRes = cfg.m_sPathRes;
   
    //--- Bottom plate
    {
        IcMeshData mshd;
   //   mshd.createPlaneXZ(TRect(-10,-10, 20,20));
        mshd.createGridXZ(TRect(-10,-10, 20,20), 16, 16);
        auto pModel = makeSp<IcModel>(mshd);
        string sFileTex = sPathRes + K_sFile_dbgTex;
        pModel->setTexture(makeSp<IcTexture>(sFileTex));
        auto pObj = makeSp<IcObject>(pModel);
        addObj(pObj);
    }
    
    //--- Load Object
    string sFile = sPathRes + K_sModel;
    auto pModel = ctl::makeSp<IcModel>(sFile);
    auto pObj   = ctl::makeSp<IcObject>(pModel);
    // Add this obj to Scene, will be rendered
    addObj(pObj);
    
    //---- Lights
    // IcScene has one default sun light
    auto pL0 = m_lights[0];
    auto& para0 = pL0->getPara();
    para0.m_mat.setColor(TColor(0.1,0.1,0.1,1), 1, 1);   // set very dark
    
    //---- Spot Light 1
    auto pL1 = makeSp<IcLight>(1);
    pL1->setLightColor(TColor(0.3,0.3,1,1)); // Blue Light
    pL1->setPos(TVec3(0, 10, 10));
    pL1->setQuat(TQuat(TVec3(deg2rad(-40), 0, 0)));
    pL1->setSpotPara(0.01, 20, 22);
    
    //---- Spot Light 2
    auto pL2 = makeSp<IcLight>(2);
    pL2->setLightColor(TColor(0.3,0.3,0,1)); // Yellow Light
    pL2->setPos(TVec3(0, 6, -1));
    pL2->setQuat(TQuat(TVec3(deg2rad(-140), 0, 0)));
    pL2->setSpotPara(0.01, 40, 45);
    
    addLight(pL1);
    addLight(pL2);

}
