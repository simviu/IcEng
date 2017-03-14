//
//  VRDemoApp.cpp
//  IcVRDemo
//
//  Created by Sherman Chen on 3/14/17.
//  Copyright © 2017 Simviu Technology Inc. All rights reserved.
//

#include "VRDemoApp.hpp"

using namespace ctl;
using namespace std;
using namespace Ic3d;
//--------------------------------------
//  onInit
//--------------------------------------
void VRDemoApp::onInit()
{
    auto pWin = makeSp<IcWindow>();
    auto pScn = makeSp<IcScene>();
    pWin->addScene(pScn);
    addWindow(pWin);
}
