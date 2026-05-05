# IcEng Code Review & Development Context

> **Last Updated:** 2026-05-05  
> **Purpose:** Serve as development iteration context for the IcEng 3D engine codebase.

---

## 1. Architecture Overview

IcEng is a cross-platform C++ 3D engine targeting **Android, iOS, and PC (Linux/macOS/Windows)**. It uses OpenGL ES 2.0 with an adapter pattern for platform abstraction, a scene graph architecture, and smart pointer memory management.

### High-Level Architecture

```
┌─────────────────────────────────────────────────────────┐
│                      User Application                     │
│              (inherits IcApp, creates scenes)              │
├─────────────┬──────────────┬──────────────┬──────────────┤
│   IcApp     │  IcWinMng    │   IcWindow   │   IcScene    │
│ (singleton) │ (singleton)  │  (multiple)  │  (multiple)  │
├─────────────┴──────────────┴──────────────┴──────────────┤
│                       IcEng (engine singleton)            │
│              ┌──────────────────────────────┐              │
│              │    IcRenderEng (singleton)   │              │
│              │  ┌────────────────────────┐  │              │
│              │  │   IcRenderAdpStd       │  │              │
│              │  │  (OpenGL ES 2.0 impl)  │  │              │
│              │  └────────────────────────┘  │              │
│              └──────────────────────────────┘              │
├───────────────────────────────────────────────────────────┤
│                    Platform Abstraction                     │
│   Android (JNI)  │   iOS (Obj-C++)  │   PC (GLUT)         │
└───────────────────────────────────────────────────────────┘
```

---

## 2. Directory Structure

```
IcEng/
├── inc/                          # Public headers
│   ├── Ic3d.h                    # Main header - ALL engine classes (monolithic)
│   ├── IcRenderAdp.h             # Render adapter base + data structs
│   ├── IcEngJNI.h                # JNI interface header
│   ├── IcViewController.h        # iOS ViewController interface
│   ├── IcMath.h                  # GLM math type aliases
│   ├── core/                     # Core render headers
│   │   ├── IcRenderEng.h
│   │   ├── IcRenderAdpStd.h
│   │   ├── OpenGLinc.h           # Cross-platform GL includes
│   │   └── lodepng.h
│   └── glm/                      # GLM math library (vendored)
│
├── src/                          # Implementation
│   ├── Ic3d/                     # Core 3D classes (IcObject, IcModel, etc.)
│   ├── IcRenderEng/              # OpenGL ES 2.0 render implementation
│   ├── IcApp/                    # Application framework
│   │   ├── IcApp.cpp             # Base app class
│   │   ├── IcWindow.cpp          # Window abstraction
│   │   ├── IcWinMng.cpp          # Window manager
│   │   ├── android/IcEngJNI.cpp  # JNI bridge
│   │   ├── pc/IcAppPC.cpp        # GLUT-based PC entry
│   │   └── iOS/IcViewController.mm # iOS Obj-C++ entry
│   ├── IcVR/                     # VR stereo rendering
│   │   ├── IcWindowVR.cpp
│   │   └── IcSceneVR.cpp
│   ├── ctl/                      # Utility library (ctl namespace)
│   │   ├── ctl.h                 # (in inc/) - Sp, SpAry, SpMap, etc.
│   │   ├── ctlLog.cpp            # Logging
│   │   ├── ctlImg.cpp            # Image loading (PNG via lodepng)
│   │   ├── ctlStr.cpp            # String utils
│   │   ├── ctlUtil.cpp           # General utilities
│   │   └── lodepng.cpp           # PNG library
│   ├── demo/                     # Demo applications
│   │   ├── IcEngDemo/            # Multi-scene demo app
│   │   └── VRDemo/               # VR demo
│   └── test/                     # Test application
│
├── IcData/                       # Assets (shaders, models, textures)
│   ├── IcShader/                 # GLSL shaders
│   │   ├── IcShaderVert.vsh      # Vertex shader
│   │   └── IcShaderFrag.vsh      # Fragment shader (16 lights, fog)
│   └── IcDemo/                   # Demo 3D models
│       ├── plane/                # Plane OBJ + texture
│       ├── monkey/               # Suzanne monkey OBJ
│       ├── planets/              # Earth/Mars textures
│       └── MixShapes/            # Complex model + wood texture
│
├── proj/                         # Platform projects
│   ├── android/                  # Android Studio projects
│   │   ├── IcEng/                # Library + test app
│   │   ├── IcEngDemo/            # Demo app
│   │   └── IcVRDemo/             # VR demo app
│   ├── linux/                    # CMake Linux builds
│   ├── mac/                      # macOS Xcode project
│   └── ios/                      # iOS Xcode project
│
├── IcEngBuild/                   # Build output (gitignored)
│   ├── android/lib/<type>/<abi>/ # Android .so files
│   └── linux/                    # Linux binaries
│
└── doc/                          # Documentation
    ├── Build_IcEng_HowTo.md      # Build guide
    ├── code_review.md            # THIS FILE
    └── IcEng_Manual.pdf          # User manual
```

