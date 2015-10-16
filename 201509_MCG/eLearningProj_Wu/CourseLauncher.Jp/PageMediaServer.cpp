// PageMediaServer.cpp : implementation file
//

#include "stdafx.h"
#include "CourseLoader.h"
#include "PageMediaServer.h"
#include "cmkapi.h"
#include "EncodeApi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageMediaServer property page

IMPLEMENT_DYNCREATE(CPageMediaServer, CPropertyPage)

CPageMediaServer::CPageMediaServer() : CPropertyPage(CPageMediaServer::IDD)
{
	//{{AFX_DATA_INIT(CPageMediaServer)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_strPreLogin = "";
	m_strPreIP = "";
	m_strPrePWD = "";
	m_strSubDir = "";
	m_strAlias = "";

	m_psp.dwFlags &= ~PSP_HASHELP;
}

CPageMediaServer::~CPageMediaServer()
{
}

void CPageMediaServer::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageMediaServer)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageMediaServer, CPropertyPage)
	//{{AFX_MSG_MAP(CPageMediaServer)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageMediaServer message handlers

BOOL CPageMediaServer::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	GetDlgItem(IDC_EDIT_PC)->SetWindowText("/Contents Name");
	GetDlgItem(IDC_EDIT_MOBILE)->SetWindowText("/Contents Name");

	char szBuf[256];
	CString strIniPath = ::GetSysPath() + "Launcher.ini";

	::GetPrivateProfileString("MediaServer", "IP", "0.0.0.0", szBuf, 256, strIniPath);
	GetDlgItem(IDC_EDIT_MSRV_IP)->SetWindowText(szBuf);
	m_strPreIP = szBuf;

	::GetPrivateProfileString("MediaServer", "LoginName", "", szBuf, 256, strIniPath);
	GetDlgItem(IDC_EDIT_MSRV_LOGIN_NAME)->SetWindowText(szBuf);
	m_strPreLogin = szBuf;

	::GetPrivateProfileString("MediaServer", "Password", "", szBuf, 256, strIniPath);
	char szPassword[256];
	KeyDecode(szBuf, szPassword);
	GetDlgItem(IDC_EDIT_MSRV_PASSWORD)->SetWindowText(szPassword);
	m_strPrePWD = szPassword;
	
	::GetPrivateProfileString("MediaServer", "SubDir", "", szBuf, 256, strIniPath);
	GetDlgItem(IDC_EDIT_MSRV_DIRECTORY)->SetWindowText(szBuf);
	m_strSubDir = szBuf;

	//add by buxiangyi
	CStringArray strArray;
	CString strPath, m_LayoutName;
	strPath = ::GetSysPath() + "Template\\js\\config.js";
	::LoadFile(strPath, strArray);
	for (int i = 0; i < strArray.GetSize(); i++)
	{
		CString strItem = strArray[i];
		strItem.TrimLeft();
		if (strItem.Find("var url") >= 0)
		{
			int BeginIndex = strItem.Find("\"");
			int EndIndex = strItem.Find(";");
			m_LayoutName = strItem.Mid(BeginIndex +1, EndIndex - BeginIndex - 2);
		}
	}
	GetDlgItem(IDC_EDIT_MSRV_PC)->SetWindowText(m_LayoutName);

	::LoadFile(strPath, strArray);
	for (int i = 0; i < strArray.GetSize(); i++)
	{
		CString strItem = strArray[i];
		strItem.TrimLeft();
		if (strItem.Find("var surl") >= 0)
		{
			int BeginIndex = strItem.Find("\"");
			int EndIndex = strItem.Find(";");
			m_LayoutName = strItem.Mid(BeginIndex +1, EndIndex - BeginIndex - 2);
		}
	}
	GetDlgItem(IDC_EDIT_MSRV_MOBILE)->SetWindowText(m_LayoutName);
	/*::GetPrivateProfileString("MediaServer", "Alias", "", szBuf, 256, strIniPath);
	GetDlgItem(IDC_EDIT_MSRV_ALIAS)->SetWindowText(szBuf);
	m_strAlias = szBuf;*/

	::GetPrivateProfileString("MediaServer", "Port", "21", szBuf, 256, strIniPath);
	GetDlgItem(IDC_EDIT_MSRV_PORT)->SetWindowText(szBuf);
	m_strPort = szBuf;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPageMediaServer::OnOK() 
{
	CString strText;
	CString strIniPath = ::GetSysPath() + "Launcher.ini";;

	GetDlgItem(IDC_EDIT_MSRV_IP)->GetWindowText(strText);
	ConvBackSlashToSlash(strText);
	::WritePrivateProfileString("MediaServer", "IP", strText, strIniPath);
	
	GetDlgItem(IDC_EDIT_MSRV_LOGIN_NAME)->GetWindowText(strText);
	::WritePrivateProfileString("MediaServer", "LoginName", strText, strIniPath);

	GetDlgItem(IDC_EDIT_MSRV_PASSWORD)->GetWindowText(strText);
	char szPassword[256];
	KeyEncode(strText, szPassword);
	::WritePrivateProfileString("MediaServer", "Password", szPassword, strIniPath);
	
	GetDlgItem(IDC_EDIT_MSRV_DIRECTORY)->GetWindowText(strText);
	ConvBackSlashToSlash(strText);
	::WritePrivateProfileString("MediaServer", "SubDir", strText, strIniPath);

	GetDlgItem(IDC_EDIT_MSRV_PC)->GetWindowText(strText);
	ConvBackSlashToSlash(strText);
	//add by buxiangyi
	CString ConfigJsPath;
	ConfigJsPath = ::GetSysPath() + "Template\\js";
	CStringArray m_array, m_oldaray;
	m_oldaray.Add("var url = "); 
	m_array.Add("var url = \"" + strText + "\";");

	GetDlgItem(IDC_EDIT_MSRV_MOBILE)->GetWindowText(strText);
	ConvBackSlashToSlash(strText);
	m_oldaray.Add("var surl = "); 
	m_array.Add("var surl = \"" + strText + "\";");
	UpdateFileByTag(ConfigJsPath, "config.js", m_oldaray, m_array, TRUE);
	//::WritePrivateProfileString("MediaServer", "Alias", strText, strIniPath);

	GetDlgItem(IDC_EDIT_MSRV_PORT)->GetWindowText(strText);
	::WritePrivateProfileString("MediaServer", "Port", strText, strIniPath);

	CPropertyPage::OnOK();
}

void CPageMediaServer::ConvBackSlashToSlash(CString& strInput)
{
	for (int i = 0; i < strInput.GetLength(); i++)
		if (strInput[i] == '\\')
			strInput.SetAt(i, '/');
}