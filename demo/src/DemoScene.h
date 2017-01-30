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
    *  Basic        : 1 cube model, Look at.
    *  Transform    : Ground Plate, Multiple Model/objects, position/rotate/scale
    *  Nested Transform : Child Object, Robot Arm Chain
    *  Models       : Complex Demo Model, rotate.
    *  ModelCreate  : Manually create model by Mesh build, img Texture and material.
    *  Animation    : Cartoon Car.
    *  Lighting     : Rotate Light, color change
 
 
 Options:
 
 Primitive Mesh Build : Sphere/Cylinder/Cone/Plane
 Multiple Scene
 Multiple Window
 Multiple Thread
 */
class DemoScene : public Ic3d::IcScene
{
public:
    struct TCfg
    {
        std::string m_sPathRes;
    };
    static TCfg m_cfg;
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


#endif /* DemoScene_h */
