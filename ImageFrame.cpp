// ImageFrame.cpp: implementation of the CImageFrame class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ImageFrame.h"
#include "ZoomInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


extern CZoomInfo* g_pZoomInfo;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CImageFrame::CImageFrame()
{

}

CImageFrame::~CImageFrame()
{
}

void CImageFrame::SetRegion(CRect rect)
{
	m_rcRegion = rect;
}

CRect CImageFrame::GetRegion()
{
	return m_rcRegion;
}

CImageFrame::CImageFrame(CSize ImageSize, CSize framesize, CRect region, bool memalloc) : CDib(framesize, 8, memalloc)
{
	//??-- ImageSize하고 memalloc은 왜 받아옴?
	m_szFrameSize = framesize;
	m_rcRegion = region;
}

void CImageFrame::DrawRegion(CDC *dc, CRect dest, CRect source)
{
	source.right += (source.left-1);
	source.bottom += (source.top-1);

	if( IsConcerned( source ) ){
		CRect sr, dr, tmp;
		CPoint lll;

		sr = InterSect(source, m_rcRegion);

		sr.right -= (sr.left-1);
		sr.bottom -= (sr.top-1);


		dr.right = dest.right;
		dr.bottom = (long)(dest.bottom*((float)sr.bottom/(source.bottom-source.top+1))+1);
		dr.top = (long)(dest.bottom*((float)(sr.top-source.top)/(source.bottom-source.top+1)))+dest.top;
		dr.left = dest.left;

		sr.top -= m_rcRegion.top;

		Draw(dc, dr, sr);
	}
}

void CImageFrame::DrawRegion(CDC *dc, CRect dest, CRect source, DWORD dwRop)
{
	source.right += (source.left-1);
	source.bottom += (source.top-1);

	if( IsConcerned( source ) ){
		CRect sr, dr, tmp;
		CPoint lll;

		sr = InterSect(source, m_rcRegion);

		sr.right -= (sr.left-1);
		sr.bottom -= (sr.top-1);


		dr.right = dest.right;
		dr.bottom = (long)(dest.bottom*((float)sr.bottom/(source.bottom-source.top+1))+1);
		dr.top = (long)(dest.bottom*((float)(sr.top-source.top)/(source.bottom-source.top+1)))+dest.top;
		dr.left = dest.left;

		sr.top -= m_rcRegion.top;

		Draw(dc, dr, sr, dwRop);
	}
}

bool CImageFrame::PtInRegion(CPoint p)
{
	if( p.x>=m_rcRegion.left && p.x<=m_rcRegion.right && p.y>=m_rcRegion.top && p.y<=m_rcRegion.bottom )	return true;
	else																									return false;
}


bool CImageFrame::IsConcerned(CRect source)
{
	CRect tmp;

	if( source.left>m_rcRegion.left )	tmp.left = source.left;
	else								tmp.left = m_rcRegion.left;

	if( source.right<m_rcRegion.right )	tmp.right = source.right;
	else								tmp.right = m_rcRegion.right;

	if( source.top>m_rcRegion.top )		tmp.top = source.top;
	else								tmp.top = m_rcRegion.top;

	if( source.bottom<m_rcRegion.bottom )	tmp.bottom = source.bottom;
	else								tmp.bottom = m_rcRegion.bottom;

	if( tmp.right < tmp.left )	return false;
	if( tmp.bottom < tmp.top )	return false;

	return true;
}





CRect CImageFrame::InterSect(CRect a, CRect b)
{
	CRect tmp;

	if( a.left>b.left )		tmp.left = a.left;
	else					tmp.left = b.left;

	if( a.right<b.right )	tmp.right = a.right;
	else					tmp.right = b.right;

	if( a.top>b.top )		tmp.top = a.top;
	else					tmp.top = b.top;

	if( a.bottom<b.bottom )	tmp.bottom = a.bottom;
	else					tmp.bottom = b.bottom;

	return tmp;
}

