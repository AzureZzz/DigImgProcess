
// DigImgProcessView.cpp: CDigImgProcessView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "DigImgProcess.h"
#endif

#include "DigImgProcessDoc.h"
#include "DigImgProcessView.h"
#include "MainFrm.h"
#include "IntensityDlg.h"
#include "SmoothDlg.h"
#include "SharpThreDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDigImgProcessView

IMPLEMENT_DYNCREATE(CDigImgProcessView, CView)

BEGIN_MESSAGE_MAP(CDigImgProcessView, CView)
	//{{AFX_MSG_MAP(CDigImgProcessView)
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(32771, &CDigImgProcessView::On32771)
	ON_COMMAND(ID_FREQ_FOUR, &CDigImgProcessView::OnFreqFour)
//	ON_COMMAND(ID_ENHA_SMOOTH, &CDigImgProcessView::OnEnhaSmooth)
	ON_COMMAND(ID_ENHA_GRADSHARP, &CDigImgProcessView::OnEnhaGradsharp)
	ON_COMMAND(ID_ENHA_SHARP, &CDigImgProcessView::OnEnhaSharp)
//	ON_COMMAND(ID_ENHA_SMOOTH, &CDigImgProcessView::OnEnhaSmooth)
ON_COMMAND(ID_ENHA_SMOOTH, &CDigImgProcessView::OnEnhaSmooth)
END_MESSAGE_MAP()

// CDigImgProcessView 构造/析构

CDigImgProcessView::CDigImgProcessView()
{
	// TODO: 在此处添加构造代码

}

CDigImgProcessView::~CDigImgProcessView()
{
}

BOOL CDigImgProcessView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CDigImgProcessView 绘图

void CDigImgProcessView::OnDraw(CDC* pDC)
{
	BeginWaitCursor();

	CDigImgProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	HDIB hDIB = pDoc->GetHDIB();
	if (hDIB != NULL)
	{
		LPSTR lpDIB = (LPSTR) ::GlobalLock((HGLOBAL)hDIB);
		int cxDIB = (int)pDoc->GetDibImage()->DIBWidth(lpDIB);
		int cyDIB = (int)pDoc->GetDibImage()->DIBHeight(lpDIB);

		::GlobalUnlock((HGLOBAL)hDIB);

		CRect rcDIB;
		rcDIB.top = rcDIB.left = 0;
		rcDIB.right = cxDIB;
		rcDIB.bottom = cyDIB;

		CRect rcDest;

		if (pDC->IsPrinting())
		{
			// 是打印，计算输出图像的位置和大小，以便符合页面

			// 获取打印页面的水平宽度(象素)
			int cxPage = pDC->GetDeviceCaps(HORZRES);
			// 获取打印页面的垂直高度(象素)
			int cyPage = pDC->GetDeviceCaps(VERTRES);
			// 获取打印机每英寸象素数
			int cxInch = pDC->GetDeviceCaps(LOGPIXELSX);
			int cyInch = pDC->GetDeviceCaps(LOGPIXELSY);

			// 计算打印图像大小（缩放，根据页面宽度调整图像大小）
			rcDest.top = rcDest.left = 0;
			rcDest.bottom = (int)(((double)cyDIB * cxPage * cyInch)
				/ ((double)cxDIB * cxInch));
			rcDest.right = cxPage;

			// 计算打印图像位置（垂直居中）
			int temp = cyPage - (rcDest.bottom - rcDest.top);
			rcDest.bottom += temp / 2;
			rcDest.top += temp / 2;
		}
		else
		{
			// 不必缩放图像
			rcDest = rcDIB;
		}

		pDoc->GetDibImage()->PaintDIB(pDC->m_hDC, &rcDest, pDoc->GetHDIB(),
			&rcDIB, pDoc->GetDocPalette());
	}

	EndWaitCursor();
}


// CDigImgProcessView 打印

BOOL CDigImgProcessView::OnPreparePrinting(CPrintInfo* pInfo)
{
	pInfo->SetMaxPage(1);
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CDigImgProcessView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CDigImgProcessView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CDigImgProcessView 诊断

#ifdef _DEBUG
void CDigImgProcessView::AssertValid() const
{
	CView::AssertValid();
}

void CDigImgProcessView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDigImgProcessDoc* CDigImgProcessView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDigImgProcessDoc)));
	return (CDigImgProcessDoc*)m_pDocument;
}
#endif //_DEBUG


