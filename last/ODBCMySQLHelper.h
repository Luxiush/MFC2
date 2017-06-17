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

// OnTable上机表记录结构
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

	//查询
	RemainTime * remainTime_query(CString uid);		//查询RemainTimeTable并返回结构体
	OnTable * ontable_query(CString uid);			//查询OnTable并...
	bool sql_isExist(CString uid, CString table);	//查询uid数据在table是否存在

	//数据插入
	bool sql_insertRemain(RemainTime record);		//remaintime表
	bool sql_insertOn(RemainTime record);			//ontable表

	//数据删除
	bool sql_delete(CString uid, CString table = _T("ontable"));

	//数据更新
	bool sql_update(CString uid, int time, CString table);

};