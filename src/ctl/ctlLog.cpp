//
//  CeLogger.cpp
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
    
    ctl::Sp<LogHandler> LogHandler::m_pLogHandler =
        ctl::makeSp<LogHandler>();
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
	//	logMsg
	//------------------------------------------
	void LogHandler::logMsg(LogHandler::TE_logLevel lvl,
                         const std::string &sMsg)
	{
		if(lvl<m_curLogLvl) return;
		LogHelper hlpr;
		string sLvl = hlpr.getLvlName(lvl);
		std::cout << "ctl:"+sLvl + ":"+ sMsg +"\n";
	}
}