// CDigImgProcessView 消息处理程序
BOOL CDigImgProcessView::OnEraseBkgnd(CDC* pDC)
{
	// 主要是为了设置子窗体默认的背景色
	// 背景色由文档成员变量m_refColorBKG指定

	CDigImgProcessDoc* pDoc = GetDocument();
	CBrush brush(pDoc->m_refColorBKG);
	CBrush* pOldBrush = pDC->SelectObject(&brush);

	CRect rectClip;
	pDC->GetClipBox(&rectClip);			// 获取重绘区域	
	pDC->PatBlt(rectClip.left, rectClip.top, rectClip.Width(),
		rectClip.Height(), PATCOPY);

	pDC->SelectObject(pOldBrush);

	return TRUE;
}

void CDigImgProcessView::OnEditCopy()
{
	// 复制当前图像

	CDigImgProcessDoc* pDoc = GetDocument();

	if (OpenClipboard())
	{
		BeginWaitCursor();
		EmptyClipboard();

		// 复制当前图像到剪贴板
		SetClipboardData(CF_DIB, pDoc->GetDibImage()->
			CopyHandle((HANDLE)pDoc->GetHDIB()));

		CloseClipboard();
		EndWaitCursor();
	}
}

void CDigImgProcessView::OnUpdateEditCopy(CCmdUI* pCmdUI)
{
	// 如果当前DIB对象不空，复制菜单项有效
	pCmdUI->Enable(GetDocument()->GetHDIB() != NULL);
}

void CDigImgProcessView::OnEditPaste()
{
	// 粘贴图像

	HDIB hNewDIB = NULL;
	CDigImgProcessDoc* pDoc = GetDocument();

	if (OpenClipboard())
	{
		BeginWaitCursor();

		// 读取剪贴板中的图像
		hNewDIB = (HDIB)pDoc->GetDibImage()->CopyHandle(::GetClipboardData(CF_DIB));

		CloseClipboard();

		if (hNewDIB != NULL)
		{
			pDoc->ReplaceHDIB(hNewDIB);		// 替换DIB，同时释放旧DIB对象		
			pDoc->InitDIBData();			// 更新DIB大小和调色板		
			pDoc->SetModifiedFlag(TRUE);	// 设置脏标记	
			SetScrollSizes(MM_TEXT, pDoc->GetDocSize());	// 重新设置滚动视图大小		
			OnDoRealize((WPARAM)m_hWnd, 0);	// 实现新的调色板		
			pDoc->UpdateAllViews(NULL);		// 更新视图
		}

		EndWaitCursor();
	}
}

void CDigImgProcessView::OnUpdateEditPaste(CCmdUI* pCmdUI)
{
	// 如果当前剪贴板中有DIB对象，粘贴菜单项有效
	pCmdUI->Enable(::IsClipboardFormatAvailable(CF_DIB));
}

LRESULT CDigImgProcessView::OnDoRealize(WPARAM wParam, LPARAM)
{
	ASSERT(wParam != NULL);

	CDigImgProcessDoc* pDoc = GetDocument();
	if (pDoc->GetHDIB() == NULL)
	{
		return 0L;
	}

	CPalette* pPal = pDoc->GetDocPalette();
	if (pPal != NULL)
	{
		CMainFrame* pAppFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
		ASSERT_KINDOF(CMainFrame, pAppFrame);

		CClientDC appDC(pAppFrame);

		// All views but one should be a background palette.
		// wParam contains a handle to the active view, so the SelectPalette
		// bForceBackground flag is FALSE only if wParam == m_hWnd (this view)
		CPalette* oldPalette = appDC.SelectPalette(pPal, ((HWND)wParam) != m_hWnd);
		if (oldPalette != NULL)
		{
			UINT nColorsChanged = appDC.RealizePalette();
			if (nColorsChanged > 0)
			{
				pDoc->UpdateAllViews(NULL);
			}
			appDC.SelectPalette(oldPalette, TRUE);
		}
		else
		{
			TRACE0("\tCCh1_1View::OnPaletteChanged中调用SelectPalette()失败！\n");
		}
	}

	return 0L;
}

void CDigImgProcessView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	CScrollView::OnActivateView(bActivate, pActivateView, pDeactiveView);

	if (bActivate)
	{
		ASSERT(pActivateView == this);
		OnDoRealize((WPARAM)m_hWnd, 0);   // same as SendMessage(WM_DOREALIZE);
	}
}

