#pragma once
#include "stdafx.h"
#include "MyImage.h"

MyImage::MyImage()
{
	IsImageInfoLoaded = false;
}
MyImage::~MyImage()
{
 
}

/////////////////////////*--1--*//////////////////////////
CSize MyImage::GetSrcSize()
{
	return m_szSrcSize;
}

CRect MyImage::GetSrcRect()
{
	return m_rcSrcRect;
}

double MyImage::GetSrcRatio()
{
	return m_dSrcRatio;
}

CPoint MyImage::GetSrcCenter()
{
	return m_ptSrcCenter;
}

void MyImage::SetSrcSize(int srcWidth, int srcHeight)
{
	m_iSrcWidth = srcWidth;
	m_iSrcHeight = srcHeight;
	m_szSrcSize = CSize(srcWidth, srcHeight);

	InitParams();
}

void MyImage::SetSrcSize(CSize srcSize)
{
	m_iSrcWidth = srcSize.cx;
	m_iSrcHeight = srcSize.cy;
	m_szSrcSize = srcSize;
	InitParams();
}


float MyImage::GetXScale()
{
	return m_fXScale;
}

float MyImage::GetYScale()
{
	return m_fYScale;
}


/////////////////////////*--2--*//////////////////////////
void MyImage::SetClientRect(CRect clientRect)
{
	m_rcClientRect = clientRect;
	CalcClientRatio();
}

CRect MyImage::GetClientRect()
{
	return m_rcClientRect;
}


double MyImage::GetClientRatio()
{
	return m_dClientRatio;
}


/////////////////////////*--3--*//////////////////////////
CRect MyImage::GetFrameRect()
{
	return m_rcFrameRect;
}

void MyImage::CalcFrameRect(double marginOffset)
{
	if (m_dClientRatio < m_dSrcRatio)
	//if (m_dClientRatio <1)
		m_szFrameSize = CSize(m_rcClientRect.Width()*marginOffset, m_rcClientRect.Width()*marginOffset / m_dSrcRatio);
	else
		m_szFrameSize = CSize(m_rcClientRect.Height()*marginOffset*m_dSrcRatio, m_rcClientRect.Height()*marginOffset);

	m_ptFrameCenter = CPoint(m_szFrameSize.cx / 2, m_szFrameSize.cy / 2);
	m_rcFrameRect = CRect(CPoint(0, 0), m_szFrameSize);
}

CPoint MyImage::GetFrameCenter()
{
	return m_ptFrameCenter;
}


CSize MyImage::GetFrameSize()
{
	return m_szFrameSize;
}


void MyImage::DisableAllOptions()
{
	IsLButtonDown = false;
	IsRButtonDown = false;
	IsWheelDown = false;
	IsWheelUp = false;

	EnableShift = false;
	EnableDrag = false;
	EnablePreviewShift = false;
	EnablePreviewDrag = false;
}







/////////////////////////////////////////////////////////////////////////////
/////////////////       Private Function        /////////////////////////////
/////////////////////////////////////////////////////////////////////////////



/////////////////////////*--1--*//////////////////////////
void MyImage::CalcSrc2FrameScale()
{
	m_fXScale = (float)m_szSrcSize.cx / (float)m_szFrameSize.cx;
	m_fYScale = (float)m_szSrcSize.cy / (float)m_szFrameSize.cy;
}


/////////////////////////*--2--*//////////////////////////
void MyImage::CalcClientRatio()
{
	m_dClientRatio = (double)m_rcClientRect.Width() / (double)m_rcClientRect.Height();
}


void MyImage::InitParams()
{
	m_dSrcRatio = (double)m_szSrcSize.cx / (double)m_szSrcSize.cy;
	m_rcSrcRect = CRect(CPoint(0, 0), m_szSrcSize);
	m_ptSrcCenter = CPoint((int)(m_szSrcSize.cx*0.5), (int)(m_szSrcSize.cy*0.5));
	CalcFrameRect();
	CalcSrc2FrameScale();
	DisableAllOptions();
	IsImageInfoLoaded = true;
}