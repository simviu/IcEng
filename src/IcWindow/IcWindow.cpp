//
//  IcWindow.cpp
//  DevEng
//
//  Created by Sherman Chen on 10/31/16.
//  Copyright (c) 2016 Simviu Technology Inc.
//  All rights reserved.
//  http://www.simviu.com/dev
//
//

#include "Ic3d.h"

namespace Ic3d
{
    using namespace ctl;
    using namespace std;
 
    //-------------------------------------------
    //  def
    //-------------------------------------------
    
    //-------------------------------------------
    //	drawObj
    //-------------------------------------------
    void IcWindow::addScene(ctl::Sp<IcScene> pScn)
    {
        m_scnAry.add(pScn);
    };
    
    void IcWindow::removeAllScene()
    {
        m_scnAry.clear();
    }
    
    //-------------------------------------------
    //	onWindowSize
    //-------------------------------------------
    // TODO: 1) restore window create with rect.
    //    2) m_winSize -> m_winSize/m_winPos
    void IcWindow::onWindowSize(const ctl::TSize& size)
    {
        m_winSize = size;
        TRect viewRect(0,0,m_winSize.w, m_winSize.h);
        for(auto pScn : m_scnAry.getAry())
            pScn->onViewRect(viewRect);
    }
    //-------------------------------------------
    //	onInit
    //-------------------------------------------
    void IcWindow::onInit()
    {
        //---- Check IcEng,
        // TODO: Multiple win nee multiple context?
        auto pEng = IcEng::getInstance();
        if(!pEng->hasInit())
            pEng->initEng();
    }
    //-------------------------------------------
    //	onScreenSize
    //-------------------------------------------
    void IcWindow::onScreenSize(const ctl::TSize& size)
    {
        // In default, call onWindowSize().
        // TODO:
        //    Consider add option to fit window into screen
        //    based on normalized coordinator, etc.
        onWindowSize(size);
    }

    //-------------------------------------------
    //	drawObj
    //-------------------------------------------
    void IcWindow::onDrawUpdate(float deltaT)
    {
        //---- Thread protection
        std::unique_lock<std::mutex> lock(m_mtx_draw);
		if(m_isDrawing) return;
		m_isDrawing = true;        

        auto pEng = IcEng::getInstance();
        //-------------
        // Frame Start
        //-------------
        pEng->onFrameStart();
        pEng->clearScreen(m_cfg.m_bkColor); // TODO: cfg of not clearScreen
        
        
        //-------------
        // Draw Update Scene
        //-------------
        for(auto pScn : m_scnAry.getAry())
        {
            pScn->onUpdate(deltaT);
            pScn->onDraw();
        }
        pEng->onFrameEnd();
		m_isDrawing = false;

    }
    //-------------------------------------------
    //	onKeyboard
    //-------------------------------------------
    void IcWindow::onKeyboard(unsigned char key)
    {
        switch (key) {
                break;
            default:
                break;
        }
    }
 
}
