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
    //  onInit
    //--------------------------------------
    void IcWindowVR::onInit()
    {
        IcWindow::onInit();
        //---- Create Display Scn
        m_pScnDisp = makeSp<VRScnDisp>();
        addScene(m_pScnMain);
        addScene(m_pScnDisp);

    }
    //--------------------------------------
    //  setMainScn
    //--------------------------------------
    void IcWindowVR::setMainScn(ctl::Sp<VRScnMain> pScnMain)
    {
        m_pScnMain = pScnMain;
    }
    //--------------------------------------
    //  onWindowSize
    //--------------------------------------
    void IcWindowVR::onWindowSize(const ctl::TSize& winSize)
    {
        IcWindow::onWindowSize(winSize);
        
        //---- Create Render To Tex
        float w = winSize.w;
        float h = winSize.h;
        
        //---- Only orientation landscape is OK for VR L/R screen
        if(w<h) return;
        float l = w/2;
        if(l<h) l= h;   // biggest as possible even cropped
        
        Sp<IcTexture> pTex[2];
        for(int i=0;i<2;i++)
            pTex[i] = makeSp<IcTexture>(TSize(l,l));

        //---- Create Display Tex
        m_pScnMain->setVRTex(pTex[0], pTex[1]);
        m_pScnDisp->setVRTex(pTex[0], pTex[1]);
    }
   
}
