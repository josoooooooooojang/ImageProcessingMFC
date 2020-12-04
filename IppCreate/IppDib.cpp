#include "pch.h"
#include "IppDib.h"

IppDib::IppDib()
	:m_nWidth(0), m_nHeight(0), m_nBitCount(0), m_nDibSize(0), m_pDib(NULL)
{
}

IppDib::IppDib(const IppDib& dib)
	: m_nWidth(dib.m_nWidth), m_nHeight(dib.m_nWidth), m_nBitCount(dib.m_nBitCount), m_nDibSize(dib.m_nDibSize), m_pDib(NULL)
{
	if (dib.m_pDib) {
		m_pDib = new BYTE[dib.m_nDibSize];
		memcpy(m_pDib, dib.m_pDib, m_nDibSize);
	}
}

IppDib::~IppDib()
{
	if (m_pDib) {
		delete[] m_pDib;
		m_pDib = NULL;
	}
}

BOOL IppDib::CreateGrayBitmap(LONG nWidth, LONG nHeight)
{
	if (m_pDib)
		Destroy();

	m_nWidth = nWidth;
	m_nHeight = nHeight;
	m_nBitCount = 8;

	DWORD dwWidthStep = (m_nWidth * m_nBitCount / 8 + 3) & ~3;
	DWORD dwSizeImage = (m_nHeight * dwWidthStep);
	m_nDibSize = sizeof(BITMAPINFOHEADER) + (sizeof(RGBQUAD) * GetPaletteNums()) + dwSizeImage;

	m_pDib = new BYTE[m_nDibSize];
	if (m_pDib == NULL)
		return FALSE;

	// BITMAPINFOHEADER 구조체 설정
	LPBITMAPINFOHEADER lpbmi = (LPBITMAPINFOHEADER)m_pDib;
	lpbmi->biSize = sizeof(BITMAPINFOHEADER);
	lpbmi->biWidth = m_nWidth;
	lpbmi->biHeight = m_nHeight;
	lpbmi->biPlanes = 1;
	lpbmi->biBitCount = m_nBitCount;
	lpbmi->biCompression = BI_RGB;
	lpbmi->biSizeImage = dwSizeImage;
	lpbmi->biXPelsPerMeter = 0;
	lpbmi->biYPelsPerMeter = 0;
	lpbmi->biClrUsed = 0;
	lpbmi->biClrImportant = 0;

	// 그레이스케일 팔레트 설정
	RGBQUAD* pPal = (RGBQUAD*)((BYTE*)m_pDib + sizeof(BITMAPINFOHEADER));
	for (int i = 0; i < 256; i++)
	{
		pPal->rgbBlue = (BYTE)i;
		pPal->rgbGreen = (BYTE)i;
		pPal->rgbRed = (BYTE)i;
		pPal->rgbReserved = 0;
		pPal++;
	}

	// 픽셀 데이터 초기화
	BYTE* pData = GetDIBitsAddr();
	memset(pData, 0, dwSizeImage);

	return TRUE;
}

BOOL IppDib::CreateRgbBitmap(LONG nWidth, LONG nHeight)
{
	if (m_pDib)
		Destroy();

	m_nWidth = nWidth;
	m_nHeight = nHeight;
	m_nBitCount = 24;

	DWORD dwWidthStep = (m_nWidth * m_nBitCount / 8 + 3) & ~3;
	DWORD dwSizeImage = (m_nHeight * dwWidthStep);
	m_nDibSize = sizeof(BITMAPINFOHEADER) + dwSizeImage;

	m_pDib = new BYTE[m_nDibSize];
	if (m_pDib == NULL)
		return FALSE;

	// BITMAPINFOHEADER 구조체 설정
	LPBITMAPINFOHEADER lpbmi = (LPBITMAPINFOHEADER)m_pDib;
	lpbmi->biSize = sizeof(BITMAPINFOHEADER);
	lpbmi->biWidth = m_nWidth;
	lpbmi->biHeight = m_nHeight;
	lpbmi->biPlanes = 1;
	lpbmi->biBitCount = m_nBitCount;
	lpbmi->biCompression = BI_RGB;
	lpbmi->biSizeImage = dwSizeImage;
	lpbmi->biXPelsPerMeter = 0;
	lpbmi->biYPelsPerMeter = 0;
	lpbmi->biClrUsed = 0;
	lpbmi->biClrImportant = 0;

	// 픽셀 데이터 초기화
	BYTE* pData = GetDIBitsAddr();
	memset(pData, 0, dwSizeImage);

	return TRUE;
}

void IppDib::Destroy()
{
	if (m_pDib)
	{
		delete[] m_pDib;
		m_pDib = NULL;
	}

	m_nWidth = 0;
	m_nHeight = 0;
	m_nBitCount = 0;
	m_nDibSize = 0;
}

#define DIB_HEADER_MARKER ((WORD) ('M' << 8) | 'B')

BOOL IppDib::Load(const char* filename)
{
	const char* ext = strrchr(filename, '.');
	if (!_strcmpi(ext, ".bmp"))
		return LoadBMP(filename);
	else
		return FALSE;
}

