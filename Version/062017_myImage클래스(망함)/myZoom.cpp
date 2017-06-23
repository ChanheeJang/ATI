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

myZoomInfo::~myZoomInfo()
{
}

void myZoomInfo::init()
{
	if (!initialized)
	{
		m_szImageSize = MyImage::GetSrcSize();
		m_nZoomX = 1;
		m_ptSrcZoomCenter = MyImage::GetSrcCenter();
		m_ptFrameZoomCenter = MyImage::GetFrameCenter();
		m_nMaxZoomFactor = 2048;
		xScale = MyImage::GetXScale();
		yScale = MyImage::GetYScale();
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
 
CPoint myZoomInfo::GetSrcZoomCenter()
{
	return m_ptSrcZoomCenter;
}

CPoint myZoomInfo::GetFrameZoomCenter()
{
	return m_ptFrameZoomCenter;
}


 
void myZoomInfo::SetSrcZoomCenter(CRect srcZoomRect)
{
	m_ptSrcZoomCenter = CPoint(srcZoomRect.Width() / 2, srcZoomRect.Height() / 2);
}
void myZoomInfo::SetSrcZoomCenter(CPoint currentSrcZoomCenter)
{
	m_ptSrcZoomCenter = currentSrcZoomCenter;
}


CRect myZoomInfo::IncreaseZoomX(CPoint clickedPt, CRect Frame, float zoomScale)
{
	if (zoomScale == NULL)
		zoomScale = 2;
	else
		ASSERT(zoomScale > 1);

	m_ptSrcZoomCenter.x += xScale* (clickedPt.x - Frame.Width() / 2);
	m_ptSrcZoomCenter.y += yScale* (clickedPt.y - Frame.Height() / 2);


	if (m_nZoomX / zoomScale < m_nMaxZoomFactor)
	{
		m_nZoomX *= zoomScale;
		CRect dstRect = getZoomedRect(m_ptSrcZoomCenter);
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
	m_ptSrcZoomCenter.x += xScale*(clickedPt.x - Frame.Width() / 2);
	m_ptSrcZoomCenter.y += yScale*(clickedPt.y - Frame.Height() / 2);

	if (m_nZoomX > zoomScale)
	{
		m_nZoomX /= zoomScale;
		CRect dstRect = getZoomedRect(m_ptSrcZoomCenter);
		calcScale(dstRect, Frame);
		return dstRect;
	}
	else  
	{
		m_nZoomX = 1;
		xScale = (float)m_szImageSize.cx / (float)Frame.Width();
		yScale = (float)m_szImageSize.cy / (float)Frame.Height();
		m_ptSrcZoomCenter = CPoint((int)(m_szImageSize.cx*0.5), (int)(m_szImageSize.cy*0.5));
		return CRect(CPoint(0, 0), m_szImageSize);
	}
	 
}


CRect myZoomInfo::Shift2NewCenter(CPoint newCenter, CSize rectSize)
{
	return CRect(newCenter.x - rectSize.cx / 2, newCenter.y - rectSize.cy / 2, newCenter.x + rectSize.cx / 2, newCenter.y + rectSize.cy / 2);
}


CRect myZoomInfo::getZoomedRect(CPoint currentSrcCenter)
{
	CPoint topLeft, bottmRight;

	CSize dstRectSize = CSize(m_szImageSize.cx / (2 * m_nZoomX), m_szImageSize.cy / (2 * m_nZoomX));
	CPoint newCntrPt = checkCntrPt(currentSrcCenter, dstRectSize);
	topLeft = CPoint(newCntrPt.x - m_szImageSize.cx / (2 * m_nZoomX), newCntrPt.y - m_szImageSize.cy / (2 * m_nZoomX));
	bottmRight = CPoint(newCntrPt.x + m_szImageSize.cx / (2 * m_nZoomX), newCntrPt.y + m_szImageSize.cy / (2 * m_nZoomX));
	//prev = CRect(topLeft, bottmRight);
	m_ptFrameZoomCenter = CPoint((topLeft.x + bottmRight.x) / 2, (topLeft.y + bottmRight.y) / 2);
	return  CRect(topLeft, bottmRight);
}

CRect myZoomInfo::getZoomedRect( )
{
	CPoint topLeft, bottmRight;

	CSize dstRectSize = CSize(m_szImageSize.cx / (2 * m_nZoomX), m_szImageSize.cy / (2 * m_nZoomX));
	CPoint newCntrPt = checkCntrPt(m_ptSrcZoomCenter, dstRectSize);
	topLeft = CPoint(newCntrPt.x - m_szImageSize.cx / (2 * m_nZoomX), newCntrPt.y - m_szImageSize.cy / (2 * m_nZoomX));
	bottmRight = CPoint(newCntrPt.x + m_szImageSize.cx / (2 * m_nZoomX), newCntrPt.y + m_szImageSize.cy / (2 * m_nZoomX));
	//prev = CRect(topLeft, bottmRight);
	m_ptFrameZoomCenter = CPoint((topLeft.x + bottmRight.x) / 2, (topLeft.y + bottmRight.y) / 2);
	return  CRect(topLeft, bottmRight);
}


CPoint myZoomInfo::checkCntrPt(CPoint currentSrcCenter , CSize dstRectSize)
{
	CPoint newCntrPt = currentSrcCenter;
	if (currentSrcCenter.x - dstRectSize.cx < 1)	newCntrPt.x = dstRectSize.cx;
	if (currentSrcCenter.y - dstRectSize.cy < 1)	newCntrPt.y = dstRectSize.cy;
	
	if (currentSrcCenter.x + dstRectSize.cx > m_szImageSize.cx - 1)	newCntrPt.x = m_szImageSize.cx - dstRectSize.cx;
	if (currentSrcCenter.y + dstRectSize.cy > m_szImageSize.cy - 1)	newCntrPt.y = m_szImageSize.cy - dstRectSize.cy;
	m_ptSrcZoomCenter = newCntrPt;
	return m_ptSrcZoomCenter;
}

CPoint myZoomInfo::checkCntrPt(CPoint currentSrcCenter, CRect dstRect)
{

	CPoint newCntrPt = currentSrcCenter;
	if (currentSrcCenter.x - dstRect.Width() / 2< 1)	newCntrPt.x = dstRect.Width() / 2;
	if (currentSrcCenter.y - dstRect.Height() / 2 < 1)	newCntrPt.y = dstRect.Height() / 2;

	if (currentSrcCenter.x + dstRect.Width() / 2 > m_szImageSize.cx - 1)	newCntrPt.x = m_szImageSize.cx - dstRect.Width() / 2;
	if (currentSrcCenter.y + dstRect.Height() / 2 > m_szImageSize.cy - 1)	newCntrPt.y = m_szImageSize.cy - dstRect.Height() / 2;
	m_ptSrcZoomCenter = newCntrPt;
	return m_ptSrcZoomCenter;
}