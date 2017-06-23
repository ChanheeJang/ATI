#pragma once


// Task2Dialog 대화 상자입니다.

class Task2Dialog : public CDialogEx
{
	DECLARE_DYNAMIC(Task2Dialog)

public:
	Task2Dialog(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~Task2Dialog();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
