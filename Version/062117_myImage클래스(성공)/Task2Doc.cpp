
// Task2Doc.cpp : implementation of the CTask2Doc class
//
#pragma once
#include "stdafx.h"
#include "Task2View.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Task2.h"
#endif

#include "Task2Doc.h"
#include "MainFrm.h"
#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CTask2Doc

IMPLEMENT_DYNCREATE(CTask2Doc, CDocument)

BEGIN_MESSAGE_MAP(CTask2Doc, CDocument)
END_MESSAGE_MAP()



// CTask2Doc construction/destruction

CTask2Doc::CTask2Doc()
{
	myImage = new myZoomInfo;
	// TODO: add one-time construction code here

}

CTask2Doc::~CTask2Doc()
{
}

BOOL CTask2Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

 



// CTask2Doc serialization

void CTask2Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here

	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CTask2Doc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CTask2Doc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CTask2Doc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CTask2Doc diagnostics

#ifdef _DEBUG
void CTask2Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTask2Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CTask2Doc commands


BOOL CTask2Doc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	CFile file;
	CFileException fe;

	if (!file.Open(lpszPathName, CFile::modeRead | CFile::shareDenyWrite, &fe)) {
		ReportSaveLoadException(lpszPathName, &fe, FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
		return FALSE;
	}

	DeleteContents();
	BeginWaitCursor();
	//Cimg.Read(&file);

 // replace calls to Serialize with ReadDIBFile function
	TRY{
		//m_DIB.Read(&file);
		m_DIB.ReadSection(&file);
		myImage->triggerInit();
	}
	CATCH(CFileException, eLoad) {
		file.Abort(); // will not throw an exception
		EndWaitCursor();
		ReportSaveLoadException(lpszPathName, eLoad, FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
		return FALSE;
	}
	END_CATCH
		EndWaitCursor();
	//if (!m_DIB.AssertValid) {
	//	// may not be DIB format
	//	CString strMsg;   
	//	strMsg.LoadString(IDS_CANNOT_LOAD_DIB);
	//	MessageBox(NULL, strMsg, NULL, MB_ICONINFORMATION | MB_OK);
	//	return FALSE;
	//}
	SetPathName(lpszPathName);
	SetModifiedFlag(FALSE);     // start off with unmodified
	// TODO 끝...
	//Cimg= CImag(1, CSize(m_DIB.GetWidth(), m_DIB.GetHeight()), CSize(m_DIB.GetWidth(), m_DIB.GetHeight()), true);


	//-- Preview Dialog check
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
 
	if (pFrame->Dlg!=NULL && pFrame->Dlg->isPreviewOn)
	{
		pFrame->Dlg->detachBG();
		pFrame->Dlg->OnInitDialog();
		pFrame->Dlg->updatePreview();
	}
	
	//--Initialize myImage
	myImage->IsImageInfoLoaded = false;
	return TRUE;
}


BOOL CTask2Doc::OnSaveDocument(LPCTSTR lpszPathName)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CFile file;
	CFileException fe;
	if (!file.Open(lpszPathName, CFile::modeCreate | CFile::modeReadWrite | CFile::shareExclusive, &fe)) {
		ReportSaveLoadException(lpszPathName, &fe, TRUE, AFX_IDP_INVALID_FILENAME);
		return FALSE;
	}
	// replace calls to Serialize with SaveDIB function
	BOOL bSuccess = FALSE;
	TRY{
		BeginWaitCursor();
		bSuccess = m_DIB.Write(&file);
		file.Close();
	}
	CATCH(CException, eSave) {
		file.Abort(); // will not throw an exception
		EndWaitCursor();
		ReportSaveLoadException(lpszPathName, eSave, TRUE, AFX_IDP_FAILED_TO_SAVE_DOC);
		return FALSE;
	}
	END_CATCH
		EndWaitCursor();
	SetModifiedFlag(FALSE);     // back to unmodified
	if (!bSuccess) {
		// may be other-style DIB (load supported but not save)
		//  or other problem in Save
		CString strMsg;
		strMsg.LoadString(IDS_CANNOT_LOAD_DIB);
		MessageBox(NULL, strMsg, NULL, MB_ICONINFORMATION | MB_OK);
	}
	return TRUE;
	//return CDocument::OnSaveDocument(lpszPathName

}