void CDigImgProcessView::CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType)
{
	CScrollView::OnInitialUpdate();
	ASSERT(GetDocument() != NULL);

	SetScrollSizes(MM_TEXT, GetDocument()->GetDocSize());
}


void CDigImgProcessView::OnPointEqua()
{
	// 灰度均衡

	CDigImgProcessDoc* pDoc = GetDocument();

	LPSTR	lpDIB;				// 指向DIB的指针	
	LPSTR    lpDIBBits;			// 指向DIB象素指针
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHDIB());
	// 找到DIB图像象素起始位置
	lpDIBBits = pDoc->GetDibImage()->FindDIBBits(lpDIB);

	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的直方图均衡，其它的可以类推）
	if (pDoc->GetDibImage()->DIBNumColors(lpDIB) != 256)
	{
		MessageBox(L"目前只支持256色位图的直方图均衡！", L"系统提示",
			MB_ICONINFORMATION | MB_OK);
		::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());

		return;
	}

	BeginWaitCursor();

	// 调用InteEqualize()函数进行直方图均衡
	pDoc->GetDibImage()->InteEqualize(lpDIBBits, pDoc->GetDibImage()->DIBWidth(lpDIB),
		pDoc->GetDibImage()->DIBHeight(lpDIB));

	pDoc->SetModifiedFlag(TRUE);	// 设置脏标记	
	pDoc->UpdateAllViews(NULL);		// 更新视图

	::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());
	EndWaitCursor();
}

void CDigImgProcessView::OnPointInvert()
{
	// 图像反色

	CDigImgProcessDoc* pDoc = GetDocument();

	LPSTR lpDIB;			// 指向DIB的指针	
	LPSTR lpDIBBits;		// 指向DIB象素指针	
	FLOAT fA;				// 线性变换的斜率	
	FLOAT fB;				// 线性变换的截距	
							// 反色操作的线性变换的方程是-x + 255
	fA = -1.0;
	fB = 255.0;

	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHDIB());
	// 找到DIB图像象素起始位置
	lpDIBBits = pDoc->GetDibImage()->FindDIBBits(lpDIB);

	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的反色，其它的可以类推）
	if (pDoc->GetDibImage()->DIBNumColors(lpDIB) != 256)
	{
		MessageBox(L"目前只支持256色位图的反色！", L"系统提示",
			MB_ICONINFORMATION | MB_OK);
		::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());

		return;
	}

	BeginWaitCursor();

	// 调用LinerTrans()函数反色
	pDoc->GetDibImage()->LinerTrans(lpDIBBits, pDoc->GetDibImage()->DIBWidth(lpDIB),
		pDoc->GetDibImage()->DIBHeight(lpDIB), fA, fB);

	pDoc->SetModifiedFlag(TRUE);	// 设置脏标记	
	pDoc->UpdateAllViews(NULL);		// 更新视图

	::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());
	EndWaitCursor();
}



void CDigImgProcessView::On32771()
{
	CDigImgProcessDoc* pDoc = GetDocument();

	LPSTR lpDIB;			// 指向DIB的指针	
	LPSTR    lpDIBBits;		// 指向DIB象素指针
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHDIB());
	// 找到DIB图像象素起始位置
	lpDIBBits = pDoc->GetDibImage()->FindDIBBits(lpDIB);

	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图，其它的可以类推）
	if (pDoc->GetDibImage()->DIBNumColors(lpDIB) != 256)
	{
		MessageBox(L"目前只支持查看256色位图灰度直方图！", L"系统提示",
			MB_ICONINFORMATION | MB_OK);
		::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());

		return;
	}

	BeginWaitCursor();

	CIntensityDlg dlgPara;
	dlgPara.m_lpDIBBits = lpDIBBits;
	dlgPara.m_lWidth = pDoc->GetDibImage()->DIBWidth(lpDIB);
	dlgPara.m_lHeight = pDoc->GetDibImage()->DIBHeight(lpDIB);
	dlgPara.m_iLowGray = 0;
	dlgPara.m_iUpGray = 255;

	// 显示对话框，提示用户设定平移量
	if (dlgPara.DoModal() != IDOK)
	{
		return;
	}

	::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());
	EndWaitCursor();
}


