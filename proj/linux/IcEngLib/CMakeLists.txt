cmake_minimum_required(VERSION 3.4.1)
# Project Name
PROJECT(ICENG_LIB)
IF( NOT CMAKE_BUILD_TYPE )
   SET( CMAKE_BUILD_TYPE Debug )
ENDIF()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -D ${CMAKE_BUILD_TYPE}")
#########################################################
# FIND GLUT
#########################################################
find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIRS})
link_directories(${GLUT_LIBRARY_DIRS})
add_definitions(${GLUT_DEFINITIONS})
if(NOT GLUT_FOUND)
    message(ERROR " GLUT not found!")
endif(NOT GLUT_FOUND)


#########################################################
# FIND OPENGL
#########################################################
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    include_directories(${GLEW_INCLUDE_DIRS})
    link_libraries(${GLEW_LIBRARIES})
endif()

link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})
if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)

#########################################################
# Include Files
#########################################################
set(ICENGLIB "${CMAKE_CURRENT_SOURCE_DIR}/../../../../IcEng")
set(ICENGBUILD "${CMAKE_CURRENT_SOURCE_DIR}/../../../IcEngBuild")

set(BUILD_LIB_DIR "${ICENGBUILD}/linux/lib/${CMAKE_HOST_SYSTEM_PROCESSOR}/${CMAKE_BUILD_TYPE}")
set(BUILD_EXE_DIR "${ICENGBUILD}/linux/bin/${CMAKE_HOST_SYSTEM_PROCESSOR}/${CMAKE_BUILD_TYPE}")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${BUILD_LIB_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BUILD_EXE_DIR}")

include_directories(
	${OpenGL_INCLUDE_DIRS}
	${ICENGLIB}/inc 
	${ICENGLIB}/inc/core
	)

add_library(IcEng SHARED
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
			${ICENGLIB}/src/IcApp/IcApp.cpp
			${ICENGLIB}/src/IcApp/IcWindow.cpp
			${ICENGLIB}/src/IcApp/IcWinMng.cpp
			${ICENGLIB}/src/IcApp/pc/IcAppPC.cpp
			)

add_executable(IcEngTest 
		${ICENGLIB}/src/test/MyTestApp.cpp
		${ICENGLIB}/src/test/main.cpp
		)

target_link_libraries(IcEng ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${GLEW_LIBRARIES})
target_link_libraries(IcEngTest IcEng ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${GLEW_LIBRARIES})

add_custom_command(TARGET IcEngTest PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E make_directory 
		   ${BUILD_LIB_DIR} 
		   ${BUILD_EXE_DIR} 
 		)

#add_custom_command(TARGET IcEng POST_BUILD
#                   COMMAND ${CMAKE_COMMAND} -E copy 
#		   $<TARGET_FILE:IcEng>
#		   ${BUILD_LIB_DIR}
#			)



