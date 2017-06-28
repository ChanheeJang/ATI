// Image.cpp: implementation of the CImag class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Image.h"
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
	
CImag::CImag()
{
	m_pZoomInfo = new CZoomInfo(); //
}

CImag::~CImag()

{
	for(int i=0; i<m_nFrameNum; i++){
		delete m_pImageFrame[i];
	}
	delete [] m_pImageFrame;

	if( m_szImageSize.cy > 0 )	delete [] m_pRectPointer;
	if( m_bMemAlloc )			delete [] m_pMemory;
	delete m_pZoomInfo;
}

CImag::CImag(int framenum, CSize imagesize, CSize framesize, bool MemAlloc)
{
	CRect region;	
	m_nFrameNum = framenum; // int
	m_szImageSize = imagesize; //CSize
	m_szFrameSize = framesize; //CSize
	m_pRectPointer = NULL;  //LPBYTE*
	m_bMemAlloc = MemAlloc;

	m_pImageFrame = new CImageFrame*[framenum]; // CImagFrame 
	
	if( MemAlloc )	m_pMemory = new byte[imagesize.cx*imagesize.cy];  // unsigned char* m_pMemory 

	//-- Region은 frame 메모리에 접근하기 위한 좌표를 나타냄.
	//??-- framenum이 무엇인가?? 큰 이미지를 쪼개는 것은 분명히 아님.
	for(int i=0; i<framenum; i++){
		region = CRect(0, m_szFrameSize.cy*i, m_szFrameSize.cx-1, m_szFrameSize.cy*(i+1)-1);
		
		m_pImageFrame[framenum-1-i] = NULL;		
		
		m_pImageFrame[framenum-1-i] = new CImageFrame(imagesize, framesize, region, false);
	}

	//	make rect pointer
	m_pRectPointer = new LPBYTE[m_szImageSize.cy];

	if( MemAlloc ){
		byte* ppp = m_pMemory;
		for(int i=0; i<framenum; i++){
			
			SetMemPointer(i, ppp);
			
			ppp += framesize.cx*framesize.cy;		
		}

		ppp = m_pMemory;
		for(int i=0; i<imagesize.cy; i++){
			m_pRectPointer[imagesize.cy-1-i] = ppp;
			ppp += imagesize.cx;
		}
	}
	//-- Initialize CZoomInfo object
	m_pZoomInfo = new CZoomInfo(imagesize, region, (m_szImageSize.cx == m_szImageSize.cy ? true:false));

}

void CImag::Draw(CDC *dc, CRect dest, CRect source)
{
	for(int i=0; i<m_nFrameNum; i++){
		m_pImageFrame[i]->DrawRegion(dc, dest, source);
	}
}

void CImag::Draw(CDC *dc, CRect dest, CRect source, DWORD dwRop)
{
	for(int i=0; i<m_nFrameNum; i++){
		m_pImageFrame[i]->DrawRegion(dc, dest, source, dwRop);
	}
}

int CImag::GetPixel(CPoint p)
{
	if( m_szImageSize.cx>p.x && p.x>-1 && m_szImageSize.cy>p.y && p.y>-1 ){
		return m_pRectPointer[p.y][p.x];
	} else {
		return 0;
	}
}

LPBYTE* CImag::GetRectPointer()
{
	return m_pRectPointer;
}

