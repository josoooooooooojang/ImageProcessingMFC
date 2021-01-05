// HistogramDlg.cpp: 구현 파일
//

#include "pch.h"
#include "ImageTool.h"
#include "HistogramDlg.h"
#include "afxdialogex.h"
#include "IppImage\IppDib.h"
#include "IppImage\IppImage.h"
#include "IppImage\IppConvert.h"
#include "IppImage\IppEnhance.h"

// CHistogramDlg 대화 상자

IMPLEMENT_DYNAMIC(CHistogramDlg, CDialogEx)

CHistogramDlg::CHistogramDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_HISTOGRAM, pParent)
{
	memset(m_Histogram, 0, sizeof(int) * 256);
}

CHistogramDlg::~CHistogramDlg()
{
}

void CHistogramDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CHistogramDlg, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CHistogramDlg 메시지 처리기


void CHistogramDlg::SetImage(IppDib* pDib)
{
	// 넘어온 이미지가 유효한 그레이스케일 이미지일 때
	if (pDib != NULL && pDib->GetBitCount() == 8) 
	{
		IppByteImage img;
		IppDibToImage(*pDib, img);

		float histo[256] = { 0.f, };
		IppHistogram(img, histo);

		float max_histo = histo[0];
		for (int i = 0; i < 256; i++)
			max_histo = histo[i] > max_histo ? histo[i] : max_histo;

		for (int i = 0; i < 256; i++)
			m_Histogram[i] = static_cast<int>(histo[i] * 100 / max_histo);

	}
	else
	{
		memset(m_Histogram, 0, sizeof(int) * 256);
	}
}


void CHistogramDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CGdiObject* pOldPen = dc.SelectStockObject(DC_PEN);

	// 히스토그램 박스
	dc.SetDCPenColor(RGB(128, 128, 128));
	dc.MoveTo(20, 20);
	dc.LineTo(20, 120);
	dc.LineTo(275, 120);
	dc.LineTo(275, 20);

	// 각 그레이스케일에 해당하는 히스토그램 출력
	dc.SetDCPenColor(RGB(0, 0, 0));
	for (int i = 0; i < 256; i++)
	{
		dc.MoveTo(20 + i, 120);
		dc.LineTo(20 + i, 120 - m_Histogram[i]);
	}

	// 그레이스케일 레벨 출력
	for (int i = 0; i < 256; i++)
	{
		dc.SetDCPenColor(RGB(i, i, i));
		dc.MoveTo(20 + i, 130);
		dc.LineTo(20 + i, 145);
	}
	dc.SelectObject(pOldPen);
}
