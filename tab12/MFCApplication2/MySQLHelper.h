#pragma once
#pragma comment(lib, "./lib/ZM124U.lib")
#include "./lib/ZM124U.h"
#include <winsock2.h>
#include <mysql.h>
#pragma comment(lib, "libmysql.lib")
#include "Helper.h"
#include <afxstr.h>

#define DEFAULTREMAINTIME 20
#define ONTABLE _T("OnTable")
#define REMAINTIMETABLE _T("RemainTimeTable")
#define TIMEFORMAT _T("%Y/%m/%d %H:%M:%S")

// RemainTimeTable
struct RemainTime
{
	CString UID;
	int RemainSeconds;
	RemainTime(CString _UID, int _RemainSeconds) {
		UID = _UID;
		RemainSeconds = _RemainSeconds;
	}
};

// OnTable�ϻ����¼�ṹ
struct OnRecord
{
	CString UID;
	int RemainSeconds;
	CString StartTime;
	BOOL isOvertime;
	OnRecord(CString _UID, int _RemainSeconds, CString _StartTime, BOOL _isOverTime = false) {
		UID = _UID;
		RemainSeconds = _RemainSeconds;
		StartTime = _StartTime;
		isOvertime = _isOverTime;
	}
};


class CAdoMySQLHelper
{
public:
	// ���캯��
	CAdoMySQLHelper();
	// virtual����Ϊ������ʱ��ֹֻ�������������������������
	virtual ~CAdoMySQLHelper();

	// �������ݿ�
	BOOL MySQL_Connect();
	// �ر����ݿ�
	void MySQL_Close();

	// ���ݿ���ɾ�Ĳ��������
	// �����ݽ��в������
	BOOL MySQL_Insert(RemainTime record);
	BOOL MySQL_Insert(OnRecord record);
	// �����ݽ���ɾ������
	BOOL MySQL_Delete(CString uid, CString table);
	// �����ݿ���и��²���
	BOOL MySQL_UpdateRemainTime(CString uid, int updateTime, CString table);
	// �����ݿ�ִ��sql���
	BOOL MySQL_ExecuteSQL(CString sql);
	// �����ݿ���в�ѯ����, ��ѯ����cond = " UID='xxxx' "
	void* MySQL_Query(CString cond, CString table);
	// �����ݿ���в�ѯ����, ��ѯUID�Ƿ���ڱ���
	BOOL MySQL_QueryByUID(CString uid, CString table);
	// ��ʱɨ��OnTable, ʹ��ɨ������Timer���µ�ǰ�ϻ��û���ʱ����׽��ʱ�û�
	void MySQL_ScanOnTable(int timer);

private:
	// ����򿪵����ݿ���������ָ��
	_ConnectionPtr m_pConnection;
};