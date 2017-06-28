#if !defined(AFX_ZOOMINFO_H__9D373282_EAEF_11D3_9A5E_52544C0B46BA__INCLUDED_)
#define AFX_ZOOMINFO_H__9D373282_EAEF_11D3_9A5E_52544C0B46BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// teachinfo.h : header file
//
#include "PointFloat.h"	// Added by ClassView

/////////////////////////////////////////////////////////////////////////////
// CZoomInfo command target

class CZoomInfo// : public CCmdTarget
{
/*	DECLARE_DYNCREATE(CZoomInfo)
*/
public:
	CZoomInfo();
	CZoomInfo(CSize, CRect, bool bRightRect = true);           // protected constructor used by dynamic creation
	virtual ~CZoomInfo();

// Attributes
public:

// Operations
public:
	CPoint GetWing();
	void SetMaxZoomFactor(int nFactor);
	bool m_bRightRect;
	int GetImageHeight();
	int GetImageWidth();
	CPointFloat PToL_Float(CPoint);
	CRect GetBound(CRect rect);
	CRect GetImageRect();
	void SetZoomBoxRect(CRect rect);
	CPointFloat LToP(CPointFloat p);
	CPoint LToP(CPoint p);
	void ZoomOut(CPoint p);
	CRect GetZoomBoxRect();
	void ZoomIn(CPoint p);
	bool IsInZoomBox(CPoint p);
	int GetZoomBoxHeight();
	int GetZoomBoxWidth();
	CRect m_rcZoomBoxRect;				
	CPoint PToL(CPoint);
	CRect GetSourceRect();

	int DecreaseZoomX();
	int IncreaseZoomX();
	int GetZoomX();
	CPoint GetCenterPoint();
	void SetCenterPoint(CPoint p);
	CPoint m_ptCP;					//Center Point					
	void SetZoomX(int num);
	int m_nZoomX;						

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CZoomInfo)
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CZoomInfo)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

//	DECLARE_MESSAGE_MAP()
protected:
	int m_nMaxZoomFactor;				
	CSize m_szImageSize;				
	int m_nWingX, m_nWingY;	// 이미지 중심빼고 양쪽 부분의 크기 			
	int m_nDeltX, m_nDeltY;	// zoom된 이미지의 크기			
	float m_fScaleX, m_fScaleY;			
};	

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INDEXES_H__9D373282_EAEF_11D3_9A5E_52544C0B46BA__INCLUDED_)
