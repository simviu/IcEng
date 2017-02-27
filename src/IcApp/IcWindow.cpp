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
    const static string K_sSubPath_shader = "IcShader/";
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
    //	onInit
    //-------------------------------------------
    void IcWindow::onInit()
    {
        //---- Check IcEng,
        // TODO: Multiple win need multiple context?
        auto pEng = IcEng::getInstance();
    //    if(!pEng->hasInit())
        {
            string sPathRes = IcApp::getInstance()->m_cfg.m_sPathRes;
            pEng->initEng(sPathRes + K_sSubPath_shader);
        }

    }
    //-------------------------------------------
    //	onInit
    //-------------------------------------------
    void IcWindow::onRelease()
    {
        m_scnAry.clear();
        auto pEng = IcEng::getInstance();
        pEng->releaseEng();
    }
    //-------------------------------------------
    //	onWindowSize
    //-------------------------------------------
    void IcWindow::onWindowSize(const ctl::TSize& size)
    {
        // In default, call onWindowSize().
        // TODO:
        //    Consider add option to fit window into screen
        //    based on normalized coordinator, etc.
        m_cfg.m_size = size;
        for(auto pScn : m_scnAry.getAry())
            pScn->onWindowSize(size);
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
            if(!pScn->hasInit())
            {
                pScn->onInit();
                pScn->onWindowSize(m_cfg.m_size);
                pScn->setHasInit(true);
            }

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
