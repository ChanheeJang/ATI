#pragma once
#include "Task2Doc.h"
#include "resource.h"
// myDial ��ȭ �����Դϴ�.


class myDial : public CDialog
{
	DECLARE_DYNAMIC(myDial)

public:
	myDial(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~myDial();


	float prev2srcRatioX;
	float prev2srcRatioY;
	CRect shiftedSrcRect;

	CSize frameSize;
	bool isBGresized;
// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG1 };

	void updatePreview();
	BOOL myDial::OnInitDialog();
	LPCRECT previewRect;
	bool isPreviewOn;
	void detachBG();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
