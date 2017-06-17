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


////Ǯ���Ĳ���
//Ǯ����ʼ��
void Tab_Develop::OnBnClickedBtninitpurse()
{
	CString balance;
	((CEdit*)GetDlgItem(IDC_EDBANLANCE))->GetWindowTextW(balance);
	if (balance.IsEmpty()) {
		// �������
		MessageBox(_T("��������ȷ�ĳ�ʼ����"));
		return;
	}
	long account = _ttol(balance);
	if (account < 0) {
		// �������
		MessageBox(_T("��ʼ���������"));
		return;
	}
	int sectionNum = 15;
	int blockNum = 0;
	unsigned char pswtype = 0x0A;
	unsigned char chpwd[8];
	int len_chpwd = 0;
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
	int sectionNum = 15;
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
		((CEdit*)GetDlgItem(IDC_EDPURSESTATE))->SetWindowTextW(balance);
		canIOPurse = true;
		LEDSet(account); // MayBe Wrong
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
	if (addAccount.IsEmpty()) {
		// ����ֵ����
		MessageBox(_T("��������ȷ�ĳ�ֵ��"));
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
	// ��Կת��
	CString pwd = _T("FFFFFFFFFFFF");
	HexCString2UnsignedCharStar(pwd, chpwd, &len_chpwd);
	// ��ȡ����
	CString uid = GetCardUID();
	// ��ֵ����
	if (add_account(sectionNum, blockNum, pswtype, chpwd, account) == IFD_OK) {
		OnBnClickedBtncheckban();
		canIOPurse = true;
		((CEdit*)GetDlgItem(IDC_EDPURSESTATE))->SetWindowTextW(_T("��ֵ�ɹ�"));
		fileRecordHelper.SaveRecharges(uid, addAccount, account + _ttol(balance), _T("�ɹ�"));
		
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
	if (subAccount.IsEmpty()) {
		// �������
		MessageBox(_T("��������ȷ�����ѽ�"));
		return;
	}
	CString balance;
	((CEdit*)GetDlgItem(IDC_EDBANLANCE))->GetWindowTextW(balance);
	long account = _ttol(subAccount);
	if (account > _ttol(balance)) {
		// ������
		MessageBox(_T("���㣡"));
		return;
	}
	int sectionNum = 15;
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
	CDatabase database;
	if (database.Open(_T("mysql5.5"))) {
		database.ExecuteSQL(_T("insert into remaintimetable(UID) values('2222')"));
		database.Close();
	}
}

//����
void Tab_Develop::OnBnClickedBtnexit()
{
	
}

//��ʱĬ�ϳ�ʼ��
void Tab_Develop::OnBnClickedBtninithour()
{
	
}

//��ʱ��ѯ
void Tab_Develop::OnBnClickedBtncheckhour()
{
	
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

