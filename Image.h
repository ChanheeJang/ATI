// Image.h: interface for the CImag class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAGE_H__9BDE26E1_0C88_11D4_82FC_000021DE7A53__INCLUDED_)
#define AFX_IMAGE_H__9BDE26E1_0C88_11D4_82FC_000021DE7A53__INCLUDED_

#include "ImageFrame.h"


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CImag  
{
public:
	void SetPixel(CPoint pt, int n);
	void ThreasHold(int n);
	void ChangePalette(COLORREF c);
	
	void ClearImage();
	
	void CopyStretch(CRect destRect, CRect sourceRect, byte** ppMem, CPointFloat ptfRotation);
	void SaveStretch(CString name, CSize destSize, CRect source, int nDegree);
	bool m_bMemAlloc;
	
	CZoomInfo* m_pZoomInfo; // 줌관련 클래스 
	
	void Decompress(LPBYTE p, int size); //  확대
	void Compress(LPBYTE p, int size);   // 압축
	void SaveRect(CString name, CRect rect); 
	void SetLim(int num, int val);
	void InitLim();
	
	void SetMemPointer(int num, LPBYTE p);
	void SetMemPointer(LPBYTE p);
	
	void CopyImage(LPBYTE p, int start, int end);
	LPBYTE GetMemPointer(int num);
	
	int GetWidth();
	int GetHeight();
	
	bool Read(CFile* f);
	bool Read(CFile* f, int nOffset);
	
	bool Write(CFile* f, bool Bit4Compress);
	LPBYTE* GetRectPointer();
	
	int GetPixel(CPoint p);
	
	void Draw(CDC* dc, CRect dest, CRect source);
	void Draw(CDC* dc, CRect dest, CRect source, DWORD);
	
	
	CImag(int framenum, CSize imagesize, CSize framesize, bool);
	CImag();
	
	virtual ~CImag();

private:
	unsigned char* m_pMemory;
	
	LPBYTE* m_pRectPointer;
	
	CSize m_szFrameSize;
	CSize m_szImageSize;


	int m_nFrameNum;
	
	CImageFrame** m_pImageFrame;
};

#endif // !defined(AFX_IMAGE_H__9BDE26E1_0C88_11D4_82FC_000021DE7A53__INCLUDED_)
