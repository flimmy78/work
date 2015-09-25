#include "stdafx.h"
#include "ppt15ctrl.h"
#include "FileApi.h"		// add itoh 2013/07/25 PPT2013
#include "macro.h"			//add 2014/7/23 kagata BugNo.131
#include "Elearning.h"		//add 2014/7/23 kagata BugNo.131
#include "cmkapi.h"			//add 2014/7/23 kagata BugNo.131
//#include "resource.h"
//#include "cmkapi.h"

CPPT15Ctrl::CPPT15Ctrl()
{
	m_hWndPPT = NULL;
}

CPPT15Ctrl::~CPPT15Ctrl()
{
}

void CPPT15Ctrl::Close(BOOL bQuit) 
{
	// Check to see whether we've started the server.
	if(m_app.m_lpDispatch == NULL)
		return;

	if (bQuit)
		m_app.Quit();
	m_app.ReleaseDispatch();	
}

BOOL CPPT15Ctrl::Create()
{
	COleException e;
	if (!m_app.CreateDispatch("Powerpoint.Application", &e)) 
		return FALSE;
	
	m_slideshowwindows = m_app.GetSlideShowWindows();
	m_presentations = m_app.GetPresentations();
	
	return TRUE;
}

BOOL CPPT15Ctrl::OpenPresToWin(CString strPath, HWND hWnd, int nWidth, int nHeight)
{
	m_presentation = m_presentations.Open(strPath,      
										 (long)0,        //Read-only
										 (long)0,         //Untitled
										 (long)0          //WithWindow
										 );

	m_slides = m_presentation.GetSlides();
//	_Slide15 slide = m_slides.Add(1, 1);	// Show the first slide of the presentation
	_Slide15 slide = m_slides.AddSlide(1, slide.GetCustomLayout());	// Show the first slide of the presentation

	if (!GetRunSlideAndGetPPTHandle())
		return FALSE;

	//HWND hWndPpt = ::FindWindow ("screenClass", NULL);
	DWORD dwStyle = ::GetWindowLong(m_hWndPPT, GWL_STYLE);
	DWORD dwNewStyle = (dwStyle & ~WS_POPUP) | WS_CHILD;
	::SetWindowLong(m_hWndPPT, GWL_STYLE, dwNewStyle);
	::SetParent(m_hWndPPT,  hWnd);

	m_slideshowwindow = m_presentation.GetSlideShowWindow();
	m_slideshowwindow.SetTop(0);
	m_slideshowwindow.SetLeft(0);
	m_pView = m_slideshowwindow.GetView();
	m_pView.SetPointerType(1);    //ppSlideShowPointerArrow

	CRect rc;
	
	m_slideshowwindow.SetWidth((float)nWidth);
	m_slideshowwindow.SetHeight((float)nHeight);
	::GetWindowRect(m_hWndPPT, rc);

	int nRealWidth = rc.Width();
	int nRealHeight = rc.Height();
	double dRatio;
	if (nRealWidth != nWidth)
	{
		dRatio = (double)nWidth / nRealWidth;

		m_slideshowwindow.SetWidth((float)(nWidth * dRatio));
		m_slideshowwindow.SetHeight((float)(nHeight * dRatio));
		::GetWindowRect(m_hWndPPT, rc);
	}

//CString strMsg;
//strMsg.Format("Width = %d, Height = %d", rc.Width(), rc.Height());
//AfxMessageBox(strMsg);
			  
	POINT pt;
	pt.x = rc.left;
	pt.y = rc.top;
	::ScreenToClient(::GetParent(hWnd), &pt);
	::MoveWindow(hWnd, pt.x, pt.y, rc.Width() + 4, rc.Height() + 4, FALSE);

	slide.Delete();

	return TRUE;
}

BOOL CPPT15Ctrl::AppendSlide()
{
//	_Slide15 slide = m_slides.Add(m_slides.GetCount() + 1, 1);	
	_Slide15 slide = m_slides.AddSlide(m_slides.GetCount() + 1, slide.GetCustomLayout());	
	return TRUE;
}

