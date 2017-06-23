
// Task2View.cpp : implementation of the CTask2View class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Task2.h"
#endif

#include "MainFrm.h"
#include "Task2Doc.h"
#include "Task2View.h"
#include "myDial.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTask2View

IMPLEMENT_DYNCREATE(CTask2View, CView)

BEGIN_MESSAGE_MAP(CTask2View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CTask2View::OnFilePrintPreview)
	//ON_WM_CONTEXTMENU()
	//ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CTask2View construction/destruction

CTask2View::CTask2View()
{
	// TODO: add construction code here
 
}

CTask2View::~CTask2View()
{
 
}

BOOL CTask2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CTask2View drawing

void CTask2View::OnDraw(CDC* pDC)
{
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	CTask2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// Get ratio of input image
	LPCTSTR path = pDoc->GetPathName();
	if (path != NULL && path[0] != 0)
	{
		if (!pDoc->myImage.IsImageInfoLoaded)
		{
			pDoc->myImage.SetSrcSize(pDoc->m_DIB.GetWidth(), pDoc->m_DIB.GetHeight());
			CRect clientRect;
			GetClientRect(&clientRect);
			pDoc->myImage.SetClientRect(clientRect);
			pDoc->myImage.CalcFrameRect();
		}
		CRect srcRect = pDoc->myImage.GetSrcRect();
		CRect dstRect = pDoc->myImage.GetFrameRect();
		//////////////////////////////////////////////////////////tt
		pDoc->myImage.init();
		float zoomScale = 1.3;
		if (pDoc->myImage.IsLButtonDown)			srcRect = pDoc->myImage.IncreaseZoomX(clickedPt, dstRect);
		else if (pDoc->myImage.IsRButtonDown)		srcRect = pDoc->myImage.DecreaseZoomX(clickedPt, dstRect);
		else if (pDoc->myImage.IsWheelUp)			srcRect = pDoc->myImage.IncreaseZoomX(pDoc->myImage.GetFrameCenter(), dstRect, zoomScale);
		else if (pDoc->myImage.IsWheelDown)			srcRect = pDoc->myImage.DecreaseZoomX(pDoc->myImage.GetFrameCenter(), dstRect, zoomScale);
		else if (pDoc->myImage.EnableShift)			srcRect = pDoc->myImage.Shift2NewCenter(pDoc->myImage.GetSrcZoomCenter(), pDoc->myImage.GetSrcSize());
		else if (pDoc->myImage.EnableDrag)			srcRect = pDoc->myImage.Shift2NewCenter(pDoc->myImage.GetSrcZoomCenter(), pDoc->myImage.GetSrcSize());
		else if (pDoc->myImage.EnablePreviewShift)	srcRect = pFrame->Dlg->shiftedSrcRect;
		else if (pDoc->myImage.EnablePreviewDrag)	srcRect = pFrame->Dlg->shiftedSrcRect;

		////////////////////////////////////////////////////////tt
		pDoc->m_DIB.Draw(pDC, dstRect, srcRect);
		pDoc->myImage.SetSrcRect(srcRect);

		pFrame->Dlg->updatePreview();

		//-- print current magnification info on statusbar
		CString msg;
		msg.Format(_T("Magnification: x%.2f"),pDoc->myImage.getZoom());
		pFrame->m_wndStatusBar.SetPaneText(2, msg);
	}
}


void CTask2View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CTask2View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTask2View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTask2View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

 

void CTask2View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif

}
 
// CTask2View diagnostics

#ifdef _DEBUG
void CTask2View::AssertValid() const
{
	CView::AssertValid();
}

void CTask2View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTask2Doc* CTask2View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTask2Doc)));
	return (CTask2Doc*)m_pDocument;	
}
#endif //_DEBUG


// CTask2View message handlers



void CTask2View::OnPaint()
{
 
	//// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	//// 그리기 메시지에 대해서는 CView::OnPaint()을(를) 호출하지 마십시오.
 

}


