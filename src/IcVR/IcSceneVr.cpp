//
//  IcSceneVR.cpp
//  IcEngLib
//
//  Created by Sherman Chen on 3/14/17.
//  Copyright (c) 2017 Simviu Technology Inc.
//  All rights reserved.
//  http://www.simviu.com/dev
//
//



#include "Ic3d.h"

namespace Ic3d
{
    using namespace ctl;
    using namespace std;
    //--------------------------------------
    // VRScnMain
    //--------------------------------------
    void IcWindowVR::VRScnMain::onInit()
    {
        IcScene::onInit();
    }
 
    void IcWindowVR::VRScnMain::onDraw()
    {
        if(m_pCntxt==nullptr) return;
        //---- Draw L/R, onto different Tex
        for(int i=0;i<2;i++)
        {
            auto pTex = m_pCntxt->getTex(i);
            setRenderToTexture(pTex);
            IcScene::onDraw();
        }
    }
    //--------------------------------------
    // VRScnDisp
    //--------------------------------------
    void IcWindowVR::VRScnDisp::onInit()
    {
        IcScene::onInit();
     }
    void IcWindowVR::VRScnDisp::onWindowSize(const ctl::TSize& winSize)
    {
        IcScene::onWindowSize(winSize);
        reInit();
    }
    //--------------------------------------
    //  VRScnDisp
    //--------------------------------------
    void IcWindowVR::VRScnDisp::reInit()
    {
        if(m_pCntxt==nullptr) return;
        clearObjs();
        const static float w = 1;
        const static float z = 1;
        //---- Create Distortion Mesh
        IcMeshData mshd; mshd.createPlaneXZ(TRect(-w/2,-w/2,w,w));
        m_pDistMesh = makeSp<IcMesh>(mshd);
        
        //---- Create display distortion plane
        for(int i=0;i<2;i++)
        {
            auto pTex = m_pCntxt->getTex(i);
            auto pModel = makeSp<IcModel>();
            pModel->setMesh(m_pDistMesh);
            pModel->setTexture(pTex);
            auto pObj = makeSp<IcObject>(pModel);
            pObj->setPos(TVec3(-w/2, 0, -z));
            m_pObjPlane[i]=pObj;
            addObj(pObj);
        }
        //---- Set Camera
        auto& cam = *getCamera();
        cam.setPos(TVec3(0,5,0));
        cam.lookAt(TVec3(0,0,0), TVec3(0,0,1));
        
    }
    
 
}
