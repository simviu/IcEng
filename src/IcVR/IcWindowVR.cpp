//
//  IcWindowVR.cpp
//  IcEngLib
//
//  Created by Sherman Chen on 3/14/17.
//  Copyright © 2017 Simviu Technology Inc. All rights reserved.
//



#include "Ic3d.h"

namespace Ic3d
{
    using namespace ctl;
    using namespace std;
    
    const static TEuler K_mouseCoef{0.02,0.02,0.02};

    //---------------------------------------
    //  IcWindowVR::CMouseHelper
    //---------------------------------------
    TQuat IcWindowVR::CMouseHelper::onMouseMove(int x, int y)
    {
        TVec2 pos(x,y);
        if(m_isFirst) m_mousePrevPos = pos;
        m_isFirst = false;
        auto dPos = pos - m_mousePrevPos;
        m_camAtt.p += dPos.y * K_mouseCoef.p;
        m_camAtt.y += dPos.x * K_mouseCoef.y;
        
        //---- Pitch in +/-90 degree
        dClamp<decltype(m_camAtt.p)>(m_camAtt.p, -M_PI/2.0, M_PI/2.0);
        m_mousePrevPos = pos;
        return m_camAtt.toQuat();
    }
    
    
    //--------------------------------------
    //  onWindowSize
    //--------------------------------------
    void IcWindowVR::VRContext::onWindowSize(const ctl::TSize& winSize)
    {
        //---- Create Render To Tex
        float w = winSize.w;
        float h = winSize.h;
        
        //---- Only orientation landscape is OK for VR L/R screen
        if(w<h) return;
        float l = w/2;
        if(l<h) l= h;   // biggest as possible even cropped
        
        for(int i=0;i<2;i++)
        {
            m_pTex[i] = makeSp<IcTexture>(TSize(l,l));
            m_pTex[i]->setAsRenderTarget();
        }
    }
    //---------------------------------------
    //  IcWindowVR::onMouseMove
    //---------------------------------------
    void IcWindowVR::onMouseMove(int x, int y)
    {
        IcWindow::onMouseMove(x, y);
        
        //---- Use mouse simulate camera attitude
        if(m_pScnMain==nullptr) return;
        auto q = m_mouseHelper.onMouseMove(x, y);
        auto pCam = m_pScnMain->getCamera();
        pCam->setQuat(q);
        pCam->updateViewMat();
    }
    //--------------------------------------
    //  onDeviceAttitude
    //--------------------------------------
    void IcWindowVR::onDeviceRot(const TQuat& q)
    {
        Ic3d::TQuat q0(TVec3(deg2rad(-90),0,0)); // rot phone face up
        auto pCam = m_pScnMain->getCamera();
        pCam->setQuat(q0*q);
        pCam->updateViewMat();
    }

    //--------------------------------------
    //  onDeviceAttitude
    //--------------------------------------
    void IcWindowVR::onDeviceAttitude(const TVec3& att)
    {
        if(m_pScnMain==nullptr) return;
        
        //---- iPhone attitude translate to VR cam rotation:
        // 1) iPhone attitude = 0 when face up, use q0 to reverse
        // 2) Euler assignment :
        //      iPhone      : is CMAttitude(yaw->Z,pitch->X,roll->Y)
        //      glm Euler   : is TVec(x,y,z) = {pitch, yaw, roll}
        //      VR : pitch->Y, yaw->X, roll->Z
        Ic3d::TQuat q0(TVec3(deg2rad(-90),0,0)); // rot phone face up
        Ic3d::TQuat q1(att); // exchange axis
        
        auto pCam = m_pScnMain->getCamera();
        pCam->setQuat(q0*q1);
        pCam->updateViewMat();
        
    }
   
    //--------------------------------------
    //  onInit
    //--------------------------------------
    void IcWindowVR::onInit()
    {
        IcWindow::onInit();
    }
    //--------------------------------------
    //  setMainScn
    //--------------------------------------
    void IcWindowVR::initWithMainScn(ctl::Sp<VRScnMain> pScnMain)
    {
        removeAllScene();
        m_pScnMain = pScnMain;
        m_pScnDisp = makeSp<VRScnDisp>();
        //---- Create Display Scn
        addScene(m_pScnMain);
        addScene(m_pScnDisp);
        
        m_pScnMain->setContext(m_pVRContext);
        m_pScnDisp->setContext(m_pVRContext);
   }
   
}