BOOL IppDib::Save(const char* filename)
{
	const char* ext = strrchr(filename, '.');
	if (!_strcmpi(ext, ".bmp"))
		return SaveBMP(filename);
	else
		return FALSE;
}

BOOL IppDib::LoadBMP(const char* filename)
{
	FILE* fp = NULL;
	fopen_s(&fp, filename, "rb");
	if (!fp)
		return false;

	BITMAPFILEHEADER bmfh;
	BITMAPINFOHEADER bmih;

	// BITMAPFILEHEADER 읽기
	if (fread(&bmfh, sizeof(BITMAPFILEHEADER), 1, fp) != 1) {
		fclose(fp);
		return false;
	}

	// BMP 파일임을 나타내는 "BM" 마커가 있는지 확인
	if (bmfh.bfType != DIB_HEADER_MARKER) {
		fclose(fp);
		return false;
	}

	// BITMAPINFOHEADER 읽기
	if (fread(&bmih, sizeof(BITMAPINFOHEADER), 1, fp) != 1) {
		fclose(fp);
		return false;
	}

	m_nWidth = bmih.biWidth;
	m_nHeight = bmih.biHeight;
	m_nBitCount = bmih.biBitCount;

	// 픽셀 데이터 공간 계산
	DWORD dwWidthStep = (DWORD)((m_nWidth * m_nBitCount / 8 + 3) & ~3);
	DWORD dwSizeImage = m_nHeight * dwWidthStep;

	if (m_nBitCount == 24)
		m_nDibSize = sizeof(BITMAPINFOHEADER) + dwSizeImage;
	else
		m_nDibSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * (1 << m_nBitCount) + dwSizeImage;

	// Packed-DIB 저장할 메모리 공간 동적 할당
	if (m_pDib)
		Destroy();

	m_pDib = new BYTE[m_nDibSize];
	if (m_pDib == NULL)
	{
		fclose(fp);
		return FALSE;
	}

	// 파일로부터 Packed-DIB 크기만큼을 읽기
	fseek(fp, sizeof(BITMAPFILEHEADER), SEEK_SET);
	if (fread(m_pDib, sizeof(BYTE), m_nDibSize, fp) != m_nDibSize)
	{
		delete[] m_pDib;
		m_pDib = NULL;
		fclose(fp);
		return FALSE;
	}

	// 파일 닫기
	fclose(fp);

	return TRUE;
}

BOOL IppDib::SaveBMP(const char* filename)
{
	if (!IsValid())
		return FALSE;

	FILE* fp;
	fopen_s(&fp, filename, "wb");
	if (!fp)
		return FALSE;

	BITMAPFILEHEADER bmfh;
	bmfh.bfType = DIB_HEADER_MARKER;
	bmfh.bfSize = (DWORD)sizeof(BITMAPFILEHEADER) + m_nDibSize;
	bmfh.bfReserved1 = 0;
	bmfh.bfReserved2 = 0;
	bmfh.bfOffBits = (DWORD)(sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
		+ (sizeof(RGBQUAD) * GetPaletteNums()));

	fwrite(&bmfh, sizeof(BITMAPFILEHEADER), 1, fp);
	fwrite(GetBitmapInfoAddr(), m_nDibSize, 1, fp);

	fclose(fp);

	return TRUE;
}

void IppDib::Draw(HDC hdc, int dx, int dy)
{
	if (m_pDib == NULL)
		return;

	LPBITMAPINFO lpbi = (LPBITMAPINFO)m_pDib;
	LPVOID lpBits = (LPVOID)GetDIBitsAddr();

	::SetDIBitsToDevice(hdc, // hdc
		dx,                  // DestX
		dy,                  // DestY
		m_nWidth,            // nSrcWidth
		m_nHeight,           // nSrcHeight
		0,                   // SrcX
		0,                   // SrcY
		0,                   // nStartScan
		m_nHeight,           // nNumScans
		lpBits,              // lpBits
		lpbi,                // lpBitsInfo
		DIB_RGB_COLORS);     // wUsage
}

void IppDib::Draw(HDC hdc, int dx, int dy, int dw, int dh, DWORD dwRop)
{
	Draw(hdc, dx, dy, dw, dh, 0, 0, m_nWidth, m_nHeight, dwRop);
}

void IppDib::Draw(HDC hdc, int dx, int dy, int dw, int dh,
	int sx, int sy, int sw, int sh, DWORD dwRop)
{
	if (m_pDib == NULL)
		return;

	LPBITMAPINFO lpbi = (LPBITMAPINFO)m_pDib;
	LPSTR lpDIBBits = (LPSTR)GetDIBitsAddr();

	SetStretchBltMode(hdc, COLORONCOLOR);
	::StretchDIBits(hdc, // hdc
		dx,              // XDest
		dy,              // YDest
		dw,              // nDestWidth
		dh,              // nDestHeight
		sx,              // XSrc
		sy,              // YSrc
		sw,              // nSrcWidth
		sh,              // nSrcHeight
		lpDIBBits,       // lpBits
		lpbi,            // lpBitsInfo
		DIB_RGB_COLORS,  // wUsage
		SRCCOPY);        // dwROP
}