bool CImag::Write(CFile *pFile, bool Bit4Compress) // 저장할때 
{
	int TableEntries = m_pImageFrame[0]->GetColorTableEntries();
	BITMAPFILEHEADER bmfh;
	LPBITMAPINFOHEADER lpBMIH = m_pImageFrame[0]->GetBMIH();
	bmfh.bfType = 0x4d42;  // 'BM'
	int nSizeHdr = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * TableEntries;
	bmfh.bfSize = 0;
	bmfh.bfSize = sizeof(BITMAPFILEHEADER) + nSizeHdr + m_szImageSize.cx*m_szImageSize.cy;
	// meaning of bfSize open to interpretation (bytes, words, dwords?) -- we won't use it
	bmfh.bfReserved1 = bmfh.bfReserved2 = 0;
	bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) +
			sizeof(RGBQUAD) * TableEntries;	

	lpBMIH->biWidth = m_szImageSize.cx;
	lpBMIH->biHeight = m_szImageSize.cy;

	if( Bit4Compress ){
		lpBMIH->biCompression = 1010;
		try {
			CFileException e;
			pFile->Write((LPVOID) &bmfh, sizeof(BITMAPFILEHEADER));
			pFile->Write((LPVOID) lpBMIH,  nSizeHdr);

			LPBYTE P = new BYTE[m_szImageSize.cx];
	
			for(int i=0; i<m_szImageSize.cy; i++){
				memcpy(P, m_pRectPointer[m_szImageSize.cy-1-i], m_szImageSize.cx);
				Compress(P, m_szImageSize.cx);

				pFile->Write(P, (int)(m_szFrameSize.cx*0.5));
			}

			delete P;
		}
		catch(CException* pe) {
			pe->Delete();
			AfxMessageBox(TEXT("write error"));
			return FALSE;
		}
	} else {
		lpBMIH->biCompression = 0000;
		try {
			CFileException e;
			pFile->Write((LPVOID) &bmfh, sizeof(BITMAPFILEHEADER));
			pFile->Write((LPVOID) lpBMIH,  nSizeHdr);
	
			for(int i=0; i<m_szImageSize.cy; i++){
				pFile->Write(m_pRectPointer[m_szImageSize.cy-1-i], sizeof(BYTE)*m_szFrameSize.cx);
			}
		}
		catch(CException* pe) {
			pe->Delete();
			AfxMessageBox(TEXT("write error"));
			return FALSE;
		}
	}

	lpBMIH->biHeight = m_szFrameSize.cy;
	
	return TRUE;
}

bool CImag::Read(CFile *pFile) // 파일읽어 올때 
{
	BITMAPFILEHEADER bmfh; // 파일정보 
	LPBITMAPINFOHEADER pBMIH; // 파일 if정도
	long nCount;  //파일헤더 크기
	int nSize; // 파일헤더를 제외한 DIB 정도 
	long lFileHeight, nWidth;  // 파일 가로 세로 길이 

	pBMIH = (LPBITMAPINFOHEADER) new char[sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * m_pImageFrame[0]->GetColorTableEntry()];

	try {
		nCount = pFile->Read((LPVOID) &bmfh, sizeof(BITMAPFILEHEADER));
		if(nCount != sizeof(BITMAPFILEHEADER)) {
//			throw new CException;
		}
		if(bmfh.bfType != 0x4d42) {
//			throw new CException;
		}
		nSize = bmfh.bfOffBits - sizeof(BITMAPFILEHEADER);

		// bfOffBits = 파일해더 + info 크기 + RGB배열 크기 
		
		nCount = pFile->Read(pBMIH, nSize); // info hdr & color table
		
		lFileHeight = pBMIH->biHeight; // 비트뱁의 픽셀의  세로 크기 
		
		if( pBMIH->biWidth > m_szImageSize.cx )	{ // 비트맵 픽셀의 가로 크기 
			
			nWidth = m_szImageSize.cx;  //작은것을 nWidth로 설정 
		}
		else{
			nWidth = pBMIH->biWidth;
		}

		if( pBMIH->biCompression == 1010 ){ //압축유형
			//	compression
		}
		else {
			long tmp;
			char ttt[10000];


			// 작은것을 기준 
			if( lFileHeight > m_szImageSize.cy ){
				//lFileHeight 실제 세로의 크기 가 크면 
				
				tmp = lFileHeight - m_szImageSize.cy;

/*				for(int i=0; i<tmp; i++){
					pFile->Read(m_pRectPointer[0], nWidth);
					if( pBMIH->biWidth>m_szImageSize.cx )	pFile->Read(ttt, pBMIH->biWidth-m_szImageSize.cx);
				}*/
				for(int i=0; i<m_szImageSize.cy; i++){
					pFile->Read(m_pRectPointer[m_szImageSize.cy-i-1], nWidth);
					
					if( pBMIH->biWidth>m_szImageSize.cx )	
					{
						pFile->Read(ttt, pBMIH->biWidth-m_szImageSize.cx);
					}
				}
			} 
			else {
				for(int i=0; i<lFileHeight; i++){
					pFile->Read(m_pRectPointer[lFileHeight-i-1], nWidth);
					if( pBMIH->biWidth>m_szImageSize.cx )	
					{
						pFile->Read(ttt, pBMIH->biWidth-m_szImageSize.cx);
					}
				}
			}
		}
		for(int i=0; i<m_nFrameNum; i++){
			m_pImageFrame[i]->SetImageRect(CRect(0, 0, m_szFrameSize.cx, m_szFrameSize.cy));
		}		
	}
	catch(CException* pe) {
		AfxMessageBox(TEXT("Read error"));
		pe->Delete();
		return FALSE;
	}

	delete [] pBMIH;

//	m_lpBMIH->biHeight = m_szFrameSize.cy;
	
	return TRUE;
}

