QT += core opengl
QT -= gui

CONFIG += c++11

TARGET = DevEng
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

ICENG = $$PWD/../../src/

INCLUDEPATH += \
    $$ICENG/ext/jsoncpp/inc \
    $$ICENG/ctl \
    $$ICENG/Ic3d \
    $$ICENG/IcRenderEng \
    $$ICENG/platform/win \
    $$ICENG/ext/glm \
    $$ICENG/ext/lodepng \
    $$ICENG/ext/glew/include/ \
    $$ICENG/platform/glut/ \

SOURCES +=  \
    $$ICENG/ctl/ctlBinBuf.cpp       \
    $$ICENG/ctl/ctlImg.cpp       \
    $$ICENG/ctl/ctlKVT.cpp       \
    $$ICENG/ctl/ctlKVTjson.cpp       \
    $$ICENG/ctl/ctlLog.cpp       \
    $$ICENG/ctl/ctlStr.cpp       \
    $$ICENG/ctl/ctlSys.cpp       \
    $$ICENG/ctl/ctlTest.cpp       \
    $$ICENG/ctl/ctlUtil.cpp       \
    $$ICENG/ctl/ctlUtil.cpp       \
    $$ICENG/ctl/ctlUtil.cpp       \
    $$ICENG/ext/lodepng/lodepng.cpp       \
    $$ICENG/ext/jsoncpp/src/json_value.cpp       \
    $$ICENG/ext/jsoncpp/src/json_writer.cpp       \
    $$ICENG/ext/jsoncpp/src/json_reader.cpp       \
    $$ICENG/IcRenderEng/IcRenderAdp.cpp       \
    $$ICENG/IcRenderEng/IcRenderAdpStd.cpp       \
    $$ICENG/IcRenderEng/IcRenderAdpStd_Mesh.cpp       \
    $$ICENG/IcRenderEng/IcRenderAdpStd_Tex.cpp       \
    $$ICENG/IcRenderEng/IcRenderAdpStd_TexPVR.cpp       \
    $$ICENG/IcRenderEng/IcRenderEng.cpp       \
    $$ICENG/IcRenderEng/IcRenderEngText.cpp       \
    $$ICENG/IcRenderEng/IcShader.cpp       \
    $$ICENG/Ic3d/Ic3d.cpp       \
    $$ICENG/Ic3d/IcCamera.cpp       \
    $$ICENG/Ic3d/IcLight.cpp       \
    $$ICENG/Ic3d/IcMaterial.cpp       \
    $$ICENG/Ic3d/IcMesh.cpp       \
    $$ICENG/Ic3d/IcMeshDataCreate.cpp       \
    $$ICENG/Ic3d/IcMeshDataSubMsh.cpp       \
    $$ICENG/Ic3d/IcModel.cpp       \
    $$ICENG/Ic3d/IcModelLoaderObj.cpp       \
    $$ICENG/Ic3d/IcObject.cpp       \
    $$ICENG/Ic3d/IcScene.cpp       \
    $$ICENG/Ic3d/IcTexture.cpp       \
    $$ICENG/Ic3d/IcUtil.cpp       \
    $$ICENG/IcWindow/IcWindow.cpp       \
    $$ICENG/IcWindow/glut/IcWindowGlut.cpp       \
    $$ICENG/test/TestScene.cpp       \

HEADERS += \
    $$ICENG/ctl/ctl.h \
    $$ICENG/ext/jsoncpp/src/json_tool.h \
    $$ICENG/ext/jsoncpp/inc/json/assertions.h \
    $$ICENG/ext/jsoncpp/inc/json/autolink.h \
    $$ICENG/ext/jsoncpp/inc/json/config.h \
    $$ICENG/ext/jsoncpp/inc/json/features.h \
    $$ICENG/ext/jsoncpp/inc/json/forwards.h \
    $$ICENG/ext/jsoncpp/inc/json/json.h \
    $$ICENG/ext/jsoncpp/inc/json/reader.h \
    $$ICENG/ext/jsoncpp/inc/json/value.h \
    $$ICENG/ext/jsoncpp/inc/json/version.h \
    $$ICENG/ext/jsoncpp/inc/json/writer.h \

win32: LIBS += -lopengl32


#win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../OpenGL/lib/x64/glew32s.lib
#else:win32-g++: PRE_TARGETDEPS += $$PWD/../OpenGL/lib/x64/libglew32s.a

DISTFILES +=

win32: LIBS += -L$$PWD/$$ICENG/ext/freeglut/lib/x64/ -lfreeglut

INCLUDEPATH += $$PWD/$$ICENG/ext/freeglut/include
DEPENDPATH += $$PWD/$$ICENG/ext/freeglut/include

#win32:!win32-g++: PRE_TARGETDEPS += $$PWD/$$ICENG/ext/freeglut/lib/x64/freeglut_static.lib
#else:win32-g++: PRE_TARGETDEPS += $$PWD/$$ICENG/ext/freeglut/lib/x64/libfreeglut_static.a

