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
#include "IcTestWindow.h"
using namespace Ic3d;
using namespace ctl;
static float K_rotSpeed = 30.0;

//---- TODO: put into IcTestApp header without cpp,
// serve as sample project.
//----------------------------
// IcTestScene
//----------------------------
// Embedded self test scene
class IcTestScene : public IcScene
{
public:
    //--------------------------
    //  onInit
    //--------------------------

    void onInit() override
    {
    	static bool isDbgOn = false;
        logInfo("IcTestScene::onInit()");
		if(isDbgOn) logErr("Multiple entry");
		isDbgOn = true;

        IcMeshData mshd;
        mshd.createCube(TVec3(1,1,1), TVec3(0,0,0));
        auto pModel = ctl::makeSp<IcModel>(mshd);
        auto pObj   = ctl::makeSp<IcObject>(pModel);
        addObj(pObj);
        auto& cam = *getCamera();
        cam.setPos(TVec3(10, 4, -6));
        cam.lookAt(TVec3(0,0,0), TVec3(0,1,0));
        m_pObj = pObj;
        
        //---- Add Text
        auto pText = ctl::makeSp<IcText>("IcEng Test");
        addText(pText);
    	logInfo("IcTestScene::onInit() done");
		isDbgOn = false;
    };
    //--------------------------
    //  onUpdate
    //--------------------------
    void onUpdate(double deltaT) override
    {        
        float dt = deltaT > 1.0 ? 1.0 : deltaT;
        if(m_pObj==nullptr) return;
        m_degree += K_rotSpeed*dt;
        if(m_degree>360) m_degree -= 360;
        TQuat q(TVec3(0, deg2rad(m_degree), 0));
        m_pObj->setQuat(q);
        
    }
    
protected:
    ctl::Sp<IcObject> m_pObj = nullptr;
    float m_degree = 0; // rotation degree
};
//---------------------------------
//  IcTestWindow
//---------------------------------
void IcTestWindow::onInit()
{
    logInfo("IcTestWindow::onInit()");
    // Call parent is necessary
    IcWindow::onInit();
    auto pScn = ctl::makeSp<IcTestScene>();
    addScene(pScn);
    logInfo("IcTestWindow::onInit() done");
}

