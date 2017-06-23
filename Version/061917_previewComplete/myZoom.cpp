// Indexes.cpp : implementation file
//

#include "stdafx.h"
//#include "VisionWorks.h"
#include "myZoom.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CZoomInfo

//IMPLEMENT_DYNCREATE(CZoomInfo, CCmdTarget)

myZoomInfo::myZoomInfo()
{
}

myZoomInfo::myZoomInfo(CSize imgSize, CRect Frame)
{
	m_szImageSize = imgSize;
	m_nZoomX = 1;
	currentCenter = CPoint((int)(m_szImageSize.cx*0.5), (int)(m_szImageSize.cy*0.5));
	m_nMaxZoomFactor = 2048;

	xScale = (float)m_szImageSize.cx / (float)Frame.Width();
	yScale = (float)m_szImageSize.cy / (float)Frame.Height();
		
		initialized = true;
}

myZoomInfo::~myZoomInfo()
{
}

void myZoomInfo::init(CSize imgSize, CRect Frame)
{
	if (!initialized)
	{
		m_szImageSize = imgSize;
		m_nZoomX = 1;
		currentCenter = CPoint((int)(m_szImageSize.cx*0.5), (int)(m_szImageSize.cy*0.5));
		m_nMaxZoomFactor = 2048;

		xScale = (float)m_szImageSize.cx / (float)Frame.Width();
		yScale = (float)m_szImageSize.cy / (float)Frame.Height();

		initialized = true;
	}
}
void myZoomInfo::triggerInit()
{
	initialized = false;
}
float myZoomInfo::getZoom()
{
	return m_nZoomX;
}
float myZoomInfo::getScale()
{
	return xScale;
}
CPoint myZoomInfo::getCurrentCenter()
{
	return currentCenter;
}

CRect myZoomInfo::getPrev()
{
	return prev;
}
void myZoomInfo::setPrev(CRect cur)
{
	prev=cur;
}
void myZoomInfo::setCenterPt(CRect rect)
{
	currentCenter = CPoint((rect.left+rect.right)/2,(rect.bottom+rect.top)/2);
}

CRect myZoomInfo::IncreaseZoomX(CPoint clickedPt, CRect Frame, float zoomScale)
{
	if (zoomScale == NULL)
		zoomScale = 2;
	else
		ASSERT(zoomScale > 1);

	currentCenter.x += xScale* (clickedPt.x - Frame.Width() / 2);
	currentCenter.y += yScale* (clickedPt.y - Frame.Height() / 2);


	if (m_nZoomX / zoomScale < m_nMaxZoomFactor)
	{
		m_nZoomX *= zoomScale;
		CRect dstRect = getZoomedRect(currentCenter);
		//cout << "Zoom : " << m_nZoomX << "    x: " << currentCenter.x << "    y: " << currentCenter.y << endl;
		//cout << "left " << dstRect.left << "__right " << dstRect.right << "__top " << dstRect.top << "__bottom " << dstRect.bottom << endl;
		//cout << "Zoom : " << m_nZoomX << "   Xscale : " << xScale << "   Yscale : " << yScale << endl;
		calcScale(dstRect, Frame);
		return dstRect;
	}
	else
	{
		AfxMessageBox(TEXT("CANNOT ZOOM ANYMORE"));
		return false;
	}
	 
}
void myZoomInfo::calcScale(CRect currentRect, CRect Frame)
{
	xScale = (float)currentRect.Width() / (float)Frame.Width();
	yScale = (float)currentRect.Height() / (float)Frame.Height();
}
CRect myZoomInfo::DecreaseZoomX(CPoint clickedPt, CRect Frame, float zoomScale)
{
	if (zoomScale == NULL)
		zoomScale = 2;
	else
		ASSERT(zoomScale > 1);
	currentCenter.x += xScale*(clickedPt.x - Frame.Width() / 2);
	currentCenter.y += yScale*(clickedPt.y - Frame.Height() / 2);

	if (m_nZoomX > zoomScale)
	{
		m_nZoomX /= zoomScale;
		CRect dstRect = getZoomedRect(currentCenter);
		calcScale(dstRect, Frame);
		return dstRect;
	}
	else  
	{
		m_nZoomX = 1;
		xScale = (float)m_szImageSize.cx / (float)Frame.Width();
		yScale = (float)m_szImageSize.cy / (float)Frame.Height();
		currentCenter = CPoint((int)(m_szImageSize.cx*0.5), (int)(m_szImageSize.cy*0.5));
		return CRect(CPoint(0, 0), m_szImageSize);
	}
	 
}

