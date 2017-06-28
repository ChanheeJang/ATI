
// MainFrm.cpp : implementation of the CMainFrame class
//

#pragma once
#include "stdafx.h"
#include "Task2.h"
#include "myDial.h"
#include "MainFrm.h"
#include "Task2View.h"
#include "tbb.h"

using namespace tbb;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	ON_WM_SETTINGCHANGE()
	ON_COMMAND(ID_Preview, &CMainFrame::OnPreview)
	ON_WM_DESTROY()
	ON_COMMAND(ID_DILATE, &CMainFrame::OnDilate)
	ON_COMMAND(ID_ERODE, &CMainFrame::OnErode)
	ON_COMMAND(ID_RESET, &CMainFrame::OnReset)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_SEPARATOR,
	ID_SEPARATOR,
	//ID_SEPARATOR,
 
};

// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2008);
	Dlg = NULL;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("Failed to create menubar\n");
		return -1;      // fail to create
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	// prevent the menu bar from taking the focus on activation
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	CString strToolBarName;
	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	m_wndToolBar.SetWindowText(strToolBarName);

	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);
	m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

	// Allow user-defined toolbars operations:
	InitUserToolbars(NULL, uiFirstUserToolBarId, uiLastUserToolBarId);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));
	m_wndStatusBar.SetPaneInfo(2, ID_SEPARATOR, SBPS_POPOUT, 120);
	m_wndStatusBar.SetPaneInfo(3, ID_SEPARATOR, SBPS_POPOUT, 80);
	
	// TODO: Delete these five lines if you don't want the toolbar and menubar to be dockable
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
	DockPane(&m_wndToolBar);


	// enable Visual Studio 2005 style docking window behavior
	CDockingManager::SetDockingMode(DT_SMART);
	// enable Visual Studio 2005 style docking window auto-hide behavior
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// Load menu item image (not placed on any standard toolbars):
	CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : 0);

	// create docking windows
	if (!CreateDockingWindows())
	{
		TRACE0("Failed to create docking windows\n");
		return -1;
	}

	m_wndFileView.EnableDocking(CBRS_ALIGN_ANY);
	m_wndClassView.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndFileView);
	CDockablePane* pTabbedBar = NULL;
	m_wndClassView.AttachToTabWnd(&m_wndFileView, DM_SHOW, TRUE, &pTabbedBar);
	m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndOutput);
	m_wndProperties.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndProperties);

	// set the visual manager and style based on persisted value
	OnApplicationLook(theApp.m_nAppLook);

	// Enable toolbar and docking window menu replacement
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// enable quick (Alt+drag) toolbar customization
	CMFCToolBar::EnableQuickCustomization();

	if (CMFCToolBar::GetUserImages() == NULL)
	{
		// load user-defined toolbar images
		if (m_UserImages.Load(_T(".\\UserImages.bmp")))
		{
			CMFCToolBar::SetUserImages(&m_UserImages);
		}
	}

	// enable menu personalization (most-recently used commands)
	// TODO: define your own basic commands, ensuring that each pulldown menu has at least one basic command.
	CList<UINT, UINT> lstBasicCommands;

	lstBasicCommands.AddTail(ID_FILE_NEW);
	lstBasicCommands.AddTail(ID_FILE_OPEN);
	lstBasicCommands.AddTail(ID_FILE_SAVE);
	lstBasicCommands.AddTail(ID_FILE_PRINT);
	lstBasicCommands.AddTail(ID_APP_EXIT);
	lstBasicCommands.AddTail(ID_EDIT_CUT);
	lstBasicCommands.AddTail(ID_EDIT_PASTE);
	lstBasicCommands.AddTail(ID_EDIT_UNDO);
	lstBasicCommands.AddTail(ID_APP_ABOUT);
	lstBasicCommands.AddTail(ID_VIEW_STATUS_BAR);
	lstBasicCommands.AddTail(ID_VIEW_TOOLBAR);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2003);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_VS_2005);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLUE);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_SILVER);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLACK);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_AQUA);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_WINDOWS_7);
	lstBasicCommands.AddTail(ID_SORTING_SORTALPHABETIC);
	lstBasicCommands.AddTail(ID_SORTING_SORTBYTYPE);
	lstBasicCommands.AddTail(ID_SORTING_SORTBYACCESS);
	lstBasicCommands.AddTail(ID_SORTING_GROUPBYTYPE);

	CMFCToolBar::SetBasicCommands(lstBasicCommands);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