bool CImag::Read(CFile *pFile, int nOffset)
{
	BITMAPFILEHEADER bmfh;
	LPBITMAPINFOHEADER pBMIH;
	
	long nCount, nSize;
	long lFileHeight, nWidth;

	pBMIH = (LPBITMAPINFOHEADER) new 
		char[sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * m_pImageFrame[0]->GetColorTableEntry()];

	try {
		nCount = pFile->Read((LPVOID) &bmfh, sizeof(BITMAPFILEHEADER));
		if(nCount != sizeof(BITMAPFILEHEADER)) {
//			throw new CException;
		}
		if(bmfh.bfType != 0x4d42) {
//			throw new CException;
		}
		nSize = bmfh.bfOffBits - sizeof(BITMAPFILEHEADER);
		nCount = pFile->Read(pBMIH, nSize); // info hdr & color table
		
		
		lFileHeight = pBMIH->biHeight; //픽셀의 세로 크기 
		
		if( pBMIH->biWidth > m_szImageSize.cx )	{
		
			nWidth = m_szImageSize.cx; //작은것을 기준 
		}
		else{
			
			nWidth = pBMIH->biWidth;
		}

		if( pBMIH->biCompression == 1010 ){
			//	compression
		} else {
			long tmp;
			char ttt[10000];

			if( nOffset < 0 )	nOffset = 0;

			if( lFileHeight+nOffset > m_szImageSize.cy ){
				
				tmp = lFileHeight - m_szImageSize.cy;

				for(int i=0; i<tmp; i++){
					pFile->Read(m_pRectPointer[0], nWidth);
					if( pBMIH->biWidth>m_szImageSize.cx )
					{
						pFile->Read(ttt, pBMIH->biWidth-m_szImageSize.cx);
					}
				}
				for(int i=0; i<m_szImageSize.cy; i++){
					pFile->Read(m_pRectPointer[m_szImageSize.cy-i-1], nWidth);
					if( pBMIH->biWidth>m_szImageSize.cx )	
					{
						pFile->Read(ttt, pBMIH->biWidth-m_szImageSize.cx);
					}
				}
			} else {
				for(int i=0; i<lFileHeight; i++){
					pFile->Read(m_pRectPointer[nOffset+lFileHeight-i-1], nWidth);
					if( pBMIH->biWidth>m_szImageSize.cx )	pFile->Read(ttt, pBMIH->biWidth-m_szImageSize.cx);
				}
			}
		}
		for(int i=0; i<m_nFrameNum; i++){
			m_pImageFrame[i]->SetImageRect(CRect(0, 0, m_szFrameSize.cx, m_szFrameSize.cy));
		}		
	}
	catch(CException* pe) {
		AfxMessageBox(TEXT("Read error"));
		pe->Delete();
		return FALSE;
	}

	delete [] pBMIH;

//	m_lpBMIH->biHeight = m_szFrameSize.cy;
	
	return TRUE;
}

int CImag::GetHeight()
{
	return m_szImageSize.cy;
}

int CImag::GetWidth()
{
	return m_szImageSize.cx;
}

LPBYTE CImag::GetMemPointer(int num)
{
	if( !m_bMemAlloc )	return (LPBYTE)( m_pImageFrame[num]->GetMemPointer() );
	else	return m_pMemory;
}

