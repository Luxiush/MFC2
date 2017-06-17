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
#define NOCARD _T("��ʼ����Ƭʧ��")

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

	// �ɹ���ȡ
	if (find_14443(buff, &buff_len) == IFD_OK) {
		uid.Empty();
		for (int i = 0; i < buff_len; i++) {
			// ����õ�UID���ݣ�1 byte��תΪ16����
			temp.Format(_T("%02x"), buff[i]);
			uid += temp;
		}
		return uid;
	}
	else {
		return _T("��ʼ����Ƭʧ��");
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


////Ǯ���Ĳ���
//Ǯ����ʼ��
void Tab_Develop::OnBnClickedBtninitpurse()
{
	CString balance;
	((CEdit*)GetDlgItem(IDC_EDBANLANCE))->GetWindowTextW(balance);

	// �������
	if (balance.IsEmpty()) {		//���Ϊ��
		MessageBox(_T("��������ȷ�ĳ�ʼ����"));
		return;
	}
	long account = _ttol(balance);
	if (account < 0) {				//�������������ת��long��������
		MessageBox(_T("��ʼ���������"));
		return;
	}
	int sectionNum = 9;					//������
	int blockNum = 0;					//���
	unsigned char pswtype = 0x0A;		//��Կ����
	unsigned char chpwd[8];				//��Կ
	int len_chpwd = 0;					//��Կ����
	// ��Կ����ת��
	CString pwd = _T("FFFFFFFFFFFF");	
	HexCString2UnsignedCharStar(pwd, chpwd, &len_chpwd);

	// ��ʼ��Ǯ��
	if (write_account(sectionNum, blockNum, pswtype, chpwd, account) == IFD_OK) {
		canIOPurse = true;
		((CEdit*)GetDlgItem(IDC_EDPURSESTATE))->SetWindowTextW(_T("��ʼ��Ǯ���ɹ�"));
	}
	else {
		canIOPurse = false;
		((CEdit*)GetDlgItem(IDC_EDPURSESTATE))->SetWindowTextW(_T("��ʼ��Ǯ��ʧ��"));
	}
}

//��ѯ���
void Tab_Develop::OnBnClickedBtncheckban()
{
	CString balance;
	long account = 0;
	int sectionNum = 9;
	int blockNum = 0;
	unsigned char pswtype = 0x0A;
	unsigned char chpwd[8];
	int len_chpwd = 0;
	// ��Կת��
	CString pwd = _T("FFFFFFFFFFFF");
	HexCString2UnsignedCharStar(pwd, chpwd, &len_chpwd);
	// ��ȡǮ��
	if (read_account(sectionNum, blockNum, pswtype, chpwd, &account) == IFD_OK) {
		balance.Format(_T("%d"), account);
		((CEdit*)GetDlgItem(IDC_EDBANLANCE))->SetWindowTextW(balance);
		canIOPurse = true;
		LEDSet(account);			//LED��ʾ���
		((CEdit*)GetDlgItem(IDC_EDPURSESTATE))->SetWindowTextW(_T("��ѯǮ���ɹ�"));
	}
	else {
		canIOPurse = false;
		((CEdit*)GetDlgItem(IDC_EDPURSESTATE))->SetWindowTextW(_T("��ѯǮ��ʧ��"));
	}
}

//��ֵ
void Tab_Develop::OnBnClickedBtnrecharge()
{
	CString addAccount;
	((CEdit*)GetDlgItem(IDC_EDRECHARGE))->GetWindowTextW(addAccount);
	// �������
	if (addAccount.IsEmpty()) {
		MessageBox(_T("��������ȷ�ĳ�ֵ��"));
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
	// ��Կת��
	CString pwd = _T("FFFFFFFFFFFF");
	HexCString2UnsignedCharStar(pwd, chpwd, &len_chpwd);
	// ��ȡ����
	CString uid = GetCardUID();
	// ��ֵ����
	if (add_account(sectionNum, blockNum, pswtype, chpwd, account) == IFD_OK) {
		canIOPurse = true;
		((CEdit*)GetDlgItem(IDC_EDPURSESTATE))->SetWindowTextW(_T("��ֵ�ɹ�"));
		fileRecordHelper.SaveRecharges(uid, addAccount, account + _ttol(balance), _T("�ɹ�"));
		OnBnClickedBtncheckban();
	}
	else {
		canIOPurse = false;
		((CEdit*)GetDlgItem(IDC_EDPURSESTATE))->SetWindowTextW(_T("��ֵʧ��"));
		fileRecordHelper.SaveRecharges(uid, addAccount, _ttol(balance), _T("ʧ��"));
	}
	// ������ʷ��¼��ʾ
	OnBnClickedBtnload();
}

//����
void Tab_Develop::OnBnClickedBtnconsume()
{
	CString subAccount;
	((CEdit*)GetDlgItem(IDC_EDCONSUME))->GetWindowTextW(subAccount);
	// �������
	if (subAccount.IsEmpty()) {
		MessageBox(_T("��������ȷ�����ѽ�"));
		return;
	}
	CString balance;
	((CEdit*)GetDlgItem(IDC_EDBANLANCE))->GetWindowTextW(balance);
	long account = _ttol(subAccount);
	// ������
	if (account > _ttol(balance)) {	
		MessageBox(_T("���㣡"));
		return;
	}
	int sectionNum = 9;
	int blockNum = 0;
	unsigned char pswtype = 0x0A;
	unsigned char chpwd[8];
	int len_chpwd = 0;
	// ��Կ����ת��
	CString pwd = _T("FFFFFFFFFFFF");
	HexCString2UnsignedCharStar(pwd, chpwd, &len_chpwd);
	// ��ȡ����
	CString uid = GetCardUID();
	// ���Ѻ���
	if (sub_account(sectionNum, blockNum, pswtype, chpwd, account) == IFD_OK) {
		OnBnClickedBtncheckban();
		canIOPurse = true;
		((CEdit*)GetDlgItem(IDC_EDPURSESTATE))->SetWindowTextW(_T("���ѳɹ�"));
		fileRecordHelper.SaveConsumptions(uid, subAccount, _ttol(balance) - account, _T("�ɹ�"));
	}
	else {
		canIOPurse = false;
		((CEdit*)GetDlgItem(IDC_EDPURSESTATE))->SetWindowTextW(_T("����ʧ��"));
		fileRecordHelper.SaveConsumptions(uid, subAccount, _ttol(balance), _T("ʧ��"));
	}
	// ������ʷ��¼��ʾ
	OnBnClickedBtnload();
}


////�����Ĳ���
//��ʼ�ϻ�
void Tab_Develop::OnBnClickedBtnstar()
{
	CString uid = GetCardUID();
	uid.Format(L"1111");
	
	if (uid != NOCARD) {
		if (!mysql.sql_isExist(uid, REMAINTIMETABLE)) {		//��������RemainTimeTable��ʾ��ʼ��
			MessageBox(_T("���ȳ�ʼ����ʱ"));
		}
		else {
			if (mysql.sql_isExist(uid, ONTABLE)) {			//���ϻ���ʾ�޷����ϻ�
				((CEdit*)GetDlgItem(IDC_EDHOURSTATE))->SetWindowTextW(_T("�ϻ��У������ظ��ϻ�"));
			}
			else {
				//�����ݿ��ȡ��UID����ʱ��Ϣ
				RemainTime * remain = (RemainTime*)mysql.remainTime_query(uid);
				if (remain->RemainSeconds <= 0) {	//��ʱΪ����ʾ��ʼ������������ʷ��¼
					((CEdit*)GetDlgItem(IDC_EDHOURSTATE))->SetWindowTextW(_T("��ʱΪ�㣬���ȳ�ʼ����ʱ"));
					fileRecordHelper.StartNets(uid, remain->RemainSeconds, _T("��ʱΪ�㣬�ϻ�ʧ��"));
				}
				else {	//ʣ����ʱ����Ϣ���뵽Ontable�У����ϻ���Ϣ��¼�����ݿ���
					if (mysql.sql_insertOn(RemainTime(remain->UID, remain->RemainSeconds))) {

						startTime = remain->RemainSeconds;		//�ڿͻ��˼�¼ʣ��ʱ��
						timenow = startTime;					//����ʵʱ����ʣ��ʱ��

						((CEdit*)GetDlgItem(IDC_EDHOURSTATE))->SetWindowTextW(_T("�ϻ��ɹ�"));
						fileRecordHelper.StartNets(uid, remain->RemainSeconds, _T("�ɹ�"));

						m_ActiveTimer = SetTimer(1, 1000, NULL);	//�򿪵���ʱ
						showTime(timenow);							//������ʾ
					}
				}
				OnBnClickedBtnload();							//������ʷ��¼��ʾ
				delete(remain);						
			}
		}
	}
	else {
		((CEdit*)GetDlgItem(IDC_EDHOURSTATE))->SetWindowTextW(_T("��ȡ�����쳣"));
	}
}

//����
void Tab_Develop::OnBnClickedBtnexit()
{
	CString uid = GetCardUID();
	uid.Format(L"1111");

	if (uid != NOCARD) {
		if (!mysql.sql_isExist(uid, ONTABLE))
			((CEdit*)GetDlgItem(IDC_EDHOURSTATE))->SetWindowTextW(_T("δ�ϻ�������ʧ��"));
		else {
			KillTimer(m_ActiveTimer);								//ֹͣ����ʱ
			mysql.sql_delete(uid);									//�»�
			if (mysql.sql_update(uid, timenow, REMAINTIMETABLE)) {	//����remaintimetable��ʱ��
				((CEdit*)GetDlgItem(IDC_EDHOURSTATE))->SetWindowTextW(_T("�ɹ�����"));
				fileRecordHelper.ExitNets(uid, startTime, timenow, L"�ɹ�");

				OnBnClickedBtnload();	//���¼�¼
			}
			else {
				MessageBox(L"������������ϵ����Ա");
			}
		}
	}
	else {
		((CEdit*)GetDlgItem(IDC_EDHOURSTATE))->SetWindowTextW(_T("��ȡ�����쳣"));
	}
}

//��ʱĬ�ϳ�ʼ��
void Tab_Develop::OnBnClickedBtninithour()
{
	CString uid = GetCardUID();
	uid.Format(L"1111");
	if (uid != NOCARD) {			//�ܴӿ�Ƭ�ж�ȡ��uid
		if (mysql.sql_isExist(uid, ONTABLE)) {	//�����OnTable�����ҵ����uid�����ϻ�
			MessageBox(L"�ϻ��У����������ٳ�ʼ����ʱ");
		}
		else {
			if (mysql.sql_isExist(uid, REMAINTIMETABLE)) {	//���֮ǰ�Ǽǹ���������RemainTimeTable
				if (mysql.sql_update(uid, INITTIME, REMAINTIMETABLE))	//��ֱ�Ӹ������ݾͺ�
					((CEdit*)GetDlgItem(IDC_EDHOURSTATE))->SetWindowTextW(_T("��ʼ����ʱ�ɹ�"));
				else
					MessageBox(L"��ʼ����ʱ��������ϵ����Ա");
			}
			else {	//������������
				RemainTime remain(uid, INITTIME);
				if (mysql.sql_insertRemain(remain))
					((CEdit*)GetDlgItem(IDC_EDHOURSTATE))->SetWindowTextW(_T("��ʼ����ʱ�ɹ�"));
				else
					MessageBox(L"��ʼ����ʱ��������ϵ����Ա");
			}
		}
	}
	else {
		((CEdit*)GetDlgItem(IDC_EDHOURSTATE))->SetWindowTextW(_T("��ȡ�����쳣"));
	}
}

//��ʱ��ѯ
void Tab_Develop::OnBnClickedBtncheckhour()
{
	CString uid = GetCardUID();
	uid.Format(L"1111");
	if (uid != NOCARD) {
		//��������ϻ�����û����Ҫ��ѯ
		if(mysql.sql_isExist(uid,ONTABLE)) {
			((CEdit*)GetDlgItem(IDC_EDHOURSTATE))->SetWindowTextW(_T("�ϻ��У������ѯ"));
		}
		else {
			if (!mysql.sql_isExist(uid, REMAINTIMETABLE))
				MessageBox(L"�޼�¼�����ȳ�ʼ����ʱ");
			else {
				RemainTime * remaintime = (RemainTime*)mysql.remainTime_query(uid);
				showTime(remaintime->RemainSeconds);			//������ʾ
				((CEdit*)GetDlgItem(IDC_EDHOURSTATE))->SetWindowTextW(_T("��ѯ��ʱ�ɹ�"));
			}
		}
	}
	else {
		((CEdit*)GetDlgItem(IDC_EDHOURSTATE))->SetWindowTextW(_T("��ȡ�����쳣"));
	}
}

/////��ʷ��¼
//��ռ�¼
void Tab_Develop::OnBnClickedBtnempty()
{
	((CEdit*)GetDlgItem(IDC_EDHISTORY))->SetWindowTextW(_T(""));
}

//���ؼ�¼
void Tab_Develop::OnBnClickedBtnload()
{
	((CEdit*)GetDlgItem(IDC_EDHISTORY))->SetWindowTextW(fileRecordHelper.LoadRecords());
}

void Tab_Develop::OnTimer(UINT_PTR nIDEvent) {
	// �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (timenow > 0) {
		timenow = timenow - 1;
		showTime(timenow);
	}
	else {
		timenow = 0;
		showTime(timenow);
		KillTimer(1);
		MessageBox(L"��ʱΪ�㣬���ֵ");
		OnBnClickedBtnexit();
	}
	CDialogEx::OnTimer(nIDEvent);
}