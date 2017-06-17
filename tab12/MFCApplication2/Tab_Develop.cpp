// Tab_Develop.cpp : implementation file
//

#include "stdafx.h"
#include "MFCApplication2.h"
#include "Tab_Develop.h"
#include "afxdialogex.h"
#include "Helper.h"
#include <odbcinst.h> 
#include "afxdb.h"

// Tab_Develop dialog

IMPLEMENT_DYNAMIC(Tab_Develop, CDialogEx)


CString Tab_Develop::GetCardUID() {
	CString uid, temp;
	unsigned char buff[1024];
	int buff_len;

	// 成功获取
	if (find_14443(buff, &buff_len) == IFD_OK) {
		uid.Empty();
		for (int i = 0; i < buff_len; i++) {
			// 将获得的UID数据（1 byte）转为16进制
			temp.Format(_T("%02x"), buff[i]);
			uid += temp;
		}
		return uid;
	}
	else {
		return _T("初始化卡片失败");
		//return TESTCARD;
	}
}

Tab_Develop::Tab_Develop(CWnd* pParent /*=NULL*/)
	: CDialogEx(Tab_Develop::IDD, pParent)
{

}

Tab_Develop::~Tab_Develop()
{
}

void Tab_Develop::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Tab_Develop, CDialogEx)
	ON_BN_CLICKED(IDC_BTNINITPURSE, &Tab_Develop::OnBnClickedBtninitpurse)
	ON_BN_CLICKED(IDC_BTNCHECKBAN, &Tab_Develop::OnBnClickedBtncheckban)
	ON_BN_CLICKED(IDC_BTNRECHARGE, &Tab_Develop::OnBnClickedBtnrecharge)
	ON_BN_CLICKED(IDC_BTNCONSUME, &Tab_Develop::OnBnClickedBtnconsume)
	ON_BN_CLICKED(IDC_BTNSTAR, &Tab_Develop::OnBnClickedBtnstar)
	ON_BN_CLICKED(IDC_BTNEXIT, &Tab_Develop::OnBnClickedBtnexit)
	ON_BN_CLICKED(IDC_BTNINITHOUR, &Tab_Develop::OnBnClickedBtninithour)
	ON_BN_CLICKED(IDC_BTNCHECKHOUR, &Tab_Develop::OnBnClickedBtncheckhour)
	ON_BN_CLICKED(IDC_BTNEMPTY, &Tab_Develop::OnBnClickedBtnempty)
	ON_BN_CLICKED(IDC_BTNLOAD, &Tab_Develop::OnBnClickedBtnload)
	
END_MESSAGE_MAP()


////钱包的部分
//钱包初始化
void Tab_Develop::OnBnClickedBtninitpurse()
{
	CString balance;
	((CEdit*)GetDlgItem(IDC_EDBANLANCE))->GetWindowTextW(balance);
	if (balance.IsEmpty()) {
		// 检查输入
		MessageBox(_T("请输入正确的初始化余额！"));
		return;
	}
	long account = _ttol(balance);
	if (account < 0) {
		// 检查输入
		MessageBox(_T("初始化金额有误！"));
		return;
	}
	int sectionNum = 15;
	int blockNum = 0;
	unsigned char pswtype = 0x0A;
	unsigned char chpwd[8];
	int len_chpwd = 0;
	// 密钥类型转换
	CString pwd = _T("FFFFFFFFFFFF");
	HexCString2UnsignedCharStar(pwd, chpwd, &len_chpwd);

	// 初始化钱包
	if (write_account(sectionNum, blockNum, pswtype, chpwd, account) == IFD_OK) {
		canIOPurse = true;
		((CEdit*)GetDlgItem(IDC_EDPURSESTATE))->SetWindowTextW(_T("初始化钱包成功"));
	}
	else {
		canIOPurse = false;
		((CEdit*)GetDlgItem(IDC_EDPURSESTATE))->SetWindowTextW(_T("初始化钱包失败"));
	}
}

//查询余额
void Tab_Develop::OnBnClickedBtncheckban()
{
	CString balance;
	long account = 0;
	int sectionNum = 15;
	int blockNum = 0;
	unsigned char pswtype = 0x0A;
	unsigned char chpwd[8];
	int len_chpwd = 0;
	// 密钥转换
	CString pwd = _T("FFFFFFFFFFFF");
	HexCString2UnsignedCharStar(pwd, chpwd, &len_chpwd);
	// 读取钱包
	if (read_account(sectionNum, blockNum, pswtype, chpwd, &account) == IFD_OK) {
		balance.Format(_T("%d"), account);
		((CEdit*)GetDlgItem(IDC_EDPURSESTATE))->SetWindowTextW(balance);
		canIOPurse = true;
		LEDSet(account); // MayBe Wrong
		((CEdit*)GetDlgItem(IDC_EDPURSESTATE))->SetWindowTextW(_T("查询钱包成功"));
	}
	else {
		canIOPurse = false;
		((CEdit*)GetDlgItem(IDC_EDPURSESTATE))->SetWindowTextW(_T("查询钱包失败"));
	}
}

