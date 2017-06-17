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
#define INITTIME 10800
#define ONTABLE _T("OnTable")
#define REMAINTIMETABLE _T("RemainTimeTable")
#define NOCARD _T("初始化卡片失败")

IMPLEMENT_DYNAMIC(Tab_Develop, CDialogEx)

void Tab_Develop::showTime(int time) {
	CString hour, min, sec;
	hour.Format(L"%d", time / 3600);
	min.Format(L"%d", (time % 3600) / 60);
	sec.Format(L"%d", (time % 3600) % 60);

	SetDlgItemText(IDC_EDHOUR, hour);
	SetDlgItemText(IDC_EDMIN, min);
	SetDlgItemText(IDC_EDSECOND, sec);
}


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
	
	runtime = false;
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
	ON_WM_TIMER()
	
END_MESSAGE_MAP()


////钱包的部分
//钱包初始化
void Tab_Develop::OnBnClickedBtninitpurse()
{
	CString balance;
	((CEdit*)GetDlgItem(IDC_EDBANLANCE))->GetWindowTextW(balance);

	// 检查输入
	if (balance.IsEmpty()) {		//如果为空
		MessageBox(_T("请输入正确的初始化余额！"));
		return;
	}
	long account = _ttol(balance);
	if (account < 0) {				//如果不能正常的转成long类型数字
		MessageBox(_T("初始化金额有误！"));
		return;
	}
	int sectionNum = 9;					//扇区号
	int blockNum = 0;					//块号
	unsigned char pswtype = 0x0A;		//密钥类型
	unsigned char chpwd[8];				//密钥
	int len_chpwd = 0;					//密钥长度
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
	int sectionNum = 9;
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
		((CEdit*)GetDlgItem(IDC_EDBANLANCE))->SetWindowTextW(balance);
		canIOPurse = true;
		LEDSet(account);			//LED显示余额
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
	// 检查输入
	if (addAccount.IsEmpty()) {
		MessageBox(_T("请输入正确的充值金额！"));
		return;
	}
	CString balance;
	((CEdit*)GetDlgItem(IDC_EDBANLANCE))->GetWindowTextW(balance);
	long account = _ttol(addAccount);
	int sectionNum = 9;
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
		canIOPurse = true;
		((CEdit*)GetDlgItem(IDC_EDPURSESTATE))->SetWindowTextW(_T("充值成功"));
		fileRecordHelper.SaveRecharges(uid, addAccount, account + _ttol(balance), _T("成功"));
		OnBnClickedBtncheckban();
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
	// 检查输入
	if (subAccount.IsEmpty()) {
		MessageBox(_T("请输入正确的消费金额！"));
		return;
	}
	CString balance;
	((CEdit*)GetDlgItem(IDC_EDBANLANCE))->GetWindowTextW(balance);
	long account = _ttol(subAccount);
	// 检查余额
	if (account > _ttol(balance)) {	
		MessageBox(_T("余额不足！"));
		return;
	}
	int sectionNum = 9;
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
	CString uid = GetCardUID();
	uid.Format(L"1111");
	
	if (uid != NOCARD) {
		if (!mysql.sql_isExist(uid, REMAINTIMETABLE)) {		//不存在于RemainTimeTable提示初始化
			MessageBox(_T("请先初始化余时"));
		}
		else {
			if (mysql.sql_isExist(uid, ONTABLE)) {			//已上机提示无法再上机
				((CEdit*)GetDlgItem(IDC_EDHOURSTATE))->SetWindowTextW(_T("上机中，请勿重复上机"));
			}
			else {
				//从数据库获取该UID的余时信息
				RemainTime * remain = (RemainTime*)mysql.remainTime_query(uid);
				if (remain->RemainSeconds <= 0) {	//余时为零提示初始化。并更新历史记录
					((CEdit*)GetDlgItem(IDC_EDHOURSTATE))->SetWindowTextW(_T("余时为零，请先初始化余时"));
					fileRecordHelper.StartNets(uid, remain->RemainSeconds, _T("余时为零，上机失败"));
				}
				else {	//剩余余时则将信息插入到Ontable中，将上机信息记录到数据库中
					if (mysql.sql_insertOn(RemainTime(remain->UID, remain->RemainSeconds))) {

						startTime = remain->RemainSeconds;		//在客户端记录剩余时间
						timenow = startTime;					//用来实时更新剩余时间

						((CEdit*)GetDlgItem(IDC_EDHOURSTATE))->SetWindowTextW(_T("上机成功"));
						fileRecordHelper.StartNets(uid, remain->RemainSeconds, _T("成功"));

						m_ActiveTimer = SetTimer(1, 1000, NULL);	//打开倒计时
						showTime(timenow);							//更新显示
					}
				}
				OnBnClickedBtnload();							//更新历史记录显示
				delete(remain);						
			}
		}
	}
	else {
		((CEdit*)GetDlgItem(IDC_EDHOURSTATE))->SetWindowTextW(_T("获取卡号异常"));
	}
}