void CTask2View::OnLButtonDown(UINT nFlags, CPoint point)
{
	CTask2Doc* pDoc = GetDocument();
	pDoc->myImage.DisableAllOptions();
	LbuttonDowned = true;
	if (nFlags &  MK_SHIFT)
	{
		pDoc->myImage.EnableShift = true;
		clickedPt = point;
	 
		int xDiff = pDoc->myImage.GetXScale()*(clickedPt.x - pDoc->myImage.GetFrameCenter().x);
		int yDiff = pDoc->myImage.GetYScale()*(clickedPt.y - pDoc->myImage.GetFrameCenter().y);
		CPoint newCenter = pDoc->myImage.checkCntrPt(CPoint(pDoc->myImage.GetSrcZoomCenter().x + xDiff, pDoc->myImage.GetSrcZoomCenter().y + yDiff), pDoc->myImage.GetSrcRect());
		pDoc->myImage.SetSrcZoomCenter(newCenter);
	}
	else if (nFlags &  MK_CONTROL)
	{
		pDoc->myImage.EnableDrag = true;
		dragStartPt = point;
	}
	else if (nFlags & MK_LBUTTON)
	{
		pDoc->myImage.IsLButtonDown = true;
	}
	Invalidate(false); //-- By setting false, it does not erase background when updating and therefore Elliminate "blinking problem"

	CView::OnLButtonDown(nFlags, point);
}


void CTask2View::OnRButtonDown(UINT nFlags, CPoint point)
{
	CTask2Doc* pDoc = GetDocument();
	
	pDoc->myImage.DisableAllOptions();
	pDoc->myImage.IsRButtonDown = true;
	Invalidate(false);  
	//CView::OnRButtonDown(nFlags, point);
}


BOOL CTask2View::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	CTask2Doc* pDoc = GetDocument();
	pDoc->myImage.DisableAllOptions();
	if (zDelta > 0)
	{
		pDoc->myImage.IsWheelUp = true;
	}
	if (zDelta < 0)
	{
		pDoc->myImage.IsWheelDown = true;
	}

 	Invalidate(false);  
	//return CView::OnMouseWheel(nFlags, zDelta, pt);
	return true;
}


void CTask2View::OnMouseMove(UINT nFlags, CPoint point)
{
	CTask2Doc* pDoc = GetDocument();
	clickedPt = point;
	//cout << "point.x " << point.x << "  point.y " << point.y << endl;

	//-- Print current position of mouse cursor on staturbar
	CString msg;
	msg.Format(_T("X: %d, Y: %d"), point.x, point.y);
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	pFrame->m_wndStatusBar.SetPaneText(3, msg);
	
	 
	if (pDoc->myImage.EnableDrag && (nFlags & MK_CONTROL))
	{
		dragNowPt.x = point.x - dragStartPt.x;
		dragNowPt.y = point.y - dragStartPt.y;
		dragStartPt = point;

		float scaleX = pDoc->myImage.GetXScale(); //-- 스케일이 너무 작으면 확대했을때 잘 안움직이는 것을 보정
		float scaleY = pDoc->myImage.GetYScale();
		dragNowPt.x *= -1.5;//(scaleX > 0.2 ? scaleX : 0.2);
		dragNowPt.y *= -1.5;//(scaleY > 0.2 ? scaleY : 0.2);
		CPoint newCenter = pDoc->myImage.checkCntrPt(CPoint(pDoc->myImage.GetSrcZoomCenter().x + dragNowPt.x, pDoc->myImage.GetSrcZoomCenter().y + dragNowPt.y), pDoc->myImage.GetSrcRect());

		pDoc->myImage.SetSrcZoomCenter(newCenter);
		Invalidate(false); 
	}
		
	CView::OnMouseMove(nFlags, point);
}

 
void CTask2View::OnLButtonUp(UINT nFlags, CPoint point)
{
	CTask2Doc* pDoc = GetDocument();
	pDoc->myImage.EnableDrag = false;
	CView::OnLButtonUp(nFlags, point);
}


void CTask2View::OnSize(UINT nType, int cx, int cy)
{
	CTask2Doc* pDoc = GetDocument();
	CView::OnSize(nType, cx, cy);
	CRect clientRect;
	GetClientRect(&clientRect);
	pDoc->myImage.SetClientRect(clientRect);
	pDoc->myImage.CalcFrameRect();
}
