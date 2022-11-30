
// Genshin_M2BC.h: PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含 'pch.h' 以生成 PCH"
#endif

#include "resource.h"		// 主符号


// CGenshinM2BCApp:
// 有关此类的实现，请参阅 Genshin_M2BC.cpp
//

class CGenshinM2BCApp : public CWinApp
{
public:
	CGenshinM2BCApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CGenshinM2BCApp theApp;