CRect myZoomInfo::getZoomedRect(CPoint currentCtr)
{
	CPoint topLeft, bottmRight;

	CSize dstRectSize = CSize(m_szImageSize.cx / (2 * m_nZoomX), m_szImageSize.cy / (2 * m_nZoomX));
	CPoint newCntrPt = checkCntrPt(currentCtr, dstRectSize);
	topLeft = CPoint(newCntrPt.x - m_szImageSize.cx / (2 * m_nZoomX), newCntrPt.y - m_szImageSize.cy / (2 * m_nZoomX));
	bottmRight = CPoint(newCntrPt.x + m_szImageSize.cx / (2 * m_nZoomX), newCntrPt.y + m_szImageSize.cy / (2 * m_nZoomX));
	//cout << "CenterX :" << currentCenter.x << "   CenterY :" << currentCenter.y << endl;
	prev = CRect(topLeft, bottmRight);
	return  CRect(topLeft, bottmRight);
}

CRect myZoomInfo::getZoomedRect( )
{
	CPoint topLeft, bottmRight;

	CSize dstRectSize = CSize(m_szImageSize.cx / (2 * m_nZoomX), m_szImageSize.cy / (2 * m_nZoomX));
	CPoint newCntrPt = checkCntrPt(currentCenter, dstRectSize);
	topLeft = CPoint(newCntrPt.x - m_szImageSize.cx / (2 * m_nZoomX), newCntrPt.y - m_szImageSize.cy / (2 * m_nZoomX));
	bottmRight = CPoint(newCntrPt.x + m_szImageSize.cx / (2 * m_nZoomX), newCntrPt.y + m_szImageSize.cy / (2 * m_nZoomX));
	//cout << "CenterX :" << currentCenter.x << "   CenterY :" << currentCenter.y << endl;
	prev = CRect(topLeft, bottmRight);
	return  CRect(topLeft, bottmRight);
}


CPoint myZoomInfo::checkCntrPt(CPoint currentCtr , CSize dstRectSize)
{
	CPoint newCntrPt = currentCtr;
	if (currentCtr.x - dstRectSize.cx < 1)	newCntrPt.x = dstRectSize.cx;
	if (currentCtr.y - dstRectSize.cy < 1)	newCntrPt.y = dstRectSize.cy;
	
	if (currentCtr.x + dstRectSize.cx > m_szImageSize.cx - 1)	newCntrPt.x = m_szImageSize.cx - dstRectSize.cx;
	if (currentCtr.y + dstRectSize.cy > m_szImageSize.cy - 1)	newCntrPt.y = m_szImageSize.cy - dstRectSize.cy;
	currentCenter = newCntrPt;
	return currentCenter;
}

CPoint myZoomInfo::checkCntrPt(CPoint currentCtr, CRect dstRect)
{

	CPoint newCntrPt = currentCtr;
	if (currentCtr.x - dstRect.Width() /2< 1)	newCntrPt.x =     dstRect.Width()/2 ;
	if (currentCtr.y - dstRect.Height()/2 < 1)	newCntrPt.y = dstRect.Height()/2;

	if (currentCtr.x + dstRect.Width() /2 > m_szImageSize.cx - 1)	newCntrPt.x = m_szImageSize.cx - dstRect.Width()/2;
	if (currentCtr.y + dstRect.Height()/2 > m_szImageSize.cy - 1)	newCntrPt.y = m_szImageSize.cy - dstRect.Height()/2;
	currentCenter = newCntrPt;
	return currentCenter;
}