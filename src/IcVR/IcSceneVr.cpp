//
//  IcSceneVr.cpp
//  IcEngLib
//
//  Created by Sherman Chen on 1/19/17.
//  Copyright (c) 2016 Simviu Technology Inc.
//  All rights reserved.
//  http://www.simviu.com/dev
//

#include "IcVR.h"

namespace Ic3d {
    using namespace ctl;
    using namespace std;
    const static bool  K_dbgEnCamObj = false; // Display a cube infront of eye
    const static float K_dbgCamObjFwd = 0.3; // eye L/R distance
    const static TVec3 K_dbgCamObjSz(0.01, 0.01, 0.01);
    const static float K_camDist = 0.1; // eye L/R distance
  
    //---------------------------------------
    //  VrCamMng::onInit
    //---------------------------------------
    void IcSceneVr::VrCamMng::setScnCam(TCamSp pCam)
    {
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
    //  VrCamMng::updateCam
    //---------------------------------------
    void IcSceneVr::VrCamMng::updateCam(const Ic3d::TVec3& camPos,
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
    //  IcSceneVr::IcSceneVr
    //---------------------------------------
    IcSceneVr::IcSceneVr(bool isLeft):
        IcSceneVr_IF(isLeft),
        m_camMng(isLeft)
    {
        auto pCam = getCamera();
        m_camMng.setScnCam(pCam);
    }

    //---------------------------------------
    //  VrCamMng::onInit
    //---------------------------------------
    void IcSceneVr::onInit()
    {
        IcSceneVr_IF::onInit();
        //---- Debug
        if(K_dbgEnCamObj)
            addObj(m_camMng.getDbgObj());
    }
    //---------------------------------------
    //  VrCamMng::onInit
    //---------------------------------------
    void IcSceneVr::updateCam(const Ic3d::TVec3& camPos,
                              const Ic3d::TQuat& camRot)
    {
        m_camMng.updateCam(camPos, camRot);
    }

}
