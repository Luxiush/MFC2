#include "Helper.h"
#include <odbcinst.h> 
#include "afxdb.h"


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
struct OnTable
{
	CString UID;
	int RemainSeconds;
	CString StartTime;
	BOOL isOvertime;
	OnTable(CString _UID, int _RemainSeconds, 
		CString _StartTime, BOOL _isOverTime = false) {
		UID = _UID;
		RemainSeconds = _RemainSeconds;
		StartTime = _StartTime;
		isOvertime = _isOverTime;
	}
};

class ODBCMySQLHelper {
private:
	CDatabase db;
public:

	ODBCMySQLHelper();
	~ODBCMySQLHelper();

	bool sql_connect();
	void sql_close();

	//��ѯ
	RemainTime * remainTime_query(CString uid);		//��ѯRemainTimeTable�����ؽṹ��
	OnTable * ontable_query(CString uid);			//��ѯOnTable��...
	bool sql_isExist(CString uid, CString table);	//��ѯuid������table�Ƿ����

	//���ݲ���
	bool sql_insertRemain(RemainTime record);		//remaintime��
	bool sql_insertOn(RemainTime record);			//ontable��

	//����ɾ��
	bool sql_delete(CString uid, CString table = _T("ontable"));

	//���ݸ���
	bool sql_update(CString uid, int time, CString table);

};