BOOL CMainFrame::CreateDockingWindows()
{
	BOOL bNameValid;

	// Create class view
	CString strClassView;
	bNameValid = strClassView.LoadString(IDS_CLASS_VIEW);
	ASSERT(bNameValid);
	if (!m_wndClassView.Create(strClassView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_CLASSVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Class View window\n");
		return FALSE; // failed to create
	}

	// Create file view
	CString strFileView;
	bNameValid = strFileView.LoadString(IDS_FILE_VIEW);
	ASSERT(bNameValid);
	if (!m_wndFileView.Create(strFileView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_FILEVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create File View window\n");
		return FALSE; // failed to create
	}

	// Create output window
	CString strOutputWnd;
	bNameValid = strOutputWnd.LoadString(IDS_OUTPUT_WND);
	ASSERT(bNameValid);
	if (!m_wndOutput.Create(strOutputWnd, this, CRect(0, 0, 100, 100), TRUE, ID_VIEW_OUTPUTWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Output window\n");
		return FALSE; // failed to create
	}

	// Create properties window
	CString strPropertiesWnd;
	bNameValid = strPropertiesWnd.LoadString(IDS_PROPERTIES_WND);
	ASSERT(bNameValid);
	if (!m_wndProperties.Create(strPropertiesWnd, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_PROPERTIESWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Properties window\n");
		return FALSE; // failed to create
	}

	SetDockingWindowIcons(theApp.m_bHiColorIcons);
	return TRUE;
}

void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
	HICON hFileViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_FILE_VIEW_HC : IDI_FILE_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndFileView.SetIcon(hFileViewIcon, FALSE);

	HICON hClassViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_CLASS_VIEW_HC : IDI_CLASS_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndClassView.SetIcon(hClassViewIcon, FALSE);

	HICON hOutputBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_OUTPUT_WND_HC : IDI_OUTPUT_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndOutput.SetIcon(hOutputBarIcon, FALSE);

	HICON hPropertiesBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_PROPERTIES_WND_HC : IDI_PROPERTIES_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndProperties.SetIcon(hPropertiesBarIcon, FALSE);

}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame message handlers

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* scan menus */);
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CFrameWndEx::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
	}

	m_wndOutput.UpdateFonts();
	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}


BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	// base class does the real work

	if (!CFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}


	// enable customization button for all user toolbars
	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	for (int i = 0; i < iMaxUserToolbars; i ++)
	{
		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		if (pUserToolbar != NULL)
		{
			pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
		}
	}

	return TRUE;
}


void CMainFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CFrameWndEx::OnSettingChange(uFlags, lpszSection);
	m_wndOutput.UpdateFonts();
}


void CMainFrame::OnPreview()
{
	Dlg = new myDial();
	Dlg->isPreviewOn = true;
	Dlg->Create(IDD_DIALOG1, this);
	Dlg->ShowWindow(SW_SHOW);
}

void CMainFrame::OnDestroy()
{
	if (Dlg!=NULL)
	{
		Dlg->DestroyWindow();
		delete Dlg;
	}
	CFrameWndEx::OnDestroy();
}

// regular 4 nested loops
    
