#pragma once
#include "stdafx.h"

class MyImage
{
public:
	MyImage();
	~MyImage();

public:
	bool IsImageInfoLoaded;

	bool IsLButtonDown;
	bool IsRButtonDown;
	bool IsWheelDown;
	bool IsWheelUp;

	bool EnableShift;
	bool EnableDrag;
	bool EnablePreviewShift;
	bool EnablePreviewDrag;

private:
	int m_iSrcWidth;
	int m_iSrcHeight;
	float m_fXScale;
	float m_fYScale;
	double m_dSrcRatio; //m_dSrcRatio =(double)srcSize.cx / (double)srcSize.cy;
	CSize m_szSrcSize;
	CRect m_rcSrcRect;
	CPoint m_ptSrcCenter;

	CSize m_szFrameSize;
	CRect m_rcFrameRect;
	CPoint m_ptFrameCenter;

	CRect m_rcClientRect;
	double m_dClientRatio; //m_dSrcRatio = (double)m_rcClientRect.Width() / (double)m_rcClientRect.Height();
//////////////////////////////////////////////
//////////////////////////////////////////////

public:
	////////*--1--*////////
	CSize GetSrcSize();
	CRect GetSrcRect();
	double GetSrcRatio();
	CPoint GetSrcCenter();

	void SetSrcSize(int srcWidth, int srcHeight);
	void SetSrcSize(CSize srcSize);
	void SetSrcRect(CRect srcRect);

	float GetXScale();
	float GetYScale();

 
	////////*--2--*////////
	void SetClientRect(CRect clientRect);
	CRect GetClientRect();

	double GetClientRatio();


	////////*--3--*////////
	CRect GetFrameRect();
	void CalcFrameRect(double marginOffset=1);
	CPoint GetFrameCenter();
	CSize GetFrameSize();


	void DisableAllOptions();
	
private:
	////////*--1--*////////
	void CalcSrc2FrameScale();


	////////*--2--*////////
	void CalcClientRatio();

	void InitParams();
};