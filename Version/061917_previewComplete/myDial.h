#pragma once
#include "Task2Doc.h"
#include "resource.h"
// myDial 대화 상자입니다.


class myDial : public CDialog
{
	DECLARE_DYNAMIC(myDial)

public:
	myDial(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~myDial();


	float prev2srcRatioX;
	float prev2srcRatioY;
	CRect shiftedSrcRect;

	CSize frameSize;
	bool isBGresized;
// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG1 };

	void updatePreview();
	BOOL myDial::OnInitDialog();
	LPCRECT previewRect;
	bool isPreviewOn;
	void detachBG();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	CBitmap BGImage;
	CBitmap resizedBGImage;
	CTask2Doc *pDoc;
	CSize BGSize;
	//CSize getFrameSize(int Framewidth);
	CSize getFrameSize();
	CDC* destDC;
	CBitmap *pOldBmp2;
	CRect imgRect;

	bool LDown;
	CPoint dragStartPt;
	CPoint dragNowPt;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnClose();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};
