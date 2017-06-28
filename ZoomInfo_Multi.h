// ZoomInfo_Multi.h: interface for the CZoomInfo_Multi class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ZOOMINFO_MULTI_H__930C7D1C_6619_45ED_A70F_BB1C8B1D8AF5__INCLUDED_)
#define AFX_ZOOMINFO_MULTI_H__930C7D1C_6619_45ED_A70F_BB1C8B1D8AF5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ZoomInfo.h"

class CZoomInfo_Multi : public CZoomInfo  
{
public:
	CRect GetImageRect(int nNum);
	CPoint ToSubImageCoord(CPoint ptPoint);
	int GetImageNum(CPoint ptPoint);
	CZoomInfo_Multi(CSize szSize, CRect rcRect, int nImageNum, bool bRightRect = true);
	CRect GetDestRect(int nNum);
	CRect GetSourceRect(int nNum);
	int GetTotalImageNum();
	CZoomInfo_Multi();
	virtual ~CZoomInfo_Multi();

private:
	int m_nTotalImageNum;		
};

#endif // !defined(AFX_ZOOMINFO_MULTI_H__930C7D1C_6619_45ED_A70F_BB1C8B1D8AF5__INCLUDED_)
