
// MFCApplication2Dlg.h : header file
//

#pragma once
#include "afxcmn.h"

#include "Tab_Debug.h"
#include "Tab_Develop.h"

#ifndef __DEBUG_MODE__
    #pragma comment(lib, "./lib/ZM124U.lib")
    #include "./lib/ZM124U.h"
#else
    #include "ZM124U_2.h"
#endif

// CMFCApplication2Dlg dialog
class CMFCApplication2Dlg : public CDialogEx
{
// Construction
public:
	CMFCApplication2Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_MFCAPPLICATION2_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_tab;
	Tab_Debug m_tabDebug;
	Tab_Develop m_tabDev;
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
};
