#pragma once

#ifndef __DEBUG_MODE__
    #pragma comment(lib, "./lib/ZM124U.lib")
    #include "./lib/ZM124U.h"
#else
    #include "ZM124U_2.h"
#endif

// Tab_Develop dialog

class Tab_Develop : public CDialogEx
{
	DECLARE_DYNAMIC(Tab_Develop)

public:
	Tab_Develop(CWnd* pParent = NULL);   // standard constructor
	virtual ~Tab_Develop();

// Dialog Data
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
