// myDial.cpp : 구현 파일입니다.
//
#pragma once
#include "stdafx.h"
#include "Task2.h"
#include "myDial.h"
#include "afxdialogex.h"
#include "Task2View.h"
#include "MainFrm.h"
#include "Task2Doc.h"

// myDial 대화 상자입니다.

IMPLEMENT_DYNAMIC(myDial, CDialog)

myDial::myDial(CWnd* pParent /*=NULL*/)	: CDialog(myDial::IDD, pParent)
{
	CTask2View* pView =
		(CTask2View*)((CMainFrame*)AfxGetApp()->m_pMainWnd)->GetActiveView();
	pDoc = pView->GetDocument();
	isPreviewOn = false;
	isShift = false;
	frameSize = CSize(NULL,NULL);
}

BOOL myDial::OnInitDialog()
{
	CDialog::OnInitDialog();

	BGImage.Attach(pDoc->m_DIB.m_hBitmap);
	BITMAP bm;
	BGImage.GetBitmap(&bm);
	BGSize = CSize(bm.bmWidth, bm.bmHeight);
	Invalidate(1);

	return true;
}
myDial::~myDial()
{
	BGImage.Detach();
}

void myDial::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

 

BEGIN_MESSAGE_MAP(myDial, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// myDial 메시지 처리기입니다.


CSize myDial::getFrameSize()
{
	CRect rcClient;
	GetClientRect(&rcClient);

	//-- This part is to keep the image ratio in preview window
	double srcRatio = ((double)BGSize.cx / (double)BGSize.cy);
	double clientRatio = (double)rcClient.Width() / (double)rcClient.Height();

	if (clientRatio < srcRatio)
		frameSize = CSize(rcClient.Width(), rcClient.Width() / srcRatio);
	else
		frameSize = CSize(rcClient.Height()*srcRatio, rcClient.Height());

	prev2srcRatioX = (float)frameSize.cx / (float)BGSize.cx;
	prev2srcRatioY = (float)frameSize.cy / (float)BGSize.cy;

	return frameSize;
}
BOOL myDial::OnEraseBkgnd(CDC* pDC)
{
	clock_t t=clock();
	 //TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CDC dcMemory;
	dcMemory.CreateCompatibleDC(pDC);
	dcMemory.SelectObject(&BGImage);
	
	if (frameSize.cx==NULL)
	getFrameSize();

	HDC hDC = pDC->m_hDC;
	hDC = pDC->GetSafeHdc();
	SetStretchBltMode(hDC, HALFTONE); //-- 이 명령어가 없으면, Windows API 는 단순히 성능을 최우선으로 Bitmap resize를 한다. 따라서 이를 통해 visual effect가 중시되도록 해줘야 함.
	pDC->StretchBlt(0, 0, frameSize.cx, frameSize.cy, &dcMemory, 0, 0, BGSize.cx, BGSize.cy, SRCCOPY);
	//::StretchDIBits(pDC->GetSafeHdc(), 0, 0, frameSize.cx, frameSize.cy, 0, 0, BGSize.cx, BGSize.cy, pDoc->m_DIB.m_lpImage, (LPBITMAPINFO)pDoc->m_DIB.m_lpBMIH, DIB_RGB_COLORS, SRCCOPY);
 
	////////////- Draw Rectangle -////////////////
	CClientDC dc(this);

	CPen pen;
	CBrush* pOldBrush;
	pen.CreatePen(PS_DOT, 3, RGB(255, 255, 0));    
	pOldBrush = (CBrush*)dc.SelectStockObject(NULL_BRUSH);
	dc.SelectObject(&pen);


	CRect imgRect=	pDoc->zoom.getZoomedRect();
	imgRect.left   *= prev2srcRatioX;
	imgRect.right  *= prev2srcRatioX;
	imgRect.top	   *= prev2srcRatioY;
	imgRect.bottom *= prev2srcRatioY;

	previewRect = imgRect;
	dc.Rectangle(previewRect);



	return true;
}


void myDial::updatePreview()
{
	CMainFrame* pFrame = (CMainFrame *)AfxGetMainWnd();
	
	if (pFrame->Dlg != NULL)
	{
		pFrame->Dlg->Invalidate(true);
		UpdateWindow();
	}

}

void myDial::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	/*cout << "Clicked on (" << point.x << "," << point.y << ")\n";
	int xDiff = (point.x - frameSize.cx / 2) / prev2srcRatioX;
	int yDiff = (point.y - frameSize.cy / 2) / prev2srcRatioY;
	CRect prev = pDoc->zoom.getPrev();
	newCenter = pDoc->zoom.checkCntrPt(CPoint(pDoc->zoom.getCurrentCenter().x + xDiff, pDoc->zoom.getCurrentCenter().y + yDiff), prev);

	cout << "newCenter  (" << point.x << "," << point.y << ")\n";
	shiftedSrcRect = CRect(newCenter.x - prev.Width() / 2, newCenter.y - prev.Height() / 2, newCenter.x + prev.Width() / 2, newCenter.y + prev.Height() / 2);
	cout << "shiftedSrcRect  (" << shiftedSrcRect.left << "," << shiftedSrcRect.top << "," << shiftedSrcRect.right << "," << shiftedSrcRect.bottom << ")\n";
	pDoc->zoom.setCenterPt(shiftedSrcRect);
	isShift = true;
	
	CTask2View* pView =
		(CTask2View*)((CMainFrame*)AfxGetApp()->m_pMainWnd)->GetActiveView();
	pView->Invalidate(false);*/
	/*else if (nFlags &  MK_CONTROL)
	{
		dragOn = true;
		dragStartPt = point;
	}
 */
	//CDialog::OnLButtonDown(nFlags, point);
}
 