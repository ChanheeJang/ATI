
// Task2View.cpp : implementation of the CTask2View class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Task2.h"
#endif

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
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	//ON_WM_PAINT()
	 
	ON_WM_LBUTTONDOWN()
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
		cout <<"pDoc->m_DIB.m_lpBMIH "<<  pDoc->m_DIB.m_lpBMIH << endl;
		cout << "srcWidth " << srcWidth << endl;
		cout << "srcHeight " << srcHeight << endl;
		CRect client;
		GetClientRect(&client);
		CSize imgSize(srcWidth, srcHeight);
		CRect srcRect(CPoint(0, 0), imgSize);

		double clientRatio = (double)client.Width() / (double)client.Height();
		CSize dstSize;
		float dsplyOffset = 0.95;

		// Change size of an image being displayed according to that of client window
		if (clientRatio < srcRatio)
			dstSize = CSize(client.Width()*dsplyOffset, client.Width()*dsplyOffset / srcRatio);
		else
			dstSize = CSize(client.Height()*dsplyOffset*srcRatio, client.Height()*dsplyOffset);

		CRect dstRect(CPoint(0, 0), dstSize);
		pDoc->m_DIB.Draw(pDC, dstRect, srcRect);
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

void CTask2View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
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
	CZoomInfo zoom;

	CView::OnLButtonDown(nFlags, point);
}
