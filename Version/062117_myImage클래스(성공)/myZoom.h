#if !defined(AFX_myZOOMINFO_H__9D373282_EAEF_11D3_9A5E_52544C0B46BA__INCLUDED_)
#define AFX_myZOOMINFO_H__9D373282_EAEF_11D3_9A5E_52544C0B46BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#include "myImage.h"
#endif // _MSC_VER > 1000
//


/////////////////////////////////////////////////////////////////////////////
// myZoomInfo command target

class myZoomInfo :public MyImage
{
	/*	DECLARE_DYNCREATE(myZoomInfo)
	*/
public:
	myZoomInfo();
	myZoomInfo(CSize, CRect Frame);           // protected constructor used by dynamic creation
	virtual ~myZoomInfo();

	// Attributes
public:
	
	// Operations
public:

	CRect IncreaseZoomX(CPoint cntrPt, CRect Frame, float zoomScale=2.0);
	CRect DecreaseZoomX(CPoint cntrPt, CRect Frame, float zoomScale=2.0);
	CRect Shift2NewCenter(CPoint newCenter, CSize rectSize);
	CPoint checkCntrPt(CPoint currentCtr, CSize dstRectSize);
	CPoint checkCntrPt(CPoint currentCtr, CRect dstRect);
	void init();
	void triggerInit();
	CRect getZoomedRect();
	float getZoom();

	CPoint GetSrcZoomCenter();
	CPoint GetFrameZoomCenter();

	void SetSrcZoomCenter(CRect rect);
	void SetSrcZoomCenter(CPoint centerPoint);

	void SetSrcZoomRect(CRect srcRect);
	CRect GetSrcZoomRect();
	CSize GetSrcZoomSize();
 
private:
	CRect getZoomedRect(CPoint cntrPt);
	//void calcScale(CRect currentRect, CRect Frame);
	bool initialized = false;
 
	CPoint m_ptSrcZoomCenter;
	CPoint m_ptFrameZoomCenter;
	CRect m_rcSrcZoomRect; //Rect that encompasses a portion of src image(zooming effect)
	
 
	float m_nZoomX;
	float xScale;
	float yScale;
	  
private:
	int m_nMaxZoomFactor;
	CSize m_szImageSize;
	CRect prev=CRect(0,0,0,0);
};
#endif // !defined(AFX_INDEXES_H__9D373282_EAEF_11D3_9A5E_52544C0B46BA__INCLUDED_)
