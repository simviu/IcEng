//
//  DemoApp.hpp
//  IcEngDemo
//
//  Created by Sherman Chen on 12/24/16.
//  Copyright (c) 2016 Simviu Technology Inc.
//  All rights reserved.
//  http://www.simviu.com/dev
//

#ifndef DemoApp_hpp
#define DemoApp_hpp

#include "Ic3d.h"
#include "DemoScene.h"

//-----------------------------------
//  DemoWindow
//-----------------------------------
class DemoWindow : public Ic3d::IcWindow
{
public:
    void reqSetDemo(int sel);
    virtual void onKeyboard(unsigned char key) override;
    virtual void onDrawUpdate(float deltaT) override;
    
protected:
    int m_demoSel_req = 0;
    int m_demoSel = -1;
};

//-----------------------------------
//  DemoApp
//-----------------------------------
class DemoApp : public Ic3d::IcApp
{
public:
    DemoApp();
    virtual void onInit() override;
    void reqSetDemo(int sel);
    virtual std::string onCmd(const std::string& sCmd) override;
protected:
    ctl::Sp<DemoWindow> m_pDemoWin = nullptr;
   
};


#endif /* DemoApp_hpp */
