//
//  ctl.h
//  CtLib
//
//  Created by Sherman Chen on 2/15/16.
//  Copyright (c) 2016 Simviu Technology Inc.
//  All rights reserved.
//  http://www.simviu.com/dev
//
//
#pragma once

#ifndef __CtLib__
#define __CtLib__

#include <stdio.h>
#include <memory>
#include <vector>
#include <list>
#include <map>
#include <string>
#include <functional>
#include <sstream>
#include <assert.h>
#include <iostream>
#include <atomic>


#define _USE_MATH_DEFINES
#include <math.h>
#include <thread>
#include <mutex>
#include <fstream>

namespace ctl {
	//-----------------------------
	//	String/value convertion
	//-----------------------------
	template <typename T>
	inline std::string v2s(T d)
	{
		std::ostringstream os ;
		os << d ;
		return os.str() ;
	};
    //----------------------
	template <typename T>
	inline bool s2v(const std::string& s, T& d)
	{
		return !((std::stringstream(s)>>d).fail());
	};
    //----------------------
    template <typename T>
    inline std::string v2hex(T d)
    {
        std::stringstream ss;
        ss << std::hex << d;
        std::string s( ss.str() );
        return s;
    }
    //-----------------------------
	//	string utils
	//-----------------------------
    extern std::vector<std::string> s2tokens(const std::string& s,
							const std::string& sDelimeter=" ,\t\r\n");


	//---- String util for file path
	extern std::string sFileExt(const std::string& sFile);
	extern std::string sFilePath(const std::string& sFile);
	extern std::string sFileBase(const std::string& sFile);
	extern std::string sFileNoPath(const std::string& sFile);
	
	//---- Case
	extern std::string s2lower(const std::string& s);
	extern std::string s2upper(const std::string& s);
	extern std::string sFromFile(const std::string& sFile, bool& isReadOK);
	extern bool s2file(const std::string& sFile, const std::string& s);
	extern bool sReplace(std::string& s,
                         const std::string& sKey,
                         const std::string& sRepl);
    
    //-----------------------------------------
    //	Math Util
    //-----------------------------------------
    inline float deg2rad(float d){ return d*M_PI/180.0; };
    inline float rad2deg(float d){ return d*180.0/M_PI; };
    template<typename T> inline void dClamp(T& d,T min,T max)
        { d = (d>max)?max:(d<min)?min:d; };
    //-----------------------------
    //	Aliase for std::shared_ptr
    //-----------------------------
    //---- aliase shared_ptr<T> to Sp<T>
    template<class T> using Sp = std::shared_ptr<T>;
    
    //---- aliase make_shared<T> to makeSp<T>
    template<class T, class ..._Args>
    inline static std::shared_ptr<T> makeSp(_Args&& ...__args)
    { return std::make_shared<T>(__args...); };
    
	//-----------------------------
	//	LogHandler
	//-----------------------------
	class LogHandler
	{
	public:
		enum TE_logLevel
		{
			L_DBG=0,
			L_INFO,
			L_WARN,
			L_ERR,
		};
		virtual void logMsg(TE_logLevel lvl, const std::string& sMsg);
        virtual void logMsgBase(const std::string& sMsg);
        static LogHandler* getCurHandler();
        static void setCurHandler(LogHandler* p);
	protected:
        static LogHandler* m_pLogHandler;
		//---- Log Level Default
#if DEBUG | NDEBUG | Debug
		TE_logLevel m_curLogLvl=L_DBG;
#else
		TE_logLevel m_curLogLvl=L_INFO;
#endif
	};