//void CMainFrame::OnDilate()
//{
//	float t = clock();
//
//	CTask2Doc *pDoc = (CTask2Doc *)GetActiveDocument();
//	pDoc->myImage->DisableAllOptions();
//	int nWidth = pDoc->myImage->GetSrcSize().cx;
//	int nHeight = pDoc->myImage->GetSrcSize().cy;
//
//	if (pDoc->oldSrc == NULL)
//	{
//		pDoc->oldSrc = new BYTE[nWidth*nHeight];
//		memcpy(pDoc->oldSrc, pDoc->m_DIB.m_lpImage, nWidth*nHeight);		 
//	}
//
//	LPBYTE prevImg = new BYTE[(nWidth+2)*(nHeight+2)];
//
// 	for (size_t i = 0; i < nHeight; i++)
//	{
//		memcpy(prevImg + (nWidth + 2)*(i + 1) + 1, pDoc->m_DIB.m_lpImage + (nWidth)*(i), nWidth);
//	}
//
//	for (size_t i = 0; i < nHeight; i++)
//	{
//		for (size_t j = 0; j < nWidth; j++)
//		{
//			int localMax = prevImg[(i+1)*(nWidth+2) + j+1];
//			for (int k = -1; k < 2; k++)
//			{
//				for (int l = -1; l < 2; l++)
//				{
//					if (localMax < prevImg[(i + l+1)* (nWidth+2) + j + k+1])
//						localMax = prevImg[(i + l+1)* (nWidth+2) + j + k+1];
//				}
//			}
//			pDoc->m_DIB.SetPixel(CPoint(j , nHeight - (i+1)), localMax);
//		}
//	}
//	cout << "Dilation :" << (((float)clock() - t) / CLOCKS_PER_SEC) << endl;
//
//	CTask2View *pView = (CTask2View*)GetActiveView();
//	pView->Invalidate(false);
////} // 4 nested-loop 0.08636 (image.bmp)  

// m1,m2,m3 Max 
      
//void CMainFrame::OnDilate()
//{
//	float t = clock();
//
//	CTask2Doc *pDoc = (CTask2Doc *)GetActiveDocument();
//	pDoc->myImage->DisableAllOptions();
//	int nWidth = pDoc->myImage->GetSrcSize().cx;
//	int nHeight = pDoc->myImage->GetSrcSize().cy;
//
//	if (pDoc->oldSrc == NULL)
//	{
//		pDoc->oldSrc = new BYTE[nWidth*nHeight];
//		memcpy(pDoc->oldSrc, pDoc->m_DIB.m_lpImage, nWidth*nHeight);
//	}
//
//	LPBYTE prevImg = new BYTE[(nWidth + 2)*(nHeight + 2)];
//
//	for (size_t i = 0; i < nHeight; i++)
//	{
//		memcpy(prevImg + (nWidth + 2)*(i + 1) + 1, pDoc->m_DIB.m_lpImage + (nWidth)*(i), nWidth);
//	}
//	cout << "Creating Pad :" << (((float)clock() - t) / CLOCKS_PER_SEC) << endl;
//	t = (float)clock();
//	int m1, m2, m3, localMax;
//	for (size_t i = 0; i < nHeight; i++)
//	{
//		for (size_t j = 0; j < nWidth; j++)
//		{
//			if (j == 0)
//			{
//				(prevImg[(i)* (nWidth + 2) + j] > prevImg[(i + 1)* (nWidth + 2) + j] ? m1 = prevImg[(i)* (nWidth + 2) + j] : m1 = prevImg[(i + 1)* (nWidth + 2) + j]);
//				(m1 > prevImg[(i + 2)* (nWidth + 2) + j] ? m1 : m1 = prevImg[(i + 2)* (nWidth + 2) + j]);
//
//				(prevImg[(i)* (nWidth + 2) + j + 1] > prevImg[(i + 1)* (nWidth + 2) + j + 1] ? m2 = prevImg[(i)* (nWidth + 2) + j + 1] : m2 = prevImg[(i + 1)* (nWidth + 2) + j + 1]);
//				(m2 > prevImg[(i + 2)* (nWidth + 2) + j + 1] ? m2 : m2 = prevImg[(i + 2)* (nWidth + 2) + j + 1]);
//			}
//			(prevImg[(i)* (nWidth + 2) + j + 2] > prevImg[(i + 1)* (nWidth + 2) + j + 2] ? m3 = prevImg[(i)* (nWidth + 2) + j + 2] : m3 = prevImg[(i + 1)* (nWidth + 2) + j + 2]);
//			(m3 > prevImg[(i + 2)* (nWidth + 2) + j + 2] ? m3 : m3 = prevImg[(i + 2)* (nWidth + 2) + j + 2]);
//
//			if (m2 > m3)
//			{
//				if (m1 > m2)
//				{
//					localMax = m1;
//				}
//				else
//				{
//					localMax = m2;
//				}
//			}
//			else
//			{
//				if (m1 > m3)
//				{
//					localMax = m1;
//				}
//				else
//				{
//					localMax = m3;
//				}
//			}
//			m1 = m2;
//			m2 = m3;
//			pDoc->m_DIB.SetPixel(CPoint(j, nHeight - (i + 1)), localMax);
//		}
//	}
//	cout << "Dilation :" << (((float)clock() - t) / CLOCKS_PER_SEC) << endl;
//
//	CTask2View *pView = (CTask2View*)GetActiveView();
//	pView->Invalidate(false);
//} //  reduced loop : 0.03624 (image.bmp)  