---

## 3. Core Modules

### 3.1 Ic3d - Scene Graph Module

**Location:** `src/Ic3d/`, declared in `inc/Ic3d.h`

| Class | File | Purpose |
|-------|------|---------|
| `IcObject` | IcObject.cpp | Base scene node: pos, quat, scale, children |
| `IcCamera` | IcCamera.cpp | Camera with FOV, near/far, lookAt |
| `IcCameraVR` | IcCamera.cpp | VR camera (extends IcCamera, no extra logic) |
| `IcLight` | IcLight.cpp | Light source (parallel/spot), up to 16 per scene |
| `IcModel` | IcModel.cpp | 3D model: mesh + material + texture + child models |
| `IcMesh` | IcMesh.cpp | Renderable mesh wrapper (adapter pattern) |
| `IcMeshData` | IcMeshDataCreate.cpp, IcMeshDataObj.cpp | Mesh data (verts, normals, UVs, faces) |
| `IcModelLoader` | IcModelLoaderObj.cpp | OBJ/MTL file parser |
| `IcTexture` | IcTexture.cpp | Texture wrapper, supports render-to-texture |
| `IcMaterial` | IcMaterial.cpp | Material properties (ambient, diffuse, specular) |
| `IcText` | IcText.cpp | 2D text overlay rendering |
| `IcScene` | IcScene.cpp | Scene container: camera, lights, objects, fog |
| `IcWindow` | IcWindow.cpp | Window abstraction, contains scenes |
| `IcWindowVR` | IcWindowVR.cpp | VR window with stereo rendering + distortion |
| `IcWinMng` | IcWinMng.cpp | Window manager singleton |
| `IcEng` | Ic3d.cpp | Engine singleton, manages render lifecycle |
| `IcApp` | IcApp.cpp | Base application class (user inherits) |

### 3.2 IcRenderEng - Rendering Module

**Location:** `src/IcRenderEng/`

| Class | File | Purpose |
|-------|------|---------|
| `IcRenderEng` | IcRenderEng.cpp | Render engine singleton, GL state management |
| `CRenderAdp` | IcRenderAdp.h (abstract) | Platform-independent render adapter interface |
| `IcRenderAdpStd` | IcRenderAdpStd.cpp | OpenGL ES 2.0 implementation |
| `CMeshAdpStd` | IcRenderAdpStd_Mesh.cpp | VBO mesh rendering |
| `CTexAdpStd` | IcRenderAdpStd_Tex.cpp | 2D texture + render-to-texture |
| `CTexAdpPVR` | IcRenderAdpStd_TexPVR.cpp | PVR compressed texture support |
| `IcShader` | IcShader.cpp | GLSL shader compilation, uniform caching |
| `IcRenderEngText` | IcRenderEngText.cpp | Text rendering engine |

### 3.3 ctl - Utility Library

**Location:** `src/ctl/`, declared in `inc/ctl.h`

| Type | Purpose |
|------|---------|
| `ctl::Sp<T>` | `std::shared_ptr<T>` alias |
| `ctl::makeSp<T>()` | `std::make_shared<T>()` alias |
| `ctl::SpAry<T>` | `std::vector<ctl::Sp<T>>` with `add()`, `runAll()` |
| `ctl::SpMap<T>` | `std::map<string, ctl::Sp<T>>` with `store()`, `lookup()` |
| `ctl::TPos` / `TSize` / `TRect` | 2D geometry types |
| `ctl::IcImg` | Image class with pixel access, PNG load/save |
| `ctl::BinBuf` | Binary buffer for raw data |
| `LogHandler` | Logging system (DBG, INFO, WARN, ERR levels) |