void CDigImgProcessView::OnFreqFour()
{
	// 图像付立叶变换

	CDigImgProcessDoc* pDoc = GetDocument();

	LPSTR	lpDIB;				// 指向DIB的指针	
	LPSTR    lpDIBBits;			// 指向DIB象素指针
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHDIB());

	// 找到DIB图像象素起始位置
	lpDIBBits = pDoc->GetDibImage()->FindDIBBits(lpDIB);

	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的付立叶变换，其它的可以类推）
	if (pDoc->GetDibImage()->DIBNumColors(lpDIB) != 256)
	{
		MessageBox(L"目前只支持256色位图的付立叶变换！", L"系统提示",
			MB_ICONINFORMATION | MB_OK);
		::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());

		return;
	}

	BeginWaitCursor();

	// 调用Fourier()函数进行付立叶变换
	if (pDoc->GetDibImage()->Fourier(lpDIBBits, pDoc->GetDibImage()->DIBWidth(lpDIB),
		pDoc->GetDibImage()->DIBHeight(lpDIB)))
	{
		pDoc->SetModifiedFlag(TRUE);		// 设置脏标记		
		pDoc->UpdateAllViews(NULL);			// 更新视图
	}
	else
	{
		MessageBox(L"分配内存失败！", L"系统提示", MB_ICONINFORMATION | MB_OK);
	}

	::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());
	EndWaitCursor();
}



void CDigImgProcessView::OnEnhaGradsharp()
{
	// 梯度锐化

	CDigImgProcessDoc* pDoc = GetDocument();

	LPSTR lpDIB;				// 指向DIB的指针	
	LPSTR    lpDIBBits;			// 指向DIB象素指针
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHDIB());

	// 找到DIB图像象素起始位置
	lpDIBBits = pDoc->GetDibImage()->FindDIBBits(lpDIB);

	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的梯度锐化，其它的可以类推）
	if (pDoc->GetDibImage()->DIBNumColors(lpDIB) != 256)
	{
		MessageBox(L"目前只支持256色位图的梯度锐化！", L"系统提示",
			MB_ICONINFORMATION | MB_OK);
		::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());

		return;
	}

	BYTE	bThre;			// 阈值

	CSharpThreDlg dlgPara;
	dlgPara.m_bThre = 10;
	// 提示用户输入阈值
	if (dlgPara.DoModal() != IDOK)
	{
		return;
	}

	bThre = dlgPara.m_bThre;
	delete dlgPara;

	BeginWaitCursor();

	// 调用GradSharp()函数进行梯度板锐化
	if (pDoc->GetDibImage()->GradSharp(lpDIBBits,
		pDoc->GetDibImage()->DIBWidth(lpDIB),
		pDoc->GetDibImage()->DIBHeight(lpDIB), bThre))
	{
		pDoc->SetModifiedFlag(TRUE);		// 设置脏标记		
		pDoc->UpdateAllViews(NULL);			// 更新视图
	}
	else
	{
		MessageBox(L"分配内存失败！", L"系统提示", MB_ICONINFORMATION | MB_OK);
	}

	::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());

	EndWaitCursor();
}


void CDigImgProcessView::OnEnhaSharp()
{
	// 图像锐化

	CDigImgProcessDoc* pDoc = GetDocument();

	LPSTR lpDIB;				// 指向DIB的指针	
	LPSTR    lpDIBBits;			// 指向DIB象素指针	
	int		iTempH;				// 模板高度	
	int		iTempW;				// 模板宽度	
	FLOAT	fTempC;				// 模板系数	
	int		iTempMX;			// 模板中心元素X坐标	
	int		iTempMY;			// 模板中心元素Y坐标	
	FLOAT	aValue[9];			// 模板元素数组

	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHDIB());
	// 找到DIB图像象素起始位置
	lpDIBBits = pDoc->GetDibImage()->FindDIBBits(lpDIB);

	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的锐化，其它的可以类推）
	if (pDoc->GetDibImage()->DIBNumColors(lpDIB) != 256)
	{
		MessageBox(L"目前只支持256色位图的锐化！", L"系统提示",
			MB_ICONINFORMATION | MB_OK);
		::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());

		return;
	}

	BeginWaitCursor();

	// 设置拉普拉斯模板参数
	iTempW = 3;
	iTempH = 3;
	fTempC = 1.0;
	iTempMX = 1;
	iTempMY = 1;
	aValue[0] = -1.0;
	aValue[1] = -1.0;
	aValue[2] = -1.0;
	aValue[3] = -1.0;
	aValue[4] = 9.0;
	aValue[5] = -1.0;
	aValue[6] = -1.0;
	aValue[7] = -1.0;
	aValue[8] = -1.0;

	// 调用Template()函数用拉普拉斯模板锐化DIB
	if (pDoc->GetDibImage()->Template(lpDIBBits, pDoc->GetDibImage()->DIBWidth(lpDIB),
		pDoc->GetDibImage()->DIBHeight(lpDIB), iTempH, iTempW, iTempMX,
		iTempMY, aValue, fTempC))
	{
		pDoc->SetModifiedFlag(TRUE);		// 设置脏标记		
		pDoc->UpdateAllViews(NULL);			// 更新视图
	}
	else
	{
		MessageBox(L"分配内存失败！", L"系统提示", MB_ICONINFORMATION | MB_OK);
	}

	::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());

	EndWaitCursor();
}



