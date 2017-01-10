//
//  main.cpp
//  IcEngLib
//
//  Created by Sherman Chen on 11/30/16.
//  Copyright © 2016 Simviu Technology Inc. All rights reserved.
//

#include "Ic3d.h"
#include "IcTestWindow.h"

using namespace Ic3d;

//--------------------
//  main
//--------------------
int main(int argc, char **argv)
{
    auto pWin = ctl::makeSp<IcTestWindow>();
    return IcWindow::runCmd(argc, argv, pWin);
}