### 3.4 IcVR - Virtual Reality Module

**Location:** `src/IcVR/`

| Class | Purpose |
|-------|---------|
| `IcWindowVR` | VR window with stereo L/R rendering |
| `IcWindowVR::VRContext` | Holds L/R render target textures |
| `IcWindowVR::VRScnMain` | Renders scene to L/R textures from eye positions |
| `IcWindowVR::VRScnDisp` | Displays distorted L/R textures on screen quads |

---

## 4. Class Relationships

### 4.1 Inheritance

```
IcObject (base scene node)
├── IcCamera
│   └── IcCameraVR
└── IcLight

CRenderAdp (abstract adapter)
└── IcRenderAdpStd (OpenGL ES 2.0)
    ├── CMeshAdpStd (VBO mesh)
    └── CTexAdpStd (textures)
        └── CRenderTexAdp (render target)

IcWindow (base window)
└── IcWindowVR (VR stereo window)

IcScene (base scene)
├── VRScnMain (VR render scene)
└── VRScnDisp (VR display scene)

IcApp (base application)
└── DemoApp, VRDemoApp, MyTestApp (user apps)
```

### 4.2 Composition (Runtime Hierarchy)

```
IcApp (singleton)
├── m_cfg { m_sPathRes, m_sPathDoc }
└── IcWinMng (singleton)
    └── IcWindow[] (m_winAry)
        ├── m_scnAry[] (IcScene)
        │   ├── m_pCamera (IcCamera)
        │   ├── m_lights[] (IcLight)
        │   ├── m_texts[] (IcText)
        │   ├── m_rootObj (IcObject)
        │   │   └── m_childObjs[] (IcObject) -- recursive tree
        │   │       └── m_pModel (IcModel)
        │   │           ├── m_pMesh (IcMesh)
        │   │           │   └── m_renderAdp (CMeshAdp)
        │   │           ├── m_pMat (IcMaterial)
        │   │           └── m_pTex (IcTexture)
        │   │               └── m_pRenderAdp (CTexAdp)
        │   └── m_subScns[] (IcScene)
        └── m_cfg { m_bkColor, m_size, m_pos }
```

### 4.3 Singletons

| Singleton | Type | Notes |
|-----------|------|-------|
| `IcApp::getInstance()` | Raw pointer | Set via `setInstance()` |
| `IcWinMng::getInstance()` | `ctl::Sp<>` | Can be replaced via `setInstance()` |
| `IcEng::getInstance()` | `ctl::Sp<>` | Auto-created on first access |
| `IcRenderEng::getInstance()` | Raw pointer | **Memory leak** - never deleted |

---

## 5. Entry Points & Lifecycle

### 5.1 Android

```
Java Layer (IcEngJNI.java, IcEngView.java, User Activity)
    │
    ├── onCreate()
    │   └── IcEngJNI.initIcApp(context)
    │       ├── createIcApp()         → Sets IcApp singleton (via IMPLEMENT_ICAPP macro)
    │       ├── initIcAppWithDir()    → IcApp::onInit() (user override: create windows)
    │       └── copyAssetDir()        → Copies IcData/ from APK assets to cache
    │
    ├── onSurfaceCreated()
    │   └── initWindow()              → IcWinMng::initWindows()
    │                                   → IcWindow::initWindow()
    │                                     → IcEng::initEng() (loads shaders)
    │                                     → IcWindow::onInit() (user override: create scenes)
    │
    ├── onSurfaceChanged(w, h)
    │   └── setScreenSize()           → IcWinMng::onScreenSize()
    │                                   → IcScene::onWindowSize()
    │
    └── onDrawFrame()  (~60fps, RENDERMODE_CONTINUOUSLY)
        └── drawUpdate(dt)            → IcWinMng::drawUpdate()
                                        → IcWindow::onDrawUpdate()
                                          → IcScene::onUpdate(dt)
                                          → IcScene::onDraw()
                                            → IcEng::onFrameStart()
                                            → IcScene::renderThis()
                                              → renderObjRecur() (scene graph traversal)
                                            → IcEng::onFrameEnd()
```

