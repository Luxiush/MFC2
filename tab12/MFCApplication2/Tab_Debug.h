#pragma once

#ifndef __DEBUG_MODE__
    #pragma comment(lib, "./lib/ZM124U.lib")
    #include "./lib/ZM124U.h"
#else
    #include "ZM124U_2.h"
#endif
#include "afxwin.h"

// Tab_Debug dialog

class Tab_Debug : public CDialogEx
{
	DECLARE_DYNAMIC(Tab_Debug)

public:
	Tab_Debug(CWnd* pParent = NULL);   // standard constructor
	virtual ~Tab_Debug();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	// 显示某一块的数据
	void ShowData(unsigned char* data, int data_len);

public:
	afx_msg void OnBnClickedBtnPoweron();
	afx_msg void OnBnClickedBtnCardinfoGet();
	afx_msg void OnBnClickedBtnCardinfoCheck();
	afx_msg void OnBnClickedBtnLedSetting();
	afx_msg void OnBnClickedBtnKeyDefault();

	int m_key_type;							/*密钥类型*/
	char m_c_key_type;
	afx_msg void OnBnClickedRbKeyType();

	unsigned char m_key[10];				/*密钥串*/
	afx_msg void OnEnKillfocusEcKeystring();

	CComboBox m_cb_section;
	int m_section_num;						/*扇区号*/
	afx_msg void OnCbnSelchangeCbSection();
	afx_msg void OnCbnDropdownCbSection();
	
	CComboBox m_cb_block;
	int m_block_num;						/*块号*/
	afx_msg void OnCbnDropdownCbBlock();

	
	
	afx_msg void OnBnClickedBtnReadSec();
	afx_msg void OnBnClickedBtnReadBlock();
	afx_msg void OnBnClickedBtnWriteBlock();
	afx_msg void OnCbnSelchangeCbBlock();
	
	
	afx_msg void OnEnChangeEcPowerstatus();
};