    void logMsg(LogHandler::TE_logLevel lvl, const std::string& sMsg);
	inline void logDbg (const std::string& sMsg)
        {logMsg(LogHandler::TE_logLevel::L_DBG,sMsg);};
	inline void logInfo(const std::string& sMsg)
        {logMsg(LogHandler::TE_logLevel::L_INFO,sMsg);};
	inline void logWarn(const std::string& sMsg)
        {logMsg(LogHandler::TE_logLevel::L_WARN,sMsg);};
	inline void logErr (const std::string& sMsg)
        {logMsg(LogHandler::TE_logLevel::L_ERR,sMsg);};
	inline void logFileErr (const std::string& sFile)
		{logErr("Fail to open file ["+sFile+"]");};
	inline bool chkLogErr(bool c, const std::string& sErr)
		{ if(!c) ctl::logErr(sErr); return c;}
	inline bool chkLogAssert(bool c, const std::string& sErr)
	{
#if DEBUG
		assert(c);
#endif
		return chkLogErr(c, sErr);
	}

    //-----------------------------
    //	UI Coordinates
    //-----------------------------
    //---- TPos
    template<typename T>
    struct TPosT    // TODO: rename TVec2, (TVec2->IcVec2)
    {
        TPosT(){};

        TPosT(float xi, float yi){ x=xi; y=yi; };
        T x=0, y=0;
        bool operator == (const TPosT& d)const
            { return (d.x==x)&&(d.y==y);};
        std::string toStr() const{ return v2s(y) + "," + v2s(x); }
        TPosT operator + (const TPosT& d) const
            { return TPosT(x+d.x, y+d.y); };
        TPosT operator - (const TPosT& d) const
            { return TPosT(x-d.x, y-d.y); };
        TPosT& operator += (const TPosT& d)
            { x+=d.x; y+=d.y; return (*this); };
        TPosT& operator -= (const TPosT& d)
            { x-=d.x; y-=d.y; return (*this); };
        TPosT& operator *= (const T& d)
            { x*=d; y*=d; return (*this); };
        TPosT& operator * (const T& d)
            { return TPosT(x*d, y*d); };
    };
    typedef TPosT<float> TPos;
    typedef TPosT<double> TPosHP;
    
    //---- TSize
    template<typename T>
    struct TSizeT
    {
        TSizeT(){};

        TSizeT(float wi, float hi){ w=wi; h=hi; };
        T w=0, h=0;
        bool operator == (const TSizeT& d)const
            { return (d.w==w)&&(d.h==h);};
        std::string toStr() const{ return v2s<T>(w) + "," + v2s<T>(h); };
        bool operator != (const TSizeT& d)const{ return !(d==(*this)); };

    };
    typedef TSizeT<float> TSize;
    typedef TSizeT<double> TSizeHP;
    //---- TRect
    template<typename T>
    struct TRectT
    {
        TRectT(){};
        TRectT(float x, float y, float w, float h):
        TRectT(TPosT<T>(x,y), TSizeT<T>(w,h)){};
        TRectT(const TPosT<T>& p0, const TPosT<T>& p1):pos0(p0), pos1(p1){};
        TRectT(const TPosT<T>& pos, const TSizeT<T>& sz)
        :pos0(pos), pos1(pos0.x+sz.w , pos0.y+sz.h){};

        TPosT<T> pos0;
        TPosT<T> pos1;
        TSizeT<T> getSize() const
        {
            return TSizeT<T>(fabs(pos0.x-pos1.x),
                             fabs(pos0.y-pos1.y));
        };
        bool operator == (const TRectT& r)const
            { return (r.pos0==pos0) && (r.pos1==pos1);};
        TPosT<T> getCenter()const
            { return TPosT<T>((pos0.x+pos1.x)/2, (pos0.y+pos1.y)/2); };
        std::string toStr() const
            { return "("+pos0.toStr() + "),(" + pos1.toStr()+")"; }
    };
    typedef TRectT<float> TRect;
    typedef TRectT<double> TRectHP;
    extern bool s2v2d(const std::string& s, TSize& sz);
    extern bool s2v2d(const std::string& s, TPos& pos);
    extern bool s2v2d(const std::string& s, TRect& r);
    
    extern std::string v2s2d(const TSize& sz);
    extern std::string v2s2d(const TPos& sz);
    extern std::string v2s2d(const TRect& sz);
    
