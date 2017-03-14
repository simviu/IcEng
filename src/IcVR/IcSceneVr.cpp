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
    void IcWindowVR::VRScnMain::setVRTex(ctl::Sp<IcTexture> pTexL,
                                         ctl::Sp<IcTexture> pTexR)
    {
        m_pTex[0] = pTexL;
        m_pTex[1] = pTexR;
    }
    void IcWindowVR::VRScnMain::onDraw()
    {
        //---- Draw L/R, onto different Tex
        for(int i=0;i<2;i++)
        {
            setRenderToTexture(m_pTex[i]);
            IcScene::onDraw();
        }
    }
    //--------------------------------------
    // VRScnDisp
    //--------------------------------------
    void IcWindowVR::VRScnDisp::onInit()
    {
        IcScene::onInit();
        const static float w = 1;
        const static float z = 1;
        //---- Create Distortion Mesh
        IcMeshData mshd; mshd.createPlaneXZ(TRect(-w/2,-w/2,w,w));
        m_pDistMesh = makeSp<IcMesh>(mshd);
        
        //---- Create display distortion plane
        for(int i=0;i<2;i++)
        {
            auto pObj = makeSp<IcObject>();
            pObj->setPos(TVec3(-w/2, 0, -z));
            m_pObjPlane[i] = pObj;
            addObj(pObj);
        }
    }
    void IcWindowVR::VRScnDisp::setVRTex(ctl::Sp<IcTexture> pTexL,
                                         ctl::Sp<IcTexture> pTexR)
    {
        for(int i=0;i<2;i++)
        {
            auto pTex = (i==0)? pTexL : pTexR;
            auto pModel = makeSp<IcModel>();
            pModel->setMesh(m_pDistMesh);
            pModel->setTexture(pTex);
            m_pObjPlane[i]->setModel(pModel);
        }
    }
  
}
