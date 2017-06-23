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
	CPoint newCenter;
	CRect shiftedSrcRect;
	bool isShift;
	CSize frameSize;
// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG1 };

	void updatePreview();
	LPCRECT previewRect;
	bool isPreviewOn;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
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
