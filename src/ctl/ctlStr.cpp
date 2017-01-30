//
//  ctlStr.cpp
//  DevEng
//
//  Created by Sherman Chen on 2/24/16.
//  Copyright (c) 2016 Simviu Technology Inc.
//  All rights reserved.
//  http://www.simviu.com/dev
//
//



#include "ctl.h"
#include <string.h>
#include <fstream>
#include <algorithm>

using namespace ctl;
namespace ctl {
    using namespace std;
	//------------------------------------------
	//	sFromFile
	//------------------------------------------
	string sFromFile(const string& sFile, bool& isReadOK)
	{
		std::ifstream ifs(sFile);
		if(!ifs.is_open())
		{
			logErr("Fail to open file ["+sFile+"]");
			isReadOK=false;
			return "";
		};
		std::string s((std::istreambuf_iterator<char>(ifs)),
						std::istreambuf_iterator<char>());
		isReadOK = true;
		ifs.close();
		return s;
	}
	//------------------------------------------
	//	sFromFile
	//------------------------------------------
	bool s2file(const string& sFile, const string& s)
	{
		std::ofstream ofs(sFile.c_str());
		if(!ofs.is_open())
		{
			logErr("Fail to open file ["+sFile+"]");
			return false;
		};
		
		ofs << s;
		ofs.close();
		return true;
	}
	//------------------------------------------
	//	sReplace
	//------------------------------------------
	bool sReplace(string& s,
				  const string& sKey,
				  const string& sRepl)
	{
		size_t start_pos = s.find(sKey);
		if(start_pos == std::string::npos)
			return false;
		s.replace(start_pos, sKey.length(), sRepl);
		return true;
	}

	//------------------------------------------
	//	TSize
	//------------------------------------------
	vector<string> s2tokens(const string& s,
							const string& sDelimeter)
	{
		vector<string> tkns;
		
		//  TODO: this is C style cast
		char* pStr = (char*)(s.c_str());
		char* pSDeli = (char*)sDelimeter.c_str();
		char * pch = strtok (pStr,pSDeli);
		while (pch != NULL)
		{
			tkns.push_back(string(pch));
			pch = strtok (NULL, sDelimeter.c_str());
		}
		return tkns;
	}
	
	//------------------------------------------
	//	File name path operation
	//------------------------------------------
	string sFileExt(const string& sFile)
	{
		size_t idx = sFile.rfind('.');
		if(idx != string::npos)
			return sFile.substr(idx+1);
		else return "";
	}
	string sFilePath(const string& sFile)
	{
		size_t found =sFile.find_last_of("/\\");
		return sFile.substr(0,found+1);
	}
	string sFileBase(const string& sFile)
	{
		string fn = sFileNoPath(sFile);
		size_t idx = fn.rfind('.');
		return fn.substr(0,idx);
	}
	string sFileNoPath(const string& sFile)
	{
		size_t found;
		found=sFile.find_last_of("/\\");
		// found+1 until end
		return sFile.substr(found+1);
	}
	//------------------------------------------
	//	case upper lower
	//------------------------------------------
	string s2lower(const string& s)
	{
		string s1 = s;
		std::transform(s1.begin(), s1.end(), s1.begin(), ::tolower);
		return s1;
	}
	string s2upper(const string& s)
	{
		string s1 = s;
		std::transform(s1.begin(), s1.end(), s1.begin(), ::toupper);
		return s1;
	}

}	// namespac ctl
