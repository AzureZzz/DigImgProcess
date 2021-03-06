#pragma once


// CIntensityDlg 对话框

class CIntensityDlg : public CDialog
{
	DECLARE_DYNAMIC(CIntensityDlg)

public:
	CIntensityDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CIntensityDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_INTENSITY };
#endif

	// Dialog Data
public:
	// 当前鼠标拖动状态，0表示未拖动，1表示正在拖动下限，2表示正在拖动上限。
	int		m_iIsDraging;
	CRect	m_MouseRect;	// 相应鼠标事件的矩形区域	
	LONG	m_lHeight;		// DIB的高度	
	LONG	m_lWidth;		// DIB的宽度	
	char *	m_lpDIBBits;	// 指向当前DIB象素的指针	
	LONG	m_lCount[256];	// 各个灰度值的计数
							//{{AFX_DATA(CIntensityDlg)
	enum { IDD = IDD_DLG_INTENSITY };
	int		m_iLowGray;		// 显示灰度区间的下限	
	int		m_iUpGray;		// 显示灰度区间的上限
							//}}AFX_DATA


							// Overrides
							// ClassWizard generated virtual function overrides
							//{{AFX_VIRTUAL(CIntensityDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support										
														//{{AFX_MSG(CIntensityDlg)
	afx_msg void OnPaint();
	afx_msg void OnKillfocusEDITLowGray();
	afx_msg void OnKillfocusEDITUpGray();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnStnClickedCoord();
public:
	afx_msg void OnBnClickedOk();
//	afx_msg void On32771();
};
