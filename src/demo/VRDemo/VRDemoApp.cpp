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
        
        //---- Let's create losts of it (NxNxN)
        const int N = 4;
        const float w = 10;
        for(int x=0;x<N;x++)
            for(int y=0;y<N;y++)
                for(int z=0;z<N;z++)
                {
                    auto pObj   = ctl::makeSp<IcObject>(pModel);
                    TVec3 pos((x-N/2)*w, (y-N/2)*w, (z-N/2)*w);
                    pObj->setPos(pos);
                    addObj(pObj);
                    
                }
        
  
    };
 
};
//--------------------------------------
//  VRDemoWin
//--------------------------------------
class VRDemoWin : public IcWindowVR
{
public:
    virtual void onInit() override
    {
        IcWindowVR::onInit();
        auto pScn = makeSp<VRDemoScn>();
        initWithMainScn(pScn);
    };
};



//--------------------------------------
//  VRDemoApp::onInit
//--------------------------------------
void VRDemoApp::onInit()
{
    IcApp::onInit();
    l_sPathRes = m_cfg.m_sPathRes;
    
    auto pWin = makeSp<VRDemoWin>();
    addWindow(pWin);
}
