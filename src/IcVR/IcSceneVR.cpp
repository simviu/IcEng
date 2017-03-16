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
    const static float K_dispQuad_w = 1;
    const static float K_dispCamHeight = 2; // Temp, TODO: change to ortho cam view
    const static float K_dispMeshGridN = 100;   // 100x100
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
        const auto& rCfg = m_pCntxt->m_rCfg;
        //---- Draw L/R, onto different Tex
        auto& cam = *getCamera();
        const TVec3 camPosOri = cam.getPos();
        const TQuat camRot = cam.getQuat();
        for(int i=0;i<2;i++)
        {
            //---- Update Tex
            auto pTex = m_pCntxt->getTex(i);
            setRenderToTexture(pTex);
            
            //---- Shift cam pos L/R a little
            float w = rCfg.K_eyeDist;
            float dv = (i==0)? -w/2 : w/2;  // L/R
            TVec3 dpos = TVec3(dv,0,0);
            dpos = camRot * dpos;
            dpos += camPosOri;
            cam.setPos(dpos);
            cam.updateViewMat();
            //---- Draw one side
            IcScene::onDraw();
        }
        cam.setPos(camPosOri);
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
        
        if(m_pCntxt==nullptr) return;
        m_pCntxt->onWindowSize(winSize);
        reInit();
       
    }
    //--------------------------------------
    //  VRScnDisp
    //--------------------------------------
    void IcWindowVR::VRScnDisp::reInit()
    {
        if(m_pCntxt==nullptr) return;
        clearObjs();
        //---- Create Distortion Mesh
        m_pDistMesh = createDistortMesh();
        
        //---- Create display distortion plane
        for(int i=0;i<2;i++)
        {
            const float w = K_dispQuad_w;
            auto pTex = m_pCntxt->getTex(i);
            auto pModel = makeSp<IcModel>();
            pModel->setMesh(m_pDistMesh);
            pModel->setTexture(pTex);
            auto pObj = makeSp<IcObject>(pModel);
            pObj->setPos(TVec3(w*((float)i-0.5), 0, 0));
            m_pObjPlane[i]=pObj;
            addObj(pObj);
        }
        //---- Set Camera
        auto& cam = *getCamera();
        cam.setPos(TVec3(0,K_dispCamHeight,0));
        cam.lookAt(TVec3(0,0,0), TVec3(0,0,-1));
        
    }
    //-----------
    auto IcWindowVR::VRScnDisp::createDistortMesh()->decltype(m_pDistMesh)
    {
        if(m_pCntxt==nullptr) return nullptr;
        
        const int N = K_dispMeshGridN;
        const float w = K_dispQuad_w;
        const auto& cfg = m_pCntxt->m_rCfg;
        const float K2 = cfg.K_distortion_k2;
        const float K4 = cfg.K_distortion_k4;
        
        
        IcMeshData mshd;
        mshd.createGridXZ(TRect(TPos(-w/2, -w/2), TSize(w, w)), N, N);
        auto& verts = mshd.m_verts;
        
        //---- Re-pos verts
        for(int i=0;i<N+1;i++)    // Row
            for(int j=0;j<N+1;j++)    // Col
            {
                size_t k = i*(N+1) + j;
                TVec3 v; verts.getAt(k, v);
                
                float r = glm::length(v);
                float r2 = r*r;
                float r_ds = 1 + K2*r2 + K4*r2*r2; // distorted
                TVec3 v_ds(v.x*r_ds, 0, v.z*r_ds);
                verts.setAt(k, v_ds);
            }
        
        auto pMsh = makeSp<IcMesh>(mshd);
        return pMsh;
    }
  
 
}
