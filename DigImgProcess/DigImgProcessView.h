
// DigImgProcessView.h: CDigImgProcessView 类的接口
//

#pragma once


class CDigImgProcessView : public CScrollView
{
protected: // 仅从序列化创建
	CDigImgProcessView();
	DECLARE_DYNCREATE(CDigImgProcessView)

// 特性
public:
	CDigImgProcessDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	virtual void CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType = adjustBorder);
// 实现
public:
	virtual ~CDigImgProcessView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg LRESULT OnDoRealize(WPARAM wParam, LPARAM lParam);  // user message
	afx_msg void OnViewIntensity();
	afx_msg void OnPointEqua();
	afx_msg void OnPointInvert();
	afx_msg void OnPointLiner();
	afx_msg void OnPointStre();
	afx_msg void OnPointThre();
	afx_msg void OnPointWind();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void On32771();
	afx_msg void OnFreqFour();
	afx_msg void OnEnhaGradsharp();
	afx_msg void OnEnhaSharp();
	afx_msg void OnEnhaSmooth();
};

#ifndef _DEBUG  // DigImgProcessView.cpp 中的调试版本
inline CDigImgProcessDoc* CDigImgProcessView::GetDocument() const
   { return reinterpret_cast<CDigImgProcessDoc*>(m_pDocument); }
#endif

