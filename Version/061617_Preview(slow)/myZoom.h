#if !defined(AFX_myZOOMINFO_H__9D373282_EAEF_11D3_9A5E_52544C0B46BA__INCLUDED_)
#define AFX_myZOOMINFO_H__9D373282_EAEF_11D3_9A5E_52544C0B46BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//


/////////////////////////////////////////////////////////////////////////////
// myZoomInfo command target

class myZoomInfo// : public CCmdTarget
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

	CRect myZoomInfo::IncreaseZoomX(CPoint cntrPt, CRect Frame, float zoomScale=NULL);
	CRect myZoomInfo::DecreaseZoomX(CPoint cntrPt, CRect Frame, float zoomScale = NULL);
	CPoint myZoomInfo::checkCntrPt(CPoint currentCtr, CSize dstRectSize);
	CPoint myZoomInfo::checkCntrPt(CPoint currentCtr, CRect dstRect);
	void init(CSize, CRect Frame);
	void myZoomInfo::triggerInit();
	CRect myZoomInfo::getZoomedRect();
	float getZoom();
	CPoint getCurrentCenter();
	float getScale();
	CRect getPrev();

	void setPrev(CRect);
	void setCenterPt(CRect rect);
private:
	CRect myZoomInfo::getZoomedRect(CPoint cntrPt);
	void myZoomInfo::calcScale(CRect currentRect, CRect Frame);
	bool initialized = false;
	CPoint currentCenter;
	float xScale;
	float yScale;
 
	float m_nZoomX;
	  
protected:
	int m_nMaxZoomFactor;
	CSize m_szImageSize;
	CRect prev=CRect(0,0,0,0);
};
#endif // !defined(AFX_INDEXES_H__9D373282_EAEF_11D3_9A5E_52544C0B46BA__INCLUDED_)
