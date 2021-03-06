// SmoothDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "DigImgProcess.h"
#include "SmoothDlg.h"
#include "afxdialogex.h"


// CSmoothDlg 对话框

IMPLEMENT_DYNAMIC(CSmoothDlg, CDialog)

CSmoothDlg::CSmoothDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DLG_SMOOTH, pParent)
	, m_fTempC(0)
	, m_iTempH(0)
	, m_iTempW(0)
	, m_iTempMX(0)
	, m_iTempMY(0)
{
	//m_intType = -1;
	m_iTempH = 0;
	m_iTempW = 0;
	m_iTempMX = 0;
	m_iTempMY = 0;
	m_fTempC = 0.0f;
}

CSmoothDlg::~CSmoothDlg()
{
}

void CSmoothDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSmoothDlg)
	DDX_Radio(pDX, IDC_RAD1, m_intType);
	DDX_Text(pDX, IDC_EDIT_TEMPH, m_iTempH);
	DDV_MinMaxInt(pDX, m_iTempH, 2, 5);
	DDX_Text(pDX, IDC_EDIT_TEMPW, m_iTempW);
	DDV_MinMaxInt(pDX, m_iTempW, 2, 5);
	DDX_Text(pDX, IDC_EDIT_MX, m_iTempMX);
	DDX_Text(pDX, IDC_EDIT_MY, m_iTempMY);
	DDX_Text(pDX, IDC_EDIT_TEMPC, m_fTempC);
	DDX_Text(pDX, IDC_EDIT_V0, m_fpArray[0]);
	DDX_Text(pDX, IDC_EDIT_V1, m_fpArray[1]);
	DDX_Text(pDX, IDC_EDIT_V2, m_fpArray[2]);
	DDX_Text(pDX, IDC_EDIT_V3, m_fpArray[3]);
	DDX_Text(pDX, IDC_EDIT_V4, m_fpArray[4]);
	DDX_Text(pDX, IDC_EDIT_V5, m_fpArray[5]);
	DDX_Text(pDX, IDC_EDIT_V6, m_fpArray[6]);
	DDX_Text(pDX, IDC_EDIT_V7, m_fpArray[7]);
	DDX_Text(pDX, IDC_EDIT_V8, m_fpArray[8]);
	DDX_Text(pDX, IDC_EDIT_V9, m_fpArray[9]);
	DDX_Text(pDX, IDC_EDIT_V10, m_fpArray[10]);
	DDX_Text(pDX, IDC_EDIT_V11, m_fpArray[11]);
	DDX_Text(pDX, IDC_EDIT_V12, m_fpArray[12]);
	DDX_Text(pDX, IDC_EDIT_V13, m_fpArray[13]);
	DDX_Text(pDX, IDC_EDIT_V14, m_fpArray[14]);
	DDX_Text(pDX, IDC_EDIT_V15, m_fpArray[15]);
	DDX_Text(pDX, IDC_EDIT_V16, m_fpArray[16]);
	DDX_Text(pDX, IDC_EDIT_V17, m_fpArray[17]);
	DDX_Text(pDX, IDC_EDIT_V18, m_fpArray[18]);
	DDX_Text(pDX, IDC_EDIT_V19, m_fpArray[19]);
	DDX_Text(pDX, IDC_EDIT_V20, m_fpArray[20]);
	DDX_Text(pDX, IDC_EDIT_V21, m_fpArray[21]);
	DDX_Text(pDX, IDC_EDIT_V22, m_fpArray[22]);
	DDX_Text(pDX, IDC_EDIT_V23, m_fpArray[23]);
	DDX_Text(pDX, IDC_EDIT_V24, m_fpArray[24]);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSmoothDlg, CDialog)
	ON_BN_CLICKED(IDC_RAD1, &CSmoothDlg::OnRad1)
	ON_BN_CLICKED(IDC_RAD2, &CSmoothDlg::OnRad2)
	ON_BN_CLICKED(IDC_RAD3, &CSmoothDlg::OnRad3)
	ON_EN_CHANGE(IDC_EDIT_TEMPH, &CSmoothDlg::OnEnChangeEditTemph)
	ON_EN_KILLFOCUS(IDC_EDIT_TEMPH, &CSmoothDlg::OnEnKillfocusEditTemph)
	ON_EN_KILLFOCUS(IDC_EDIT_TEMPW, &CSmoothDlg::OnEnKillfocusEditTempw)
END_MESSAGE_MAP()


// CSmoothDlg 消息处理程序
void CSmoothDlg::OnRad1()
{
	// 3×3平均模板
	m_intType = 0;
	m_iTempH = 3;
	m_iTempW = 3;
	m_iTempMX = 1;
	m_iTempMY = 1;
	m_fTempC = (FLOAT)(1.0 / 9.0);

	// 设置模板元素
	m_fpArray[0] = 1.0;
	m_fpArray[1] = 1.0;
	m_fpArray[2] = 1.0;
	m_fpArray[3] = 0.0;
	m_fpArray[4] = 0.0;
	m_fpArray[5] = 1.0;
	m_fpArray[6] = 1.0;
	m_fpArray[7] = 1.0;
	m_fpArray[8] = 0.0;
	m_fpArray[9] = 0.0;
	m_fpArray[10] = 1.0;
	m_fpArray[11] = 1.0;
	m_fpArray[12] = 1.0;
	m_fpArray[13] = 0.0;
	m_fpArray[14] = 0.0;
	m_fpArray[15] = 0.0;
	m_fpArray[16] = 0.0;
	m_fpArray[17] = 0.0;
	m_fpArray[18] = 0.0;
	m_fpArray[19] = 0.0;
	m_fpArray[20] = 0.0;
	m_fpArray[21] = 0.0;
	m_fpArray[22] = 0.0;
	m_fpArray[23] = 0.0;
	m_fpArray[24] = 0.0;

	UpdateEdit();
	UpdateData(FALSE);
}

