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
	void init(CSize, CRect Frame);
	void myZoomInfo::triggerInit();
	
	float getZoom();
	CPoint getCurrentCenter();
	float getScale();
	CRect getPrev();

	void setPrev(CRect);
	void setCenterPt(CRect rect);
private:
	CRect myZoomInfo::getZoomedRect(CPoint cntrPt);
	CPoint myZoomInfo::checkCntrPt(CPoint cntrPt);
	void myZoomInfo::calcScale(CRect currentRect, CRect Frame);
	bool initialized = false;
	CPoint currentCenter;
	float xScale;
	float yScale;
 
	float m_nZoomX;
	  
protected:
	int m_nMaxZoomFactor;
	CSize m_szImageSize;
	CRect prev;
};
#endif // !defined(AFX_INDEXES_H__9D373282_EAEF_11D3_9A5E_52544C0B46BA__INCLUDED_)
