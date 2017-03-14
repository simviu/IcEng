//
//  IcVR.h
//  SkyDive
//
//  Created by Sherman Chen on 06/02/2017.
//  Copyright © 2017 Simviu Technology Inc. All rights reserved.
//

#ifndef IcVR_h
#define IcVR_h
#include "Ic3d.h"
namespace  Ic3d {
    //--------------------------
    //  IcWindowVr
    //--------------------------
    class IcWindowVr : public Ic3d::IcWindow
    {
    public:
        IcWindowVr();
        virtual void onInit() override;
        virtual void onWindowSize(const ctl::TSize& size) override;
        void setRootObj(ctl::Sp<Ic3d::IcObject> p);
        ctl::Sp<IcObject> getRootObj(){ return m_pRootObj; };
        
        //---- Can be override
        void updateVrCamPos(const TVec3& pos);
        void updateVrCamRot(const TQuat& rot);
        //        virtual void onCamAttitude(float pitch, float roll, float yaw);
        virtual void onMouseMove(int x, int y) override;
        //---- Vr CFG
        struct TVrCfg
        {
            //---- Scene Cfg
            IcCamera::TCfg m_camCfg;
            TFogPara m_fogPara;
        };
        TVrCfg m_vrCfg;
        
        //---- VR window status
        struct TVrStts
        {
            TVec3 m_camPos;
            TQuat m_camRot;
        };
        TVrStts m_vrStts;
        //--------------------------
        //  IcSceneVr
        //--------------------------
        // Be implemented in cpp
        class IcSceneVr_IF : public IcScene
        {
        public:
            IcSceneVr_IF(bool isLeft): m_isLeft(isLeft){};
            virtual void updateCam(const Ic3d::TVec3& camPos,
                                   const Ic3d::TQuat& camRot){};
        protected:
            bool m_isLeft = false;
        };
    protected:
        ctl::Sp<IcObject> m_pRootObj = ctl::makeSp<IcObject>();
        //---- VR scene left/right
        ctl::Sp<IcSceneVr_IF> m_vrScn[2]{nullptr, nullptr};
        void updateVrFogPara();
        
        //------------------------
        //  MouseHelper
        //------------------------
        // simulate cam tilte by mouse
        class CMouseHelper
        {
        public:
            TQuat onMouseMove(int x, int y);
        protected:
            TEuler  m_camAtt;
            TVec2   m_mousePrevPos;
            bool    m_isFirst = true;
        };
        CMouseHelper m_mouseHelper;
    };
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
#endif /* IcVR_h */
