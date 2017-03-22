//
//  main.cpp
//  IcVRDemo
//
//  Created by Sherman Chen on 3/14/17.
//  Copyright © 2017 Simviu Technology Inc. All rights reserved.
//

#include "VRDemoApp.hpp"

int main(int argc,  char * argv[]) {
    VRDemoApp app;
    app.runCmdLine(argc, argv, "../../../../../IcData/");
    
    return 0;
}
