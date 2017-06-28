// ZoomInfo_Multi.cpp: implementation of the CZoomInfo_Multi class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ZoomInfo_Multi.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CZoomInfo_Multi::CZoomInfo_Multi()
{

}

CZoomInfo_Multi::~CZoomInfo_Multi()
{

}

int CZoomInfo_Multi::GetTotalImageNum()
{
	return m_nTotalImageNum;
}


CRect CZoomInfo_Multi::GetSourceRect(int nNum)	// nNum: 이미지 여러개인가?	
{
	CRect rect = CZoomInfo::GetSourceRect();

	rect.right += rect.left;

	int nLeft = nNum*m_szImageSize.cx/m_nTotalImageNum;
	int nRight = nLeft + m_szImageSize.cx/m_nTotalImageNum-1;

	if( nRight < rect.left || nLeft > rect.right )	return CRect(0, 0, 0, 0);
//	else if( nLeft >= rect.left && nRight <= rect.right ){
//		rect.left = rect.left;
//		rect.right = nRight-nLeft;

//		return rect;
	
	 else {
		if( rect.left <= nLeft )	rect.left = nLeft;
		if( rect.right >= nRight )	rect.right = nRight;

		
		rect.right = (rect.right-rect.left+1);
		rect.left -= nNum*m_szImageSize.cx/m_nTotalImageNum;

		return rect;
	}
}


CRect CZoomInfo_Multi::GetDestRect(int nNum)
{
	CRect rect = GetSourceRect(nNum);

	rect.left += m_szImageSize.cx/m_nTotalImageNum*nNum;
	rect.right += rect.left-1;
	rect.bottom += rect.top-1;

	rect = GetBound(rect);

	rect.right -= (rect.left-1);
	rect.bottom -= (rect.top-1);
	
	return rect;
}


CZoomInfo_Multi::CZoomInfo_Multi(CSize szSize, CRect rcRect, int nImageNum, bool bRightRect)
:CZoomInfo(szSize, rcRect, bRightRect)
{
	m_nTotalImageNum = nImageNum;
}



int CZoomInfo_Multi::GetImageNum(CPoint ptPoint)
{
	CRect rect;
	
	for(int i=0; i<m_nTotalImageNum; i++){
		rect = GetImageRect(i);
		if( ptPoint.x >= rect.left && ptPoint.x <= rect.right )	return i;
	}

	return 0;
}


CPoint CZoomInfo_Multi::ToSubImageCoord(CPoint ptPoint)
{
	int num = GetImageNum(ptPoint);

	ptPoint.x -= GetImageRect(num).left;
	
	return ptPoint;
}


CRect CZoomInfo_Multi::GetImageRect(int nNum)
{
	CRect ret;
	int nWidth = m_szImageSize.cx/m_nTotalImageNum;		

	ret.left = nWidth * nNum;
	ret.right = nWidth*(nNum+1)-1;
	ret.top = 0;
	ret.bottom = m_szImageSize.cy;

	return ret;
}
