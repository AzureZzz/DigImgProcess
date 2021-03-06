#pragma once


// CSmoothDlg 对话框

class CSmoothDlg : public CDialog
{
	DECLARE_DYNAMIC(CSmoothDlg)


public:
	FLOAT * m_fpArray;		// 模板元素数组指针
	int		m_intType;		// 模板类型	


public:
	void UpdateEdit();

public:
	CSmoothDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSmoothDlg();

	// 对话框数据
	enum { IDD = IDD_DLG_SMOOTH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	float m_fTempC;
public:
	int m_iTempH;
public:
	int m_iTempW;
public:
	int m_iTempMX;
public:
	int m_iTempMY;
public:
	afx_msg void OnRad1();
public:
	afx_msg void OnRad2();
public:
	afx_msg void OnRad3();
public:
	afx_msg void OnEnChangeEditTemph();
protected:
	virtual void OnOK();
public:
	afx_msg void OnEnKillfocusEditTemph();
public:
	afx_msg void OnEnKillfocusEditTempw();
};
