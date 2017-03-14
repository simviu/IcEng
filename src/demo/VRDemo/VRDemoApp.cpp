//
//  VRDemoApp.cpp
//  IcVRDemo
//
//  Created by Sherman Chen on 3/14/17.
//  Copyright © 2017 Simviu Technology Inc. All rights reserved.
//

#include "VRDemoApp.hpp"

using namespace ctl;
using namespace std;
using namespace Ic3d;

static const string K_sModel = "IcDemo/MixShapes/MixShapes.obj";
static string l_sPathRes = "";

//--------------------------------------
//  VRDemoScn
//--------------------------------------
class VRDemoScn : public IcWindowVR::VRScnMain
{
public:
    using VRScnMain::VRScnMain;
    virtual void onInit() override
    {
        //--- Always call parent class onInit()
        VRScnMain::onInit();
        
        //--- Load Object
        string sFile = l_sPathRes + K_sModel;
        auto pModel = ctl::makeSp<IcModel>(sFile);
        auto pObj   = ctl::makeSp<IcObject>(pModel);
        // Add this obj to Scene, will be rendered
        addObj(pObj);
 
    };
    virtual void onDraw() override
    {
        VRScnMain::onDraw();
    };
};


//--------------------------------------
//  VRDemoApp::onInit
//--------------------------------------
void VRDemoApp::onInit()
{
    IcApp::onInit();
    l_sPathRes = m_cfg.m_sPathRes;
    
    auto pWin = makeSp<IcWindowVR>();
    auto pScn = makeSp<VRDemoScn>();
    pWin->initWithMainScn(pScn);
    addWindow(pWin);
}
