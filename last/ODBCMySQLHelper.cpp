#include "stdafx.h"
#include "ODBCMySQLHelper.h"

ODBCMySQLHelper::ODBCMySQLHelper() {
	this->sql_connect();
}

ODBCMySQLHelper::~ODBCMySQLHelper() {
	this->sql_close();
}

bool ODBCMySQLHelper::sql_connect() {
	return db.Open(_T("mysql5.5"));
}

void ODBCMySQLHelper::sql_close() {
	db.Close();
}

RemainTime * ODBCMySQLHelper::remainTime_query(CString uid) {
	CString order;
	order.Format(L"select * from remainTimeTable where UID=\'%s\'", uid);
	RemainTime * remain;
	CRecordset rs(&db);
	rs.Open(CRecordset::forwardOnly, order);
	if (rs.IsEOF()) {
		remain = new RemainTime(L"0000", int(0000));
	}
	else {
		CString UID, retime;
		int RemainSeconds;
		rs.GetFieldValue(L"UID", UID);
		rs.GetFieldValue(L"RemainTime", retime);
		RemainSeconds = _ttoi(retime);
		remain = new RemainTime(UID, RemainSeconds);
	}
	return remain;
}

OnTable * ODBCMySQLHelper::ontable_query(CString uid) {
	OnTable * ontable;
	CString order;
	order.Format(L"select * from ontable where UID=\'%s\'", uid);
	CRecordset rs(&db);
	rs.Open(CRecordset::forwardOnly, order);
	if (rs.IsEOF()) {
		ontable = new OnTable(L"0000", int(0000), L"0000");
	}
	else {
		CString UID, retime, start, flag;
		int RemainSeconds;
		bool myflag;
		rs.GetFieldValue(L"UID", UID);
		rs.GetFieldValue(L"RemainTime", retime);
		rs.GetFieldValue(L"StartTime", start);
		rs.GetFieldValue(L"isOvertime", flag);
		RemainSeconds = _ttoi(retime);
		if (flag == L'0') myflag = false;
		else myflag = true;
		ontable = new OnTable(UID, RemainSeconds, start, myflag);
	}
	return ontable;
}

bool ODBCMySQLHelper::sql_isExist(CString uid, CString table) {
	CString order;
	order.Format(L"select * from %s where UID=\'%s\';", table, uid);
	if (!db.IsOpen()) {		//检查是否连接数据库
		return false;
	}
	else {
		//执行查询并将接收数据到rs中
		CRecordset rs(&db);	
		rs.Open(CRecordset::forwardOnly, order);

		if (rs.IsEOF()) return false;	//查询为空
		return true;					//查询成功
	}
}

bool ODBCMySQLHelper::sql_insertRemain(RemainTime record) {
	CString order;
	order.Format(L"insert into RemainTimeTable values(\'%s\',\'%d\');", record.UID, record.RemainSeconds);
	if (!db.IsOpen())
		return false;
	else {
		if(sql_isExist(record.UID, L"remaintimeTable"))
			return false;
		else {
			db.ExecuteSQL(order);
			return true;
		}
	}
}

bool ODBCMySQLHelper::sql_insertOn(RemainTime record) {
	CString order;
	order.Format(_T("insert into OnTable(UID, RemainTime) values(\'%s\', \'%d\');"), record.UID, record.RemainSeconds);
	if(db.IsOpen())
		if (!sql_isExist(record.UID, L"OnTable")) {
			db.ExecuteSQL(order);
			return true;
		}
	return false;
}

bool ODBCMySQLHelper::sql_delete(CString uid, CString table) {
	CString order;
	order.Format(_T("delete from %s where UID= \'%s\';"), table, uid);
	if (db.IsOpen())
		if (sql_isExist(uid, L"ontable")) {
			db.ExecuteSQL(order);
			return true;
		}
	return false;
}

bool ODBCMySQLHelper::sql_update(CString uid, int time, CString table) {
	CString order;
	order.Format(_T("Update %s set RemainTime = \'%d\'where UID = \'%s\';"), table, time, uid);
	if (db.IsOpen()) {
		db.ExecuteSQL(order);
		return true;
	}
	return false;
}