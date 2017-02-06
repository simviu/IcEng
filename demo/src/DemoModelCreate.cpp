//
//  DemoModelCreate.cpp
//  DevEng
//
//  Created by Sherman Chen on 10/26/16.
//  Copyright (c) 2016 Simviu Technology Inc.
//  All rights reserved.
//  http://www.simviu.com/dev
//

#include "DemoScene.h"

using namespace std;
using namespace ctl;
using namespace Ic3d;

static const string K_sPath_demo = "IcDemo/planets/";
//----------------------------------------------
//  DemoModelCreate::onInit
//----------------------------------------------
void DemoModelCreate::onInit()
{
    //--- Always call parent class onInit()
    IcScene::onInit();
    
    //--- Load Sphere mesh
    std::string sPath = K_sPath_demo;

    //---- Sphere Mesh
    IcMeshData mshdSphr; mshdSphr.createSphere(1, 32, 32);
    auto pMeshSphr = ctl::makeSp<IcMesh>(mshdSphr);
    
   
    //---- Load Textures
    auto pTex0 = ctl::makeSp<IcTexture>(sPath + "tex_mars.png");
    auto pTex1 = ctl::makeSp<IcTexture>(sPath + "tex_earth.png");
   
    //---- Planet material
    auto pMatSphr = ctl::makeSp<IcMaterial>();
   
    
    //---- Build model by mesh/material/texture.
    auto pModel0 = makeSp<IcModel>(pMeshSphr,   pTex0, pMatSphr );
    auto pModel1 = makeSp<IcModel>(pMeshSphr,   pTex1, pMatSphr );
    
    auto pObj0 = makeSp<IcObject>(pModel0);
    auto pObj1 = makeSp<IcObject>(pModel1);

    //---- Position Obj
    pObj0->setPos(TVec3(4,-2,3));
    pObj1->setPos(TVec3(0,0,0));
    pObj1->setScale(TVec3(2,2,2));  // Earth is twice bigger
    
    //---- Rotate North pole to +y
    TQuat q(TVec3(deg2rad(-90),0,0));
    pObj0->setQuat(q);
    pObj1->setQuat(q);

    
    // Add this obj to Scene, will be rendered
    addObj(pObj0);
    addObj(pObj1);

    //---- Set Camera
    auto& cam = *getCamera();
    
    // Put Camera somewhere apart.
    cam.setPos(TVec3(-4, 6, 20));
    
    // Set camera look at obj, up vector is +y.
    // Our obj is at (0,0,0) position by default.
    cam.lookAt(TVec3(0,0,0), TVec3(0,1,0));
    
}