void CMainFrame::OnDilate()
{
	float t = clock();

	CTask2Doc *pDoc = (CTask2Doc *)GetActiveDocument();
	pDoc->myImage->DisableAllOptions();
	int nWidth = pDoc->myImage->GetSrcSize().cx;
	int nHeight = pDoc->myImage->GetSrcSize().cy;

 
	if (pDoc->oldSrc == NULL)
	{
		pDoc->oldSrc = new BYTE[(pDoc->m_DIB.GetRowSize())*nHeight];
		memcpy(pDoc->oldSrc, pDoc->m_DIB.m_lpImage, (pDoc->m_DIB.GetRowSize())*nHeight);
	}

	LPBYTE prevImg = new BYTE[(nWidth + 2)*(nHeight + 2)];

	for (size_t i = 0; i < nHeight; i++)
	{
		memcpy(prevImg + (nWidth + 2)*(i + 1) + 1, pDoc->m_DIB.m_lpImage + (pDoc->m_DIB.GetRowSize())*(i), nWidth);
	}

	cout << "Creating Pad :" << (((float)clock() - t) / CLOCKS_PER_SEC) << endl;

	tbb::parallel_for(tbb::blocked_range2d<int>(0, nHeight, 0, nWidth), [&](const tbb::blocked_range2d<int> &r)
	{
		int m1, m2, m3, localMax;
		for (int i = r.rows().begin(), i_end = r.rows().end(); i < i_end; i++)
		{
			BYTE *line1 = &prevImg[(i)* (nWidth + 2)];
			BYTE *line2 = &prevImg[(i + 1)* (nWidth + 2)];
			BYTE *line3 = &prevImg[(i + 2)* (nWidth + 2)];
			bool colValInit = false;
			for (int j = r.cols().begin(), j_end = r.cols().end(); j<j_end; j++)
			{
				if (!colValInit)
				{
					m1 = (line1[j]> line2[j] ? line1[j] : line2[j]);
					m1 = (m1 > line3[j] ? m1 : line3[j]);

					m2 = (line1[j + 1] > line2[j + 1] ? line1[j + 1] : line2[j + 1]);
					m2 = (m2 > line3[j + 1] ? m2 : line3[j + 1]);
					colValInit = true;
				}

				m3 = (line1[j + 2] > line2[j + 2] ? line1[j + 2] : line2[j + 2]);
				m3 = (m3 > line3[j + 2] ? m3 : line3[j + 2]);

				if (m2 > m3)
				{
					localMax = (m1 > m2 ? m1 : m2);
				}
				else
				{
					localMax = (m1 > m3 ? m1 : m3);
				}
				m1 = m2;
				m2 = m3;
				pDoc->m_DIB.SetPixel(CPoint(j, nHeight - (i + 1)), localMax);
			}
		}
	});
	cout << "Dilation <TBB> :" << (((float)clock() - t) / CLOCKS_PER_SEC) << endl;
 
	delete prevImg;
	CTask2View *pView = (CTask2View*)GetActiveView();
	pView->Invalidate(false);
} //  TBB 



