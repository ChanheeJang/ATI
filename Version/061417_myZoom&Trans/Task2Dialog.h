#pragma once


// Task2Dialog ��ȭ �����Դϴ�.

class Task2Dialog : public CDialogEx
{
	DECLARE_DYNAMIC(Task2Dialog)

public:
	Task2Dialog(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~Task2Dialog();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
