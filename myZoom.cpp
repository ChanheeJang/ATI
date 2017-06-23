// Indexes.cpp : implementation file
//
#pragma once
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
		//m_ptFrameZoomCenter = MyImage::GetFrameCenter();
		m_rcSrcZoomRect = MyImage::GetSrcRect();
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
	return CPoint(m_ptSrcZoomCenter.x/xScale,m_ptSrcZoomCenter.y/yScale);
}


 
void myZoomInfo::SetSrcZoomCenter(CRect srcZoomRect)
{
	m_ptSrcZoomCenter = CPoint((srcZoomRect.left+srcZoomRect.right)/2, (srcZoomRect.top+srcZoomRect.bottom) / 2);
}

void myZoomInfo::SetSrcZoomCenter(CPoint currentSrcZoomCenter)
{
	m_ptSrcZoomCenter = currentSrcZoomCenter;
}


CRect myZoomInfo::IncreaseZoomX(CPoint clickedPt, CRect Frame, float zoomScale)
{
	m_ptSrcZoomCenter.x += ((float)GetSrcZoomSize().cx / (float)GetFrameSize().cx)* (clickedPt.x - Frame.Width() / 2);
	m_ptSrcZoomCenter.y += ((float)GetSrcZoomSize().cy / (float)GetFrameSize().cy)* (clickedPt.y - Frame.Height() / 2);

	if (m_nZoomX / zoomScale < m_nMaxZoomFactor)
	{
		m_nZoomX *= zoomScale;
		CRect dstRect = getZoomedRect(m_ptSrcZoomCenter);
		return dstRect;
	}
	else
	{
		AfxMessageBox(TEXT("CANNOT ZOOM ANYMORE"));
		return false;
	}
}
 

CRect myZoomInfo::DecreaseZoomX(CPoint clickedPt, CRect Frame, float zoomScale)
{
	m_ptSrcZoomCenter.x += ((float)GetSrcZoomSize().cx / (float)GetFrameSize().cx)*(clickedPt.x - Frame.Width() / 2);
	m_ptSrcZoomCenter.y += ((float)GetSrcZoomSize().cy / (float)GetFrameSize().cy)*(clickedPt.y - Frame.Height() / 2);

	if (m_nZoomX > zoomScale)
	{
		m_nZoomX /= zoomScale;
		CRect dstRect = getZoomedRect(m_ptSrcZoomCenter);
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


CRect myZoomInfo::Shift2NewCenter(CPoint currentCenter, CSize rectSize)
{
	return CRect(currentCenter.x - rectSize.cx / 2, currentCenter.y - rectSize.cy / 2, currentCenter.x + rectSize.cx / 2, currentCenter.y + rectSize.cy / 2);
}


CRect myZoomInfo::getZoomedRect(CPoint srcZoomCenter)
{
	CPoint topLeft, bottmRight;

	CSize dstRectSize = CSize(m_szImageSize.cx / (2 * m_nZoomX), m_szImageSize.cy / (2 * m_nZoomX));
	CPoint newCntrPt = checkCntrPt(srcZoomCenter, dstRectSize);
	topLeft = CPoint(newCntrPt.x - m_szImageSize.cx / (2 * m_nZoomX), newCntrPt.y - m_szImageSize.cy / (2 * m_nZoomX));
	bottmRight = CPoint(newCntrPt.x + m_szImageSize.cx / (2 * m_nZoomX), newCntrPt.y + m_szImageSize.cy / (2 * m_nZoomX));
	
	m_ptFrameZoomCenter = CPoint((topLeft.x + bottmRight.x)/xScale / 2, (topLeft.y + bottmRight.y)/yScale / 2);
	return  CRect(topLeft, bottmRight);
}

CRect myZoomInfo::getZoomedRect( )
{
	CPoint topLeft, bottmRight;

	CSize dstRectSize = CSize(m_szImageSize.cx / (2 * m_nZoomX), m_szImageSize.cy / (2 * m_nZoomX));
	CPoint newCntrPt = checkCntrPt(m_ptSrcZoomCenter, dstRectSize);
	topLeft = CPoint(newCntrPt.x - m_szImageSize.cx / (2 * m_nZoomX), newCntrPt.y - m_szImageSize.cy / (2 * m_nZoomX));
	bottmRight = CPoint(newCntrPt.x + m_szImageSize.cx / (2 * m_nZoomX), newCntrPt.y + m_szImageSize.cy / (2 * m_nZoomX));
	
	m_ptFrameZoomCenter = CPoint((topLeft.x + bottmRight.x)/xScale / 2, (topLeft.y + bottmRight.y)/yScale / 2);
	return  CRect(topLeft, bottmRight);
}


CPoint myZoomInfo::checkCntrPt(CPoint srcZoomCenter, CSize dstRectSize)
{
	CPoint newCntrPt = srcZoomCenter;
	if (srcZoomCenter.x - dstRectSize.cx < 0)	newCntrPt.x = dstRectSize.cx;
	if (srcZoomCenter.y - dstRectSize.cy < 0)	newCntrPt.y = dstRectSize.cy;
	
	if (srcZoomCenter.x + dstRectSize.cx > m_szImageSize.cx - 1)	newCntrPt.x = m_szImageSize.cx - dstRectSize.cx;
	if (srcZoomCenter.y + dstRectSize.cy > m_szImageSize.cy - 1)	newCntrPt.y = m_szImageSize.cy - dstRectSize.cy;
	m_ptSrcZoomCenter = newCntrPt;
	return m_ptSrcZoomCenter;
}

CPoint myZoomInfo::checkCntrPt(CPoint srcZoomCenter, CRect dstRect)
{

	CPoint newCntrPt = srcZoomCenter;
	if (srcZoomCenter.x - dstRect.Width() / 2< 0)	newCntrPt.x = dstRect.Width() / 2;
	if (srcZoomCenter.y - dstRect.Height() / 2 < 0)	newCntrPt.y = dstRect.Height() / 2;

	if (srcZoomCenter.x + dstRect.Width() / 2 > m_szImageSize.cx - 1)	newCntrPt.x = m_szImageSize.cx - dstRect.Width() / 2;
	if (srcZoomCenter.y + dstRect.Height() / 2 > m_szImageSize.cy - 1)	newCntrPt.y = m_szImageSize.cy - dstRect.Height() / 2;
	m_ptSrcZoomCenter = newCntrPt;
	return m_ptSrcZoomCenter;
}



void myZoomInfo::SetSrcZoomRect(CRect zoomRect)
{
	m_rcSrcZoomRect = zoomRect;
	SetSrcZoomCenter(zoomRect);
}

CRect myZoomInfo::GetSrcZoomRect()
{
	return m_rcSrcZoomRect;
}

CSize myZoomInfo::GetSrcZoomSize()
{
	return CSize(m_rcSrcZoomRect.Width(),m_rcSrcZoomRect.Height());
}