    //-----------------------------
    //	BinBuf
    //-----------------------------
    typedef unsigned char TByte;
    class BinBuf
    {
    public:
        BinBuf(){};
        BinBuf(size_t N){ setSize(N); };
        BinBuf(const TByte* pBuf,int N)
        { setSize(N); fill(pBuf,0, N);};
        BinBuf(const BinBuf& bbuf){ copy(bbuf); };
        virtual ~BinBuf(){};
        
        bool copy(const BinBuf& bbuf);
        bool setSize(size_t N);
        bool loadFile(const std::string& sFile);
        bool saveFile(const std::string& sFile) const;
        size_t fill(const TByte* pBuf, size_t iStart, size_t N);
        size_t pick(TByte* pBufOut, size_t iStart, size_t N) const;
        bool append(const BinBuf& d);
        bool setAt(size_t i, TByte d);
        TByte  operator [](size_t i) const;
        size_t size()const { return m_size; };
        TByte* getBuf() const{ return m_pBuf.get(); };
        void clear(){ m_pBuf = nullptr; m_size=0; };
        
        //---- Call back of exceed boundry error
        typedef std::function<void(const BinBuf& buf, size_t i, bool isWr)>  TErrFuncCB;
        void setFuncCB(TErrFuncCB pFunc)
            { m_pFuncErrCB = pFunc; };
        
    protected:
        size_t	m_size = 0;
        std::unique_ptr<TByte> m_pBuf = nullptr;
        TErrFuncCB m_pFuncErrCB = nullptr;
        
    };
    
   
    //-----------------------------
    //	IcImg
    //-----------------------------
    class IcImg
    {
    public:
        IcImg(){};
        IcImg(const TSize& size) { setSize(size); };
        virtual ~IcImg(){};
        
        struct TPixel{
            TPixel(){};
            TPixel(TByte ri, TByte gi, TByte bi, TByte ai):
            r(ri), g(gi), b(bi), a(ai){};
            TByte r=0,g=0,b=0,a=0; };
        bool setPx(const TPos& pos, const TPixel& c);
        bool getPx(const TPos& pos, TPixel& c) const;
        bool loadFile(const std::string& sFile);
        bool saveFile(const std::string& sFile) const;
        bool setSize(const TSize& size);
        void scaleTo(const TSize& size);
        TSize size()const{ return m_size; };
        BinBuf& getBuf(){ return m_buf; };
        const BinBuf& getBuf()const{ return m_buf; };
        void fillColor(const TPixel& c);
        
        //---- Format Extension Adapter, to support other format externally
        struct CFormatAdp
        {
            virtual bool loadFile(IcImg& img, const std::string& sFile)
            { return false; };
            virtual bool saveFile(const IcImg& img, const std::string& sFile) const
            { return false; };
            
        private:
        };
        static void addFormatAdp(const std::string& sExtList,
                                 ctl::Sp<CFormatAdp> pAdp);
        void copy(const IcImg& img);
    private:
        ctl::BinBuf	m_buf;
        TSize		m_size;
        static void checkSysInit();
        static void checkSysInitPlatform();
        
    };
    
	//-----------------------------
	//	SafeAry
	//-----------------------------
	template<class T>
	class SafeAry
	{
	public:
		SafeAry(){};
		virtual ~SafeAry() {};
		inline bool getAt(size_t i, T& d)const
		{ if(i>=m_ary.size()) return false;
			d = m_ary[i]; return true; };
        inline bool setAt(size_t i, const T& d)
        { if(i>=m_ary.size()) return false;
            m_ary[i] = d; return true; };
		void add(const T& d){ m_ary.push_back(d); };
		void clear(){ m_ary.clear(); };
		size_t size()const { return m_ary.size(); };
		
		//---- Lambda func
		typedef std::function<void(size_t idx, T& d)> TFunc;
		void runAll(TFunc f) const
		{	size_t i=0;
			for(auto& d : m_ary) f(i++, d); };
		std::vector<T>& getAry(){ return m_ary; };
		const std::vector<T>& getAry() const{ return m_ary; };
		
