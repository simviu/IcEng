//
//  IcWindowVr.cpp
//  VrScenery
//
//  Created by Sherman Chen on 12/23/16.
//  Copyright © 2016 Simviu Technology Inc. All rights reserved.
//

#include "Ic3d.h"

namespace Ic3d {
    using namespace ctl;
    using namespace std;

    const static TVec3 K_camPos(0.2,1.8,-0);
    const static float K_camDist = 0.1; // eye L/R distance
    
    const static bool  K_dbgEnCamObj = false; // Display a cube infront of eye
    const static float K_dbgCamObjFwd = 0.3; // eye L/R distance
    const static TVec3 K_dbgCamObjSz(0.01, 0.01, 0.01);
    
    //---------------------------------------
    //  VrCamMng::onInit
    //---------------------------------------
    void IcWindowVr::VrCamMng::onInit(bool isLeft, TCamSp pCam)
    {
        m_isLeft = isLeft;
        m_pCam = pCam;
        
        //---- Build Dbg Obj
        if(K_dbgEnCamObj)
        {
            IcMeshData mshd; mshd.createCube(K_dbgCamObjSz, TVec3(0,0,0));
            auto pModel = ctl::makeSp<IcModel>(mshd);
            m_pDbgObj = ctl::makeSp<IcObject>(pModel);
        }
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
            
            //---- Debug
            if(K_dbgEnCamObj)
                pObj->addChildObj(m_camMng[i].getDbgObj());
        }
    }

    //---------------------------------------
    //  VrCamMng::updateCam
    //---------------------------------------
    void IcWindowVr::VrCamMng::updateCam(const Ic3d::TVec3& camPos,
                                         const Ic3d::TQuat& camRot)
    {
        
        //---- Set Camera
        float dv = (m_isLeft)? -K_camDist/2 : K_camDist/2;  // L/R
        TVec3 dpos = TVec3(dv,0,0);
        auto& cam = *m_pCam;
        dpos = camRot * dpos;
        dpos += camPos;
        cam.setPos(dpos);
        cam.setQuat(camRot);
        //cam.lookAt(posLookAt, upVec);  // tmp debug
        cam.updateViewMat();
        
        //---- For dbg
        if(K_dbgEnCamObj && m_pDbgObj!=nullptr)
        {
            TVec3 dbgPos(0,0,-K_dbgCamObjFwd);    // In front of eyes;
            dbgPos = camRot * dbgPos;
            dbgPos += dpos;
            m_pDbgObj->setPos(dbgPos);
        }
        
    }
    //---------------------------------------
    //  IcWindowVr::onInit
    //---------------------------------------
    void IcWindowVr::onInit()
    {
        IcWindow::onInit();
        for(int i=0;i<2;i++)
        {
            auto pScn = ctl::makeSp<IcScene>();
            auto pCam = pScn->getCamera();
            m_camMng[i].onInit((i==0), pCam);
            addScene(pScn);
        }
        setRootObj(m_pRootObj);
    }
 
    //---------------------------------------
    //  IcWindowVr::onUpdate
    //---------------------------------------
    void IcWindowVr::setCamRot(const Ic3d::TQuat &camRot)
    {
        m_camMng[0].updateCam(K_camPos, camRot);
        m_camMng[1].updateCam(K_camPos, camRot);
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
