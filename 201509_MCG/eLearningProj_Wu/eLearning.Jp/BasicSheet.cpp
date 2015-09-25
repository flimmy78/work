// BasicSheet.cpp : implementation file
//

#include "stdafx.h"
#include "BasicSheet.h"
#include <afxpriv.h>
#include <afxdlgs.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WM_RESIZEPAGE WM_APP+1

enum { CDF_CENTER, CDF_TOPLEFT, CDF_NONE };

// helper function which sets the font for a window and all its children
// and also resizes everything according to the new font
void ChangeDialogFont(CWnd* pWnd, CFont* pFont, int nFlag)
{
	/*
	CRect windowRect;

	// grab old and new text metrics
	TEXTMETRIC tmOld, tmNew;
	CDC * pDC = pWnd->GetDC();
	CFont * pSavedFont = pDC->SelectObject(pWnd->GetFont());
	pDC->GetTextMetrics(&tmOld);
	pDC->SelectObject(pFont);
	pDC->GetTextMetrics(&tmNew);
	pDC->SelectObject(pSavedFont);
	pWnd->ReleaseDC(pDC);

	long oldHeight = tmOld.tmHeight+tmOld.tmExternalLeading;
	long newHeight = tmNew.tmHeight+tmNew.tmExternalLeading;

	if (nFlag != CDF_NONE)
	{
		// calculate new dialog window rectangle
		CRect clientRect, newClientRect, newWindowRect;

		pWnd->GetWindowRect(windowRect);
		pWnd->GetClientRect(clientRect);
		long xDiff = windowRect.Width() - clientRect.Width();
		long yDiff = windowRect.Height() - clientRect.Height();
	
		newClientRect.left = newClientRect.top = 0;
		newClientRect.right = clientRect.right * tmNew.tmAveCharWidth / tmOld.tmAveCharWidth;
		newClientRect.bottom = clientRect.bottom * newHeight / oldHeight;

		if (nFlag == CDF_TOPLEFT) // resize with origin at top/left of window
		{
			newWindowRect.left = windowRect.left;
			newWindowRect.top = windowRect.top;
			newWindowRect.right = windowRect.left + newClientRect.right + xDiff;
			newWindowRect.bottom = windowRect.top + newClientRect.bottom + yDiff;
		}
		else if (nFlag == CDF_CENTER) // resize with origin at center of window
		{
			newWindowRect.left = windowRect.left - 
							(newClientRect.right - clientRect.right)/2;
			newWindowRect.top = windowRect.top -
							(newClientRect.bottom - clientRect.bottom)/2;
			newWindowRect.right = newWindowRect.left + newClientRect.right + xDiff;
			newWindowRect.bottom = newWindowRect.top + newClientRect.bottom + yDiff;
		}
		//pWnd->MoveWindow(newWindowRect);
	}
*/
	pWnd->SetFont(pFont);

	// iterate through and move all child windows and change their font.
	CWnd* pChildWnd = pWnd->GetWindow(GW_CHILD);

	while (pChildWnd)
	{
		pChildWnd->SetFont(pFont);
//		pChildWnd->GetWindowRect(windowRect);

//		CString strClass;
//		::GetClassName(pChildWnd->m_hWnd, strClass.GetBufferSetLength(32), 31);
//		strClass.MakeUpper();
//		if(strClass==_T("COMBOBOX"))
//		{
//			CRect rect;
//			pChildWnd->SendMessage(CB_GETDROPPEDCONTROLRECT,0,(LPARAM) &rect);
//			windowRect.right = rect.right;
//			windowRect.bottom = rect.bottom;
//		}
//
//		pWnd->ScreenToClient(windowRect);
//		windowRect.left = windowRect.left * tmNew.tmAveCharWidth / tmOld.tmAveCharWidth;
//		windowRect.right = windowRect.right * tmNew.tmAveCharWidth / tmOld.tmAveCharWidth;
//		windowRect.top = windowRect.top * newHeight / oldHeight;
//		windowRect.bottom = windowRect.bottom * newHeight / oldHeight;
		//pChildWnd->MoveWindow(windowRect);
		
		pChildWnd = pChildWnd->GetWindow(GW_HWNDNEXT);
	}
}
/////////////////////////////////////////////////////////////////////////////
// CBasicSheet

