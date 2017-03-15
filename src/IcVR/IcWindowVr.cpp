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
    //--------------------------------------
    //  onWindowSize
    //--------------------------------------
    void IcWindowVR::onWindowSize(const ctl::TSize& winSize)
    {
       IcWindow::onWindowSize(winSize);
        if(m_pVRContext!=nullptr)
            m_pVRContext->onWindowSize(m_cfg.m_size);
        m_pScnDisp->reInit();
        
    }
   
}