void CImag::CopyImage(LPBYTE p, int start, int end)
{
	LPBYTE t = p;

	for(long i=end; i>=start; i--){
		memcpy(t, m_pRectPointer[i], sizeof(BYTE)*m_szImageSize.cx);
		t += m_szImageSize.cx;
	}
}

void CImag::SetMemPointer(int num, LPBYTE p)  // int num은 프레인번호 , 바이트 포인터 
{
	LPBYTE PP;

	if( num < m_nFrameNum ){
		
		m_pImageFrame[num]->SetMemPointer(p);

		
		PP = p + m_szFrameSize.cx*(m_szFrameSize.cy-1);
		
		long sss = (m_nFrameNum-num-1)*m_szFrameSize.cy;
		
		for(int i=0; i<m_szFrameSize.cy; i++){
			m_pRectPointer[sss+i] = PP;
			PP -= m_szFrameSize.cx;
		}

		CRect rect = m_pImageFrame[num]->GetRegion();
		
		for(int j=rect.top; j<rect.bottom+1; j++){
			PP = m_pRectPointer[j];
			for(int i=rect.left; i<rect.right+1; i++){
				*PP = i+j;
				PP++;
			}
		}
	}
}

void CImag::SetMemPointer(LPBYTE p)
{
	LPBYTE PP, p1;

	for(int num=0; num<m_nFrameNum; num++){
		p1 = p+m_szFrameSize.cx*m_szFrameSize.cy*num;
		m_pImageFrame[num]->SetMemPointer(p1);

		PP = p1 + m_szFrameSize.cx*(m_szFrameSize.cy-1);
		
		long sss = (m_nFrameNum-num-1)*m_szFrameSize.cy;
		
		//long sss = m_nFrameNum*m_szFrameSize.cy-num*m_szFrameSize.cy-1;
		
		for(int i=0; i<m_szFrameSize.cy; i++){
			m_pRectPointer[sss+i] = PP;
			PP -= m_szFrameSize.cx;
		}

		CRect rect = m_pImageFrame[num]->GetRegion();
		for(int j=rect.top; j<rect.bottom+1; j++){
			PP = m_pRectPointer[j];
			for(int i=rect.left; i<rect.right+1; i++){
				*PP = i+j;
				PP++;
			}
		}
	}
}

void CImag::InitLim()
{
	SetLim(0, 255);
	SetLim(1, 255);
	SetLim(2, 0);
	SetLim(3, 0);
}

void CImag::SetLim(int num, int val)
{
	//	이미지의 최 외곽으로부터 num만큼 떨어진 테우리의 값을 변경하기
	for(int i=num; i<m_szImageSize.cx-num; i++){
		m_pRectPointer[num][i] = val;
	}

	for(int i=num; i<m_szImageSize.cx-num; i++){
		m_pRectPointer[m_szImageSize.cy-1-num][i] = val;
	}

	for(int i=num; i<m_szImageSize.cy-num; i++){
		m_pRectPointer[i][num] = val;
	}

	for(int i=num; i<m_szImageSize.cy-num; i++){
		m_pRectPointer[i][m_szImageSize.cx-1-num] = val;
	}
}


