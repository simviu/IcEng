//
//  IcShaderFrag.vsh
//  IcEngLib
//
//  Created by Sherman Chen on 12/24/16.
//  Copyright (c) 2016 Simviu Technology Inc.
//  All rights reserved.
//  http://www.simviu.com/dev
//
//
//


 #ifdef GL_ES
 precision highp float;
 #endif
 
uniform sampler2D u_texSampler;

varying vec2 v_texCo;
varying vec3 v_ecNormal;
varying vec4 v_ecVert;

#define K_MAX_LIGHTS 16

//-------------------------
//  TMaterial
//-------------------------
struct TMaterial
{
    vec4    amb;
    vec4    dfs;
    vec4    spc;
    vec4    ems;
    float   shns;
    float   alpha;
};

//-------------------------
//  TLight
//-------------------------
struct TLight
{
    vec3    dir;
    vec3    pos;
    float   isPara;     // 1.0 para, 0.0 spot
    float   Kd2;        // atn = 1/(1 + Kd2 * dist^2)
    float   cosConeAngle;
    float   cosCutOff;
    TMaterial   mat;
};


//-------------------------
//  TFogPara
//-------------------------
struct TFogPara
{
    float   start;
    float   k_linear;
    float   k_exp;
    vec4    color;
};

//-------------------------
//  Uniform of Material/Light
//-------------------------
uniform TMaterial   u_material;
uniform TLight      u_lightAry[K_MAX_LIGHTS];
uniform TFogPara    u_fogPara;
uniform int         u_lightNum;


//-------------------------
//  calcFog
//-------------------------
vec4 calcFog(TFogPara para, vec4 c_in, vec4 ecVert)
{
    float L = length(ecVert)-para.start;
    float f0 = para.k_linear*L; // Linear
    float f1 = 1.0 - 1.0/exp(L*para.k_exp);  // Exp
    float f = f0 + f1;
  //  f = 0.7; // dbg
    vec4 c = mix(c_in, para.color, f);
    return c;
}


//-------------------------
//  spotLightCoef
//-------------------------
float calcSpotLight(TLight light, vec3 L_spot)
{
    float a0 = light.cosConeAngle;
    float a1 = light.cosCutOff;
    
    vec3 L_dir = normalize(-light.dir);
    float ang = dot(L_spot, L_dir);
    float edge = max(0.0, (a0 - ang)/max(0.0001,  a0 - a1));
    float coef = clamp(mix(1.0, 0.0, edge), 0.0, 1.0);
//    coef = (ang > a1)? 1.0 : 0.0;
    coef = (light.isPara == 1.0)? 1.0 : coef;
    return coef;
}

//-------------------------
//  calcColor
//-------------------------
vec4 calcColor(TMaterial mtl, TLight light, vec3 ecNormal, vec4 ecVert)
{
    //--- Normal will be defected by scaling transform,
    // so a pre-calculated normalMatrix is provided
    // to bring the model normal into eye space.
    
    //---- Parallel Light Light
    vec3 L_dir = normalize(-light.dir);
    
    //---- Spot Light
    vec3 L_spot = normalize(light.pos - ecVert.xyz);
    float spotCoef = calcSpotLight(light, L_spot);
   
    //---- Check Parallel/Spot light
    vec3 L0 = (light.isPara == 1.0)? L_dir : L_spot;
    vec3 N = normalize(ecNormal);
    vec3 L = normalize(L0);
    vec3 E = vec3(0.0, 0.0, 1.0);
    vec3 H = normalize(L + E);
    float df = max(0.0, dot(N, L));
    float sf = max(0.0, dot(N, H));
    sf = pow(sf, mtl.shns);
    
    TMaterial ml = light.mat;   // Light Material
    float d = distance(light.pos, ecVert.xyz);
    float li = 1.0/(1.0+ light.Kd2 * pow(d,2.0)) * spotCoef ;// Light Attenuetion
    vec4 c = mtl.amb * ml.amb * li + df * mtl.dfs * ml.dfs* li + sf * mtl.spc * ml.spc * li + mtl.ems;
    return c;
    
}



//-------------------------
//  main
//-------------------------
void main()
{
    vec4 c = vec4(0,0,0,1);
    for(int i = 0; i < u_lightNum; ++i)
        c += calcColor(u_material, u_lightAry[i], v_ecNormal, v_ecVert);
    vec4 cAlpha = vec4(c.rgb, u_material.alpha);
    c = texture2D(u_texSampler, v_texCo);
    c = calcFog(u_fogPara, c, v_ecVert);
	gl_FragColor = c * cAlpha;
}
