
// DigImgProcessDoc.h: CDigImgProcessDoc 类的接口
//


#pragma once
#include "DibImage.h"

class CDigImgProcessDoc : public CDocument
{
protected: // 仅从序列化创建
	CDigImgProcessDoc();
	DECLARE_DYNCREATE(CDigImgProcessDoc)

// 特性
public:
	HDIB GetHDIB() const
	{
		return m_hDIB;
	}
	CPalette* GetDocPalette() const
	{
		return m_palDIB;
	}
	CSize GetDocSize() const
	{
		return m_sizeDoc;
	}
	CDibImage* GetDibImage() const
	{
		return m_pDibImage;
	}
// 操作
public:

// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 实现
public:
	virtual ~CDigImgProcessDoc();
	void ReplaceHDIB(HDIB hDIB);
	void InitDIBData();
	COLORREF m_refColorBKG;		// 背景色
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CDibImage * m_pDibImage;
	HDIB m_hDIB;
	CPalette* m_palDIB;
	CSize m_sizeDoc;

// 生成的消息映射函数
protected:
	afx_msg void OnFileReopen();
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 用于为搜索处理程序设置搜索内容的 Helper 函数
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