void CImag::SaveRect(CString name, CRect rect)
{
	CFile pFile;
	int width = (int)(rect.right*0.25);
	int owidth = rect.right;
	if( width*4 != rect.right )	rect.right = (width+1)*4;

	if( rect.left < 0 || rect.top < 0 || rect.right+rect.left > m_szImageSize.cx ||
		rect.top+rect.bottom >= m_szImageSize.cy )	return;

	if( pFile.Open((LPCTSTR)name, CFile::modeWrite|CFile::modeCreate|CFile::typeBinary) != NULL ){
		int TableEntries = m_pImageFrame[0]->GetColorTableEntries();
		BITMAPFILEHEADER bmfh;
		LPBITMAPINFOHEADER lpBMIH = m_pImageFrame[0]->GetBMIH();
	
		lpBMIH->biHeight = rect.bottom;
		lpBMIH->biWidth = owidth;
	
		bmfh.bfType = 0x4d42;  // 'BM'
		int nSizeHdr = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * TableEntries;
		bmfh.bfSize = 0;
		bmfh.bfSize = sizeof(BITMAPFILEHEADER) + nSizeHdr + rect.right*rect.bottom;
		// meaning of bfSize open to interpretation (bytes, words, dwords?) -- we won't use it
		bmfh.bfReserved1 = bmfh.bfReserved2 = 0;
		bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) +
				sizeof(RGBQUAD) * TableEntries;	
	
		try {
			CFileException e;
			pFile.Write((LPVOID) &bmfh, sizeof(BITMAPFILEHEADER));
			pFile.Write((LPVOID) lpBMIH,  nSizeHdr);
	
			for(int i=rect.bottom-1; i>-1; i--){
				pFile.Write(&m_pRectPointer[rect.top+i][rect.left], sizeof(BYTE)*(rect.right));
			}
		}
		catch(CException* pe) {
			pe->Delete();
			AfxMessageBox(TEXT("write error"));
		}
	
		lpBMIH->biWidth = m_szFrameSize.cx;
		lpBMIH->biHeight = m_szFrameSize.cy;

		pFile.Flush();

		pFile.Close();
	}
}

void CImag::Compress(LPBYTE p, int size)
{
	LPBYTE t;
	size = int(size * 0.5);

	t = &p[size];

	for(int i=0; i<size; i++){
		if( *t < 240 ){
			if( *t&8 )	*t += 16;
		}
		*t &= 240;

		if( *p < 240 ){
			if( *p&8 )	*p += 16;
		}
		*p &= 240;
		*p += (*t>>4);

		t++;
		p++;
	}
}

void CImag::Decompress(LPBYTE p, int size)
{
	LPBYTE t;
	size = int(size * 0.5);

	t = &p[size];

	for(int i=0; i<size; i++){
		*t = *p;
		*p &= 240;
		*t = (*t<<4);

		t++;
		p++;
	}
}

void CImag::SaveStretch(CString name, CSize destSize, CRect source, int nDegree)
{
	CFile pFile;
	
	int width = (int)(source.right*0.25), width1 = (int)(destSize.cx*0.25);
	int owidth = source.right, odwidth = destSize.cx;
	if( width*4 != source.right )	source.right = (width+1)*4;
	if( width1*4 != destSize.cx )	destSize.cx = (width1+1)*4;

//	if( source.left < 0 || source.top < 0 || source.right+source.left >= m_szImageSize.cx ||
//		source.top+source.bottom >= m_szImageSize.cy )	return;

	if( pFile.Open((LPCTSTR)name, CFile::modeWrite|CFile::modeCreate|CFile::typeBinary) != NULL ){
		int TableEntries = m_pImageFrame[0]->GetColorTableEntries();
		BITMAPFILEHEADER bmfh;
		LPBITMAPINFOHEADER lpBMIH = m_pImageFrame[0]->GetBMIH();
	
		lpBMIH->biHeight = destSize.cy;
		lpBMIH->biWidth = odwidth;
	
		bmfh.bfType = 0x4d42;  // 'BM'
		int nSizeHdr = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * TableEntries;
		bmfh.bfSize = 0;
		bmfh.bfSize = sizeof(BITMAPFILEHEADER) + nSizeHdr + destSize.cx*destSize.cy;
		// meaning of bfSize open to interpretation (bytes, words, dwords?) -- we won't use it
		bmfh.bfReserved1 = bmfh.bfReserved2 = 0;
		bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) +
				sizeof(RGBQUAD) * TableEntries;	

		CDib* pTmp = new CDib(destSize, 8);
		float fScaleX, fScaleY;
		byte** pMem = pTmp->GetRectPointer();
		byte** pMem1 = GetRectPointer();

		fScaleX = (float)(owidth+1)/(odwidth+1);
		fScaleY = (float)(source.bottom+1)/(destSize.cy+1);

		for(int y=0; y<destSize.cy; y++){
			for(int x=0; x<destSize.cx; x++){
				pMem[y][x] = pMem1[(int)(fScaleY*y+source.top)][(int)(fScaleX*x+source.left)];
			}
		}

		switch(nDegree){
		case 90:	//	90도 회전
			{
				byte bTmp;
				for(int y=0; y<destSize.cy; y++){
					for(int x=y; x<destSize.cx; x++){
						bTmp = pMem[y][x];
						pMem[y][x] = pMem[x][y];
						pMem[x][y] = bTmp;
					}
				}

				for(int y=0; y<destSize.cy; y++){
					for(int x=0; x<destSize.cx*0.5; x++){
						bTmp = pMem[y][x];
						pMem[y][x] = pMem[y][destSize.cx-1-x];
						pMem[y][destSize.cx-1-x] = bTmp;
					}
				}
			}
			break;

		case 270:	//	270도 회전
			{
				byte bTmp;
				for(int y=0; y<destSize.cy; y++){
					for(int x=y; x<destSize.cx; x++){
						bTmp = pMem[y][x];
						pMem[y][x] = pMem[x][y];
						pMem[x][y] = bTmp;
					}
				}

				for(int x=0; x<destSize.cx; x++){
					for(int y=0; y<destSize.cy*0.5; y++){
						bTmp = pMem[y][x];
						pMem[y][x] = pMem[destSize.cy-1-y][x];
						pMem[destSize.cy-1-y][x] = bTmp;
					}
				}
			}
			break;
		}


		try {
			CFileException e;
			pFile.Write((LPVOID) &bmfh, sizeof(BITMAPFILEHEADER));
			pFile.Write((LPVOID) lpBMIH,  nSizeHdr);
	
			for(int i=destSize.cy-1; i>-1; i--){
				pFile.Write(pMem[i], sizeof(BYTE)*(destSize.cx));
			}
		}
		catch(CException* pe) {
			pe->Delete();
			AfxMessageBox(TEXT("write error"));
		}
	
		lpBMIH->biWidth = m_szFrameSize.cx;
		lpBMIH->biHeight = m_szFrameSize.cy;

		pFile.Flush();

		pFile.Close();

		delete pTmp;
	}
}

