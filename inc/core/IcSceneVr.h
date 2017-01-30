//
//  IcSceneVr.h
//  IcEngLib
//
//  Created by Sherman Chen on 1/19/17.
//  Copyright (c) 2016 Simviu Technology Inc.
//  All rights reserved.
//  http://www.simviu.com/dev
//
//

#ifndef IcSceneVr_h
#define IcSceneVr_h

#include "Ic3d.h"

namespace Ic3d {
    //--------------------------
    //  IcSceneVr
    //--------------------------
    // Implementation in cpp
    class IcSceneVr : public IcWindowVr::IcSceneVr_IF
    {
    public:
        IcSceneVr(bool isLeft);
        virtual void updateCam(const Ic3d::TVec3& camPos,
                               const Ic3d::TQuat& camRot) override;
        virtual void onInit() override;
    protected:
        //--------------------------
        //  VR Camera
        //--------------------------
        struct VrCamMng
        {
            VrCamMng(bool isLeft):m_isLeft(isLeft){};
            typedef ctl::Sp<Ic3d::IcCamera> TCamSp;
            void setScnCam(TCamSp pCams);
            void updateCam(const Ic3d::TVec3& camPos,
                           const Ic3d::TQuat& camRot);
            ctl::Sp<Ic3d::IcObject> getDbgObj(){ return m_pDbgObj; };
            //---- Use mouse simulate camera move
        protected:
            bool    m_isLeft = false;
            TCamSp  m_pCam = nullptr;
            ctl::Sp<IcObject> m_pDbgObj = nullptr;
        };
        
        //---- Camera Mng
        VrCamMng    m_camMng;
        //---- TODO: --> isRight
        bool m_isLeft = false;
        
    };
 
};

#endif /* IcSceneVr_h */
