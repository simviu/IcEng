//
//  CeBinBuf.cpp
//  EngDemo
//
//  Created by Sherman Chen on 2/24/16.
//
//


#include "ctl.h"
#include <fstream>
namespace ctl {
    using namespace std;
	//------------------------------------------
	//	setSize
	//------------------------------------------
	bool BinBuf::setSize(size_t N)
	{
		m_size =0;
		m_pBuf = unique_ptr<TByte>(new TByte[N]);
		if(m_pBuf==nullptr) return false;
		m_size = N;
		return true;
	}
	//------------------------------------------
	//	loadFile
	//------------------------------------------
	bool BinBuf::loadFile(const string& sFile)
	{
		ifstream ifs(sFile.c_str(),
					 std::ifstream::binary);
		if(!ifs.is_open())
		{
			logErr("Failed load file ["+sFile+"]");
			return false;
		};
		size_t N = ifs.tellg();
		if(!setSize(N)) return false;
		unsigned char* p = m_pBuf.get();
		ifs.read(reinterpret_cast<char*>(p), N);
		if(ifs) return true;
		else return false;

	}
	//------------------------------------------
	//	saveFile
	//------------------------------------------
	bool BinBuf::saveFile(const string& sFile) const
	{
		ofstream ofs(sFile.c_str());
		unsigned char* p = m_pBuf.get();
		ofs.write(reinterpret_cast<char*>(p), m_size);
		if(ofs) return true;
		else return false;
	}
	//------------------------------------------
	//	fill
	//------------------------------------------
	size_t BinBuf::fill(const TByte* pBuf,
						size_t iStart, size_t N)
	{
		size_t iEnd = iStart + N-1;
		if(iEnd>=m_size) iEnd = m_size-1;
		size_t j=0;
		for(size_t i=iStart;i<=iEnd;i++)
			m_pBuf.get()[i] = pBuf[j++];
		return iEnd - iStart;
	}
    //------------------------------------------
    //	pick
    //------------------------------------------
    size_t BinBuf::pick(TByte* pBufOut,
                        size_t iStart, size_t N) const
    {
        size_t iEnd = iStart + N-1;
        if(iEnd>=m_size) iEnd = m_size-1;
        size_t j=0;
        const TByte* p = m_pBuf.get();
        for(size_t i=iStart;i<=iEnd;i++)
             pBufOut[j++] = p[i];
        return iEnd - iStart;
    }
	//------------------------------------------
	//	pick
	//------------------------------------------
	bool BinBuf::copy(const BinBuf& bbuf)
	{
		size_t N = bbuf.size();
		TByte* p = bbuf.getBuf();
		if(!setSize(N)) return false;
		for(size_t i=0;i<N;i++)
			m_pBuf.get()[i] = p[i];
		
		return true;
	}

	//------------------------------------------
	//	append
	//------------------------------------------
	bool BinBuf::append(const BinBuf& d)
	{
		BinBuf a(*this);
		size_t N0 =m_size;
		size_t N1 =d.size();
		if(!setSize(N0+N1))
			return false;
		fill(a.getBuf(), 0,  N0);
		fill(d.getBuf(), N0, N1);
		return true;
	}
	//------------------------------------------
	//	setAt
	//------------------------------------------
	bool BinBuf::setAt(size_t i, TByte d)
	{
		if(i>=m_size) return false;
		m_pBuf.get()[i] = d;
		return true;
	}
	//------------------------------------------
	//	operator
	//------------------------------------------
	TByte BinBuf::operator [](size_t i) const
	{
		if(i>=m_size) return 0xEE;	// TODO: Report Err
		return m_pBuf.get()[i];
	}
}
