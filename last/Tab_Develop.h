#pragma once

#include "RecordHelper.h"
#include "ODBCMySQLHelper.h"
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
	void showTime(int time);
// Dialog Data
	enum { IDD = IDD_DIALOG2 };

private:
	bool canIOPurse;
	CRecordHelper fileRecordHelper;
	ODBCMySQLHelper mysql;
	int startTime;
	int timenow;
	UINT_PTR m_ActiveTimer;
	bool runtime;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CString GetCardUID();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtninitpurse();
	afx_msg void OnBnClickedBtncheckban();
	afx_msg void OnBnClickedBtnrecharge();
	afx_msg void OnBnClickedBtnconsume();
	afx_msg void OnBnClickedBtnstar();
	afx_msg void OnBnClickedBtnexit();
	afx_msg void OnBnClickedBtninithour();
	afx_msg void OnBnClickedBtncheckhour();
	afx_msg void OnBnClickedBtnempty();
	afx_msg void OnBnClickedBtnload();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

};