	protected:
		std::vector<T>	m_ary;
	
	};

	//-----------------------------
	//	SpAry
	//-----------------------------
	template<class T>
	class SpAry
	{
	public:
		SpAry(){};
		virtual ~SpAry(){};
		void add(Sp<T> p) { m_ary.push_back(p); };
		size_t size() const{ return m_ary.size(); };
		void clear(){ m_ary.clear(); };
		typedef std::function<void(size_t idx, Sp<T>)> TFunc;
		
		//---- Normal access
		Sp<T> operator [](size_t idx) const
			{	if(idx<0 || idx>=m_ary.size()) return nullptr;
				return m_ary[idx];
			};
		const std::vector<Sp<T>>& all() const { return m_ary; };
		
		//---- Access use Lambda
		void runAll(TFunc f) const
			{	size_t i=0;
				for(auto p : m_ary) f(i++, p); };
		bool run(size_t idx, TFunc f) const
			{	if(idx<0 || idx>=m_ary.size()) return false;
				f(idx, m_ary[idx]); return true; };
		
		//---- Acess use Call back class
		struct CallBK
		{	virtual void run(size_t idx, Sp<T> p)=0; };
		void runAll(CallBK& cbk) const
			{	size_t i=0; for(auto p : m_ary) cbk.run(i++, p); };
		bool run(size_t idx, CallBK& cbk) const
			{	if(idx<0 || idx>=m_ary.size()) return false;
				cbk.run(idx, m_ary[idx]); return true; };
		std::vector<Sp<T>>& getAry(){ return m_ary; };

    protected:
		std::vector<Sp<T>> m_ary;
	};
	//-----------------------------
	//	SpMap
	//-----------------------------
	template<class T>
	class SpMap
	{
	public:
		SpMap(){};
		virtual ~SpMap(){};
		typedef std::function<void(const std::string& sKey, Sp<T>)> TFunc;

		void store(const std::string& sKey, Sp<T> p) { m_map[sKey] = p; };
		size_t size() const{ return m_map.size(); };
		void clear(){ m_map.clear(); };
		Sp<T> lookup(const std::string& sKey) const
			{	auto it = m_map.find(sKey);
				if(it==m_map.end()) return nullptr;
				return (*it).second; };
		void getKeys(std::vector<std::string> sKeys)const
			{	sKeys.clear();
				for(auto it = m_map.begin(); it!=m_map.end(); ++it)
					sKeys.push_back((*it).first);	};
		
	
		std::map<std::string, Sp<T>>& all() const { return m_map; };

		//---- Access via Lambda
		void runAll(TFunc f) const
			{	for(auto it = m_map.begin(); it!=m_map.end(); ++it)
				f((*it).first, (*it).second); };
		bool run(const std::string& sKey, TFunc f) const
			{	auto it = m_map.find(sKey);
				if(it==m_map.end()) return false;
				f((*it).first, (*it).second); return true; };
		
		//---- Acess use Call back class
		struct CallBK
		{	virtual void run(const std::string& sKey, Sp<T> p)=0; };
		void runAll(CallBK& cbk) const
			{	for(auto it = m_map.begin(); it!=m_map.end(); ++it)
				cbk.run((*it).first, (*it).second); };
		bool run(const std::string& sKey, CallBK& cbk) const
			{	auto it = m_map.find(sKey);
				if(it==m_map.end()) return false;
				cbk.run((*it).first, (*it).second); return true; };
        std::map<std::string, Sp<T>>& getMap(){ return m_map; };

    protected:
		std::map<std::string, Sp<T>> m_map;
	};
    //-----------------------------
    //	UnitTest
    //-----------------------------
    class UnitTest
    {
    public:
        bool runUnitTest();
        
    protected:
        bool runTestImg();
        bool runTestSpMap();
        bool runTestSpAry();
        
    };
	
}

#endif /* defined(__CtLib__) */
