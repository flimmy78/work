#if !defined(AFX_VIDEOCOMPRESSORS_H__7AD70BB2_C707_4D79_8E66_918EA2EE96B8__INCLUDED_)
#define AFX_VIDEOCOMPRESSORS_H__7AD70BB2_C707_4D79_8E66_918EA2EE96B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.

/////////////////////////////////////////////////////////////////////////////
// CVideoCompressors wrapper class

class CVideoCompressors : public COleDispatchDriver
{
public:
	CVideoCompressors() {}		// Calls COleDispatchDriver default constructor
	CVideoCompressors(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CVideoCompressors(const CVideoCompressors& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	short GetCount();
	void SetCount(short);

// Operations
public:
	short FindVideoCompressor(LPCTSTR strName);
	CString FindVideoCompressorName(short index);
	LPDISPATCH GetItem(const VARIANT& Key);
	BOOL ShowPropertyPage(short iIndex);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIDEOCOMPRESSORS_H__7AD70BB2_C707_4D79_8E66_918EA2EE96B8__INCLUDED_)
