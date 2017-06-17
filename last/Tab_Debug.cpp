// Tab_Debug.cpp : implementation file
//

#include "stdafx.h"
#include "MFCApplication2.h"
#include "Tab_Debug.h"
#include "afxdialogex.h"


// Tab_Debug dialog

IMPLEMENT_DYNAMIC(Tab_Debug, CDialogEx)

Tab_Debug::Tab_Debug(CWnd* pParent /*=NULL*/)
	: CDialogEx(Tab_Debug::IDD, pParent)
	, m_key_type(0)
	, m_c_key_type(0x0a)
	, m_section_num(CB_ERR)
	, m_block_num(CB_ERR)
{
	memset(m_key, 0, sizeof(m_key));
}

Tab_Debug::~Tab_Debug()
{
}

void Tab_Debug::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, ID1_RB_KEY_TYPEA, m_key_type);
	DDX_Control(pDX, ID1_CB_SECTION, m_cb_section);
	DDX_CBIndex(pDX, ID1_CB_SECTION, m_section_num);
	DDX_Control(pDX, ID1_CB_BLOCK, m_cb_block);
	DDX_CBIndex(pDX, ID1_CB_BLOCK, m_block_num);
}

BEGIN_MESSAGE_MAP(Tab_Debug, CDialogEx)
	ON_BN_CLICKED(ID1_BTN_POWERON, &Tab_Debug::OnBnClickedBtnPoweron)
	ON_BN_CLICKED(ID1_BTN_CARDINFO_GET, &Tab_Debug::OnBnClickedBtnCardinfoGet)
	ON_BN_CLICKED(ID1_BTN_CARDINFO_CHECK, &Tab_Debug::OnBnClickedBtnCardinfoCheck)
	ON_BN_CLICKED(ID1_BTN_LED_SETTING, &Tab_Debug::OnBnClickedBtnLedSetting)

	ON_BN_CLICKED(ID1_RB_KEY_TYPEA, &Tab_Debug::OnBnClickedRbKeyType)
	ON_BN_CLICKED(ID1_RB_KEY_TYPEB, &Tab_Debug::OnBnClickedRbKeyType)
	ON_EN_KILLFOCUS(ID1_EC_KEYSTRING, &Tab_Debug::OnEnKillfocusEcKeystring)
	ON_BN_CLICKED(ID1_BTN_KEY_DEFAULT, &Tab_Debug::OnBnClickedBtnKeyDefault)

	ON_CBN_DROPDOWN(ID1_CB_SECTION, &Tab_Debug::OnCbnDropdownCbSection)
	ON_CBN_SELCHANGE(ID1_CB_SECTION, &Tab_Debug::OnCbnSelchangeCbSection)

	ON_CBN_DROPDOWN(ID1_CB_BLOCK, &Tab_Debug::OnCbnDropdownCbBlock)
	ON_CBN_SELCHANGE(ID1_CB_BLOCK, &Tab_Debug::OnCbnSelchangeCbBlock)

	ON_BN_CLICKED(ID1_BTN_READ_SEC, &Tab_Debug::OnBnClickedBtnReadSec)
	ON_BN_CLICKED(ID1_BTN_READ_BLOCK, &Tab_Debug::OnBnClickedBtnReadBlock)
	ON_BN_CLICKED(ID1_BTN_WRITE_BLOCK, &Tab_Debug::OnBnClickedBtnWriteBlock)
	
	ON_EN_CHANGE(ID1_EC_POWERSTATUS, &Tab_Debug::OnEnChangeEcPowerstatus)
END_MESSAGE_MAP()


/*BTN [开启设备]*/
void Tab_Debug::OnBnClickedBtnPoweron()
{
	((CEdit*)GetDlgItem(ID1_EC_POWERSTATUS))->SetWindowTextW(GetIFDStatusString(IDD_PowerOn()));
}

