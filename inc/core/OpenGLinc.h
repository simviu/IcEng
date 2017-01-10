//
//  OpenGLES.h
//  Warbirds
//
//  Created by Sherman Chen on 6/7/13.
//
//

#ifndef _OpenGLinc_H
#define _OpenGLinc_H


#ifdef __APPLE__
#include "TargetConditionals.h"
#endif

//#define USE_OPENGL1					// without shader

//------- iPhone
#if TARGET_OS_IPHONE
	//------- OpenGL
	#ifdef USE_OPENGL1
		#include <OpenGLES/ES1/gl.h>
		#include <OpenGLES/ES1/glext.h>
	#else
		#include <OpenGLES/ES2/gl.h>
		#include <OpenGLES/ES2/glext.h>
	#endif
//------- Glut
#else
	//---- Use GLUT for none iOS
	#ifdef __APPLE__
        #include <GLUT/glut.h>
	//	#include <GLEW/glew.h>

  #elif   ANDROID
  #else //   WIN32/LINUX
        #define _STDCALL_SUPPORTED
        #define _M_IX86
        #define GLEW_STATIC
        #include <GL/glew.h>
    //    #include "GL/glut.h"
	#endif
#endif

#ifdef ANDROID
	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>
#endif // ANDROID

#endif	//Warbirds_OpenGLES_h
