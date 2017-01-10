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
    vec4 ecVert     = u_matModelView * a_vert;   // vert in Eye coordinator
    vec3 ecNormal   = u_matNormal * a_normal;

    v_texCo     = a_texCo;
    v_ecVert    = ecVert;
    v_ecNormal  = ecNormal;
    gl_Position = u_matProj * ecVert;

}




