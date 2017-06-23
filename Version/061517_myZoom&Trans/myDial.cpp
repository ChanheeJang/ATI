// myDial.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Task2.h"
#include "myDial.h"
#include "afxdialogex.h"


// myDial 대화 상자입니다.

IMPLEMENT_DYNAMIC(myDial, CDialogEx)

myDial::myDial(CWnd* pParent /*=NULL*/)
	: CDialogEx(myDial::IDD, pParent)
{

}

myDial::~myDial()
{
}

void myDial::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(myDial, CDialogEx)
END_MESSAGE_MAP()


// myDial 메시지 처리기입니다.
