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
    )

add_library(IcEng SHARED
    #------ IcEng src
    			${ICENGLIB}/src/ctl/ctlBinBuf.cpp
    			${ICENGLIB}/src/ctl/ctlImg.cpp
    			${ICENGLIB}/src/ctl/ctlLog.cpp
    			${ICENGLIB}/src/ctl/ctlStr.cpp
    			${ICENGLIB}/src/ctl/ctlTest.cpp
    			${ICENGLIB}/src/ctl/ctlTest_img.cpp
    			${ICENGLIB}/src/ctl/ctlUtil.cpp
    			${ICENGLIB}/src/ctl/lodepng.cpp
    			${ICENGLIB}/src/IcRenderEng/IcRenderAdpStd.cpp
    			${ICENGLIB}/src/IcRenderEng/IcRenderAdpStd_Mesh.cpp
    			${ICENGLIB}/src/IcRenderEng/IcRenderAdpStd_Tex.cpp
    			${ICENGLIB}/src/IcRenderEng/IcRenderAdpStd_TexPVR.cpp
    			${ICENGLIB}/src/IcRenderEng/IcRenderEng.cpp
    			${ICENGLIB}/src/IcRenderEng/IcRenderEngText.cpp
    			${ICENGLIB}/src/IcRenderEng/IcShader.cpp
    			${ICENGLIB}/src/Ic3d/Ic3d.cpp
    			${ICENGLIB}/src/Ic3d/IcCamera.cpp
    			${ICENGLIB}/src/Ic3d/IcLight.cpp
    			${ICENGLIB}/src/Ic3d/IcMaterial.cpp
    			${ICENGLIB}/src/Ic3d/IcMesh.cpp
    			${ICENGLIB}/src/Ic3d/IcMeshDataCreate.cpp
    			${ICENGLIB}/src/Ic3d/IcMeshDataSubMsh.cpp
    			${ICENGLIB}/src/Ic3d/IcModel.cpp
    			${ICENGLIB}/src/Ic3d/IcModelLoaderObj.cpp
    			${ICENGLIB}/src/Ic3d/IcObject.cpp
    			${ICENGLIB}/src/Ic3d/IcScene.cpp
    			${ICENGLIB}/src/Ic3d/IcText.cpp
    			${ICENGLIB}/src/Ic3d/IcTexture.cpp
    			${ICENGLIB}/src/Ic3d/IcUtil.cpp
    			${ICENGLIB}/src/IcVR/IcSceneVR.cpp
    			${ICENGLIB}/src/IcVR/IcWindowVR.cpp
    			${ICENGLIB}/src/IcApp/IcApp.cpp
    			${ICENGLIB}/src/IcApp/IcWindow.cpp
    			${ICENGLIB}/src/IcApp/IcWinMng.cpp
    			${ICENGLIB}/src/IcApp/android/IcEngJNI.cpp
			)



# add lib dependencies
target_link_libraries(IcEng
                      android
                      log
                      EGL
                      GLESv2)


#----------------------------------
#	IcEng Post-build copy out
#----------------------------------
add_custom_command(TARGET IcEng PRE_BUILD
           COMMAND ${CMAKE_COMMAND} -E make_directory
		   ${ICENGBUILD_BIN}
			)

#---- Copy IcEng Lib Build out
add_custom_command(TARGET IcEng POST_BUILD
           COMMAND ${CMAKE_COMMAND} -E copy
		   $<TARGET_FILE:IcEng>
		   ${ICENGBUILD_BIN}
			)
