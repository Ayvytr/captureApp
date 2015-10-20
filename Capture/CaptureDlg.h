#pragma once
#include "afxwin.h"

#define WM_COUNTDOWNTIMER (WM_USER + 11)

// CCaptureDlg �Ի���
class CCaptureDlg : public CDialog
{
// ����
public:
	CCaptureDlg(CWnd* pParent = NULL);	// ��׼���캯��

	enum { IDD = IDD_CAPTURE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	BOOL OnInitDialog();

// ʵ��
protected:
	HICON m_hIcon;

	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedOk();
	LRESULT OnCaptureFinish(WPARAM wParam,LPARAM lParam);
	virtual BOOL DestroyWindow();

private:
	HMODULE m_hModule;
public:
	CString m_sDialogTitle;
	CButton m_BeginCapture;
	CButton m_HideSelf;
	int m_iCountDown;
	CStatic mctrl_CountDown;
	HANDLE m_hEvent;
private:
	CComboBox mctrl_SecondsToDelay;
public:
	char * GetDesktopCaptureFilePath();
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
protected:
	afx_msg LRESULT OnStaticCountDown(WPARAM wParam, LPARAM lParam);
};