//充值
void Tab_Develop::OnBnClickedBtnrecharge()
{
	CString addAccount;
	((CEdit*)GetDlgItem(IDC_EDRECHARGE))->GetWindowTextW(addAccount);
	if (addAccount.IsEmpty()) {
		// 检查充值输入
		MessageBox(_T("请输入正确的充值金额！"));
		return;
	}
	CString balance;
	((CEdit*)GetDlgItem(IDC_EDBANLANCE))->GetWindowTextW(balance);
	long account = _ttol(addAccount);
	int sectionNum = 15;
	int blockNum = 0;
	unsigned char pswtype = 0x0A;
	unsigned char chpwd[8];
	int len_chpwd = 0;
	// 密钥转换
	CString pwd = _T("FFFFFFFFFFFF");
	HexCString2UnsignedCharStar(pwd, chpwd, &len_chpwd);
	// 获取卡号
	CString uid = GetCardUID();
	// 充值函数
	if (add_account(sectionNum, blockNum, pswtype, chpwd, account) == IFD_OK) {
		OnBnClickedBtncheckban();
		canIOPurse = true;
		((CEdit*)GetDlgItem(IDC_EDPURSESTATE))->SetWindowTextW(_T("充值成功"));
		fileRecordHelper.SaveRecharges(uid, addAccount, account + _ttol(balance), _T("成功"));
		
	}
	else {
		canIOPurse = false;
		((CEdit*)GetDlgItem(IDC_EDPURSESTATE))->SetWindowTextW(_T("充值失败"));
		fileRecordHelper.SaveRecharges(uid, addAccount, _ttol(balance), _T("失败"));
		
	}
	// 更新历史记录显示
	OnBnClickedBtnload();
}

//消费
void Tab_Develop::OnBnClickedBtnconsume()
{
	CString subAccount;
	((CEdit*)GetDlgItem(IDC_EDCONSUME))->GetWindowTextW(subAccount);
	if (subAccount.IsEmpty()) {
		// 检查输入
		MessageBox(_T("请输入正确的消费金额！"));
		return;
	}
	CString balance;
	((CEdit*)GetDlgItem(IDC_EDBANLANCE))->GetWindowTextW(balance);
	long account = _ttol(subAccount);
	if (account > _ttol(balance)) {
		// 检查余额
		MessageBox(_T("余额不足！"));
		return;
	}
	int sectionNum = 15;
	int blockNum = 0;
	unsigned char pswtype = 0x0A;
	unsigned char chpwd[8];
	int len_chpwd = 0;
	// 密钥类型转换
	CString pwd = _T("FFFFFFFFFFFF");
	HexCString2UnsignedCharStar(pwd, chpwd, &len_chpwd);
	// 获取卡号
	CString uid = GetCardUID();
	// 消费函数
	if (sub_account(sectionNum, blockNum, pswtype, chpwd, account) == IFD_OK) {
		OnBnClickedBtncheckban();
		canIOPurse = true;
		((CEdit*)GetDlgItem(IDC_EDPURSESTATE))->SetWindowTextW(_T("消费成功"));
		fileRecordHelper.SaveConsumptions(uid, subAccount, _ttol(balance) - account, _T("成功"));
		
	}
	else {
		canIOPurse = false;
		((CEdit*)GetDlgItem(IDC_EDPURSESTATE))->SetWindowTextW(_T("消费失败"));
		fileRecordHelper.SaveConsumptions(uid, subAccount, _ttol(balance), _T("失败"));
	
	}
	// 更新历史记录显示
	OnBnClickedBtnload();
}


////上网的部分
//开始上机
void Tab_Develop::OnBnClickedBtnstar()
{
	CDatabase database;
	if (database.Open(_T("mysql5.5"))) {
		database.ExecuteSQL(_T("insert into remaintimetable(UID) values('2222')"));
		database.Close();
	}
}

//下线
void Tab_Develop::OnBnClickedBtnexit()
{
	
}

//余时默认初始化
void Tab_Develop::OnBnClickedBtninithour()
{
	
}

//余时查询
void Tab_Develop::OnBnClickedBtncheckhour()
{
	
}

/////历史纪录
//清空记录
void Tab_Develop::OnBnClickedBtnempty()
{
	((CEdit*)GetDlgItem(IDC_EDHISTORY))->SetWindowTextW(_T(""));
}

//加载记录
void Tab_Develop::OnBnClickedBtnload()
{
	((CEdit*)GetDlgItem(IDC_EDHISTORY))->SetWindowTextW(fileRecordHelper.LoadRecords());
}