**Key Files:**
- `proj/android/IcEng/IcEngLib/src/main/java/com/simviu/IcEng/IcEngJNI.java`
- `proj/android/IcEng/IcEngLib/src/main/java/com/simviu/IcEng/IcEngView.java`
- `src/IcApp/android/IcEngJNI.cpp`

### 5.2 iOS

```
Obj-C++ Layer (IcViewController.mm, GLKViewController)
    │
    ├── viewDidLoad()
    │   └── initIcApp(pApp)           → Sets IcApp singleton
    │       → IcApp::onInit()
    │       → EAGLContext setup (ES 2.0)
    │
    └── glkView:drawInRect: (called by CADisplayLink)
        ├── First call: Ic3d_onInitWindow() → IcWinMng::initWindows()
        ├── Ic3d_onViewRect()         → IcWinMng::onScreenSize()
        └── Ic3d_onDrawUpdate(dt)     → Same as Android drawUpdate()
```

### 5.3 PC (Linux/macOS/Windows)

```cpp
// user main.cpp
int main(int argc, char* argv[]) {
    MyApp app;
    app.runCmdLine(argc, argv, "../../../../../IcData/");
}

// In IcApp::runCmdLine():
//   1. setInstance(this)
//   2. Create IcWinMngGlut (GLUT window manager)
//   3. pMng->initMng(argc, argv)  → glutInit(), glutCreateWindow()
//   4. onInit()                    → User creates windows/scenes
//   5. pMng->startMainLoop()       → glutMainLoop()

// GLUT callbacks:
//   cbk_display()   → IcWindow::onDrawUpdate() → glutSwapBuffers()
//   cbk_reshape()   → IcWindow::onWindowSize()
//   cbk_keyboard()  → IcWindow::onKeyboard()
//   cbk_mouse*()    → IcWindow::onMouse*()
```

---

## 6. JNI Interface (Android)

### 6.1 Java-to-C++ Mapping

| Java Method | C++ Function | Purpose |
|-------------|--------------|---------|
| `IcEngJNI.createIcApp()` | `Java_..._createIcApp` | Sets `IcApp` singleton |
| `IcEngJNI.initIcAppWithDir(res, doc)` | `Java_..._initIcAppWithDir` | Calls `IcApp::onInit()` |
| `IcEngJNI.initWindow()` | `Java_..._initWindow` | `IcWinMng::initWindows()` |
| `IcEngJNI.setScreenSize(w, h)` | `Java_..._setScreenSize` | `IcWinMng::onScreenSize()` |
| `IcEngJNI.drawUpdate(dt)` | `Java_..._drawUpdate` | `IcWinMng::drawUpdate()` |
| `IcEngJNI.releaseWindow()` | `Java_..._releaseWindow` | `IcWinMng::releaseWindows()` |
| `IcEngJNI.updateDeviceRot(x,y,z,w)` | `Java_..._updateDeviceRot` | `IcWindow::onDeviceRot()` |
| `IcEngJNI.sendAppCmd(cmd)` | `Java_..._sendAppCmd` | `IcApp::onCmd()` → returns string |
| `IcEngJNI.debugPrint(msg)` | `Java_..._debugPrint` | `__android_log_print()` |

### 6.2 User App Registration (Android)

```cpp
// In user's C++ code (e.g., IcAppJNI.cpp):
#include "MyTestApp.h"
#include "IcEngJNI.h"

IMPLEMENT_ICAPP(MyTestApp)
```

The `IMPLEMENT_ICAPP(T)` macro (in `inc/IcEngJNI.h`) expands to:
```cpp
static T l_app;  // Static instance
extern "C" JNIEXPORT void JNICALL Java_com_simviu_IcEng_IcEngJNI_createIcApp(...) {
    Ic3d::IcEngJNI::setIcAppInstance(&l_app);
}
```

### 6.3 Asset Pipeline (Android)

