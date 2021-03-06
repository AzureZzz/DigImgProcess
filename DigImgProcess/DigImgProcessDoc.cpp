
// DigImgProcessDoc.cpp: CDigImgProcessDoc 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "DigImgProcess.h"
#endif

#include "DigImgProcessDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CDigImgProcessDoc

IMPLEMENT_DYNCREATE(CDigImgProcessDoc, CDocument)

BEGIN_MESSAGE_MAP(CDigImgProcessDoc, CDocument)
END_MESSAGE_MAP()


// CDigImgProcessDoc 构造/析构

CDigImgProcessDoc::CDigImgProcessDoc()
{
	// 默认背景色，灰色
	m_refColorBKG = 0x00808080;

	// 初始化变量
	m_pDibImage = NULL;
	m_hDIB = NULL;
	m_palDIB = NULL;
	m_sizeDoc = CSize(1, 1);
}

CDigImgProcessDoc::~CDigImgProcessDoc()
{
	if (m_hDIB != NULL)
	{
		// 清除DIB对象
		::GlobalFree((HGLOBAL)m_hDIB);
	}

	// 判断调色板是否存在
	if (m_palDIB != NULL)
	{
		// 清除调色板
		delete m_palDIB;
		m_palDIB = NULL;
	}

	// 判断DibImage对象是否存在
	if (m_pDibImage != NULL)
	{
		// 清除DibImage对象
		delete m_pDibImage;
		m_pDibImage = NULL;
	}
}

BOOL CDigImgProcessDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}


// CDigImgProcessDoc 序列化

void CDigImgProcessDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}

#ifdef SHARED_HANDLERS

// 缩略图的支持
void CDigImgProcessDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改此代码以绘制文档数据
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 搜索处理程序的支持
void CDigImgProcessDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:     strSearchContent = _T("point;rectangle;circle;ole object;")；
	SetSearchContent(strSearchContent);
}

void CDigImgProcessDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CDigImgProcessDoc 诊断

#ifdef _DEBUG
void CDigImgProcessDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDigImgProcessDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CDigImgProcessDoc 命令

void CDigImgProcessDoc::ReplaceHDIB(HDIB hDIB)
{
	// 替换DIB，在功能粘贴中用到该函数	
	if (m_hDIB != NULL)
	{
		::GlobalFree((HGLOBAL)m_hDIB);
	}
	m_hDIB = hDIB;
}

void CDigImgProcessDoc::InitDIBData()
{
	// 初始化DIB对象

	if (m_palDIB != NULL)
	{
		delete m_palDIB;
		m_palDIB = NULL;
	}
	if (m_hDIB == NULL)
	{
		return;
	}

	LPSTR lpDIB = (LPSTR) ::GlobalLock((HGLOBAL)m_hDIB);

	// 判断图像是否过大
	if (m_pDibImage->DIBWidth(lpDIB) > INT_MAX || m_pDibImage->DIBHeight(lpDIB)
		> INT_MAX)
	{
		::GlobalUnlock((HGLOBAL)m_hDIB);
		::GlobalFree((HGLOBAL)m_hDIB);

		m_hDIB = NULL;

		CString strMsg;
		strMsg = "BMP图像太大！";
		MessageBox(NULL, strMsg, L"系统提示", MB_ICONINFORMATION | MB_OK);

		return;
	}

	m_sizeDoc = CSize((int)m_pDibImage->DIBWidth(lpDIB),
		(int)m_pDibImage->DIBHeight(lpDIB));

	::GlobalUnlock((HGLOBAL)m_hDIB);

	// 创建新调色板
	m_palDIB = new CPalette;
	if (m_palDIB == NULL)
	{
		::GlobalFree((HGLOBAL)m_hDIB);
		m_hDIB = NULL;

		return;
	}

	if (m_pDibImage->CreateDIBPalette(m_hDIB, m_palDIB) == NULL)
	{
		delete m_palDIB;
		m_palDIB = NULL;

		return;
	}
}

