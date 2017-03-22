## How to Build IcEng

The IcEng Pre-Build binary is provided on :[https://github.com/simviu/IcEngBuild].
It's also easy to build by your own.

## Mac
  * The XCode project file provided in IcEng/proj/mac/IcEng/.  Double click to open project file in xcode.
  * select the target ALL.
  * Select menu Product->Build for Running/Profiling.
  * Check result in IcEng/IcEngBuild/mac/, there will be bin/lib dir inside, contain libIcEng.a.

If you want re-create Xcode project, here is the checklist:
  * Build output dir : "IcEng/IcEngBuild/mac/lib/${ARCHS}".
  * user header search path : "IcEng/inc" and "IcEng/inc/core"
  * Source code : 
    * IcEng/src/ctl
    * IcEng/src/IcRenderEng
    * IcEng/src/Ic3d
    * IcEng/src/IcVR
    * IcEng/src/IcApp/
    * IcEng/src/IcApp/iOS/
    
 
