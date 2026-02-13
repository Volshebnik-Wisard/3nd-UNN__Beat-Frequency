
// MFCApplication7.h : главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить stdafx.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CMFCApplication7App:
// О реализации данного класса см. MFCApplication7.cpp
//

class CMFCApplication7App : public CWinApp
{
public:
	CMFCApplication7App();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CMFCApplication7App theApp;