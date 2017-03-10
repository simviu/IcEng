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
//  DemoRenderToTex::onDraw
//----------------------------------------------
void DemoRenderToTex::onDraw()
{
    DemoScene::onDraw();
    // for debug
}
//----------------------------------------------
//  DemoRenderToTex::onInit
//----------------------------------------------
void DemoRenderToTex::onUpdate(double deltaT)
{
    DemoScene::onUpdate(deltaT);
    
    if(m_pScn0!=nullptr)
        m_camMng.onUpdate(deltaT, *m_pScn0->getCamera());
 
}

//----------------------------------------------
//  DemoRenderToTex::onInit
//----------------------------------------------
void DemoRenderToTex::onInit()
{
    //--- Always call parent class onInit()
    IcScene::onInit();
    
    string sPathRes = DemoScene::m_cfg.m_sPathRes;
    //--- Load Objectif
    if(1)
    {
        auto pScn = makeSp<IcScene>();
        
        string sFile = sPathRes + K_sModel;
        auto pModel = makeSp<IcModel>(sFile);
        auto pObj   = makeSp<IcObject>(pModel);
        // Add this obj to Scene, will be rendered
        pScn->addObj(pObj);
        
        //---- Set to render Texture
        auto pTex = makeSp<IcTexture>();
        pScn->setRenderToTexture(pTex);
        m_pRenderTex = pTex;
        addSubScn(pScn);
        pScn->m_cfg.m_enClrScrn = true;
        m_pScn0 = pScn;
    }
    
    //---- Create Plane has texture
    if(1)
    {
        auto pScn = makeSp<IcScene>();
        IcMeshData mshd;
        mshd.createPlaneXZ(TRect(0,0,2,2));
        auto pMsh = makeSp<IcMesh>(mshd);
        auto pMtl = makeSp<IcMaterial>();
        
   //   auto pTex = makeSp<IcTexture>(); auto pTex = m_pRenderTex;  // Debug
        auto pTex = m_pRenderTex;
        
        auto pModel = makeSp<IcModel>();
        pModel->setMshMtlTex(pMsh, pTex, pMtl);
        auto pObj = makeSp<IcObject>(pModel);
        pScn->addObj(pObj);
        addSubScn(pScn);
        m_pScn1 = pScn;
    }
}

