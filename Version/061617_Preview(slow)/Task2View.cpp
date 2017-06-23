
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
		double srcRatio;
		int srcWidth = pDoc->m_DIB.GetWidth();
		int srcHeight = pDoc->m_DIB.GetHeight();

		srcRatio = ((double)srcWidth / (double)srcHeight);
		//cout << "srcWidth " << srcWidth << endl;
		//cout << "srcHeight " << srcHeight << endl;
		CRect client;
		GetClientRect(&client);
		CSize imgSize(srcWidth, srcHeight);
		CRect srcRect(CPoint(0, 0), imgSize);
		if (pDoc->zoom.getPrev().Width() != 0) srcRect = pDoc->zoom.getPrev();
		double clientRatio = (double)client.Width() / (double)client.Height();
		CSize frameSize;
		float dsplyOffset = 1;

		// Change size of an image being displayed according to that of client window
		if (clientRatio < srcRatio)
			frameSize = CSize(client.Width()*dsplyOffset, client.Width()*dsplyOffset / srcRatio);
		else
			frameSize = CSize(client.Height()*dsplyOffset*srcRatio, client.Height()*dsplyOffset);

		CRect dstRect(CPoint(0, 0), frameSize);

		//////////////////////////////////////////////////////////tt
		pDoc->zoom.init(imgSize, dstRect);
		float zoomScale = 1.3;
		if (LbtnDown)srcRect = pDoc->zoom.IncreaseZoomX(clickedPt, dstRect );
		else if (RbtnDown)srcRect = pDoc->zoom.DecreaseZoomX(clickedPt, dstRect );
		else if (wheelUp)
		{
			srcRect = pDoc->zoom.IncreaseZoomX(CPoint(frameSize.cx/2,frameSize.cy/2), dstRect, zoomScale);
		}
		else if (wheelDown)
		{
			srcRect = pDoc->zoom.DecreaseZoomX(CPoint(frameSize.cx / 2, frameSize.cy / 2), dstRect, zoomScale);
		}
		else if (moveOn &&  pDoc->zoom.getZoom() > 1)
		{
			CRect prev = pDoc->zoom.getPrev();
			int xDiff = pDoc->zoom.getScale()*(clickedPt.x - frameSize.cx / 2);
			int yDiff = pDoc->zoom.getScale()*(clickedPt.y - frameSize.cy / 2);
			CPoint newCenter = pDoc->zoom.checkCntrPt(CPoint(pDoc->zoom.getCurrentCenter().x + xDiff, pDoc->zoom.getCurrentCenter().y+ yDiff), prev);
	
			srcRect = CRect(newCenter.x - prev.Width() / 2, newCenter.y - prev.Height()/2, newCenter.x + prev.Width() / 2, newCenter.y+prev.Height()/2);
			pDoc->zoom.setCenterPt(srcRect);
		}
		else if (dragOn &&  pDoc->zoom.getZoom() > 1)
		{
			CRect prev = pDoc->zoom.getPrev();
			float scale = pDoc->zoom.getScale(); //-- 스케일이 너무 작으면 확대했을때 잘 안움직이는 것을 보정
			dragNowPt.x *= -(scale > 0.2 ? scale : 0.2);
			dragNowPt.y *= -(scale > 0.2 ? scale : 0.2);
			CPoint newCenter = pDoc->zoom.checkCntrPt(CPoint(pDoc->zoom.getCurrentCenter().x + dragNowPt.x, pDoc->zoom.getCurrentCenter().y + dragNowPt.y), prev);

			srcRect = CRect(newCenter.x - prev.Width() / 2, newCenter.y - prev.Height() / 2, newCenter.x + prev.Width() / 2, newCenter.y + prev.Height() / 2);
			pDoc->zoom.setCenterPt(srcRect);
		}
		if (pFrame->Dlg!=NULL && pFrame->Dlg->isShift)
		{
			srcRect = pFrame->Dlg->shiftedSrcRect;
			pFrame->Dlg->isShift = false;
		}


		////////////////////////////////////////////////////////tt

		pDoc->m_DIB.Draw(pDC, dstRect, srcRect);
		pDoc->zoom.setPrev(srcRect);

		//-- print current magnification info on statusbar
		CString msg;
		msg.Format(_T("Magnification: x%.2f"),pDoc->zoom.getZoom());
		pFrame->m_wndStatusBar.SetPaneText(2, msg);

		pFrame->Dlg->updatePreview();
		 
	}
	 
}
// CTask2View printing


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
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	killFunctions();
	LbuttonDowned = true;
	if (nFlags &  MK_SHIFT)
	{
		moveOn = true; 
		clickedPt = point;
	}
	else if (nFlags &  MK_CONTROL)
	{
		dragOn = true; 
		dragStartPt = point;
	}
	else if (nFlags & MK_LBUTTON)
	{
		LbtnDown = true;
	}
	Invalidate(false); //-- By setting false, it does not erase background when updating and therefore Elliminate "blinking problem"

	CView::OnLButtonDown(nFlags, point);
}


void CTask2View::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	killFunctions();
	RbtnDown = true;
	Invalidate(false);  
	//CView::OnRButtonDown(nFlags, point);
}


BOOL CTask2View::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	killFunctions();
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (zDelta > 0)
	{
		wheelUp = true;
	}
	if (zDelta < 0)
	{
		wheelDown = true;
	}

 	Invalidate(false);  
	//return CView::OnMouseWheel(nFlags, zDelta, pt);
	return true;
}


void CTask2View::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	clickedPt = point;
	//cout << "point.x " << point.x << "  point.y " << point.y << endl;

	//-- Print current position of mouse cursor on staturbar
	CString msg;
	msg.Format(_T("X: %d, Y: %d"), point.x, point.y);
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	pFrame->m_wndStatusBar.SetPaneText(3, msg);
	
	 
	if (dragOn && (nFlags & MK_CONTROL))
	{
		dragNowPt.x = point.x - dragStartPt.x;
		dragNowPt.y = point.y - dragStartPt.y;
		dragStartPt = point;
		Invalidate(false); 
	}
		
	CView::OnMouseMove(nFlags, point);
}


void CTask2View::killFunctions()
{
	LbtnDown = false;
	RbtnDown = false;
	wheelUp = false;
	wheelDown = false;
	moveOn = false;
	dragOn = false;
}

void CTask2View::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
 
	dragOn = false;
	CView::OnLButtonUp(nFlags, point);
}
