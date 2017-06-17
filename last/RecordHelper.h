#pragma once

#pragma comment(lib, "./lib/ZM124U.lib")
#include "./lib/ZM124U.h"

#define SAVEFILENAME _T("./records.txt")
#define TIMEFORMAT _T("%Y/%m/%d %H:%M:%S")

class CRecordHelper {
public:
	CRecordHelper(CString _mSaveFile = SAVEFILENAME);
	virtual ~CRecordHelper();

	// ����Ǯ������ ��ֵ ������¼����
	void SaveRecharges(CString uid, CString accounts, long remainings, CString result);
	void SaveConsumptions(CString uid, CString accounts, long remainings, CString result);
	// �ϻ������ϻ� �˳�������¼����
	void StartNets(CString uid, int remainTime, CString result);
	void ExitNets(CString uid, int startTime, int remainTime, CString result);
	// ���ؼ�¼
	CString LoadRecords();
	// ��ռ�¼
	BOOL EmptyRecords();
private:
	// ������ʷ��¼�ļ���
	CString mSaveFile;
};

