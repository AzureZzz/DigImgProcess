#pragma once


// CSharpThreDlg 对话框

class CSharpThreDlg : public CDialog
{
	DECLARE_DYNAMIC(CSharpThreDlg)

public:
	CSharpThreDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CSharpThreDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_THRE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	BYTE m_bThre;
};
