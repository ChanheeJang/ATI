
// Task2View.h : interface of the CTask2View class
//

#pragma once
#include "stdafx.h"

class CTask2View : public CView
{
protected: // create from serialization only
	CTask2View();
	DECLARE_DYNCREATE(CTask2View)

// Attributes
public:
	CTask2Doc* GetDocument() const;
	CImag Cimg;
	CPoint clickedPt;

	bool LbtnDown = false;
	bool RbtnDown = false;
	bool wheelUp = false;
	bool wheelDown = false;
		
	bool moveOn = false;
	bool dragOn = false;
	CPoint dragStartPt;
	CPoint dragNowPt;

	bool LbuttonDowned = false;
	void killFunctions();
// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CTask2View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
private:
	CDib m_dibFile;
	CDib m_dibResource;
protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // debug version in Task2View.cpp
inline CTask2Doc* CTask2View::GetDocument() const
   { return reinterpret_cast<CTask2Doc*>(m_pDocument); }
#endif