void CDigImgProcessView::OnEnhaSmooth()
{
	// 图像平滑

	CDigImgProcessDoc* pDoc = GetDocument();

	LPSTR	lpDIB;				// 指向DIB的指针	
	LPSTR   lpDIBBits;			// 指向DIB象素指针	
	int		iTempH;				// 模板高度	
	int		iTempW;				// 模板宽度	
	FLOAT	fTempC;				// 模板系数	
	int		iTempMX;			// 模板中心元素X坐标	
	int		iTempMY;			// 模板中心元素Y坐标
	FLOAT	aValue[25];			// 模板元素数组

	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHDIB());
	// 找到DIB图像象素起始位置
	lpDIBBits = pDoc->GetDibImage()->FindDIBBits(lpDIB);

	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的平滑，其它的可以类推）
	if (pDoc->GetDibImage()->DIBNumColors(lpDIB) != 256)
	{
		MessageBox(L"目前只支持256色位图的平滑！", L"系统提示",
			MB_ICONINFORMATION | MB_OK);
		::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());

		return;
	}

	CSmoothDlg dlgPara;
	// 给模板数组赋初值（为平均模板）
	aValue[0] = 1.0;
	aValue[1] = 1.0;
	aValue[2] = 1.0;
	aValue[3] = 0.0;
	aValue[4] = 0.0;
	aValue[5] = 1.0;
	aValue[6] = 1.0;
	aValue[7] = 1.0;
	aValue[8] = 0.0;
	aValue[9] = 0.0;
	aValue[10] = 1.0;
	aValue[11] = 1.0;
	aValue[12] = 1.0;
	aValue[13] = 0.0;
	aValue[14] = 0.0;
	aValue[15] = 0.0;
	aValue[16] = 0.0;
	aValue[17] = 0.0;
	aValue[18] = 0.0;
	aValue[19] = 0.0;
	aValue[20] = 0.0;
	aValue[21] = 0.0;
	aValue[22] = 0.0;
	aValue[23] = 0.0;
	aValue[24] = 0.0;

	dlgPara.m_intType = 0;
	dlgPara.m_iTempH = 3;
	dlgPara.m_iTempW = 3;
	dlgPara.m_iTempMX = 1;
	dlgPara.m_iTempMY = 1;
	dlgPara.m_fTempC = (FLOAT)(1.0 / 9.0);
	dlgPara.m_fpArray = aValue;

	// 显示对话框，提示用户设定平移量
	if (dlgPara.DoModal() != IDOK)
	{
		return;
	}

	iTempH = dlgPara.m_iTempH;
	iTempW = dlgPara.m_iTempW;
	iTempMX = dlgPara.m_iTempMX;
	iTempMY = dlgPara.m_iTempMY;
	fTempC = dlgPara.m_fTempC;

	delete dlgPara;
	BeginWaitCursor();

	// 调用Template()函数平滑DIB
	if (pDoc->GetDibImage()->Template(lpDIBBits, pDoc->GetDibImage()->DIBWidth(lpDIB),
		pDoc->GetDibImage()->DIBHeight(lpDIB), iTempH, iTempW, iTempMX,
		iTempMY, aValue, fTempC))
	{
		pDoc->SetModifiedFlag(TRUE);		// 设置脏标记		
		pDoc->UpdateAllViews(NULL);			// 更新视图
	}
	else
	{
		MessageBox(L"分配内存失败！", L"系统提示", MB_ICONINFORMATION | MB_OK);
	}

	::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());

	EndWaitCursor();
}