//下线
void Tab_Develop::OnBnClickedBtnexit()
{
	CString uid = GetCardUID();
	uid.Format(L"1111");

	if (uid != NOCARD) {
		if (!mysql.sql_isExist(uid, ONTABLE))
			((CEdit*)GetDlgItem(IDC_EDHOURSTATE))->SetWindowTextW(_T("未上机，下线失败"));
		else {
			KillTimer(m_ActiveTimer);								//停止倒计时
			mysql.sql_delete(uid);									//下机
			if (mysql.sql_update(uid, timenow, REMAINTIMETABLE)) {	//更新remaintimetable的时间
				((CEdit*)GetDlgItem(IDC_EDHOURSTATE))->SetWindowTextW(_T("成功下线"));
				fileRecordHelper.ExitNets(uid, startTime, timenow, L"成功");

				OnBnClickedBtnload();	//更新记录
			}
			else {
				MessageBox(L"发生错误，请联系管理员");
			}
		}
	}
	else {
		((CEdit*)GetDlgItem(IDC_EDHOURSTATE))->SetWindowTextW(_T("获取卡号异常"));
	}
}

//余时默认初始化
void Tab_Develop::OnBnClickedBtninithour()
{
	CString uid = GetCardUID();
	uid.Format(L"1111");
	if (uid != NOCARD) {			//能从卡片中读取到uid
		if (mysql.sql_isExist(uid, ONTABLE)) {	//如果在OnTable中能找到这个uid则已上机
			MessageBox(L"上机中，请先下线再初始化余时");
		}
		else {
			if (mysql.sql_isExist(uid, REMAINTIMETABLE)) {	//如果之前登记过，存在于RemainTimeTable
				if (mysql.sql_update(uid, INITTIME, REMAINTIMETABLE))	//则直接更新数据就好
					((CEdit*)GetDlgItem(IDC_EDHOURSTATE))->SetWindowTextW(_T("初始化余时成功"));
				else
					MessageBox(L"初始化余时出错，请联系管理员");
			}
			else {	//否则插入该数据
				RemainTime remain(uid, INITTIME);
				if (mysql.sql_insertRemain(remain))
					((CEdit*)GetDlgItem(IDC_EDHOURSTATE))->SetWindowTextW(_T("初始化余时成功"));
				else
					MessageBox(L"初始化余时出错，请联系管理员");
			}
		}
	}
	else {
		((CEdit*)GetDlgItem(IDC_EDHOURSTATE))->SetWindowTextW(_T("获取卡号异常"));
	}
}

//余时查询
void Tab_Develop::OnBnClickedBtncheckhour()
{
	CString uid = GetCardUID();
	uid.Format(L"1111");
	if (uid != NOCARD) {
		//如果正在上机，则没有需要查询
		if(mysql.sql_isExist(uid,ONTABLE)) {
			((CEdit*)GetDlgItem(IDC_EDHOURSTATE))->SetWindowTextW(_T("上机中，无需查询"));
		}
		else {
			if (!mysql.sql_isExist(uid, REMAINTIMETABLE))
				MessageBox(L"无记录，请先初始化余时");
			else {
				RemainTime * remaintime = (RemainTime*)mysql.remainTime_query(uid);
				showTime(remaintime->RemainSeconds);			//更新显示
				((CEdit*)GetDlgItem(IDC_EDHOURSTATE))->SetWindowTextW(_T("查询余时成功"));
			}
		}
	}
	else {
		((CEdit*)GetDlgItem(IDC_EDHOURSTATE))->SetWindowTextW(_T("获取卡号异常"));
	}
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

void Tab_Develop::OnTimer(UINT_PTR nIDEvent) {
	// 在此添加消息处理程序代码和/或调用默认值
	if (timenow > 0) {
		timenow = timenow - 1;
		showTime(timenow);
	}
	else {
		timenow = 0;
		showTime(timenow);
		KillTimer(1);
		MessageBox(L"余时为零，请充值");
		OnBnClickedBtnexit();
	}
	CDialogEx::OnTimer(nIDEvent);
}