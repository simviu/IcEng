cmake_minimum_required(VERSION 3.4.1)
# Project Name
PROJECT(ICENG_LIB)
IF( NOT CMAKE_BUILD_TYPE )
   SET( CMAKE_BUILD_TYPE Debug )
ENDIF()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")
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
# IcEngDemo
#########################################################

set(ICENGDEMO "${CMAKE_CURRENT_SOURCE_DIR}/../../../../IcEng/src/demo")
set(ICENGLIB   "${CMAKE_CURRENT_SOURCE_DIR}/../../../../IcEng")
set(ICENGBUILD "${CMAKE_CURRENT_SOURCE_DIR}/../../../../IcEng/IcEngBuild")
set(ICENGBUILD_LIB 	"${ICENGBUILD}/linux/lib/${CMAKE_SYSTEM_PROCESSOR}/${CMAKE_BUILD_TYPE}")
set(BUILD_EXE_DIR "${ICENGBUILD}/linux/bin/${CMAKE_SYSTEM_PROCESSOR}/${CMAKE_BUILD_TYPE}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BUILD_EXE_DIR}")

include_directories(
	${OpenGL_INCLUDE_DIRS}
	${ICENGLIB}/inc 
	${ICENGDEMO}/VRDemo/
	)

add_executable(IcVRDemo
	${ICENGDEMO}/VRDemo/main.cpp		
	${ICENGDEMO}/VRDemo/VRDemoApp.cpp		
	)


target_link_libraries(IcVRDemo 
	${ICENGBUILD_LIB}/libIcEng.so
	${OPENGL_LIBRARIES} 
	${GLUT_LIBRARY} 
	${GLEW_LIBRARY} 
	)


