// DibImage.h: interface for the CDibImage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIBIMAGE_H__254F3D1E_BB20_40DA_AE07_E8E0219DFA8C__INCLUDED_)
#define AFX_DIBIMAGE_H__254F3D1E_BB20_40DA_AE07_E8E0219DFA8C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

DECLARE_HANDLE(HDIB);		// DIB句柄
#define PALVERSION   0x300	// DIB常量

/* DIB宏 */
// 判断是否是Win 3.0的DIB
#define IS_WIN30_DIB(lpbi)  ((*(LPDWORD)(lpbi)) == sizeof(BITMAPINFOHEADER))
// 计算矩形区域的宽度
#define RECTWIDTH(lpRect)     ((lpRect)->right - (lpRect)->left)
// 计算矩形区域的高度
#define RECTHEIGHT(lpRect)    ((lpRect)->bottom - (lpRect)->top)

// 在计算图像大小时，采用公式：biSizeImage = biWidth' × biHeight。
// 是biWidth'，而不是biWidth，这里的biWidth'必须是4的整倍数，表示
// 大于或等于biWidth的，离4最近的整倍数。WIDTHBYTES就是用来计算
// biWidth'
#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4)

// Dib文件头标志（字符串"BM"，写DIB时用到该常数）
#define DIB_HEADER_MARKER   ((WORD) ('M' << 8) | 'B')

// 图像几何变换函数//////////////////////////////////////////
// 常数π
#define PI 3.1415926535
//角度到弧度转化的宏
#define RADIAN(angle) ((angle)*PI/180.0) 
/////////////////////////////////////////////////////////////

// 图像正交变换函数//////////////////////////////////////////
#include <complex>
using namespace std;
/////////////////////////////////////////////////////////////

class CDibImage  
{
	// Constructor and Destructor ///////////////////////////////
public:
	CDibImage();
	virtual ~CDibImage();

	// function /////////////////////////////////////////////////
public:
	//  DIB(Independent Bitmap) 函数
	BOOL    PaintDIB (HDC, LPRECT, HDIB, LPRECT, CPalette* pPal);
	BOOL    CreateDIBPalette(HDIB hDIB, CPalette* cPal);
	LPSTR   FindDIBBits (LPSTR lpbi);
	DWORD   DIBWidth (LPSTR lpDIB);
	DWORD   DIBHeight (LPSTR lpDIB);
	WORD    PaletteSize (LPSTR lpbi);
	WORD    DIBNumColors (LPSTR lpbi);
	HGLOBAL CopyHandle (HGLOBAL h);
	BOOL    SaveDIB (HDIB hDib, CFile& file);
	HDIB    ReadDIBFile(CFile& file);

	//  图像点运算函数
	BOOL	LinerTrans(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, FLOAT fA, 
				FLOAT fB);
	BOOL	ThresholdTrans(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, BYTE bThre);
	BOOL	WindowTrans(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, BYTE bLow, 
				BYTE bUp);
	BOOL	GrayStretch(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, BYTE bX1, 
				BYTE bY1, BYTE bX2, BYTE bY2);
	BOOL	InteEqualize(LPSTR lpDIBBits, LONG lWidth, LONG lHeight);

	// 图像几何变换函数
	BOOL	TranslationDIB1 (LPSTR lpDIBBits, LONG lWidth, LONG lHeight, 
				LONG lXOffset, LONG lYOffset);
	BOOL	TranslationDIB (LPSTR lpDIBBits, LONG lWidth, LONG lHeight, 
				LONG lXOffset, LONG lYOffset);
	BOOL	MirrorDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, BOOL bDirection);
	BOOL	TransposeDIB(LPSTR lpbi);
	HGLOBAL ZoomDIB(LPSTR lpbi, float fXZoomRatio, float fYZoomRatio);
	HGLOBAL RotateDIB(LPSTR lpbi, int iRotateAngle);
	HGLOBAL RotateDIB2(LPSTR lpbi, int iRotateAngle);
	unsigned char Interpolation (LPSTR lpDIBBits, LONG lWidth, LONG lHeight, 
				FLOAT x, FLOAT y);

	// 图像正交变换函数
	VOID FFT(complex<double> * TD, complex<double> * FD, int r);
	VOID IFFT(complex<double> * FD, complex<double> * TD, int r);
	VOID DCT(double *f, double *F, int power);
	VOID IDCT(double *F, double *f, int power);
	VOID WALSH(double *f, double *F, int r);
	VOID IWALSH(double *F, double *f, int r);
	
	BOOL Fourier(LPSTR lpDIBBits, LONG lWidth, LONG lHeight);
	BOOL DIBDct(LPSTR lpDIBBits, LONG lWidth, LONG lHeight);
	BOOL DIBWalsh(LPSTR lpDIBBits, LONG lWidth, LONG lHeight);
	BOOL DIBWalsh1(LPSTR lpDIBBits, LONG lWidth, LONG lHeight);

	// 图像模板变换函数
	BOOL Template(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, 
		int iTempH, int iTempW, 
		int iTempMX, int iTempMY,
		FLOAT * fpArray, FLOAT fCoef);
	BOOL MedianFilter(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, 
		int iFilterH, int iFilterW, 
		int iFilterMX, int iFilterMY);
	unsigned char GetMedianNum(unsigned char * bArray, int iFilterLen);
	BOOL GradSharp(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, BYTE bThre);
	BOOL ReplaceColorPal(LPSTR lpDIB, BYTE * bpColorsTable);
};

#endif // !defined(AFX_DIBIMAGE_H__254F3D1E_BB20_40DA_AE07_E8E0219DFA8C__INCLUDED_)