bool CImageFrame::WriteCustom(CFile *pFile, CSize sizeimage)
{
	BITMAPFILEHEADER bmfh;
	bmfh.bfType = 0x4d42;  // 'BM'
	int nSizeHdr = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * m_nColorTableEntries;
	bmfh.bfSize = 0;
	bmfh.bfSize = sizeof(BITMAPFILEHEADER) + nSizeHdr + sizeimage.cx*sizeimage.cy;
	// meaning of bfSize open to interpretation (bytes, words, dwords?) -- we won't use it
	bmfh.bfReserved1 = bmfh.bfReserved2 = 0;
	
	//-- Indicate from where image pixel information starts.
	bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) +
			sizeof(RGBQUAD) * m_nColorTableEntries;	

	m_lpBMIH->biWidth = sizeimage.cx;
	m_lpBMIH->biHeight = sizeimage.cy;

	try {
		CFileException e;
		pFile->Write((LPVOID) &bmfh, sizeof(BITMAPFILEHEADER));
		pFile->Write((LPVOID) m_lpBMIH,  nSizeHdr);

		int num = GetHeight()/GetWidth();
		LPBYTE P = (LPBYTE)m_lpImage;
		for(int i=0; i<num; i++){
			pFile->Write(P, GetWidth()*GetWidth());
			P+=GetWidth()*GetWidth();
		}
	}
	catch(CException* pe) {
		pe->Delete();
		AfxMessageBox(TEXT("write error"));
		return FALSE;
	}
	
	return TRUE;
}

bool CImageFrame::ReadCustom(CFile *pFile)
{
	// 1. read file header to get size of info hdr + color table
	// 2. read info hdr (to get image size) and color table
	// 3. read image
	// can't use bfSize in file header
	Empty();
	int nCount, nSize;
	BITMAPFILEHEADER bmfh;
	try {
		nCount = pFile->Read((LPVOID) &bmfh, sizeof(BITMAPFILEHEADER));

		int nSizeHdr = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * m_nColorTableEntries;
		bmfh.bfSize = 0;
		bmfh.bfSize = sizeof(BITMAPFILEHEADER) + nSizeHdr + m_szFrameSize.cx*m_szFrameSize.cy;
		// meaning of bfSize open to interpretation (bytes, words, dwords?) -- we won't use it
		bmfh.bfReserved1 = bmfh.bfReserved2 = 0;

		if(nCount != sizeof(BITMAPFILEHEADER)) {
//			throw new CException;
		}
		if(bmfh.bfType != 0x4d42) {
//			throw new CException;
		}
		nSize = bmfh.bfOffBits - sizeof(BITMAPFILEHEADER);
		m_lpBMIH = (LPBITMAPINFOHEADER) new char[nSize];
		m_nBmihAlloc = m_nImageAlloc = crtAlloc;
		nCount = pFile->Read(m_lpBMIH, nSize); // info hdr & color table
		ComputeMetrics();
		ComputePaletteSize(m_lpBMIH->biBitCount);
		MakePalette();
		m_lpImage = new BYTE[m_szFrameSize.cx*m_szFrameSize.cy];
		LPBYTE p = (LPBYTE)m_lpImage;

//		nCount = pFile->Read(m_lpImage, m_dwSizeImage); // image only
		int num = GetHeight()/GetWidth();
		LPBYTE P = (LPBYTE)m_lpImage;
		for(int i=0; i<num; i++){
			pFile->Read(P, GetWidth()*GetWidth());
			P+=GetWidth()*GetWidth();
		}

		m_rImageRect = CRect(0, 0, GetWidth(), GetHeight());
	}
	catch(CException* pe) {
		AfxMessageBox(TEXT("Read error"));
		pe->Delete();
		return FALSE;
	}
	return TRUE;
}

void CImageFrame::SetMemPointer(LPBYTE p)
{
	m_lpImage = p;
}



int CImageFrame::GetColorTableEntry()
{
	return m_nColorTableEntries;
}
