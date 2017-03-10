//
//  IcShader.cpp
//  DevEng
//
//  Created by Sherman Chen on 9/28/16.
//  Copyright (c) 2016 Simviu Technology Inc.
//  All rights reserved.
//  http://www.simviu.com/dev
//

#include "IcRenderAdp.h"
#include <fstream>
#include <streambuf>
#include <stdlib.h>
#include "OpenGLinc.h"
#include "glm/gtc/type_ptr.hpp"
#include <iostream>


namespace Ic3d
{
    using namespace std;
    using namespace ctl;
    
    //----------------------------------
    //  IcShader
    //----------------------------------
    IcShader::IcShader(const string& sFileVert,
                       const string& sFileFrag)
    {
         m_isValid = loadFile(sFileVert, sFileFrag);
    }
    //----------------------------------
    //  ~IcShader
    //----------------------------------
    IcShader::~IcShader() {
        unbindShader();
        if(m_isValid)
        {

            m_isValid = false;
            glDeleteProgram(m_uProgram);
            glDeleteShader(m_uVertexShader);
            glDeleteShader(m_uFragmentShader);
            logInfo("IcShader::~IcShader()-- Shader Released");
       }
    }
    //----------------------------------
    //  loadFile
    //----------------------------------
    bool IcShader::loadFile(const string& sFileVert,
                            const string& sFileFrag)
    {
        logInfo("IcShader::loadFile()-- Vert Shader    :["+sFileVert+"]");
        logInfo("IcShader::loadFile()-- Fragment Shader:["+sFileFrag+"]");

        std::ifstream t1(sFileVert.c_str());
        if(!t1.is_open())
        {
            cout << "Error : Fail to open file '"+sFileVert +"'\n";
            return false;
        }
        
        std::string tmp1((std::istreambuf_iterator<char>(t1)),
                         std::istreambuf_iterator<char>());
        mVertexShaderStr = tmp1;
        
        std::ifstream t2(sFileFrag.c_str());
        if(!t2.is_open())
        {
            cout << "Error : Fail to open file '"+sFileFrag +"'\n";
            return false;
        }
        
        std::string tmp2((std::istreambuf_iterator<char>(t2)),
                         std::istreambuf_iterator<char>());
        mFragmentShaderStr = tmp2;
        buildShader();
        return true;
    }
    //----------------------------------
    //  compileShader
    //----------------------------------
    void IcShader::compileShader()
    {
        const int LOG_LEN = 500;
        char compileLog[LOG_LEN];
        unsigned int returnedLen;
     	logInfo("IcShader::compileShader()");    
        glCompileShader(m_uVertexShader);
        GLint iVertexShaderCompileStatus = GL_TRUE;
        glGetShaderiv(m_uVertexShader, GL_COMPILE_STATUS, &iVertexShaderCompileStatus);
        if ((iVertexShaderCompileStatus) && (glIsShader(m_uVertexShader))) {
            logDbg("shader compiled");
        } else {
            glGetShaderInfoLog(m_uVertexShader, (GLsizei)LOG_LEN, (GLsizei*)&returnedLen, (GLchar*)compileLog);
            ctl::logErr("shader not compiled, info:"+ string(compileLog));
        }
        
        glCompileShader(m_uFragmentShader);
        GLint iFragmentShaderCompileStatus;
        glGetShaderiv(m_uFragmentShader, GL_COMPILE_STATUS, &iFragmentShaderCompileStatus);
        if ((iFragmentShaderCompileStatus) && (glIsShader(m_uFragmentShader))) {
            logDbg("Fragment shader compiled");
        }
        else {
            glGetShaderInfoLog(m_uFragmentShader, (GLsizei)LOG_LEN, (GLsizei*)&returnedLen, (GLchar*)compileLog);
            ctl::logErr("Fragment shader not compiled, info: "+ string(compileLog));
        }
    }
    //----------------------------------
    //  buildShader
    //----------------------------------
    void IcShader::buildShader()
    {
        logInfo("IcShader::buildShader()");    
	 m_isValid = false;
        m_uVertexShader = glCreateShader(GL_VERTEX_SHADER);
        m_uFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        
        const GLchar* vertexShaderCStr = mVertexShaderStr.c_str();
        const GLchar* fragShaderCStr  = mFragmentShaderStr.c_str();
        glShaderSource(m_uVertexShader, 1, &vertexShaderCStr, NULL);
        glShaderSource(m_uFragmentShader, 1, &fragShaderCStr, NULL);
        
        compileShader();
        
        m_uProgram = glCreateProgram();
        glAttachShader(m_uProgram, m_uVertexShader);
        glAttachShader(m_uProgram, m_uFragmentShader);
        
        glLinkProgram(m_uProgram);
        
        bindShader();
        
        GLint iLinkedStatus;
        glGetProgramiv(m_uProgram, GL_LINK_STATUS, &iLinkedStatus);
        if ((iLinkedStatus) && (glIsProgram(m_uProgram)))
        {
            logDbg("shader linked");
        }
        else
        {
            GLint infoLen = 0;
            glGetProgramiv(m_uProgram, GL_INFO_LOG_LENGTH, &infoLen);
            char* infoLog = (char*)malloc(sizeof(char) * infoLen);
            glGetProgramInfoLog(m_uProgram, infoLen, NULL, infoLog);
            ctl::logErr("Error linking program, info: "+ string(infoLog));
            free(infoLog);
        }
        //bind should after link program.
        m_isValid = true;
    }
    
 
    //----------------------------------
    //  bind/unbind
    //----------------------------------
    void IcShader::bindShader() const {
        glUseProgram(m_uProgram);
    }
    
    void IcShader::unbindShader() const {
        glUseProgram(0);
    }
    
    //----------------------------------
    //  Get Location
    //----------------------------------
    int IcShader::GetAttribLocation(const std::string sName) const {
        int attrLoc = glGetAttribLocation(m_uProgram, sName.c_str());
        if(attrLoc == -1)
            ctl::logErr(string("Location not found for Attri '")+ sName +"'");
        return attrLoc;
    }
    
    int IcShader::GetUniformLocation(const std::string sName) const {
        int attrLoc =  glGetUniformLocation(m_uProgram, sName.c_str());
        if(attrLoc == -1)
            ctl::logErr(string("Location not found for Uniform '") + sName+"'");
        return attrLoc;
    }
 
} // namespace Ic3d