BOOL CPPT15Ctrl::GetRunSlideAndGetPPTHandle()
{
	HWND hWndArray[10];
	int i = 0;
	for (i = 0; i < 10; i++)
		hWndArray[i] = NULL;
	EnumWindows(EnumWindowsProc, (LPARAM)hWndArray);

	//Run the show
	m_slideshow = m_presentation.GetSlideShowSettings();
	m_slideshow.Run();

	HWND hNewWndArray[10];
	for (i = 0; i < 10; i++)
		hNewWndArray[i] = NULL;
	EnumWindows(EnumWindowsProc, (LPARAM)hNewWndArray);
	
	HWND hWndPpt = NULL;
	for (i = 0; i < 10; i++)
	{
		HWND hNewWnd = hNewWndArray[i];
		if (hNewWnd == NULL)
			continue;
		BOOL bFound = FALSE;
		HWND hWnd;
		for (int j = 0; j < 10; j++)
		{
			hWnd = hWndArray[j];
			if (hWnd == NULL)
				continue;
			if (hWnd == hNewWnd)
			{
				bFound = TRUE;
				break;
			}
		}
		if (!bFound)
		{
			hWndPpt = hNewWnd;
			break;	
		}
	}

	m_hWndPPT = hWndPpt;
	if (!hWndPpt)
		return FALSE;
	::SetWindowText(m_hWndPPT, "photon:ppt:controler");
	//::SetWindowPos(m_hWndPPT, HWND_TOP, 0, 0, 0, 0, 
	//			              SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW);
	//::BringWindowToTop(m_hWndPPT);
	//::SetForegroundWindow(m_hWndPPT);

	return TRUE;
}

BOOL CALLBACK CPPT15Ctrl::EnumWindowsProc(HWND hWnd, LPARAM lParam)
{
	TCHAR szClassName[256];
	GetClassName(hWnd, szClassName, 256);
	if (lstrcmp(szClassName, "screenClass") != 0)
		return TRUE;

	HWND* phWndArray = (HWND*)lParam;
	for (int i = 0; i <10; i++)
	{
		if (phWndArray[i] == NULL)
		{
			phWndArray[i] = hWnd;
			break;
		}
	}

	return TRUE;
}

BOOL CPPT15Ctrl::OpenPPT(CString strPath)
{
	m_app.Activate();
    m_app.SetVisible((long)TRUE);
    m_app.SetWindowState((long) 2); 

	m_presentation = m_presentations.Open(strPath,      
										 (long)-1,         //Read-only     TRUE
										 (long)0,          //Untitled      FALSE
										 (long)0          //WithWindow    FALSE		//replase 2014/7/28 kagata chiba
										 //(long)-1          //WithWindow    TRUE
										 );
	m_slides = m_presentation.GetSlides();

	return TRUE;
}

BOOL CPPT15Ctrl::OpenPresentation(CString strPath)
{
	m_app.Activate();
    m_app.SetVisible((long)TRUE);
    m_app.SetWindowState((long) 1);                       //有時候,最小化PowerPoint會無法產生課程, why?
	m_presentation = m_presentations.Open(strPath,      
										 (long)-1,        //Read-only
										 (long)0,         //Untitled
										 (long)0          //WithWindow
										 );
	m_slides = m_presentation.GetSlides();

	if (!GetRunSlideAndGetPPTHandle())
		return FALSE;

	m_slideshowwindow = m_presentation.GetSlideShowWindow();
	m_pView = m_slideshowwindow.GetView();
	m_pView.SetPointerType(1);    //ppSlideShowPointerArrow

	return TRUE;
}

void CPPT15Ctrl::ShowPPT(BOOL bShow)
{
	if (m_hWndPPT)
	{
		::ShowWindow(m_hWndPPT, (bShow) ? SW_SHOW : SW_HIDE);
		if (bShow)
		{
		    m_app.SetWindowState((long) 1);                         //有時候,最小化PowerPoint會無法產生課程,故還��E

			BringWindowToTop(m_hWndPPT);
			SetForegroundWindow(m_hWndPPT);
		}
		else
		{
		    m_app.SetWindowState((long) 2);                         //最小化方便錄製 
		}
	}
}