void CMainFrame::OnErode()
{
	float t = clock();
	CTask2Doc *pDoc = (CTask2Doc *)GetActiveDocument();
	pDoc->myImage->DisableAllOptions();
	int nWidth = pDoc->myImage->GetSrcSize().cx;
	int nHeight = pDoc->myImage->GetSrcSize().cy;

	if (pDoc->oldSrc == NULL)
	{
		pDoc->oldSrc = new BYTE[(pDoc->m_DIB.GetRowSize())*nHeight];
		memcpy(pDoc->oldSrc, pDoc->m_DIB.m_lpImage, (pDoc->m_DIB.GetRowSize())*nHeight);
	}

	LPBYTE prevImg = new BYTE[(nWidth+2)*(nHeight+2)];
	for (int i = 0; i < (nHeight); i++)
		memcpy(prevImg + (nWidth + 2)*(i + 1) + 1, pDoc->m_DIB.m_lpImage + (pDoc->m_DIB.GetRowSize())*(i), nWidth);

	cout << "Creating Pad :" << (((float)clock() - t) / CLOCKS_PER_SEC) << endl;

	tbb::parallel_for(tbb::blocked_range2d<int>(0, nHeight, 0, nWidth), [&](const tbb::blocked_range2d<int> &r)
	{
		int m1, m2, m3, localMin;
		for (int i = r.rows().begin(), i_end = r.rows().end(); i < i_end; i++)
		{
			BYTE *line1 = &prevImg[(i)* (nWidth + 2)];
			BYTE *line2 = &prevImg[(i + 1)* (nWidth + 2)];
			BYTE *line3 = &prevImg[(i + 2)* (nWidth + 2)];
			bool colValInit = false;
			for (int j = r.cols().begin(), j_end = r.cols().end(); j<j_end; j++)
			{
				if (!colValInit)
				{
					m1 = (line1[j] < line2[j] ? line1[j] : line2[j]);
					m1 = (m1 < line3[j] ? m1 : line3[j]);

					m2 = (line1[j + 1] < line2[j + 1] ? line1[j + 1] : line2[j + 1]);
					m2 = (m2 < line3[j + 1] ? m2 : line3[j + 1]);
					colValInit = true;
				}

				m3 = (line1[j + 2] < line2[j + 2] ? line1[j + 2] : line2[j + 2]);
				m3 = (m3 < line3[j + 2] ? m3 : line3[j + 2]);

				if (m2 < m3)
				{
					localMin = (m1 < m2 ? m1 : m2);
				}
				else
				{
					localMin = (m1 < m3 ? m1 : m3);
				}
				m1 = m2;
				m2 = m3;

				pDoc->m_DIB.SetPixel(CPoint(j, nHeight - i - 1), localMin);
			}
		}
	});
	cout << "Erosion :" << (((float)clock() - t) / CLOCKS_PER_SEC) << endl;

	CTask2View *pView = (CTask2View*)GetActiveView();
	pView->Invalidate(false);
}


void CMainFrame::OnReset()
{
	CTask2Doc *pDoc = (CTask2Doc *)GetActiveDocument();
	if (pDoc->oldSrc != NULL)
	{
		memcpy(pDoc->m_DIB.m_lpImage, pDoc->oldSrc, pDoc->myImage->GetSrcSize().cx*pDoc->myImage->GetSrcSize().cy);
	 
		CTask2View *pView = (CTask2View*)GetActiveView();
		pView->Invalidate(false);
	}
}
