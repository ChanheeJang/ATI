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
	CPoint newCenter;
	CRect shiftedSrcRect;
	bool isShift;
	CSize frameSize;
// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG1 };

	void updatePreview();
	LPCRECT previewRect;
	bool isPreviewOn;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	BOOL myDial::OnInitDialog();
	CBitmap BGImage;
	CBitmap resizedBGImage;
	CTask2Doc *pDoc;
	CSize BGSize;
	CSize getFrameSize();


	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
