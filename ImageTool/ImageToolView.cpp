
// ImageToolView.cpp: CImageToolView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "ImageTool.h"
#endif

#include "ImageToolDoc.h"
#include "ImageToolView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CImageToolView

IMPLEMENT_DYNCREATE(CImageToolView, CScrollView)

BEGIN_MESSAGE_MAP(CImageToolView, CScrollView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CImageToolView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_VIEW_ZOOM1, &CImageToolView::OnViewZoom1)
	ON_COMMAND(ID_VIEW_ZOOM2, &CImageToolView::OnViewZoom2)
	ON_COMMAND(ID_VIEW_ZOOM3, &CImageToolView::OnViewZoom3)
	ON_COMMAND(ID_VIEW_ZOOM4, &CImageToolView::OnViewZoom4)
	ON_COMMAND(ID_VIEW_ZOOM5, &CImageToolView::OnViewZoom5)
	ON_COMMAND(ID_VIEW_ZOOM6, &CImageToolView::OnViewZoom6)
	ON_COMMAND(ID_VIEW_ZOOM7, &CImageToolView::OnViewZoom7)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOM1, &CImageToolView::OnUpdateViewZoom1)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOM2, &CImageToolView::OnUpdateViewZoom2)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOM3, &CImageToolView::OnUpdateViewZoom3)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOM4, &CImageToolView::OnUpdateViewZoom4)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOM5, &CImageToolView::OnUpdateViewZoom5)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOM6, &CImageToolView::OnUpdateViewZoom6)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOM7, &CImageToolView::OnUpdateViewZoom7)
END_MESSAGE_MAP()

// CImageToolView 생성/소멸

CImageToolView::CImageToolView() noexcept
	:m_nZoom(1.0)
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CImageToolView::~CImageToolView()
{
}

BOOL CImageToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CScrollView::PreCreateWindow(cs);
}

// CImageToolView 그리기

void CImageToolView::OnDraw(CDC* pDC)
{
	CImageToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (pDoc->m_Dib.IsValid())
	{
		int w = pDoc->m_Dib.GetWidth();
		int h = pDoc->m_Dib.GetHeight();
		pDoc->m_Dib.Draw(pDC->m_hDC, 0, 0, (int)(w * m_nZoom), (int)(h * m_nZoom));
	}
}

void CImageToolView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	SetScrollSizeToFit();
}

void CImageToolView::SetScrollSizeToFit()
{
	CSize sizeTotal;
	CImageToolDoc* pDoc = GetDocument();
	if (pDoc->m_Dib.IsValid())
	{
		int w = pDoc->m_Dib.GetWidth();
		int h = pDoc->m_Dib.GetHeight();

		sizeTotal.cx = (int)(w * m_nZoom);
		sizeTotal.cy = (int)(h * m_nZoom);
	}
	else
	{
		sizeTotal.cx = sizeTotal.cy = 100;
	}

	SetScrollSizes(MM_TEXT, sizeTotal);

	ResizeParentToFit(TRUE);
}




// CImageToolView 인쇄


void CImageToolView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CImageToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CImageToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CImageToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CImageToolView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CImageToolView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CImageToolView 진단

#ifdef _DEBUG
void CImageToolView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CImageToolView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CImageToolDoc* CImageToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImageToolDoc)));
	return (CImageToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CImageToolView 메시지 처리기


BOOL CImageToolView::OnEraseBkgnd(CDC* pDC)
{
	CBrush br;
	//br.CreateHatchBrush(HS_HORIZONTAL, RGB(123, 55, 11));
	//br.CreateHatchBrush(HS_VERTICAL, RGB(123, 55, 11));
	//br.CreateHatchBrush(HS_FDIAGONAL, RGB(123, 55, 11));
	//br.CreateHatchBrush(HS_BDIAGONAL, RGB(123, 55, 11));
	//br.CreateHatchBrush(HS_CROSS, RGB(123, 55, 11));
	//br.CreateHatchBrush(HS_DIAGCROSS, RGB(123, 55, 11));
	br.CreateHatchBrush(HS_API_MAX, RGB(123, 55, 11));
	FillOutsideRect(pDC, &br);

	return TRUE;
}


void CImageToolView::OnViewZoom1()
{
	m_nZoom = 1.0;
	SetScrollSizeToFit();
	Invalidate(TRUE);
}

void CImageToolView::OnViewZoom2()
{
	m_nZoom = 2.0;
	SetScrollSizeToFit();
	Invalidate(TRUE);
}

void CImageToolView::OnViewZoom3()
{
	m_nZoom = 3.0;
	SetScrollSizeToFit();
	Invalidate(TRUE);
}

void CImageToolView::OnViewZoom4()
{
	m_nZoom = 4.0;
	SetScrollSizeToFit();
	Invalidate(TRUE);
}

void CImageToolView::OnViewZoom5()
{
	m_nZoom = 0.5;
	SetScrollSizeToFit();
	Invalidate(TRUE);
}

void CImageToolView::OnViewZoom6()
{
	m_nZoom = 0.33;
	SetScrollSizeToFit();
	Invalidate(TRUE);
}

void CImageToolView::OnViewZoom7()
{
	m_nZoom = 0.25;
	SetScrollSizeToFit();
	Invalidate(TRUE);
}

void CImageToolView::OnUpdateViewZoom1(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nZoom == 1.0);
}

void CImageToolView::OnUpdateViewZoom2(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nZoom == 2.0);
}

void CImageToolView::OnUpdateViewZoom3(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nZoom == 3.0);
}

void CImageToolView::OnUpdateViewZoom4(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nZoom == 4.0);
}

void CImageToolView::OnUpdateViewZoom5(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nZoom == 0.5);
}

void CImageToolView::OnUpdateViewZoom6(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck((int)(m_nZoom * 10) == 3);
}

void CImageToolView::OnUpdateViewZoom7(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nZoom == 0.25);
}