void CPPT15Ctrl::ClosePresentation(BOOL bQuit) 
{
	// Check to see whether we've started the server.
	if(m_app.m_lpDispatch == NULL)
		return;

	if (m_presentation.m_lpDispatch)
		m_presentation.Close();
	
	if (bQuit)
		m_app.Quit();

	m_app.ReleaseDispatch();	
}

BOOL CPPT15Ctrl::SaveAsJPG(CString strDir)
{
	m_presentation.SaveAs(strDir, 17, 0);
	return TRUE;
}

//BOOL CPPT15Ctrl::WebPublish(CString strDir)
BOOL CPPT15Ctrl::WebPublish(CString strPath, CString strPPTPath, CString strDir)
{
//	WebOptions15 wo;
//	wo = m_presentation.GetWebOptions();
//	wo.SetResizeGraphics(1);
//	wo.SetIncludeNavigation(1);
//	wo.SetShowSlideAnimation(-1);
   // wo.SetEncoding(65001); //Utf8 encoding
//	PublishObjects15 pos;
//	pos = m_presentation.GetPublishObjects();

//	PublishObject15 po = pos.Item(1);
//	po.SetFileName(strDir);
//	po.SetSourceType(1);
//	po.Publish();	// del itoh 2013/07/04 PPT2013

	// add kagata 2013/12/05 BugNo.94
	CString strTmpPath = strPath + "temp\\";
	// add itoh 2013/07/04 PPT2013
	CString strExePath = strPath + "bin\\cnv\\SlideGoConverter.exe";
	// replase kagata 2013/12/05 BugNo.94
	//CString strArgs = " -i \"" + strPPTPath + "\" -o \"" + strDir + "html\" -s -temp \"" + strTmpPath + "\"";
	// replase kagata 2014/7/23 BugNo.131
	// replase kagata 2014/8/7 BugNo.131
	//CString strArgs = " -i \"" + strPPTPath + "\" -o \"" + strDir + "html\" -s -temp \"" + strTmpPath + "\" -thumb1 THUMB1 -thumbname1 Slide -w1 1024 -h1 768 -thumbaspect h";
	CString strArgs = " -i \"" + strPPTPath + "\" -o \"" + strDir + "html\" -temp \"" + strTmpPath + "\" -thumb1 THUMB1 -thumbname1 Slide -w1 1024 -h1 768 -thumbaspect h";
	::WinExec(strExePath + strArgs, SW_SHOWNORMAL);

/*
	CFile cf;
	char strRead[32];
	for(int i = 0;i < 3600;i++) { // add itoh 2013/07/05 PPT2013 output wait 30min
		::Sleep(500);
		if(cf.Open (strDir + "html\\convert.log", CFile::modeRead) != 0) {
			cf.Seek(-11, CFile::end);
			cf.Read(strRead, 9);
			if(strncmp(strRead,"Completed",9) == 0) {
				cf.Close();
				break;
			}
			cf.Close();
		}
	}
	// add end
*/
	FILE *stream_in;
	int iBreak = 0;
	char* pline1 = new char[4096];
	char* pline2 = new char[4096];
	int i = 0;
	for(i = 0;i < 900;i++) { // add itoh 2013/07/05 PPT2013 output wait 30min
		::Sleep(2000);
		PumpMessage();
		if ((stream_in = fopen(strDir + "html\\convert.log", "r" )) != NULL) {
			for(;;) {
				PumpMessage();
				if (fgets(pline1, 4096, stream_in) != NULL) {
					if(strncmp(pline1, "Encounter error", 15) != 0) {
						if(strstr(pline1, "Completed") != 0) {
							fclose(stream_in);
							iBreak = 1;
							break;
						}
						if(strncmp(pline1, "Processing slide ", 17) == 0) {
							strcpy(pline2, pline1);
						}
					} else {
						char* pline3 = new char[256];
						if(strncmp(pline2, "Processing slide ", 17) == 0) {
							strcpy(pline3, "�X���C�h���������s���������B\n�G���[�X���C�h�����i");
							strncat(pline3, &pline2[17], strlen(&pline2[17]) - 4);
							strcat(pline3, "�j\n�G���[�����������������髏n���������������������B");
						} else {
							strcpy(pline3, "�X���C�h���������s���������B\n�G���[�����������������髏n���������������������B");
						}
						AfxMessageBox(pline3);
						fclose(stream_in);
						delete [] pline3;
						iBreak = 1;
						break;
					}
				} else {
					break;
				}
			}
			if(iBreak == 1) break;
			fclose(stream_in);
		}
	}
	delete [] pline2;
	delete [] pline1;


	// add itoh 2013/07/25 PPT2013 Slide Count
//	FILE *stream_in;
	if ((stream_in = fopen(strDir + "html\\convert.log", "r" )) == NULL)
		return FALSE;

	char* pline = new char[65535];
	char *slideNo; 
	int iSlideNo;
	for(i = 0;;i++) {
		PumpMessage();
		if (fgets(pline, 4096, stream_in) == NULL)
		{
			delete [] pline;
			fclose(stream_in);
			return FALSE;
		}
		if(strncmp(pline,"Processing slide ",17) == 0) {
			slideNo = strstr(pline,"of ");
			iSlideNo = atoi(&slideNo[3]);
			break;
		}
	}
	fclose(stream_in);


	delete [] pline;

	SetThumb(strDir, iSlideNo);

	::DeleteFile(strDir + "html\\convert.log");			// delete convert.log

	::DeleteFile(strDir + "html\\Slide.html");
	rename(strDir + "html\\index.html", strDir + "html\\Slide.html");

	::DeleteFile(strDir + "html\\iframe_sample.html");	// delete iframe_sample.html
	// add end itoh
	// added by angf 2015/01/05
	::DeleteFile(strDir + "html\\thumbnail.jpg");
	::DeleteFile(strDir + "html\\thumb.png");
	::DeleteFile(strDir + "html\\note.png");
	::DeleteFile(strDir + "html\\close.png");
	deleteDirectory(strDir + "html\\assets");
	// add end angf

	return TRUE;
}