/*BTN [获取] UID*/
void Tab_Debug::OnBnClickedBtnCardinfoGet()
{
	CString uid(""), tmp("");
	unsigned char buff[10];
	int buff_len = 0;

	int res = find_14443(buff, &buff_len);
	if (IFD_OK == res && buff_len!=0){
		uid = HexStr2CStr(buff, buff_len);
		((CEdit*)GetDlgItem(ID1_EC_CARDINFO_UID))->SetWindowTextW(uid);
		
	}
	else {
		((CEdit*)GetDlgItem(ID1_EC_CARDINFO_UID))->SetWindowTextW(CString(""));
	}
	((CEdit*)GetDlgItem(ID1_EC_CARDINFO_STATUS))->SetWindowTextW(GetIFDStatusString(res));
}

// BTN [查看] 版本信息
void Tab_Debug::OnBnClickedBtnCardinfoCheck()
{	
	unsigned char buff[1024];
	int buff_len = 0;

	int res = Reader_Version(buff, &buff_len);
	CString ver("");
	if (res == IFD_OK){
		ver = UnsignedChar2CStr(buff, buff_len);
	}
	((CEdit*)GetDlgItem(ID1_EC_CARDINFO_VER))->SetWindowTextW(ver);
	((CEdit*)GetDlgItem(ID1_EC_CARDINFO_STATUS))->SetWindowTextW(GetIFDStatusString(res));
}

// BTN LED [设置】
void Tab_Debug::OnBnClickedBtnLedSetting()
{
	CString info, digit;
	char data[1024]; 
	int data_len = 0;
	memset(data, 0, 1024);
	unsigned char point = 0;

	((CEdit*)GetDlgItem(ID1_EC_LED_INFO))->GetWindowTextW(info);
	((CEdit*)GetDlgItem(ID1_EC_LED_DIGIT))->GetWindowTextW(digit);
	data_len = info.GetLength();
//	memcpy(data, info.GetBuffer(),info.GetAllocLength());//CString => char*
	CString2CharStar(info, data, data_len);
	point = (unsigned char)(digit.GetAt(0)-L'0'+1);

	/*CString tmp("");
	tmp.Format(_T("point: %d num: %s, info: %s" ), point, UnsignedChar2CStr((unsigned char*)data,data_len), info);
	MessageBoxW(tmp);*/
	
	int res = LED(data, data_len, point);

	((CEdit*)GetDlgItem(ID1_EC_LED_STATUS))->SetWindowTextW(GetIFDStatusString(res));
}

// BTN [默认密钥]
void Tab_Debug::OnBnClickedBtnKeyDefault()
{
	CString key_buff("FFFFFFFFFFFF");
#ifdef __DEBUG_MODE__
	key_buff = CString("123456789ABC");
#endif

	((CEdit*)GetDlgItem(ID1_EC_KEYSTRING))->SetWindowTextW(key_buff);
	CStr2HexStr(key_buff, m_key, NULL);
}

// RB 密钥类型选择
void Tab_Debug::OnBnClickedRbKeyType()
{
	UpdateData(TRUE);
	if (m_key_type == 0) {
		m_c_key_type = 0x0a;
	}
	else {
		m_c_key_type = 0x0b;
	}

	CString temp("");
	temp.Format(_T("key_type: %02x "), m_c_key_type);
	MessageBox(temp);
}

// 密钥输入失去焦点
void Tab_Debug::OnEnKillfocusEcKeystring()
{
	CString key_buff("");
	((CEdit*)GetDlgItem(ID1_EC_KEYSTRING))->GetWindowTextW(key_buff);

	if (key_buff.IsEmpty()){
		MessageBoxW(CString("密钥不能为空"));
		return;
	}
	CStr2HexStr(key_buff, m_key, NULL);
}

/*扇区组合框*/
void Tab_Debug::OnCbnDropdownCbSection()
{
	m_cb_section.ResetContent();
	for (int i = 0; i < 16; ++i){
		CString temp("");
		temp.Format(_T("%d"), i);
		m_cb_section.InsertString(i, temp);
	}
}
void Tab_Debug::OnCbnSelchangeCbSection()
{
	m_section_num = m_cb_section.GetCurSel();
	CString temp("");
	temp.Format(_T("m_sec_num: %d "), m_section_num);
	MessageBox(temp);
}


