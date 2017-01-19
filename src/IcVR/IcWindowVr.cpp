//
//  IcWindowVr.cpp
//  VrScenery
//
//  Created by Sherman Chen on 12/23/16.
//  Copyright © 2016 Simviu Technology Inc. All rights reserved.
//

#include "Ic3d.h"
#include "IcSceneVr.h"
namespace Ic3d {
    using namespace ctl;
    using namespace std;

    const static TVec3 K_camPos(0.2,1.8,-0);
    //---------------------------------------
    //  IcWindowVr::IcWindowVr
    //---------------------------------------
    IcWindowVr::IcWindowVr()
    {
        m_vrScn[0] = makeSp<IcSceneVr>(true);
        m_vrScn[1] = makeSp<IcSceneVr>(false);
        addScene(m_vrScn[0]);
        addScene(m_vrScn[1]);
    }
    //---------------------------------------
    //  IcWindowVr::setRootObj
    //---------------------------------------
    void IcWindowVr::setRootObj(Sp<IcObject> pObj)
    {
        m_pRootObj = pObj;
        if(m_scnAry.size()<2) return;   // TODO: assert
        for(int i=0;i<2;i++)
        {
            auto pScn = m_scnAry[i];
            if(pScn==nullptr) return;
            pScn->clearObjs();
            pScn->addObj(pObj);
            
        }
    }

    //---------------------------------------
    //  IcWindowVr::onInit
    //---------------------------------------
    void IcWindowVr::onInit()
    {
        IcWindow::onInit();
         
        setRootObj(m_pRootObj);
    }
 
    //---------------------------------------
    //  IcWindowVr::onUpdate
    //---------------------------------------
    void IcWindowVr::setCamRot(const Ic3d::TQuat &camRot)
    {
        
        m_vrScn[0]->updateCam(K_camPos, camRot);
        m_vrScn[1]->updateCam(K_camPos, camRot);
     }
    //---------------------------------------
    //  onCamAttitude
    //---------------------------------------
    void IcWindowVr::onCamAttitude(float pitch, float roll, float yaw)
    {
        const Ic3d::TQuat K_rotPre(0,0,0,1);
        Ic3d::TVec3 e1 = { roll, -pitch, yaw  };
        Ic3d::TQuat qInv(TVec3(deg2rad(-90),0,0));
        Ic3d::TQuat q(e1);
        q = qInv * q;
        setCamRot(q);
    }
    //---------------------------------------
    //  IcWindowVr::onMouseMove
    //---------------------------------------
    void IcWindowVr::onMouseMove(int x, int y)
    {
        IcWindow::onMouseMove(x, y);
        //---- Use mouse simulate camera attitude
    }
   
    
    //---------------------------------------
    //  IcWindowVr::onWindowSize
    //---------------------------------------
    void IcWindowVr::onWindowSize(const ctl::TSize& winSize)
    {
        auto sz = winSize;
        sz.w = sz.w/2;
        TRect rect[2];
        rect[0] = TRect(0,0, sz.w, sz.h);
        rect[1] = TRect(sz.w,0, sz.w, sz.h);
        for(int i=0;i<2;i++)
        {
            auto pScn = m_scnAry[i];
            pScn->onViewRect(rect[i]);
        }
    }

}