IMPLEMENT_DYNAMIC(CBasicSheet, CPropertySheet)

CBasicSheet::CBasicSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	m_pFont = NULL;
}

CBasicSheet::CBasicSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	m_pFont = NULL;
}

CBasicSheet::~CBasicSheet()
{
	//if (m_fntPage.m_hObject)
	//	VERIFY (m_fntPage.DeleteObject ());
}

BEGIN_MESSAGE_MAP(CBasicSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CBasicSheet)
	//}}AFX_MSG_MAP
	//ON_MESSAGE (WM_RESIZEPAGE, OnResizePage)	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBasicSheet message handlers

void CBasicSheet::BuildPropPageArray()
{
	CPropertySheet::BuildPropPageArray();

/*
	// get first page
	CPropertyPage* pPage = GetPage (0);
	ASSERT (pPage);
	
	// dialog template class in afxpriv.h
	CDialogTemplate dlgtemp;
	// load the dialog template
	VERIFY (dlgtemp.Load (pPage->m_psp.pszTemplate));
	// get the font information
	CString strFace;
	WORD	wSize;
	VERIFY (dlgtemp.GetFont (strFace, wSize));
	if (m_fntPage.m_hObject)
		VERIFY (m_fntPage.DeleteObject ());
	// create a font using the info from first page
	VERIFY (m_fntPage.CreatePointFont (wSize*10, strFace));
*/
}

BOOL CBasicSheet::OnInitDialog() 
{
	CPropertySheet::OnInitDialog();

	// get the font for the first active page
	CPropertyPage* pPage = GetActivePage ();
	ASSERT (pPage);

	// change the font for the sheet
	ChangeDialogFont (this, m_pFont, CDF_CENTER);
	// change the font for each page
	for (int iCntr = 0; iCntr < GetPageCount (); iCntr++)
	{
		VERIFY (SetActivePage (iCntr));
		CPropertyPage* pPage = GetActivePage ();
		ASSERT (pPage);
		ChangeDialogFont (pPage, m_pFont, CDF_CENTER);
	}
	//Modify by angf
	if ( NULL != pPage )
	{
		VERIFY (SetActivePage (pPage));
	}

	// set and save the size of the page
	CTabCtrl* pTab = GetTabControl ();
	ASSERT (pTab);

	if (m_psh.dwFlags & PSH_WIZARD)
	{
		pTab->ShowWindow (SW_HIDE);
		GetClientRect (&m_rctPage);

		CWnd* pButton = GetDlgItem (ID_WIZBACK);
		ASSERT (pButton);
		CRect rc;
		pButton->GetWindowRect (&rc);
		ScreenToClient (&rc);
		m_rctPage.bottom = rc.top-2;
	}
	else
	{
		pTab->GetWindowRect (&m_rctPage);
		ScreenToClient (&m_rctPage);
		pTab->AdjustRect (FALSE, &m_rctPage);
	}

	// resize the page	
	//pPage->MoveWindow (&m_rctPage);

	return TRUE;
}

/*
BOOL CBasicSheet::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	NMHDR* pnmh = (LPNMHDR) lParam;

	// the sheet resizes the page whenever it is activated so we need to size it correctly
	if (TCN_SELCHANGE == pnmh->code)
		PostMessage (WM_RESIZEPAGE);
	
	return CPropertySheet::OnNotify(wParam, lParam, pResult);
}


LONG CBasicSheet::OnResizePage (UINT, LONG)
{
	// resize the page
	CPropertyPage* pPage = GetActivePage ();
	ASSERT (pPage);
	//pPage->MoveWindow (&m_rctPage);

	return 0;
}


BOOL CBasicSheet::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// the sheet resizes the page whenever the Apply button is clicked so we need to size it correctly
	if (ID_APPLY_NOW == wParam ||
		ID_WIZNEXT == wParam ||
		ID_WIZBACK == wParam)
		PostMessage (WM_RESIZEPAGE);
	
	return CPropertySheet::OnCommand(wParam, lParam);
}
*/