/*块号组合框*/
void Tab_Debug::OnCbnDropdownCbBlock()
{
	m_cb_block.ResetContent();
	for (int i = 0; i < 4; ++i){
		CString temp("");
		temp.Format(_T("%d"), i);
		m_cb_block.InsertString(i, temp);
	}
}
void Tab_Debug::OnCbnSelchangeCbBlock()
{
	m_block_num = m_cb_block.GetCurSel();
	CString temp("");
	temp.Format(_T("m_block_num: %d "), m_block_num);
	MessageBox(temp);
}

// BTN [读块]
void Tab_Debug::OnBnClickedBtnReadBlock()
{
	if (m_section_num == CB_ERR){
		MessageBoxW(CString("请先选择扇区号"));
		return;
	}
	if (m_block_num == CB_ERR){
		MessageBoxW(CString("请先选择块号"));
		return;
	}
	if (m_key[0]=='\0'){
		MessageBoxW(CString("密钥不能为空"));
		return;
	}

	unsigned char data[20]; //最多 16byte
	memset(data, 0, sizeof(data));
	int data_len(0);
	// 读块
	int res = read_block(
		m_section_num, m_block_num, 
		m_c_key_type, m_key,
		data, &data_len
	);

	//显示读到的数据
	if (res != IFD_OK) data_len = 0;
	ShowData(data, data_len);

	//显示状态
	((CEdit*)GetDlgItem(ID1_EC_RW_STATUS))->SetWindowTextW(GetIFDStatusString(res));

#ifdef __DEBUG_MODE__
	CString msg("");
	msg.AppendFormat(_T("section_num: %d, block_num: %d, key_type:%d, key: %s"), m_section_num, m_block_num, m_key_type, HexStr2CStr(m_key, 6));

	MessageBoxW(msg);
#endif
}

// BTN [写块]
void Tab_Debug::OnBnClickedBtnWriteBlock()
{
	if (m_section_num == CB_ERR){
		MessageBoxW(CString("请先选择扇区号"));
		return;
	}
	if (m_block_num == CB_ERR){
		MessageBoxW(CString("请先选择块号"));
		return;
	}
	if (m_key[0]=='\0'){
		MessageBoxW(CString("密钥不能为空"));
		return;
	}

	//获取编辑框的数据
	unsigned char data[20]; //最多 16byte
	int data_len(0);
	CString data_buff("");
	switch (m_block_num){
	case 0:
		((CEdit*)GetDlgItem(ID1_EC_BLOCK_DATA_0))->GetWindowTextW(data_buff);
		break;
	case 1:
		((CEdit*)GetDlgItem(ID1_EC_BLOCK_DATA_1))->GetWindowTextW(data_buff);
		break;
	case 2:
		((CEdit*)GetDlgItem(ID1_EC_BLOCK_DATA_2))->GetWindowTextW(data_buff);
		break;
	case 3:
		CString tmp("");
		((CEdit*)GetDlgItem(ID1_EC_BLOCK_DATA_3_0))->GetWindowTextW(tmp);
		data_buff.Append(tmp);
		((CEdit*)GetDlgItem(ID1_EC_BLOCK_DATA_3_1))->GetWindowTextW(tmp);
		data_buff.Append(tmp);
		((CEdit*)GetDlgItem(ID1_EC_BLOCK_DATA_3_2))->GetWindowTextW(tmp);
		data_buff.Append(tmp);
		break;
	}
	if (data_buff.IsEmpty()){
		MessageBoxW(CString("请输入要写的数据"));
		return;
	}
	CString tmp(data_buff);
	data_buff.Empty();
	for (int i = 0; i < tmp.GetLength(); ++i){
		if (tmp[i] != ' '){
			data_buff.AppendChar(tmp[i]);
		}
	}

	CStr2HexStr(data_buff, data, &data_len);

	//写块
	int res = write_block(m_block_num, m_section_num, m_c_key_type, m_key, data, data_len);

	//显示状态
	((CEdit*)GetDlgItem(ID1_EC_RW_STATUS))->SetWindowTextW(GetIFDStatusString(res));
}

