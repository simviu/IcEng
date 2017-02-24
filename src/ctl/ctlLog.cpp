//
//  ctlLog.cpp
//  DevEng
//
//  Created by Sherman Chen on 2/16/16.
//  Copyright (c) 2016 Simviu Technology Inc.
//  All rights reserved.
//  http://www.simviu.com/dev
//


#include "ctl.h"
#include <iostream>

namespace ctl {
    using namespace std;
    //------------------------------------------
    //	LogHelper instance
    //------------------------------------------
    static LogHandler l_dflt_logHandler;
    LogHandler* LogHandler::m_pLogHandler = &l_dflt_logHandler;
    LogHandler* LogHandler::getCurHandler()
        { return m_pLogHandler; };
    void LogHandler::setCurHandler(LogHandler* p)
        {
            m_pLogHandler = p;
            logDbg("LogHandler set to [0x%x]"+v2hex((intptr_t)p));
       };
    //------------------------------------------
    //	logMsg
    //------------------------------------------
    void logMsg(LogHandler::TE_logLevel lvl, const std::string& sMsg)
    {
        LogHandler::getCurHandler()->logMsg(lvl,sMsg);
    };
	//------------------------------------------
	//	LogHelper
	//------------------------------------------
	struct LogHelper
	{
		string getLvlName(LogHandler::TE_logLevel lvl)
		{
			switch(lvl)
			{
				case LogHandler::L_DBG	: return "DEBUG";  break;
				case LogHandler::L_INFO	: return "INFO"; break;
				case LogHandler::L_WARN	: return "WARN"; break;
				case LogHandler::L_ERR	: return "ERROR";  break;
			};
			return "";
		};
	};
	
	//------------------------------------------
	//	logMsg/logMsgBase
	//------------------------------------------
    void LogHandler::logMsg(LogHandler::TE_logLevel lvl, const std::string &sMsg)
	{
		if(lvl<m_curLogLvl) return;
		LogHelper hlpr;
		string sLvl = hlpr.getLvlName(lvl);
        logMsgBase("ctl:"+sLvl + ":"+ sMsg +"\n");
	}
    void LogHandler::logMsgBase(const std::string& sMsg)
    {
        std::cout << sMsg;
    }

}