1. Assets packaged in APK under `app/src/main/assets/IcData/`
2. `IcAssetHelper.copyAssetsDir("IcData")` copies to `context.getCacheDir()/IcData/`
3. Native code reads from cache: `IcApp::m_cfg.m_sPathRes` → `/data/user/0/<pkg>/cache/IcData/`
4. **Important:** Android AssetManager does NOT follow symlinks. Assets must be real files.

---

## 7. Key APIs & Usage Patterns

### 7.1 Creating an Application

```cpp
class MyApp : public Ic3d::IcApp {
    virtual void onInit() override {
        logInfo("MyApp::onInit()");
        
        // Create window
        auto pWin = ctl::makeSp<IcWindow>();
        addWindow(pWin);
    }
};

// Android: IMPLEMENT_ICAPP(MyApp) in JNI cpp
// PC:      MyApp app; app.runCmdLine(argc, argv, "path/to/IcData/");
// iOS:     [viewController initIcApp:&app];
```

### 7.2 Creating a Scene

```cpp
void MyWindow::onInit() {
    auto pScn = ctl::makeSp<IcScene>();
    
    // Scene config
    pScn->m_cfg.m_bkColor = TColor(0.2, 0.5, 0.7, 1.0);
    pScn->m_cfg.m_enClrScrn = true;
    
    // Camera setup
    auto pCam = pScn->getCamera();
    pCam->setPos(TVec3(10, 4, -6));
    pCam->lookAt(TVec3(0,0,0), TVec3(0,1,0));
    pCam->m_cfg.m_FOV = 50.0f;
    pCam->m_cfg.m_zNear = 0.1f;
    pCam->m_cfg.m_zFar = 1000.0f;
    
    // Add lights
    auto pLight = ctl::makeSp<IcLight>(0);
    pLight->setLightColor(TColor(1,1,1,1));
    pLight->pointAt(TVec3(0,0,-1));
    pScn->addLight(pLight);
    
    // Add objects
    IcMeshData mshd;
    mshd.createCube(TVec3(1,1,1), TVec3(0,0,0));
    auto pModel = makeSp<IcModel>(mshd);
    auto pObj = makeSp<IcObject>(pModel);
    pScn->addObj(pObj);
    
    // Update callback
    pScn->setOnUpdatCallBack([pObj](float dt) {
        static float deg = 0;
        deg += 30.0f * dt;
        pObj->setQuat(TQuat(TVec3(0, deg2rad(deg), 0)));
    });
    
    addScene(pScn);
}
```

### 7.3 Loading Models from OBJ

```cpp
// OBJ files loaded relative to m_sPathRes
auto pModel = ctl::makeSp<IcModel>("IcDemo/monkey/monkey.obj");
auto pObj = ctl::makeSp<IcObject>(pModel);
pScn->addObj(pObj);
```

### 7.4 Object Transform Hierarchy

```cpp
// Parent-child transforms are multiplied
auto pParent = ctl::makeSp<IcObject>(parentModel);
pParent->setPos(TVec3(0, 2, 0));

auto pChild = ctl::makeSp<IcObject>(childModel);
pChild->setPos(TVec3(1, 0, 0));  // Relative to parent

pParent->addChildObj(pChild);
pScn->addObj(pParent);
```

### 7.5 Procedural Meshes

```cpp
IcMeshData mshd;
mshd.createCube(TVec3(sx, sy, sz), TVec3(ox, oy, oz));
mshd.createSphere(radius, N_stack, N_slice);
mshd.createPlaneXY(rect, texRect);
mshd.createGridXY(rect, Nx, Ny, texRect);
// TODO: createCylinder(), createCone() not implemented
```

---

## 8. Build System

### 8.1 Android (Gradle + CMake + NDK)

| Project | Package | Main Activity | Dependencies |
|---------|---------|---------------|--------------|
| `IcEng` | `com.simviu.IcEng` | `MyTestActivity` | Builds `IcEngLib` AAR + test app |
| `IcEngDemo` | `com.simviu.IcEngDemo` | `DemoListActivity` | Uses `IcEngLib-debug.aar` |
| `IcVRDemo` | `com.simviu.IcVRDemo` | `MainActivity` | Uses `IcEngLib-debug.aar` |

