// Task2Dialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Task2.h"
#include "Task2Dialog.h"
#include "afxdialogex.h"


// Task2Dialog 대화 상자입니다.

IMPLEMENT_DYNAMIC(Task2Dialog, CDialogEx)

Task2Dialog::Task2Dialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(Task2Dialog::IDD, pParent)
{

}

Task2Dialog::~Task2Dialog()
{
}

void Task2Dialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Task2Dialog, CDialogEx)
END_MESSAGE_MAP()


// Task2Dialog 메시지 처리기입니다.
