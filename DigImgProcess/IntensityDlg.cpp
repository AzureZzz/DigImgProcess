// IntensityDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "DigImgProcess.h"
#include "IntensityDlg.h"
#include "afxdialogex.h"

#include "DibImage.h"


// CIntensityDlg 对话框

IMPLEMENT_DYNAMIC(CIntensityDlg, CDialog)

CIntensityDlg::CIntensityDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DLG_INTENSITY, pParent)
{
	m_iLowGray = 0;
	m_iUpGray = 0;
}

CIntensityDlg::~CIntensityDlg()
{
}

void CIntensityDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_LOWGRAY, m_iLowGray);
	DDV_MinMaxInt(pDX, m_iLowGray, 0, 255);
	DDX_Text(pDX, IDC_EDIT_UPGRAY, m_iUpGray);
	DDV_MinMaxInt(pDX, m_iUpGray, 0, 255);
}


BEGIN_MESSAGE_MAP(CIntensityDlg, CDialog)
	//{{AFX_MSG_MAP(CIntensityDlg)
	ON_WM_PAINT()
	ON_EN_KILLFOCUS(IDC_EDIT_LOWGRAY, OnKillfocusEDITLowGray)
	ON_EN_KILLFOCUS(IDC_EDIT_UPGRAY, OnKillfocusEDITUpGray)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
	ON_STN_CLICKED(IDC_COORD, &CIntensityDlg::OnStnClickedCoord)
	ON_BN_CLICKED(IDOK, &CIntensityDlg::OnBnClickedOk)
//	ON_COMMAND(32771, &CIntensityDlg::On32771)
END_MESSAGE_MAP()


// CIntensityDlg 消息处理程序
BOOL CIntensityDlg::OnInitDialog()
{
	unsigned char * lpSrc;		// 指向源图像象素的指针
	LONG i, j;

	CDialog::OnInitDialog();

	CWnd* pWnd = GetDlgItem(IDC_COORD);
	pWnd->GetClientRect(m_MouseRect);
	pWnd->ClientToScreen(&m_MouseRect);

	CRect rect;
	GetClientRect(rect);
	ClientToScreen(&rect);
	m_MouseRect.top -= rect.top;
	m_MouseRect.left -= rect.left;

	// 设置接受鼠标事件的有效区域
	m_MouseRect.top += 25;
	m_MouseRect.left += 10;
	m_MouseRect.bottom = m_MouseRect.top + 255;
	m_MouseRect.right = m_MouseRect.left + 256;

	for (i = 0; i < 256; i++)
	{
		m_lCount[i] = 0;
	}

	LONG lLineBytes;			// 图像每行的字节数
	lLineBytes = WIDTHBYTES(m_lWidth * 8);

	// 计算各个灰度值的计数
	for (i = 0; i < m_lHeight; i++)
	{
		for (j = 0; j < m_lWidth; j++)
		{
			lpSrc = (unsigned char *)m_lpDIBBits + lLineBytes * i + j;
			m_lCount[*(lpSrc)]++;
		}
	}

	m_iIsDraging = 0;

	return TRUE;
}

void CIntensityDlg::OnKillfocusEDITLowGray()
{
	UpdateData(TRUE);

	// 判断是否下限超过上限
	if (m_iLowGray > m_iUpGray)
	{
		int iTemp = m_iLowGray;
		m_iLowGray = m_iUpGray;
		m_iUpGray = iTemp;
		UpdateData(FALSE);
	}

	InvalidateRect(m_MouseRect, TRUE);
}

void CIntensityDlg::OnKillfocusEDITUpGray()
{
	UpdateData(TRUE);

	// 判断是否下限超过上限
	if (m_iLowGray > m_iUpGray)
	{
		int iTemp = m_iLowGray;
		m_iLowGray = m_iUpGray;
		m_iUpGray = iTemp;
		UpdateData(FALSE);
	}

	InvalidateRect(m_MouseRect, TRUE);
}

void CIntensityDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// 判断是否在接受鼠标事件的有效区域中
	if (m_MouseRect.PtInRect(point))
	{
		if (point.x == (m_MouseRect.left + m_iLowGray))
		{
			m_iIsDraging = 1;		// 设置拖动状态1，拖动下限
			::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
		}
		else if (point.x == (m_MouseRect.left + m_iUpGray))
		{
			m_iIsDraging = 2;		// 设置拖动状态为2，拖动上限
			::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
		}
	}

	CDialog::OnLButtonDown(nFlags, point);
}

void CIntensityDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// 判断是否在接受鼠标事件的有效区域中
	if (m_MouseRect.PtInRect(point))
	{
		// 判断是否正在拖动
		if (m_iIsDraging != 0)
		{
			// 判断正在拖动上限还是下限
			if (m_iIsDraging == 1)
			{
				// 判断是否下限<上限
				if (point.x - m_MouseRect.left < m_iUpGray)
				{
					m_iLowGray = point.x - m_MouseRect.left;	// 更改下限
				}
				else
				{
					m_iLowGray = m_iUpGray - 1;		// 下限拖过上限，设置为上限-1					
					point.x = m_MouseRect.left + m_iUpGray - 1; // 重设鼠标位置
				}
			}
			else
			{
				// 正在拖动上限		
				// 判断是否上限>下限
				if (point.x - m_MouseRect.left > m_iLowGray)
				{
					m_iUpGray = point.x - m_MouseRect.left;		// 更改下限
				}
				else
				{
					m_iUpGray = m_iLowGray + 1;	// 下限拖过上限，设置为下限＋1				
					point.x = m_MouseRect.left + m_iLowGray + 1;// 重设鼠标位置
				}
			}

			::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));

			UpdateData(FALSE);
			InvalidateRect(m_MouseRect, TRUE);
		}
		else if (point.x == (m_MouseRect.left + m_iLowGray) || point.x ==
			(m_MouseRect.left + m_iUpGray))
		{
			::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
		}
	}

	CDialog::OnMouseMove(nFlags, point);
}

void CIntensityDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// 当用户释放鼠标左键停止拖动
	if (m_iIsDraging != 0)
	{
		m_iIsDraging = 0;
	}

	CDialog::OnLButtonUp(nFlags, point);
}

void CIntensityDlg::OnPaint()
{
	CString str;
	LONG i;
	LONG lMaxCount = 0;			// 最大计数

	CPaintDC dc(this);

	CWnd* pWnd = GetDlgItem(IDC_COORD);
	CDC* pDC = pWnd->GetDC();
	pWnd->Invalidate();
	pWnd->UpdateWindow();
	pDC->Rectangle(0, 0, 330, 300);

	CPen* pPenRed = new CPen;						// 创建画笔对象	
	pPenRed->CreatePen(PS_SOLID, 1, RGB(255, 0, 0));	// 红色画笔	
	CPen* pPenBlue = new CPen;						// 创建画笔对象	
	pPenBlue->CreatePen(PS_SOLID, 1, RGB(0, 0, 255));	// 蓝色画笔	
	CPen* pPenGreen = new CPen;						// 创建画笔对象	
	pPenGreen->CreatePen(PS_DOT, 1, RGB(0, 255, 0));	// 绿色画笔

														// 选中当前红色画笔，并保存以前的画笔
	CGdiObject* pOldPen = pDC->SelectObject(pPenRed);
	pDC->MoveTo(10, 10);								// 绘制坐标轴	
	pDC->LineTo(10, 280);							// 垂直轴	
	pDC->LineTo(320, 280);							// 水平轴

													// 写X轴刻度值
	str.Format(_T("%d"), 0);
	pDC->TextOut(10, 283, str);
	str.Format(_T("%d"), 50);
	pDC->TextOut(60, 283, str);
	str.Format(_T("%d"), 100);
	pDC->TextOut(110, 283, str);
	str.Format(_T("%d"), 150);
	pDC->TextOut(160, 283, str);
	str.Format(_T("%d"), 200);
	pDC->TextOut(210, 283, str);
	str.Format(_T("%d"), 255);
	pDC->TextOut(265, 283, str);

	// 绘制X轴刻度
	for (i = 0; i < 256; i += 5)
	{
		if ((i & 1) == 0)
		{
			pDC->MoveTo(i + 10, 280);
			pDC->LineTo(i + 10, 284);
		}
		else
		{
			pDC->MoveTo(i + 10, 280);
			pDC->LineTo(i + 10, 282);
		}
	}

	// 绘制X轴箭头
	pDC->MoveTo(315, 275);
	pDC->LineTo(320, 280);
	pDC->LineTo(315, 285);

	// 绘制X轴箭头
	pDC->MoveTo(10, 10);
	pDC->LineTo(5, 15);
	pDC->MoveTo(10, 10);
	pDC->LineTo(15, 15);

	// 计算最大计数值
	for (i = m_iLowGray; i <= m_iUpGray; i++)
	{
		// 判断是否大于当前最大值
		if (m_lCount[i] > lMaxCount)
		{
			lMaxCount = m_lCount[i];
		}
	}

	// 输出最大计数值
	pDC->MoveTo(10, 25);
	pDC->LineTo(14, 25);
	str.Format(_T("%d"), lMaxCount);
	pDC->TextOut(11, 26, str);

	// 更改成绿色画笔
	pDC->SelectObject(pPenGreen);

	// 绘制窗口上下限
	pDC->MoveTo(m_iLowGray + 10, 25);
	pDC->LineTo(m_iLowGray + 10, 280);
	pDC->MoveTo(m_iUpGray + 10, 25);
	pDC->LineTo(m_iUpGray + 10, 280);

	// 更改成蓝色画笔
	pDC->SelectObject(pPenBlue);

	// 判断是否有计数
	if (lMaxCount > 0)
	{
		// 绘制直方图
		for (i = m_iLowGray; i <= m_iUpGray; i++)
		{
			pDC->MoveTo(i + 10, 280);
			pDC->LineTo(i + 10, 281 - (int)(m_lCount[i] * 256 / lMaxCount));
		}
	}

	// 恢复以前的画笔
	pDC->SelectObject(pOldPen);

	delete pPenRed;
	delete pPenBlue;
	delete pPenGreen;
}

void CIntensityDlg::OnOK()
{
	// 判断是否下限超过上限
	if (m_iLowGray > m_iUpGray)
	{
		int iTemp = m_iLowGray;
		m_iLowGray = m_iUpGray;
		m_iUpGray = iTemp;
	}

	CDialog::OnOK();
}

void CIntensityDlg::OnStnClickedCoord()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CIntensityDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}

