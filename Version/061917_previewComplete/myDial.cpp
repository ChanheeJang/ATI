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
	
}


BOOL myDial::OnInitDialog()
{
	CDialog::OnInitDialog();

	BGImage.Attach(pDoc->m_DIB.m_hBitmap);
	BITMAP bm;
	BGImage.GetBitmap(&bm);
	BGSize = CSize(bm.bmWidth, bm.bmHeight);
	Invalidate(1);
	//getFrameSize(300);
	//SetWindowPos(NULL, 0, 0, frameSize.cx, frameSize.cy, SWP_NOMOVE);
	frameSize = CSize(0, 0);
	isPreviewOn = false;
	destDC = new CDC;
	isBGresized = false;
	LDown = false;

	return true;
}
myDial::~myDial()
{
	BGImage.Detach();
	destDC->SelectObject(pOldBmp2);
	destDC->DeleteDC();
}

void myDial::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

void myDial::detachBG()
{
	BGImage.Detach();
}

BEGIN_MESSAGE_MAP(myDial, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_CLOSE()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
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

//CSize myDial::getFrameSize(int Framewidth)
//{
// 	//-- This part is to keep the image ratio in preview window
//	double srcRatio = ((double)BGSize.cx / (double)BGSize.cy);
// 
// 	frameSize = CSize(Framewidth, Framewidth/srcRatio);
//
//	prev2srcRatioX = (float)frameSize.cx / (float)BGSize.cx;
//	prev2srcRatioY = (float)frameSize.cy / (float)BGSize.cy;
//
//	return frameSize;
//}
//BOOL myDial::OnEraseBkgnd(CDC* pDC)
//{
//	clock_t t=clock();
//	 //TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
//	CDC* dcMemory = new CDC;
//	dcMemory->CreateCompatibleDC(pDC);
//	dcMemory->SelectObject(&BGImage);
//	
//	if (frameSize.cx==NULL)
//	getFrameSize();
//
//	HDC hDC = pDC->m_hDC;
//	hDC = pDC->GetSafeHdc();
//	SetStretchBltMode(hDC, HALFTONE); //-- 이 명령어가 없으면, Windows API 는 단순히 성능을 최우선으로 Bitmap resize를 한다. 따라서 이를 통해 visual effect가 중시되도록 해줘야 함.
//
//	pDC->StretchBlt( 0, 0, frameSize.cx, frameSize.cy,dcMemory, 0, 0, BGSize.cx, BGSize.cy, SRCCOPY);
//	  
//	////////////- Draw Rectangle -////////////////
//	CClientDC dc(this);
//	CPen pen;
//	CBrush* pOldBrush;
//	pen.CreatePen(PS_SOLID, 3, RGB(255, 255, 0));    
//	pOldBrush = (CBrush*)dc.SelectStockObject(NULL_BRUSH);
//	dc.SelectObject(&pen);
//	
//	CRect imgRect=	pDoc->zoom.getZoomedRect();
//	imgRect.left   *= prev2srcRatioX;
//	imgRect.right  *= prev2srcRatioX;
//	imgRect.top	   *= prev2srcRatioY;
//	imgRect.bottom *= prev2srcRatioY;
//
//	previewRect = imgRect;
//	dc.Rectangle(previewRect);
//
//	//Release CPen object
//	dc.SelectObject(pOldBrush);
//	pen.DeleteObject();
//	
//	return false;
//}
  
BOOL myDial::OnEraseBkgnd(CDC* pDC)
{
	if (!isBGresized)
	{
		getFrameSize();
		CDC srcDC;
		srcDC.CreateCompatibleDC(pDC);
		destDC->CreateCompatibleDC(pDC);


		HDC hDC = destDC->m_hDC;
		hDC = destDC->GetSafeHdc();
		SetStretchBltMode(hDC, HALFTONE); //-- 이 명령어가 없으면, Windows API 는 단순히 성능을 최우선으로 Bitmap resize를 한다. 따라서 이를 통해 visual effect가 중시되도록 해줘야 함.
		CBitmap destBmp ;
		destBmp.CreateCompatibleBitmap(pDC, frameSize.cx, frameSize.cy);

		CBitmap *pOldBmp1 = srcDC.SelectObject(&BGImage);
		pOldBmp2 = destDC->SelectObject(&destBmp);

		destDC->StretchBlt(0, 0, frameSize.cx, frameSize.cy, &srcDC, 0, 0, BGSize.cx, BGSize.cy, SRCCOPY);

		srcDC.SelectObject(pOldBmp1);
		srcDC.DeleteDC();
		destBmp.Detach();
		isBGresized = true;
	}

	//Draw resized BG on dialog
	pDC->BitBlt(0, 0, frameSize.cx, frameSize.cy, destDC, 0, 0, SRCCOPY);

	////////////- Draw Rectangle -////////////////
	CClientDC dc(this);
	CPen pen;
	CBrush* pOldBrush;
	pen.CreatePen(PS_SOLID, 3, RGB(255, 255, 0));
	pOldBrush = (CBrush*)dc.SelectStockObject(NULL_BRUSH);
	dc.SelectObject(&pen);

	imgRect = pDoc->zoom.getZoomedRect();
	imgRect.left *= prev2srcRatioX;
	imgRect.right *= prev2srcRatioX;
	imgRect.top *= prev2srcRatioY;
	imgRect.bottom *= prev2srcRatioY;

	previewRect = imgRect;
	dc.Rectangle(previewRect);

	//Release CPen object
	dc.SelectObject(pOldBrush);
	pen.DeleteObject();
	isPreviewOn = true;
	return false;
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
	//cout << "Clicked on (" << point.x << "," << point.y << ")\n";
	SetFocus();
	CTask2View* pView =	(CTask2View*)((CMainFrame*)AfxGetApp()->m_pMainWnd)->GetActiveView();
	pView->killFunctions();
	pView->previewShifted = true;
	
	
	int xDiff = (point.x - (imgRect.left + imgRect.right) / 2) / prev2srcRatioX;
	int yDiff = (point.y - (imgRect.top + imgRect.bottom) / 2) / prev2srcRatioY;
	CRect prev = pDoc->zoom.getPrev();
	CPoint newCenter = pDoc->zoom.checkCntrPt(CPoint(pDoc->zoom.getCurrentCenter().x + xDiff, pDoc->zoom.getCurrentCenter().y + yDiff), prev);

	shiftedSrcRect = CRect(newCenter.x - prev.Width() / 2, newCenter.y - prev.Height() / 2, newCenter.x + prev.Width() / 2-1, newCenter.y + prev.Height() / 2-1);

	pDoc->zoom.setCenterPt(shiftedSrcRect);

	pView->Invalidate(false); 
	updatePreview();


	//-- Drag function
	dragStartPt = point;
	LDown = true;

	CDialog::OnLButtonDown(nFlags, point);
}


void myDial::OnClose()
{
	isPreviewOn = false;
	CDialog::OnClose();
}


void myDial::OnLButtonUp(UINT nFlags, CPoint point)
{
	LDown = false;
	CDialog::OnLButtonUp(nFlags, point);
}


void myDial::OnMouseMove(UINT nFlags, CPoint point)
{
	if (LDown)
	{
		CTask2View* pView = (CTask2View*)((CMainFrame*)AfxGetApp()->m_pMainWnd)->GetActiveView();
		pView->killFunctions();
		pView->previewDragged = true;

		dragNowPt.x = (point.x - dragStartPt.x) / prev2srcRatioX;
		dragNowPt.y = (point.y - dragStartPt.y) / prev2srcRatioX;
		dragStartPt = point;
		CRect prev = pDoc->zoom.getPrev();
		CPoint newCenter = pDoc->zoom.checkCntrPt(CPoint(pDoc->zoom.getCurrentCenter().x + dragNowPt.x, pDoc->zoom.getCurrentCenter().y + dragNowPt.y), prev);

		shiftedSrcRect = CRect(newCenter.x - prev.Width() / 2, newCenter.y - prev.Height() / 2, newCenter.x + prev.Width() / 2 + 1, newCenter.y + prev.Height() / 2 + 1);

		pDoc->zoom.setCenterPt(shiftedSrcRect);
		pView->Invalidate(false);
		updatePreview();
	}
	CDialog::OnMouseMove(nFlags, point);
}


BOOL myDial::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	CTask2View* pView = (CTask2View*)((CMainFrame*)AfxGetApp()->m_pMainWnd)->GetActiveView();
	pView->killFunctions();
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (zDelta > 0)
	{
		pView->wheelUp = true;
	}
	if (zDelta < 0)
	{
		pView->wheelDown = true;
	}

	pView->Invalidate(false);
	//return CView::OnMouseWheel(nFlags, zDelta, pt);
	return true;
}
