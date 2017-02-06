//
//  DemoScene.h
//  DevEng
//
//  Created by Sherman Chen on 10/24/16.
//  Copyright (c) 2016 Simviu Technology Inc.
//  All rights reserved.
//  http://www.simviu.com/dev
//

#ifndef DemoScene_h
#define DemoScene_h

#include "Ic3d.h"
#include "ctl.h"

//-----------------------------
//	DemoScene
//-----------------------------
/*
 0)  Basic            : 1 Mix Shapes model load from OBJ.
 1)  Nested Transform : Child Object, Robot Arm Chain
 2)  ModelCreate      : Manually create model by Mesh build, img Texture and material.
 3)  Lighting         : Lighting demo
 
 */
class DemoScene : public Ic3d::IcScene
{
public:
    struct TCfg
    {
        std::string m_sPathRes;
    };
    static TCfg m_cfg;
    virtual void onUpdate(double deltaT) override;
protected:
    //---- Camera Manage, rotate around scene
    struct CCamMng
    {
        void onUpdate(double deltaT, IcCamera& cam);
    protected:
        float m_angle = 0;  // Roate angle
    };
    CCamMng m_camMng;
};
//-----------------------------
//	DemeScnMng
//-----------------------------
class DemeScnMng
{
public:
    DemeScnMng();
    ctl::Sp<DemoScene> getDemoScn(int sel)
    {
        auto pItem = m_scnAry[sel];
        if(pItem==nullptr) return nullptr;
        return pItem->m_pScn;
    };
    size_t getNum() const { return m_scnAry.size(); };
protected:
    struct TItem
    {
        std::string m_sName;
        ctl::Sp<DemoScene> m_pScn = nullptr;
    };
    ctl::SpAry<TItem> m_scnAry;
};
//-----------------------------
//	DemoBasic
//-----------------------------
class DemoBasic : public DemoScene
{
public:
    virtual void onInit() override;
};
//-----------------------------
//	DemoLights
//-----------------------------
class DemoLights : public DemoScene
{
public:
    virtual void onInit() override;
};

//-----------------------------
//	DemoNestedTrans
//-----------------------------
class DemoNestedTrans : public DemoScene
{
public:
    virtual void onInit() override;
};
//-----------------------------
//	DemoModelCreate
//-----------------------------
class DemoModelCreate : public DemoScene
{
public:
    virtual void onInit() override;
};




#endif /* DemoScene_h */
