// SharpThreDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "DigImgProcess.h"
#include "SharpThreDlg.h"
#include "afxdialogex.h"


// CSharpThreDlg 对话框

IMPLEMENT_DYNAMIC(CSharpThreDlg, CDialog)

CSharpThreDlg::CSharpThreDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DLG_THRE, pParent)
	, m_bThre(0)
{

}

CSharpThreDlg::~CSharpThreDlg()
{
}

void CSharpThreDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_Thre, m_bThre);
}


BEGIN_MESSAGE_MAP(CSharpThreDlg, CDialog)
END_MESSAGE_MAP()


// CSharpThreDlg 消息处理程序
