//
//  main.cpp
//  IcEngLib
//
//  Created by Sherman Chen on 2/14/17.
//  Copyright © 2017 Simviu Technology Inc. All rights reserved.
//

#include "Ic3d.h"


//--------------------
//  main
//--------------------
int main(int argc, char **argv)
{
    auto& app = Ic3d::getIcAppInstance();
    return app.runCmd(argc, argv);
}
