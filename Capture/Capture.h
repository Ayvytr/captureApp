#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号

// CCaptureApp: 
// 有关此类的实现，请参阅 Capture.cpp
class CCaptureApp : public CWinApp
{
public:
	CCaptureApp();

public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

extern CCaptureApp theApp;