BOOL CPPT15Ctrl::PrevSlide()
{
	m_slideshowwindow.Activate();
	m_pView.Previous();
	return TRUE;
}

BOOL CPPT15Ctrl::NextSlide()
{
	m_slideshowwindow.Activate();
	m_pView.Next();
	return TRUE;
}

BOOL CPPT15Ctrl::GotoSlide(int n)
{
	if (n <= 0 || n > GetSlideCount())
		return FALSE;

	m_pView.GotoSlide(n, 0);
	return TRUE;
}

int CPPT15Ctrl::GetCurrentSlideNumber()
{
	//_Slide slide = m_pView.GetSlide();
	//return slide.GetSlideNumber();
	return m_pView.GetCurrentShowPosition();
}

int CPPT15Ctrl::GetCurrentAnimaNumber()
{
	return m_pView.GetClickIndex();
}

BOOL CPPT15Ctrl::FirstSlide()
{
	GotoSlide(1);
	return TRUE;
}

BOOL CPPT15Ctrl::LastSlide()
{
	GotoSlide(GetSlideCount());
	return TRUE;
}

int CPPT15Ctrl::GetSlideCount()
{
	return m_slides.GetCount();
}

CRect CPPT15Ctrl::GetSlideSize()
{
	PageSetup15 pagesetup = m_presentation.GetPageSetup();

	int r = pagesetup.GetSlideWidth();
	int b = pagesetup.GetSlideHeight();
	float zoom = zoomp(r, b);
	float rx = zoom * r;
	float bx = zoom * b; 
	CRect size;

	size.left = 0;
	size.top = 0;
	size.right = rx;
	size.bottom = bx;

	return size;
}