void CSmoothDlg::OnRad2()
{
	// 3×3高斯模板
	m_intType = 1;
	m_iTempH = 3;
	m_iTempW = 3;
	m_iTempMX = 1;
	m_iTempMY = 1;
	m_fTempC = (FLOAT)(1.0 / 16.0);

	// 设置模板元素
	m_fpArray[0] = 1.0;
	m_fpArray[1] = 2.0;
	m_fpArray[2] = 1.0;
	m_fpArray[3] = 0.0;
	m_fpArray[4] = 0.0;
	m_fpArray[5] = 2.0;
	m_fpArray[6] = 4.0;
	m_fpArray[7] = 2.0;
	m_fpArray[8] = 0.0;
	m_fpArray[9] = 0.0;
	m_fpArray[10] = 1.0;
	m_fpArray[11] = 2.0;
	m_fpArray[12] = 1.0;
	m_fpArray[13] = 0.0;
	m_fpArray[14] = 0.0;
	m_fpArray[15] = 0.0;
	m_fpArray[16] = 0.0;
	m_fpArray[17] = 0.0;
	m_fpArray[18] = 0.0;
	m_fpArray[19] = 0.0;
	m_fpArray[20] = 0.0;
	m_fpArray[21] = 0.0;
	m_fpArray[22] = 0.0;
	m_fpArray[23] = 0.0;
	m_fpArray[24] = 0.0;

	UpdateEdit();
	UpdateData(FALSE);
}

void CSmoothDlg::OnRad3()
{
	m_intType = 2;			// 自定义模板	
	UpdateEdit();			// 更新文本框状态	
}

void CSmoothDlg::OnEnChangeEditTemph()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}

void CSmoothDlg::OnOK()
{
	UpdateData(TRUE);

	// 判断设置是否有效
	if ((m_iTempMX < 0) || (m_iTempMX > m_iTempW - 1) ||
		(m_iTempMY < 0) || (m_iTempMY > m_iTempH - 1))
	{
		MessageBox(L"中心元素参数设置错误！", L"系统提示",
			MB_ICONINFORMATION | MB_OK);
		return;
	}
	// 更新模板元素数组（将有效元素放置在数组的前面）
	for (int i = 0; i < m_iTempH; i++)
	{
		for (int j = 0; j < m_iTempW; j++)
		{
			m_fpArray[i * m_iTempW + j] = m_fpArray[i * 5 + j];
		}
	}
	UpdateData(FALSE);

	CDialog::OnOK();
}

void CSmoothDlg::OnEnKillfocusEditTemph()
{
	UpdateData(TRUE);
	UpdateEdit();
}

void CSmoothDlg::OnEnKillfocusEditTempw()
{
	UpdateData(TRUE);
	UpdateEdit();
}


void CSmoothDlg::UpdateEdit()
{
	BOOL	bEnable;
	int		i, j;

	// 判断是不是自定义模板
	if (m_intType == 2)
	{
		bEnable = TRUE;
	}
	else
	{
		bEnable = FALSE;
	}

	(CEdit *)GetDlgItem(IDC_EDIT_TEMPH)->EnableWindow(bEnable);
	(CEdit *)GetDlgItem(IDC_EDIT_TEMPW)->EnableWindow(bEnable);
	(CEdit *)GetDlgItem(IDC_EDIT_TEMPC)->EnableWindow(bEnable);
	(CEdit *)GetDlgItem(IDC_EDIT_MX)->EnableWindow(bEnable);
	(CEdit *)GetDlgItem(IDC_EDIT_MY)->EnableWindow(bEnable);

	// IDC_EDIT_V0等ID其实是一个整数，它的数值定义在Resource.h中定义。

	for (i = IDC_EDIT_V0; i <= IDC_EDIT_V24; i++)
	{
		(CEdit *)GetDlgItem(i)->EnableWindow(bEnable);
	}
	for (i = 0; i < m_iTempH; i++)
	{
		for (j = 0; j < m_iTempW; j++)
		{
			(CEdit *)GetDlgItem(IDC_EDIT_V0 + i * 5 + j)->ShowWindow(SW_SHOW);
		}
	}
	for (i = 0; i < m_iTempH; i++)
	{
		for (j = m_iTempW; j < 5; j++)
		{
			(CEdit *)GetDlgItem(IDC_EDIT_V0 + i * 5 + j)->ShowWindow(SW_HIDE);
		}
	}
	for (i = m_iTempH; i < 5; i++)
	{
		for (j = 0; j < 5; j++)
		{
			(CEdit *)GetDlgItem(IDC_EDIT_V0 + i * 5 + j)->ShowWindow(SW_HIDE);
		}
	}
}