**Build commands:**
```bash
cd proj/android/IcEng
./gradlew clean :app:assembleDebug --no-daemon
./gradlew :IcEngNative:assembleDebug  # Build native lib only
./gradlew :IcEngLib:assembleDebug     # Build AAR only
```

**Native library:** `libIcEng.so` + `libIcAppJNI.so` (per-app)

### 8.2 PC (CMake + GLUT/GLEW)

```bash
cd proj/linux/IcEngLib
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

Output: `IcEngBuild/linux/lib/<arch>/Debug/libIcEng.so`

### 8.3 iOS/macOS (Xcode)

- macOS: `proj/mac/IcEng/` → outputs to `IcEngBuild/mac/lib/${ARCHS}/`
- iOS: `proj/ios/` → uses `IcViewController.mm`

### 8.4 Required Dependencies

| Platform | Dependencies |
|----------|-------------|
| Android | NDK 27+, CMake, OpenGL ES 2.0 |
| iOS | GLKit, OpenGLES, Xcode |
| PC | GLUT, GLEW, OpenGL, X11 (Linux) |

---

## 9. Rendering Pipeline

### 9.1 Frame Flow

```
IcWindow::onDrawUpdate(dt)
    │
    ├── [Thread lock: m_mtx_draw]
    │
    ├── IcEng::onFrameStart()
    │   └── Enable GL state (depth test, blend, etc.)
    │
    ├── IcEng::clearScreen(bkColor)
    │   └── glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    │
    ├── For each scene in m_scnAry:
    │   ├── IcScene::onUpdate(dt)          // User logic
    │   └── IcScene::onDraw()
    │       ├── Camera: updateViewMat()
    │       ├── Set viewport (from viewRect)
    │       ├── Apply fog parameters
    │       ├── Set lights (transform to eye space)
    │       ├── renderObjRecur(cam, rootObj, identityMatrix)
    │       │   ├── Calculate modelMatrix = parentMatrix * obj.calcMat()
    │       │   ├── IcShader: applyMatrix(modelMatrix, viewMatrix, projMatrix)
    │       │   ├── IcShader: applyMaterial, applyLights, applyFog
    │       │   ├── obj.getModel().draw()
    │       │   │   └── mesh.draw() → adapter.glDrawElements()
    │       │   └── Recurse children
    │       └── Draw text overlays
    │
    ├── IcEng::onFrameEnd()
    │
    └── [Unlock: m_isDrawing = false]
```

### 9.2 Shader Architecture

**Vertex Shader** (`IcShaderVert.vsh`):
- Inputs: `a_vert`, `a_texCo`, `a_normal`
- Uniforms: `u_matProj`, `u_matModelView`, `u_matNormal`
- Outputs: `v_texCo`, `v_ecNormal`, `v_ecVert`

**Fragment Shader** (`IcShaderFrag.vsh`):
- 16 lights max (`K_MAX_LIGHTS`)
- Material: ambient, diffuse, specular, emissive, shininess
- Fog: linear + exponential
- Spotlight: cone angle, cutoff, attenuation
- Single texture sampler (`u_texSampler`)

### 9.3 Texture Requirements

- Must be **power-of-2 square** (2^n × 2^n)
- Non-conforming textures are auto-resized
- Formats: PNG (lodepng), PVR (PowerVR compression)

---

## 10. Math Types (GLM Wrappers)

```cpp
namespace Ic3d {
    typedef glm::quat   TQuat;    // Quaternion
    typedef glm::vec4   TVec4;    // 4D vector / color
    typedef glm::vec3   TVec3;    // 3D vector / position
    typedef glm::vec2   TVec2;    // 2D vector
    typedef glm::mat3   TMat3;    // 3x3 matrix
    typedef glm::mat4   TMat4;    // 4x4 matrix
    typedef glm::vec4   TColor;   // RGBA color (0-1 range)
}
```

**Useful functions:**
- `deg2rad(deg)` / `rad2deg(rad)`
- `v2s(val)` - value to string
- `v2hex(ptr)` - pointer to hex string

---

## 11. Container Patterns

### 11.1 ctl::SpAry<T> - Shared Pointer Array

```cpp
ctl::SpAry<IcObject> m_childObjs;
m_childObjs.add(pObj);           // Add element
auto p = m_childObjs[0];         // Access by index
m_childObjs.clear();             // Clear all

