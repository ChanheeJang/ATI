// ImageFrame.h: interface for the CImageFrame class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAGEFRAME_H__9BDE26E2_0C88_11D4_82FC_000021DE7A53__INCLUDED_)
#define AFX_IMAGEFRAME_H__9BDE26E2_0C88_11D4_82FC_000021DE7A53__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "cdib.h"
#include "ZoomInfo.h"

class CImageFrame : public CDib  
{
public:
	int GetColorTableEntry();
	CImageFrame(CSize imagesize, CSize framesize, CRect region, bool );
	void SetMemPointer(LPBYTE p);
	bool ReadCustom(CFile* f);
	bool WriteCustom(CFile* f, CSize sizeimage);
	CRect InterSect(CRect a, CRect b);
	bool IsConcerned(CRect source);
	bool PtInRegion(CPoint p);
	void DrawRegion(CDC* dc, CRect dest, CRect source);
	void DrawRegion(CDC* dc, CRect dest, CRect source, DWORD);
	CRect GetRegion();
	void SetRegion(CRect rect);
	CImageFrame();
	virtual ~CImageFrame();

private:
	CSize m_szFrameSize;	//	origin & size
	CRect m_rcRegion;	//	absolute coordinates
};

#endif // !defined(AFX_IMAGEFRAME_H__9BDE26E2_0C88_11D4_82FC_000021DE7A53__INCLUDED_)