void CImag::CopyStretch(CRect destRect, CRect sourceRect, byte** ppMem, CPointFloat ptfRotation)
{
	float fScaleX, fScaleY;
	float fx, fy;
	byte** pMem = ppMem;
	byte** pMem1 = GetRectPointer();

	fScaleX = (float)(sourceRect.right-sourceRect.left+1)/(destRect.right-destRect.left+1);
	fScaleY = (float)(sourceRect.bottom-sourceRect.top+1)/(destRect.bottom-destRect.top+1);

	for(int y=0; y<destRect.bottom-destRect.top+1; y++){
		for(int x=0; x<destRect.right-destRect.left+1; x++){
			fx = fScaleX*x*ptfRotation.x - fScaleY*y*ptfRotation.y;
			fy = fScaleX*x*ptfRotation.y + fScaleY*y*ptfRotation.x;

			pMem[destRect.top+y][destRect.left+x] = 0;

			if( (int)(fy+sourceRect.top) > -1 && (int)(fx+sourceRect.left)>-1 &&
				(int)(fy+sourceRect.top) < sourceRect.bottom-1 && (int)(fx+sourceRect.left)<sourceRect.right-1 ){
				pMem[destRect.top+y][destRect.left+x] = pMem1[(int)(fy+sourceRect.top)][(int)(fx+sourceRect.left)];			
			}
		}
	}
}

void CImag::ClearImage()
{
	for(int i=0; i<m_szImageSize.cy; i++){
		memset(m_pRectPointer[i], 0, m_szImageSize.cx);
	}
}

void CImag::ChangePalette(COLORREF c)
{
	for(int i=0; i<m_nFrameNum; i++){
		m_pImageFrame[i]->ChangePalette(c);
	}
}

void CImag::ThreasHold(int n)
{
	for(int i=0; i<m_nFrameNum; i++){
		m_pImageFrame[i]->ThreasHold(n);
	}
}

void CImag::SetPixel(CPoint p, int n)
{
	if( m_szImageSize.cx>p.x && p.x>-1 && m_szImageSize.cy>p.y && p.y>-1 ){
		m_pRectPointer[p.y][p.x] = n;
	}
}