BOOL CPPT15Ctrl::GetNote()
{
	_Slide15 l_Slide;
	CSlideRange l_SlideRange;
	Shapes15 l_Shapes;
	CPlaceholders l_Placeholders;
	TextFrame15 l_TextFrame;
	TextRange15 l_TextRange;
	CString l_Text;
	
	l_Slide = m_pView.GetSlide();
	l_SlideRange = l_Slide.GetNotesPage();
	l_Shapes = l_SlideRange.get_Shapes();
	l_Placeholders = l_Shapes.GetPlaceholders();
	l_TextFrame = l_Placeholders.Item(2);
	l_TextRange = l_TextFrame.GetTextRange();
	l_Text = l_TextRange.GetText();
	
	return TRUE;
}

//�g�����Z�o���� �������g����
float CPPT15Ctrl::zoomp(float ssizex, float ssizey) {
    float qx = 0;
    float qy = 0;

    qx = GetSystemMetrics(SM_CXSCREEN) / ssizex;
    qy = GetSystemMetrics(SM_CYSCREEN) / ssizey;

    if (qx >= 1 && qy >= 1) {
        if (qx <= qy) {
            return qx;
        } else {
            return qy;
        }
    } else if (qx < 1 && qy < 1) {
        if (qx >= qy) {
            return qy;
        } else {
            return qx;
        }
    } else {
        if (qx >= qy) {
            return qy;
        } else {
            return qx;
        }
    }
}

BOOL CPPT15Ctrl::SaveMedia(CString strDir)
{
	CStringArray strArrayContent;
	PageSetup15 ps = m_presentation.GetPageSetup();
	int nWidth = (int)ps.GetSlideWidth();
	int nHeight = (int)ps.GetSlideHeight();
	CString strTmp;
	strTmp.Format("%d %d", nWidth, nHeight);
	strArrayContent.Add(strTmp);

	BOOL bHasMedia = FALSE;
	int nSlideCount = GetSlideCount();
	CString strSlide;
	for (int i = 1; i <= nSlideCount; i++)
	{
		_Slide15 slide = m_slides.Item(COleVariant((long)i));
		Shapes15 shapes = slide.GetShapes();
		int nShapeCount = shapes.GetCount();
		strSlide.Format("%d", i);
		bHasMedia = FALSE;
		for (int j = 1; j <= nShapeCount; j++)
		{
			Shape15 shape = shapes.Item(COleVariant((long)j));
			if (!shape.GetHasTextFrame())
			{
				long nType = shape.GetType();
				if (nType == 16)    //msoMedia
				{
					long lType = shape.GetMediaType();
					if (lType == 2 || lType == 3)   //ppMediaTypeMovie || ppMediaTypeSound  
					{
						bHasMedia = TRUE;
						int nLeft = (int)shape.GetLeft();
						int nTop = (int)shape.GetTop();
						int nWidth = (int)shape.GetWidth();
						int nHeight = (int)shape.GetHeight();
						strTmp.Format("%d %d %d %d", nLeft, nTop, nWidth, nHeight);
						strSlide = strSlide + " " + strTmp;
					}
				}
			}
		}

		if (bHasMedia)
			strArrayContent.Add(strSlide);
	}

	if (strDir.Right(1) != "\\")
		strDir += "\\";
	CString strPath = strDir + "media.lrn";
	SaveFile(strPath, strArrayContent, TRUE);

	return TRUE;
}

BOOL CPPT15Ctrl::WithMedia()
{
	int nSlideCount = GetSlideCount();
	for (int i = 1; i <= nSlideCount; i++)
	{
		_Slide15 slide = m_slides.Item(COleVariant((long)i));
		Shapes15 shapes = slide.GetShapes();
		int nShapeCount = shapes.GetCount();
		for (int j = 1; j <= nShapeCount; j++)
		{
			Shape15 shape = shapes.Item(COleVariant((long)j));
			if (!shape.GetHasTextFrame())
			{
				if (shape.GetType() == 16)
					return TRUE;
			}
		}
	}

	return FALSE;
}

