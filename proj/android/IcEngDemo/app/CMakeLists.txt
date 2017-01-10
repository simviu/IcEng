cmake_minimum_required(VERSION 3.4.1)

# now build app's shared lib
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")
set(ICENGROOT    "${CMAKE_CURRENT_SOURCE_DIR}/../../../..")
set(ICENGLIB     "${ICENGROOT}/IcEngLib")
set(ICENGBUILD   "${ICENGROOT}/IcEngBuild")
set(ICENGDEMO    "${ICENGROOT}/IcEngDemo")
set(ICENGBUILD_BIN 	"${ICENGBUILD}/android/lib/${CMAKE_BUILD_TYPE}/${ANDROID_ABI}")

add_definitions (-D_HAS_EXCEPTIONS=1)

include_directories(${ICENGLIB}/inc ${ICENGDEMO}/src)
link_directories(${ICENGBUILD_BIN})

add_library(IcEngDemo SHARED
    #------ IcEngDemo src
	src/main/cpp/native-lib.cpp
    ${ICENGDEMO}/src/DemoScene.cpp
    ${ICENGDEMO}/src/DemoAnimation.cpp
    ${ICENGDEMO}/src/DemoBasic.cpp
    ${ICENGDEMO}/src/DemoLights.cpp
    ${ICENGDEMO}/src/DemoModelCreate.cpp
    ${ICENGDEMO}/src/DemoModels.cpp
    ${ICENGDEMO}/src/DemoNestedTrans.cpp
    ${ICENGDEMO}/src/DemoTransform.cpp
    ${ICENGDEMO}/src/DemoWindow.cpp
			)



# add lib dependencies
target_link_libraries(IcEngDemo
                      IcEng
                      android
                      log
                      EGL
                      GLESv2)
