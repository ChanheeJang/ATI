
// Task2Doc.h : interface of the CTask2Doc class
//


#pragma once


class CTask2Doc : public CDocument
{
protected: // create from serialization only
	CTask2Doc();
	DECLARE_DYNCREATE(CTask2Doc)

// Attributes
public:

// Operations
public:

	
// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CTask2Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	CDib m_DIB;
	myZoomInfo zoom;
	CZoomInfo *Czoom;
	 
protected:

 
// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);

	//BOOL IsValid() const { return m_DIB.IsValid(); }
	//DWORD Width()  { return (DWORD)m_DIB.GetWidth(); }
	//DWORD Height()  { return (DWORD)m_DIB.GetHeight(); }
	//BOOL  PaintDIB(HDC hDC, LPRECT pDCRect, LPRECT pDIBRect) const { return m_DIB.Paint(hDC, pDCRect, pDIBRect); }
};