BOOL CPPT15Ctrl::GetSliderTitle(CStringArray& strArray)
{
	strArray.RemoveAll();
	int nSlideCount = GetSlideCount();
	for (int i = 1; i <= nSlideCount; i++)
	{
		_Slide15 slide = m_slides.Item(COleVariant((long)i));
		Shapes15 shapes = slide.GetShapes();
		if (shapes.GetHasTitle())
		{
			Shape15 title = shapes.GetTitle();
			if (title.GetHasTextFrame())
			{
				TextFrame15 txFrame = title.GetTextFrame();
				TextRange15 txRange = txFrame.GetTextRange();
				strArray.Add(txRange.GetText());
			}
		}
		else
		{
			strArray.Add("");
		}
	}

	return TRUE;
}

BOOL CPPT15Ctrl::SaveFile(LPCTSTR strFileName, CStringArray& strArray, BOOL bNoEmpty)
{ 
	CFile cf;
	if (!cf.Open(strFileName, CFile::modeCreate | CFile::modeWrite))
		return FALSE;

	for (int i = 0; i < strArray.GetSize(); i++)
	{
		CString str = strArray[i];
		if (bNoEmpty && str == "")
			continue;
		cf.Write(str + "\r\n", str.GetLength() + 2);
	}

	return TRUE;
};

/* del itoh 2013/07/09 PPT2013
BOOL CPPT15Ctrl::SetPublishtoDir()
{
	WebOptions15 wo = m_presentation.GetWebOptions();
	wo.SetOrganizeInFolder(1);
	return TRUE;
}
*/

//add 2014/7/23 kagata BugNo.131
BOOL CPPT15Ctrl::SetThumb(CString strDir, int SlideCnt)
{
	FILE *stream_in;
	char* pline;
	//html�t�@�C�����t���p�X
	CString htmlPath = MACRO_TEMPLATE_PATH + "Slide1.html";
	CString slidPath = strDir + "html\\THUMB1\\";
	//Slide�����沼tml���R�s�[
	for(int v = 1; v <= SlideCnt; v++) {
		CString str;
		CString slidePath = "";
		str.Format(_T("%d"), v);
		slidePath = slidPath + "Slide" + str + ".html";
		CopyFile(htmlPath, slidePath, FALSE);
	}

	for(int i = 1; i <= SlideCnt; i++) {

		pline = new char[65535];

		CString str;
		str.Format(_T("%d"), i);
		if ((stream_in = fopen(strDir + "html\\THUMB1\\Slide" + str + ".html", "r" )) == NULL)
			return FALSE;
		for(int q = 0;;q++) {
			if (fgets(pline, 65535, stream_in) == NULL)
			{
				delete [] pline;
				fclose(stream_in);
				return FALSE;
			}
			if(strncmp(pline,"<img src=\"Slide1.png", 20) == 0) {
				break;
			}
		}
		fclose(stream_in);

		CStringArray strArrayTag, strArrayReplace;
		CString strSlide = "<img src=\"Slide" + str + ".png\" id=\"thumb\">";
		strArrayTag.Add(pline);
		strArrayReplace.Add(strSlide);
		::UpdateFileByTag(strDir + "html\\THUMB1", "Slide" + str + ".html", strArrayTag, strArrayReplace, FALSE);

		delete [] pline;
	}

	return TRUE;
};

void CPPT15Ctrl::deleteDirectory(CString directory_path)
{
	CFileFind finder;
    CString path;
    path.Format("%s/*.*",directory_path);
    BOOL bWorking = finder.FindFile(path);
    while(bWorking){
        bWorking = finder.FindNextFile();
        if(finder.IsDirectory() && !finder.IsDots()){
            deleteDirectory(finder.GetFilePath());
            RemoveDirectory(finder.GetFilePath());
        }
        else{
            DeleteFile(finder.GetFilePath());
        }
    }
}

void CPPT15Ctrl::PumpMessage()
{
	MSG msg;
	while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}
