#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������

// CCaptureApp: 
// �йش����ʵ�֣������ Capture.cpp
class CCaptureApp : public CWinApp
{
public:
	CCaptureApp();

public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

extern CCaptureApp theApp;