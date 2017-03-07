//
//  main.cpp
//  IcEngLib
//
//  Created by Sherman Chen on 2/14/17.
//  Copyright © 2017 Simviu Technology Inc. All rights reserved.
//

#include "MyTestApp.h"


//--------------------
//  main
//--------------------
int main(int argc, char **argv)
{
    MyTestApp app;
    app.m_cfg.m_sPathRes = "../../../../../IcData/";
    return app.runCmdLine(argc, argv);
}
