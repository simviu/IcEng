cmake_minimum_required(VERSION 3.4.1)

# now build app's shared lib
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")
set(ICENGROOT    "${CMAKE_CURRENT_SOURCE_DIR}/../../../../..")
set(ICENGLIB     "${ICENGROOT}/IcEng")
set(ICENGBUILD   "${ICENGROOT}/IcEng/IcEngBuild")
set(ICENGBUILD_BIN 	"${ICENGBUILD}/android/lib/${CMAKE_BUILD_TYPE}/${ANDROID_ABI}")

add_definitions (-D_HAS_EXCEPTIONS=1)

include_directories(
    ${ICENGLIB}/inc
    ${ICENGLIB}/inc/core
    ${ICENGLIB}/src/test
    )

link_directories(${ICENGBUILD_BIN})

add_library(IcAppJNI SHARED
    		${ICENGLIB}/src/test/MyTestApp.cpp
            src/main/cpp/IcAppJNI.cpp
			)

# add lib dependencies
target_link_libraries(IcAppJNI
                      IcEng
                      android
                      log
                      EGL
                      GLESv2)


