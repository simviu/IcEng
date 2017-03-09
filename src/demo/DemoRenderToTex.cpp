//
//  DemoRenderToTex.cpp
//  IcEngDemo
//
//  Created by Sherman Chen on 3/9/17.
//  Copyright (c) 2017 Simviu Technology Inc.
//  All rights reserved.
//  http://www.simviu.com/dev
//


#include "DemoScene.h"

using namespace std;
using namespace ctl;
using namespace Ic3d;
static const string K_sModel = "IcDemo/MixShapes/MixShapes.obj";
static const string K_sFile_dbgTex = "IcDemo/planets/tex_mars.png";

//----------------------------------------------
//  DemoBasic::onInit
//----------------------------------------------
void DemoRenderToTex::onInit()
{
    //--- Always call parent class onInit()
    IcScene::onInit();
    
    string sPathRes = DemoScene::m_cfg.m_sPathRes;
    //--- Load Objectif
    if(0)
    {
        string sFile = sPathRes + K_sModel;
        auto pModel = makeSp<IcModel>(sFile);
        auto pObj   = makeSp<IcObject>(pModel);
        // Add this obj to Scene, will be rendered
        addObj(pObj);
    }
    
    //---- Create Plane has texture
    if(1)
    {
        IcMeshData mshd;
        mshd.createPlaneXZ(TRect(0,0,2,2));
        auto pMsh = makeSp<IcMesh>(mshd);
        auto pMtl = makeSp<IcMaterial>();
        auto pTex = makeSp<IcTexture>();
        pTex->loadFile(sPathRes + K_sFile_dbgTex);
        auto pModel = makeSp<IcModel>();
        pModel->setMshMtlTex(pMsh, pTex, pMtl);
        addObj(makeSp<IcObject>(pModel));
    }
    
    
    
}
