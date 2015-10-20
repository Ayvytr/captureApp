#pragma once
#include "afxwin.h"

#define WM_COUNTDOWNTIMER (WM_USER + 11)

// CCaptureDlg 对话框
class CCaptureDlg : public CDialog
{
// 构造
public:
	CCaptureDlg(CWnd* pParent = NULL);	// 标准构造函数

	enum { IDD = IDD_CAPTURE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	BOOL OnInitDialog();

// 实现
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
	CString m_sDialogTitle;
	CButton m_BeginCapture;
	CButton m_HideSelf;
	int m_iCountDown;
	CStatic mctrl_CountDown;
	CComboBox mctrl_SecondsToDelay;

private:
	char * GetDesktopCaptureFilePath();
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