// 显示某一块的数据
void Tab_Debug::ShowData(unsigned char* data, int data_len){
	int ec_id[] = { ID1_EC_BLOCK_DATA_0, ID1_EC_BLOCK_DATA_1, ID1_EC_BLOCK_DATA_2};
	int ec_id_block3[] = { ID1_EC_BLOCK_DATA_3_0, ID1_EC_BLOCK_DATA_3_1, ID1_EC_BLOCK_DATA_3_2};

	if (m_block_num != 3){
		CString str = HexStr2CStr(data, data_len);
		for (int i = 0; i < 3; ++i){
			if (m_block_num == i){
				((CEdit*)GetDlgItem(ec_id[i]))->SetWindowTextW(str);
			}
			else{
				((CEdit*)GetDlgItem(ec_id[i]))->SetWindowTextW(CString(""));
			}
			((CEdit*)GetDlgItem(ec_id_block3[i]))->SetWindowTextW(CString(""));
		}
	}
	else{
		for (int i = 0; i < 3; ++i){
			((CEdit*)GetDlgItem(ec_id[i]))->SetWindowTextW(CString(""));
			if (data_len < 16){
				((CEdit*)GetDlgItem(ec_id_block3[i]))->SetWindowTextW(CString(""));
			}
		}
		if (data_len >= 16){
			((CEdit*)GetDlgItem(ec_id_block3[0]))->SetWindowTextW(HexStr2CStr(data, 6));
			((CEdit*)GetDlgItem(ec_id_block3[1]))->SetWindowTextW(HexStr2CStr(data + 6, 4));
			((CEdit*)GetDlgItem(ec_id_block3[2]))->SetWindowTextW(HexStr2CStr(data + 10, 6));
		}
	}
}

// BTN [读扇区]
void Tab_Debug::OnBnClickedBtnReadSec()
{
	if (m_section_num == CB_ERR){
		MessageBoxW(CString("请先选择扇区号"));
		return;
	}
	if (m_key[0] == '\0'){
		MessageBoxW(CString("密钥不能为空"));
		return;
	}

	// 读块
	unsigned char data[4][20]; //最多 16byte
	int data_len[4];
	memset(data, 0, sizeof(data));
	memset(data_len, 0, sizeof(data_len));
	int res = 0; 
	for (int i = 0; i < 4; ++i){
		res = read_block( 
			m_section_num, i,
			m_c_key_type, m_key,
			data[i], &(data_len[i])
			);
		if (res != IFD_OK) break;
	}

	// 显示读到的数据
	int ec_id[] = { ID1_EC_BLOCK_DATA_0, ID1_EC_BLOCK_DATA_1, ID1_EC_BLOCK_DATA_2 };
	int ec_id_block3[] = { ID1_EC_BLOCK_DATA_3_0, ID1_EC_BLOCK_DATA_3_1, ID1_EC_BLOCK_DATA_3_2 };

	if (res != IFD_OK){
		for (int i = 0; i < 3; ++i){
			((CEdit*)GetDlgItem(ec_id[i]))->SetWindowTextW(CString(""));
			((CEdit*)GetDlgItem(ec_id_block3[i]))->SetWindowTextW(CString(""));
		}
	}
	else{
		for (int i = 0; i < 3; ++i){
			((CEdit*)GetDlgItem(ec_id[i]))->SetWindowTextW(HexStr2CStr(data[i],data_len[i]));
		}
		((CEdit*)GetDlgItem(ec_id_block3[0]))->SetWindowTextW(HexStr2CStr(data[3], 6));
		((CEdit*)GetDlgItem(ec_id_block3[1]))->SetWindowTextW(HexStr2CStr(data[3] + 6, 4));
		((CEdit*)GetDlgItem(ec_id_block3[2]))->SetWindowTextW(HexStr2CStr(data[3] + 10, 6));
	}

	((CEdit*)GetDlgItem(ID1_EC_RW_STATUS))->SetWindowTextW(GetIFDStatusString(res));
}










void Tab_Debug::OnEnChangeEcPowerstatus()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}
