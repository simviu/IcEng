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
    
    const auto& cfg = IcApp::getInstance()->m_cfg;
    string sPathRes = cfg.m_sPathRes;
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
        auto pTex = makeSp<IcTexture>(TSize(2048, 2048));
        pTex->setAsRenderTarget();
        m_pRenderTex = pTex;

        pScn->setRenderToTexture(pTex);
        pScn->m_cfg.m_enClrScrn = true;
        pScn->m_cfg.m_enAutoResize = false;
        
        addSubScn(pScn);
        m_pScn0 = pScn;
    }
    
    //---- Create Plane has texture
    if(1)
    {
        auto pScn = makeSp<IcScene>();
        IcMeshData mshd;
        
        //---- Create Plane as display quad.
        // Note: Texture is y-reversed, as standard of OpenGL.
        mshd.createPlaneXY(TRect(TPos(-1,1), TPos(1,-1)),
                           TRect(TPos(0,1), TPos(1,0)));
        auto pMsh = makeSp<IcMesh>(mshd);
        auto pMtl = makeSp<IcMaterial>();
        
   //   auto pTex = makeSp<IcTexture>(); auto pTex = m_pRenderTex;  // Debug
        auto pTex = m_pRenderTex;
        
        auto pModel = makeSp<IcModel>();
        pModel->setMshMtlTex(pMsh, pTex, pMtl);
        auto pObj = makeSp<IcObject>(pModel);
        pScn->addObj(pObj);
        
        //---- set cam
        auto pCam = pScn->getCamera();
        pCam->setPos(TVec3(1,4,6));
        pCam->lookAt(TVec3(0,0,0), TVec3(0,1,0));

        
        addSubScn(pScn);
        m_pScn1 = pScn;
    }
}