// Iterate with lambda
m_childObjs.runAll([](size_t idx, ctl::Sp<IcObject> p) {
    p->draw();
});

// Get raw array
for(auto p : m_childObjs.getAry()) {
    p->update(dt);
}
```

### 11.2 ctl::SpMap<T> - Named Shared Pointer Map

```cpp
ctl::SpMap<IcTexture> m_texLib;
m_texLib.store("wood", pTex);     // Store by name
auto p = m_texLib.lookup("wood"); // Retrieve by name
```

---

## 12. Logging System

```cpp
// In C++ code:
logDbg("Debug message");      // DEBUG level
logInfo("Info message");      // INFO level
logWarn("Warning message");   // WARNING level
logErr("Error message");      // ERROR level

// Log output routed to:
// - Android: __android_log_print (tag: "IcAppJNI")
// - PC: stdout/stderr
// - iOS: NSLog
```

---

## 13. Tech Tips & Gotchas

### 13.1 Asset Paths

| Platform | Path Resolution | Example |
|----------|----------------|---------|
| Android | `getCacheDir() + "/IcData/"` | `/data/user/0/com.simviu.IcEng/cache/IcData/` |
| iOS | `[NSBundle mainBundle] + "/IcData/"` | `/var/containers/.../IcData/` |
| PC | Passed in `runCmdLine()` | `../../../../../IcData/` |

**Rule:** All asset paths in code are **relative to `m_sPathRes`**.

### 13.2 Android Asset Packaging

- Assets go in `app/src/main/assets/IcData/`
- Must be **real files**, not symlinks (AssetManager ignores symlinks)
- Copied to cache directory on first launch by `IcAssetHelper`

### 13.3 Thread Safety

- `IcWindow::m_mtx_draw` protects the render loop
- `m_isDrawing` atomic prevents re-entrant draws
- Scene graph is **not** protected for modification during render
- **Rule:** Modify scene graph in `onUpdate()`, not in `onDraw()`

### 13.4 VBO Memory Management

- `CMeshAdpStd` creates temporary buffers during `glBufferData()`
- These are held in member variables (`m_aryPoint`, `m_bufPnt`) but not freed
- **Not a leak** - buffers are reused, but memory is retained

### 13.5 Quaternion Convention

- GLM quaternions: `glm::quat(angle, axis)` where axis is normalized
- Android sensor data requires axis swap: `TQuat(x, z, w, y)` (note y/z swap)
- Device rotation correction: `q0 * q` where `q0 = quat(deg2rad(-90), X_AXIS)`

### 13.6 Shader Location Caching

- Uniform/attribute locations cached in `TShaderData` after compilation
- Invalid locations set to `-1` (`K_invld`)
- If shader compilation fails, render adapter creation fails silently

### 13.7 Frame Rate

- Android: `IcEngView` calculates dt from `System.currentTimeMillis()`
- Initial frame uses `K_initDeltaT = 0.01f` (10ms)
- dt capped at 1.0s to prevent spiral-of-death

---

## 14. Known Issues & TODOs

### 14.1 Critical (Fix Soon)

| Location | Issue | Impact |
|----------|-------|--------|
| `IcRenderEng::getInstance()` | Raw `new`, never deleted | Memory leak (one-time) |
| `IcWindow` / derived classes | No virtual destructor | `-Wdelete-non-abstract-non-virtual-dtor` warnings |
| `IcAppPC.cpp` | `N_MAX_GLUT_WIN = 8` hardcoded | PC limited to 8 windows |
| Asset copy | No version check, always copies | Slow startup on large assets |

### 14.2 Medium Priority

| Location | Issue | Notes |
|----------|-------|-------|
| `Ic3d.h:50-51` | `createCylinder()`, `createCone()` not implemented | TODO since 2016 |
| `Ic3d.h:239` | `update(float deltaT)` should use double | Precision loss over time |
| `Ic3d.h:270` | Camera view matrix not auto-updated | Must call `updateViewMat()` manually |
| `Ic3d.h:426` | Render-to-texture incomplete | `m_pTargetTex` exists but not wired |
| `Ic3d.h:418` | Frame count in IcScene, should be in IcWindow | Architectural mismatch |
| `ctl.h:160` | `TPosT` naming inconsistent with `TVec2` | Confusing type names |
| `IcRenderAdpStd.cpp` | Uniform sends scattered | Should move to IcShader |

### 14.3 Low Priority / Technical Debt

| Issue | Notes |
|-------|-------|
| Namespace `Ic3d` should be `IcEng` | Planned rename (TODO in header) |
| `TLight` should be `TLightPara` | Naming inconsistency |
| `IcFrameRateCalc` commented out | Chrono support now universal |
| VBO temp buffers should be local | Currently retained in CMeshAdpStd |
| Minimal `glGetError()` calls | Debugging OpenGL errors is difficult |
| File path case sensitivity | OBJ loader may fail on Linux |

---

## 15. Development Workflow

### 15.1 Adding a New Demo App

1. **Create C++ app class:**
   ```cpp
   // src/demo/MyApp/MyApp.hpp
   class MyApp : public IcApp {
       virtual void onInit() override;
   };
   
   // src/demo/MyApp/MyApp.cpp
   void MyApp::onInit() {
       IcApp::onInit();
       auto pWin = makeSp<IcWindow>();
       addWindow(pWin);
   }
   ```

2. **Create Android project:**
   - Copy `proj/android/IcEngDemo` → `proj/android/MyApp`
   - Update `applicationId`, `namespace`, package names
   - Update `app/src/main/cpp/IcAppJNI.cpp`:
     ```cpp
     #include "MyApp.hpp"
     #include "IcEngJNI.h"
     IMPLEMENT_ICAPP(MyApp)
     ```

3. **Add assets:**
   - Copy `IcData/` to `app/src/main/assets/IcData/`

4. **Build and test:**
   ```bash
   cd proj/android/MyApp
   ./gradlew :app:assembleDebug
   adb install -r app/build/outputs/apk/debug/app-debug.apk
   ```

### 15.2 Modifying Core Engine

1. **Edit headers:** `inc/Ic3d.h` (monolithic) or `inc/core/*.h`
2. **Edit implementation:** `src/<module>/<file>.cpp`
3. **Rebuild:**
   - Android: `cd proj/android/IcEng && ./gradlew :IcEngNative:assembleDebug`
   - PC: `cd proj/linux/IcEngLib && cmake --build build`
4. **Copy .so to demo apps** (if using flatDir AAR):
   ```bash
   cp IcEngBuild/android/lib/Debug/arm64-v8a/libIcEng.so \
      proj/android/IcEngDemo/app/src/main/jniLibs/arm64-v8a/
   ```

### 15.3 Debugging

| Task | Command |
|------|---------|
| View logs | `adb logcat \| grep IcAppJNI` |
| Clear logs | `adb logcat -c` |
| Force stop app | `adb shell am force-stop com.simviu.IcEng` |
| Launch activity | `adb shell am start -n com.simviu.IcEng/.MyTestActivity` |
| Screen awake | `adb shell svc power stayon true` |
| GL errors | Add `IcEng::getGLErrCode()` calls in render loop |

---

## 16. Reference Files

| File | Purpose |
|------|---------|
| `inc/Ic3d.h` | **Main header** - all engine class declarations |
| `inc/IcRenderAdp.h` | Render adapter interface + data structures |
| `inc/IcEngJNI.h` | JNI macro + interface |
| `inc/ctl.h` | Utility library (Sp, SpAry, SpMap, etc.) |
| `src/IcApp/android/IcEngJNI.cpp` | JNI implementation |
| `src/IcApp/IcWindow.cpp` | Window lifecycle (init/draw/release) |
| `src/Ic3d/IcScene.cpp` | Scene rendering pipeline |
| `src/IcRenderEng/IcShader.cpp` | Shader compilation |
| `src/test/MyTestApp.cpp` | Minimal reference app |
| `src/demo/IcEngDemo/DemoScene.cpp` | Multi-demo factory |
| `IcData/IcShader/*.vsh` | GLSL shaders |