BOOL CDigImgProcessDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	CFile file;
	CFileException fe;

	if (!file.Open(lpszPathName, CFile::modeRead | CFile::shareDenyWrite, &fe))
	{
		ReportSaveLoadException(lpszPathName, &fe, FALSE,
			AFX_IDP_FAILED_TO_OPEN_DOC);

		return FALSE;
	}

	DeleteContents();

	BeginWaitCursor();

	if (m_pDibImage != NULL)
	{
		delete m_pDibImage;
		m_pDibImage = NULL;
	}
	m_pDibImage = new CDibImage;

	TRY
	{
		m_hDIB = m_pDibImage->ReadDIBFile(file);
	}
		CATCH(CFileException, eLoad)
	{
		file.Abort();
		EndWaitCursor();

		ReportSaveLoadException(lpszPathName, eLoad, FALSE,
			AFX_IDP_FAILED_TO_OPEN_DOC);

		m_hDIB = NULL;

		if (m_pDibImage != NULL)
		{
			delete m_pDibImage;
			m_pDibImage = NULL;
		}

		return FALSE;
	}
	END_CATCH

	InitDIBData();
	EndWaitCursor();

	// 判断读取文件是否成功
	if (m_hDIB == NULL)
	{
		CString strMsg;
		strMsg = "读取图像时出错！可能是不支持该类型的图像文件！";
		MessageBox(NULL, strMsg, L"系统提示", MB_ICONINFORMATION | MB_OK);

		if (m_pDibImage != NULL)
		{
			delete m_pDibImage;
			m_pDibImage = NULL;
		}

		return FALSE;
	}

	SetPathName(lpszPathName);			// 设置文件名称	
	SetModifiedFlag(FALSE);				// 初始化胀标记为FALSE

	return TRUE;
}

BOOL CDigImgProcessDoc::OnSaveDocument(LPCTSTR lpszPathName)
{

	CFile file;
	CFileException fe;

	if (!file.Open(lpszPathName, CFile::modeCreate | CFile::modeReadWrite
		| CFile::shareExclusive, &fe))
	{
		ReportSaveLoadException(lpszPathName, &fe, TRUE, AFX_IDP_INVALID_FILENAME);
		return FALSE;
	}

	BOOL bSuccess = FALSE;
	TRY
	{
		BeginWaitCursor();
	bSuccess = m_pDibImage->SaveDIB(m_hDIB, file);
	file.Close();
	}
		CATCH(CException, eSave)
	{
		file.Abort();
		EndWaitCursor();
		ReportSaveLoadException(lpszPathName, eSave, TRUE,
			AFX_IDP_FAILED_TO_SAVE_DOC);

		return FALSE;
	}
	END_CATCH

		EndWaitCursor();
	SetModifiedFlag(FALSE);

	if (!bSuccess)
	{
		// 保存失败，可能是其它格式的DIB，可以读取但是不能保存
		// 或者是SaveDIB函数有误
		CString strMsg;
		strMsg = "无法保存BMP图像！";
		MessageBox(NULL, strMsg, L"系统提示", MB_ICONINFORMATION | MB_OK);
	}

	return bSuccess;
}

void CDigImgProcessDoc::OnFileReopen()
{
	// 重新打开图像，放弃所有修改

	// 判断当前图像是否已经被改动
	if (IsModified())
	{
		if (MessageBox(NULL, L"重新打开图像将丢失所有改动！是否继续？",
			L"系统提示", MB_ICONQUESTION | MB_YESNO) == IDNO)
		{
			return;
		}
	}

	CFile file;
	CFileException fe;
	CString strPathName;
	strPathName = GetPathName();


	BeginWaitCursor();

	if (m_pDibImage != NULL)
	{
		delete m_pDibImage;
		m_pDibImage = NULL;
	}
	m_pDibImage = new CDibImage;

	TRY
	{
		m_hDIB = m_pDibImage->ReadDIBFile(file);
	}
	CATCH(CFileException, eLoad)
	{
		file.Abort();
		EndWaitCursor();

		ReportSaveLoadException(strPathName, eLoad, FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);

		m_hDIB = NULL;
		if (m_pDibImage != NULL)
		{
			delete m_pDibImage;
			m_pDibImage = NULL;
		}

		return;
	}
	END_CATCH

	InitDIBData();

	if (m_hDIB == NULL)
	{
		CString strMsg;
		strMsg = "读取图像时出错！可能是不支持该类型的图像文件！";
		MessageBox(NULL, strMsg, L"系统提示", MB_ICONINFORMATION | MB_OK);

		EndWaitCursor();

		if (m_pDibImage != NULL)
		{
			delete m_pDibImage;
			m_pDibImage = NULL;
		}

		return;
	}

	SetModifiedFlag(FALSE);
	UpdateAllViews(NULL);
	EndWaitCursor();

	return;
}
