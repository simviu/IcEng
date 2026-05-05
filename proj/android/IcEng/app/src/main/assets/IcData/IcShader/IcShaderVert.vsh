//
//  IcShaderVert.vsh
//  IcEngLib
//
//  Created by Sherman Chen on 12/24/16.
//  Copyright (c) 2016 Simviu Technology Inc.
//  All rights reserved.
//  http://www.simviu.com/dev
//
//
//
//#version 100
#ifdef GL_ES
precision highp float;
#endif

uniform mat4 u_matProj;
uniform mat4 u_matModelView;
uniform mat3 u_matNormal;

attribute vec4 a_vert;
attribute vec2 a_texCo;
attribute vec3 a_normal;

varying vec2 v_texCo;
varying vec3 v_ecNormal;
varying vec4 v_ecVert;
//-------------------------
//  main
//-------------------------
void main()
{
    // vert/normal in Eye coordinator
    vec4 ecVert     = u_matModelView * a_vert;
    vec3 ecNormal   = u_matNormal * a_normal;

    v_texCo     = a_texCo;
    v_ecVert    = ecVert;
    v_ecNormal  = ecNormal;
    gl_Position = u_matProj * ecVert;

}




