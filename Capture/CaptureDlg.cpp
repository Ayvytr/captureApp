#include "stdafx.h"
#include "Capture.h"
#include "CaptureDlg.h"
#include "resource.h"

typedef int(*FnStartScreenCapture)(const char* szAuth, const char* szDefaultSavePath, void *pCallBack, unsigned long hWndNotice, unsigned int noticeMsg);
FnStartScreenCapture gl_StartScreenCapture = NULL;
typedef int(*FnInitScreenCapture)(unsigned long trackerColor, unsigned long editBorderColor, int nTransparent, int flag);
FnInitScreenCapture gl_InitCapture = NULL;

CCaptureDlg::CCaptureDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCaptureDlg::IDD, pParent)
	, m_iCountDown(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCaptureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_BeginCapture);
	DDX_Control(pDX, CHECK_HIDESELF, m_HideSelf);
	DDX_Control(pDX, COMBO_DELAY, mctrl_SecondsToDelay);
	DDX_Text(pDX, STATIC_COUNTDOWN, m_iCountDown);
	DDX_Control(pDX, STATIC_COUNTDOWN, mctrl_CountDown);
}

BEGIN_MESSAGE_MAP(CCaptureDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_USER + 1111, OnCaptureFinish)
	ON_BN_CLICKED(IDOK, &CCaptureDlg::OnBnClickedOk)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

BOOL CCaptureDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	CenterWindow();
	ShowWindow(SW_SHOW);

	////TODO: ����λͼ̫���ÿ����ˣ���ʱע����
	//CBitmap m_btnBitmap;
	//m_btnBitmap.LoadBitmap(IDB_BTNBITMAP);
	//m_BeginCapture.SetBitmap(m_btnBitmap);

	m_sDialogTitle.LoadString(STR_STARTTITLE);
	SetWindowText(m_sDialogTitle);

	CString snum;
	for (int i = 0; i <= 10; ++i)
	{
		snum.Format(L"%d", i);
		mctrl_SecondsToDelay.InsertString(i, snum);
	}
	mctrl_SecondsToDelay.SetCurSel(0);

	//���ؽ�ͼ�ؼ�
	m_hModule = LoadLibrary(L"NiuNiuCapture.dll");
	if (!m_hModule)
	{
		AfxMessageBox(L"δ�ҵ�dll�ؼ���");
		m_BeginCapture.EnableWindow(FALSE);
		return FALSE;
	}
	DWORD nError = GetLastError();
	gl_StartScreenCapture = (FnStartScreenCapture)GetProcAddress(m_hModule, "StartScreenCapture");
	gl_InitCapture = (FnInitScreenCapture)GetProcAddress(m_hModule, "InitScreenCapture");

	//���ý�ͼ�߿����ʽ  
	gl_InitCapture(RGB(255, 0, 0), RGB(0, 174, 255), 180, 0);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}


// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�
void CCaptureDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
HCURSOR CCaptureDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

HWND gl_hWnd = NULL;
void CaptureNotice(int nType)
{
	::PostMessage(gl_hWnd, WM_USER + 1111, 1, 1);
}

void CCaptureDlg::OnBnClickedOk()
{
	int nSel = -1;
	if ((nSel = mctrl_SecondsToDelay.GetCurSel()) > 0)
	{
		m_iCountDown = nSel;
		
		while (m_iCountDown)
		{
			m_iCountDown--;
			mctrl_CountDown.ShowWindow(SW_SHOW);
			UpdateData(FALSE);
			Sleep(1000);
		}
		mctrl_CountDown.ShowWindow(SW_HIDE);
	}

	if (m_HideSelf.GetCheck())
	{
		ShowWindow(SW_HIDE);
		//Sleep for enough time to hide this window
		Sleep(200);
	}

	char * cstrDefaultCaptureSavePath = GetDesktopCaptureFilePath();

	m_sDialogTitle.LoadString(STR_BEGINCAPTURE);
	SetWindowText(m_sDialogTitle);

	//ͨ���ص�������֪ͨ��ͼ����¼�
	gl_hWnd = m_hWnd;
	gl_StartScreenCapture("niuniu", cstrDefaultCaptureSavePath, CaptureNotice, 0, 0);
	delete[] cstrDefaultCaptureSavePath;
}


LRESULT CCaptureDlg::OnCaptureFinish(WPARAM wParam, LPARAM lParam)
{
	if (wParam == lParam && wParam == 1)
	{
		m_sDialogTitle.LoadString(STR_SAVECAPTURE);
		SetWindowText(m_sDialogTitle);
		if (m_HideSelf.GetCheck())
		{
			ShowWindow(SW_SHOW);
		}
	}
	return 0;
}

BOOL CCaptureDlg::DestroyWindow()
{
	FreeLibrary(m_hModule);

	return CDialog::DestroyWindow();
}

//return save path at ".../Desktop/capture.jpg"
char * CCaptureDlg::GetDesktopCaptureFilePath()
{
	//TCHAR strDesktop[MAX_PATH] = L"";
	//int i = 256;
	//while (i++ <= 1000)
	//{
	//	// i = 0 | 16 | 25 | 256 eg. are Desktop Path;
	//	SHGetSpecialFolderPath(0, strDesktop, i, 0);
	//	m_sDesktopDir = strDesktop;
	//	if (m_sDesktopDir.Find(L"desktop", 0) > 0 || m_sDesktopDir.Find(L"Desktop", 0) > 0)
	//	{
	//		m_sDesktopDir.Empty();
	//		m_sDesktopDir.Format(L"%d ", i);
	//		m_sDesktopDir += strDesktop;
	//		break;
	//	}
	//}
	//AfxMessageBox(strDesktop);

	CString sDefSaveFilePath;
	TCHAR sDesktopDir[MAX_PATH] = L"";
	SHGetSpecialFolderPath(0, sDesktopDir, 0, 0);
	sDefSaveFilePath = sDesktopDir + CString("\\Capture.jpg");
	char *cstrDefaultCaptureSavePath = new char[MAX_PATH];
	int strLen = sDefSaveFilePath.GetLength();
	int i = 0;
	while (i <= strLen)
	{
		cstrDefaultCaptureSavePath[i] = (char)sDefSaveFilePath.GetAt(i);
		++i;
	}
	cstrDefaultCaptureSavePath[i] = 0;

	return cstrDefaultCaptureSavePath;
}


HBRUSH CCaptureDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_STATIC)
	{
		switch (pWnd->GetDlgCtrlID())
		{
			case STATIC_COUNTDOWN:
			{
				CFont *pFont = new CFont();
				pFont->CreateFont(64, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET,
					OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FF_SCRIPT, L"΢���ź�");
				pDC->SelectObject(pFont);
				pDC->SetTextColor(RGB(255, 0, 0));
			}
				break;

			default:
			{
				pDC->SetTextColor(RGB(13, 177, 129));
			}
				break;
		}
	}
	
	if (nCtlColor == CTLCOLOR_BTN)
	{
		switch (pWnd->GetDlgCtrlID())
		{
			case IDOK:
			{
				CFont *pFont = new CFont();
				pFont->CreateFont(24, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET,
					OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FF_SCRIPT, L"΢���ź�");
				pDC->SelectObject(pFont);
				pDC->SetTextColor(RGB(247, 132, 70));
			}
				break;
		}
	}

	return hbr;
}
