// cdib.h declaration for Inside Visual C++ CDib class

#ifndef _INSIDE_VISUAL_CPP_CDIB
#define _INSIDE_VISUAL_CPP_CDIB

class CDib : public CObject
{
	DECLARE_SERIAL(CDib)
public:
	LPVOID m_lpvColorTable;							
	HBITMAP m_hBitmap;								
	LPBYTE m_lpImage;  // starting address of DIB bits
	LPBITMAPINFOHEADER m_lpBMIH; //  buffer containing the BITMAPINFOHEADER
protected:
	enum Alloc {noAlloc, crtAlloc, heapAlloc};
	HGLOBAL m_hGlobal; // For external windows we need to free;
	                   //  could be allocated by this class or allocated externally
	Alloc m_nBmihAlloc;								
	Alloc m_nImageAlloc;							
	DWORD m_dwSizeImage; // of bits -- not BITMAPINFOHEADER or BITMAPFILEHEADER
	int m_nColorTableEntries;						
	
	HANDLE m_hFile;									
	HANDLE m_hMap;									
	LPVOID m_lpvFile;								
	HPALETTE m_hPalette;			
	int m_nRowSize;
public:
	void SetPixel(CPoint pt, int n);
	void SetPixel(int x, int y, int n);
	void SetPixel(int y, byte* n);
	void ThreasHold(int n);
	void ChangePalette(COLORREF c);
	void GreenPalette();
	void RedPalette();
	void BluePalette();
	void SetImageRect(CRect rect);
	LPBITMAPINFOHEADER GetBMIH();
	int GetColorTableEntries();


	void MakeBitmapPalette();
	BYTE** GetRectPointer();
	LPBYTE GetMemPointer();
	int GetPixel(CPoint point);
	bool InImage(int x, int y);
	int GetPixel(int x, int y);
	int GetHeight();
	int GetWidth();
	
	//-------------
	int GetRowSize();
	int CalRowSize(int width = NULL);
	int GetOffBit();
	void SetSizeImage(int size);
	//--------------

	CDib();
	CDib(CSize size, int nBitCount);	// builds BITMAPINFOHEADER
	CDib(CSize size, int nBitCount, bool bAllocMem);
	~CDib();

	/*This function returns the number of bytes in the DIB image (excluding the info header and the color table)*/
	int GetSizeImage() {return m_dwSizeImage;}

	/*This function returns the number of bytes in the info header and color table combined.*/
	int GetSizeHeader()
		{return sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * m_nColorTableEntries;}

	/*This function returns the width and height of a DIB in pixels.*/
	CSize GetDimension();
	
	/*This function opens a memory-mapped file in read mode and attaches it to the CDib object.
	The return is immediate because the file isn't actually read into memory until it is used.
	When you access the DIB, however, a delay might occur as the file is paged in.
	The AttachMapFile() function releases existing allocated memory
	and closes any previously attached memory-mapped file.*/
	bool AttachMapFile(const char* strPathname, bool bShare = FALSE);

	/*This function creates a new memory-mapped file and copies the existing CDib data to the file's memory, 
	releasing any previously allocated memory and closing any existing memory-mapped file. 
	The data isn't actually written to disk until the new file is closed, 
	but that happens when the CDib object is reused or destroyed.*/
	bool CopyToMapFile(const char* strPathname);

	/*This function associates an existing CDib object with a DIB in memory. 
	This memory could be in the program's resources, or it could be clipboard or OLE data object memory. 
	Memory might have been allocated from the CRT heap with the new operator, 
	or it might have been allocated from the Windows heap with GlobalAlloc().*/
	bool AttachMemory(LPVOID lpvMem, bool bMustDelete = FALSE, HGLOBAL hGlobal = NULL);
	
	/*This function outputs the CDib object to the display (or to the printer) 
	with a call to the Win32 StretchDIBits() function. 
	The bitmap will be stretched as necessary to fit the specified rectangle.*/
	bool Draw(CDC* pDC, CRect dest, CRect source, DWORD dwRop);
	bool Draw(CDC* pDC, CPoint origin, CSize size);  // until we implemnt CreateDibSection
	bool Draw(CDC* pDC, CRect dest, CRect source);  // until we implemnt CreateDibSection

	/*This function creates a DIB section by calling the Win32 CreateDIBSection() function and stores the handle. 
	The image memory will be uninitialized.*/
	HBITMAP CreateSection(CDC* pDC = NULL);

	/*This function selects the CDib object's logical palette into the device context and then realizes the palette. 
	The Draw() member function calls UsePalette() prior to painting the DIB.*/
	UINT UsePalette(CDC* pDC, bool bBackground = FALSE);

	/*If the color table exists, this function reads it and creates a Windows palette.
	The HPALETTE handle is stored in a data member.*/
	bool MakePalette();

	/*If you have a 16-bpp, 24-bpp, or 32-bpp DIB that doesn't have a color table, 
	you can call this function to create for your CDib object a logical palette 
	that matches the palette returned by the CreateHalftonePalette() function. 
	If your program is running on a 256-color palletized display and you don't call SetSystemPalette(), 
	you'll have no palette at all, and only the 20 standard Windows colors will appear in your DIB.*/
	bool SetSystemPalette(CDC* pDC);

	/*This function regenerates the DIB as a compressed or an uncompressed DIB. 
	Internally, it converts the existing DIB to a GDI bitmap and then makes a new compressed 
	or an uncompressed DIB. Compression is supported only for 4-bpp and 8-bpp DIBs. 
	You can't compress a DIB section.*/
	bool Compress(CDC* pDC, bool bCompress = TRUE); // FALSE means decompress

	/*This function creates a GDI bitmap from an existing DIB and is called by the Compress() function. 
	Don't confuse this function with CreateSection(), which generates a DIB and stores the handle.*/
	HBITMAP CreateBitmap(CDC* pDC);

	/*This function reads a DIB from a file into the CDib object. 
	The file must have been successfully opened. 
	If the file is a BMP file, reading starts from the beginning of the file. 
	If the file is a document, reading starts from the current file pointer.*/
	bool Read(CFile* pFile);

	/*This function reads the info header from a BMP file, calls CreateDIBSection() to allocate image memory, 
	and then reads the image bits from the file into that memory.	
	Use this function if you want to read a DIB from disk and then edit it by calling GDI functions. 
	You can write the DIB back to disk with Write or CopyToMapFile().*/
	bool ReadSection(CFile* pFile, CDC* pDC = NULL);

	/*This function writes a DIB from the CDib object to a file. 
	The file must have been successfully opened or created.*/
	bool Write(CFile* pFile);

	/*The CDib::Serialize function, which overrides the MFC CObject::Serialize function, 
	calls the Read() and Write() member functions.*/
	void Serialize(CArchive& ar);

	/*This function empties the DIB, 
	freeing allocated memory and closing the map file if necessary.*/
	void Empty();
protected:
	BYTE** m_ppBuff;								
	CRect m_rImageRect;								
	void DetachMapFile();
	void ComputePaletteSize(int nBitCount);
	void ComputeMetrics();
private:
	bool m_bImageAlloc;
};
#endif // _INSIDE_VISUAL_CPP_CDIB

 


 
 
 