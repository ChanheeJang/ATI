
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

		double clientRatio = (double)client.Width() / (double)client.Height();
		CSize frameSize;
		float dsplyOffset = 0.95;

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
			//!!-- 수정해야 할 사항 : 가장자리쪽을 선택해서, 첫 zoom에 원하는 위치가 center로 안오면 그다음부터 센터에 안맞춰짐
			//!!--						아마도, 처음 위치를 기억해서, 그 위치가 중앙에 올 때 까지 each zoom마다 조금씩 움직이는거??
			//if (pDoc->zoom.getZoom() != 1)clickedPt = CPoint(frameSize.cx/2,frameSize.cy/2);
			srcRect = pDoc->zoom.IncreaseZoomX(CPoint(frameSize.cx/2,frameSize.cy/2), dstRect, zoomScale);
		}
		else if (wheelDown)
		{
			//if (pDoc->zoom.getZoom() != 1)clickedPt = CPoint(frameSize.cx / 2, frameSize.cy / 2);
			
			srcRect = pDoc->zoom.DecreaseZoomX(CPoint(frameSize.cx / 2, frameSize.cy / 2), dstRect, zoomScale);
		}


		if (moveOn &&  pDoc->zoom.getZoom() > 1)
		{
			CRect prev = pDoc->zoom.getPrev();
			int xDiff = pDoc->zoom.getScale()*(clickedPt.x - frameSize.cx / 2);
			int yDiff = pDoc->zoom.getScale()*(clickedPt.y - frameSize.cy / 2);
			srcRect = CRect(prev.left + xDiff, prev.top + yDiff, prev.right + xDiff, prev.bottom + yDiff);
			pDoc->zoom.setCenterPt(srcRect);
			pDoc->zoom.setPrev(srcRect);
		}
		else if (dragOn &&  pDoc->zoom.getZoom() > 1)
		{
			CRect prev = pDoc->zoom.getPrev();
			dragNowPt.x *= pDoc->zoom.getScale();
			dragNowPt.y *= pDoc->zoom.getScale();
			srcRect = CRect(prev.left - dragNowPt.x, prev.top - dragNowPt.y, prev.right - dragNowPt.x, prev.bottom - dragNowPt.y);
			pDoc->zoom.setCenterPt(srcRect);
			pDoc->zoom.setPrev(srcRect);
		}
		////////////////////////////////////////////////////////tt

		pDoc->m_DIB.Draw(pDC, dstRect, srcRect);
  
 

		//-- print current magnification info on statusbar
		CString msg;
		msg.Format(_T("Magnification: x%.2f"),pDoc->zoom.getZoom());
		CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
		pFrame->m_wndStatusBar.SetPaneText(2, msg);
	}

	 




	//AfxMessageBox(TEXT("Did you call me?"));
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CView::OnPaint()을(를) 호출하지 마십시오.


	//CDC memDC;
	//BITMAP bmpinfo;

	//memDC.CreateCompatibleDC(pDC);

	//CBitmap bmp;
	//CBitmap* pOldBmp = NULL;
	//bmp.LoadBitmapW(IDB_TEST);
	//bmp.GetBitmap(&bmpinfo);

	//pOldBmp = memDC.SelectObject(&bmp);
	//pDC->BitBlt(0, 0, bmpinfo.bmWidth, bmpinfo.bmHeight, &memDC, 0, 0, SRCCOPY);
	//memDC.SelectObject(pOldBmp);
	// 
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
	//CPaintDC dc(this); // device context for painting
	//// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	//// 그리기 메시지에 대해서는 CView::OnPaint()을(를) 호출하지 마십시오.
	//CDC memDC;
	//BITMAP bmpinfo;

	//memDC.CreateCompatibleDC(&dc);

	//CBitmap bmp;
	//CBitmap* pOldBmp = NULL;
	//bmp.LoadBitmapW(IDB_TEST);
	//bmp.GetBitmap(&bmpinfo);

	//pOldBmp = memDC.SelectObject(&bmp);
	//dc.BitBlt(0, 0, bmpinfo.bmWidth, bmpinfo.bmHeight, &memDC, 0, 0, SRCCOPY);
	//memDC.SelectObject(pOldBmp);
	//CView::OnPaint();

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
	Invalidate(false); //-- By setting false, it does not erase background when updating and therefore Elliminate "blinking problem"
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

 	Invalidate(false); //-- By setting false, it does not erase background when updating and therefore Elliminate "blinking problem"
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
		Invalidate(false); //-- By setting false, it does not erase background when updating and therefore Elliminate "blinking problem"
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
