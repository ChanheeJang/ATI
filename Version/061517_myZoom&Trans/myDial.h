#pragma once


// myDial ��ȭ �����Դϴ�.

class myDial : public CDialogEx
{
	DECLARE_DYNAMIC(myDial)

public:
	myDial(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~myDial();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
