//
//  IcWindowsPC.cpp
//  IcEngLib
//
//  Created by Sherman Chen on 1/6/17.
//  Copyright © 2017 Simviu Technology Inc. All rights reserved.
//

#include "Ic3d.h"

namespace Ic3d
{
    using namespace ctl;
    using namespace std;
    using namespace Ic3d;
    //----------------------------------
    //  runCmd
    //----------------------------------
    int IcWindow::runCmd(int argc, char **argv, ctl::Sp<IcWindow> pWin)
    {
        auto pMng = IcWinMng::getInstance();
        pMng->addWindow(pWin);
        pMng->initMng(argc, argv);
        pMng->startMainLoop();
        return 0;
    }

}
