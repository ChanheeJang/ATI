// myDial.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Task2.h"
#include "myDial.h"
#include "afxdialogex.h"


// myDial ��ȭ �����Դϴ�.

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


// myDial �޽��� ó�����Դϴ�.
