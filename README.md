# Overview
I-Cube 3D Engine (IcEng) is an open source, light weight OpenGL/C++ 3D graphics Engine. It's designed with simple structure and interface to fit small cross platform 3D application. It can also serve as OpenGL example, or foundation code base that you can develop your own 3D engine upon.
The source or binary are provided with free BSD license, so you can use this engine on personal or commercial project with no restriction. See License section.
For more information check website http://www.simviu.com/dev.

#Features
* OpenGL 2.0, with C++ 0x11 code and shader.
* Cross Platform ( Mac/Windows/Linux/iOS/Android ), project file provided.
* Auto memory and resource management with C++ 0x11 std::shared_ptr.
* Model loading with Waveform .OBJ file. ( More format in future )
* Basic lighting, material.
* Abstract C++ wrapper class with OpenGL function hidden.


#Architecture
* The IcEng applications are wrapped in IcApp/IcWindow/IcScene structure. 
* There is only one IcApp instance(singleton). 
* An IcApp application can contain several IcWindow. 
* Each window has it's own OpenGL context. 
* Each window one or more Quick Start. 
* Each IcScene contain one IcCamera, and many IcObject.
* An IcObject has it's IcModel, loaded from OBJ file or created dynamically.
* IcModel are constructed by IcMesh, IcTexture and IcMaterial.

In most case your simple 3d application has one IcWindow and one IcScene. For iOS/Android mobile App. There should only be one window.
