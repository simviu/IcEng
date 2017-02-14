//
//  IcTestWindow.cpp
//  IcEngLib
//
//  Created by Sherman Chen on 12/23/16.
//  Copyright (c) 2016 Simviu Technology Inc.
//  All rights reserved.
//  http://www.simviu.com/dev
//
//

#include "Ic3d.h"

using namespace Ic3d;
using namespace ctl;


//----------------------------
// MyTestScn
//----------------------------
class MyTestScn : public IcScene
{
protected:
    ctl::Sp<IcObject> m_pObj = nullptr;
    float m_degree = 0; // rotation degree
    
public:
    //--------------------------
    //  onInit
    //--------------------------
    void onInit() override
    {
        logInfo("IcTestScene::onInit()");
        IcMeshData mshd;
        mshd.createCube(TVec3(1,1,1), TVec3(0,0,0));
        auto pModel = ctl::makeSp<IcModel>(mshd);
        auto pObj   = ctl::makeSp<IcObject>(pModel);
        addObj(pObj);
        auto& cam = *getCamera();
        cam.setPos(TVec3(10, 4, -6));
        cam.lookAt(TVec3(0,0,0), TVec3(0,1,0));
        m_pObj = pObj;
    };
    //--------------------------
    //  onUpdate
    //--------------------------
    void onUpdate(double deltaT) override
    {        
        static float K_rotSpeed = 30.0;
        float dt = deltaT > 1.0 ? 1.0 : deltaT;
        if(m_pObj==nullptr) return;
        m_degree += K_rotSpeed*dt;
        if(m_degree>360) m_degree -= 360;
        TQuat q(TVec3(0, deg2rad(m_degree), 0));
        m_pObj->setQuat(q);
    }
};
//---------------------------------
//  MyTestApp
//---------------------------------
class MyTestApp : public IcApp
{
    //---- Override onInit
    virtual void onInit() override
    {
        // 1. Call super onInit()
        IcApp::onInit();
        // 2. Create Window
        auto pWin = makeSp<IcWindow>();
        // 3. Create our scene
        auto pScn = ctl::makeSp<MyTestScn>();
        // 4. add scene to the window
        pWin->addScene(pScn);
        // 5. add the window to App
        addWindow(pWin);
    };
};
//---- Put our App instance here statically
static MyTestApp l_app;

//---------------------------------
//  Implementation of IcApp instance
//---------------------------------
namespace Ic3d
{
    IcApp& getIcAppInstance()
    {
        return l_app;
    }
}

