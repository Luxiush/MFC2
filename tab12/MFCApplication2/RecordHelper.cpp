#include "stdafx.h"
#include "RecordHelper.h"


BOOL FileUnicodeEncode(CFile &mFile) {
	// �ú���ֻ����mFile.Close()ǰ����
	WORD unicode = 0xFEFF;
	mFile.SeekToBegin();
	mFile.Write(&unicode, 2); // Unicode
	return true;
}

CRecordHelper::CRecordHelper(CString _mSaveFile) {
	// ���ļ�·��
	this->mSaveFile = _mSaveFile;
}


CRecordHelper::~CRecordHelper() {

}

void CRecordHelper::SaveRecharges(CString uid, CString accounts, long remainings, CString result) {
	// ���ļ�
	CFile mFile(this->mSaveFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite);
	// ��ȡ��ǰʱ��
	CTime curTime = CTime::GetCurrentTime();
	// ��ʽ�����
	CString contents;
	contents.Format(_T("���ţ�%s\r\nʱ�䣺%s\r\n�����%s\r\n���ݣ��û���ֵ\r\n��%s\r\n��%d\r\n\r\n"),
		uid, curTime.Format(TIMEFORMAT), result, accounts, remainings);
	// ָ���ļ�ĩβ��д��
	mFile.SeekToEnd();
	mFile.Write(contents, wcslen(contents)*sizeof(wchar_t));
	// �ر��ļ�
	mFile.Close();
}

void CRecordHelper::SaveConsumptions(CString uid, CString accounts, long remainings, CString result) {
	// ���ļ�
	CFile mFile(this->mSaveFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite);
	// ��ȡ��ǰʱ��
	CTime curTime = CTime::GetCurrentTime();
	// ��ʽ�����
	CString contents;
	contents.Format(_T("���ţ�%s\r\nʱ�䣺%s\r\n�����%s\r\n���ݣ��û�����\r\n��%s\r\n��%d\r\n\r\n"),
		uid, curTime.Format(TIMEFORMAT), result, accounts, remainings);
	// ָ���ļ�ĩβ��д��
	mFile.SeekToEnd();
	mFile.Write(contents, wcslen(contents)*sizeof(wchar_t));
	// �ر��ļ�
	mFile.Close();
}

void CRecordHelper::StartNets(CString uid, int remainTime, CString result) {
	// ���ļ�
	CFile mFile(this->mSaveFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite);
	// ��ȡ��ǰʱ��
	CTime curTime = CTime::GetCurrentTime();
	// ��ʽ����ʱ
	CString sRemainTime;
	sRemainTime.Format(_T("%dʱ%d��%d��"), remainTime / 3600, (remainTime % 3600) / 60, remainTime % 60);
	// ��ʽ�����
	CString contents;
	contents.Format(_T("���ţ�%s\r\nʱ�䣺%s\r\n�����%s\r\n���ݣ��û��ϻ�\r\n��ʱ��%s\r\n\r\n"),
		uid, curTime.Format(TIMEFORMAT), result, sRemainTime);
	// ָ���ļ�ĩβ��д��
	mFile.SeekToEnd();
	mFile.Write(contents, wcslen(contents)*sizeof(wchar_t));
	// �ر��ļ�
	mFile.Close();
}

void CRecordHelper::ExitNets(CString uid, int remainTime, int overSeconds, CString result) {
	CFile mFile(this->mSaveFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite);
	// ��ȡ��ǰʱ��
	CTime curTime = CTime::GetCurrentTime();
	// ��ʽ���ϻ�ʱ��
	CString sOverTime;
	sOverTime.Format(_T("%dʱ%d��%d��"), overSeconds / 3600, (overSeconds % 3600) / 60, overSeconds % 60);
	// ��ʽ����ʱ
	CString sRemainTime;
	sRemainTime.Format(_T("%dʱ%d��%d��"), remainTime / 3600, (remainTime % 3600) / 60, remainTime % 60);
	// ��ʽ�����
	CString contents;
	contents.Format(_T("���ţ�%s\r\nʱ�䣺%s\r\n�����%s\r\n���ݣ��û��˳�\r\n��ʱ��%s\r\n"),
		uid, curTime.Format(TIMEFORMAT), result, sRemainTime);
	//CString contents; // TODO: �ϻ�ʱ�䣨��ʱ���Ƚ�����������
	//contents.Format(_T("���ţ�%s\r\nʱ�䣺%s\r\n�����%s\r\n���ݣ��û��˳�\r\n��ʱ:%s\r\n��ʱ��%s\r\n"),
	//	uid, curTime.Format(TIMEFORMAT), result, sOverTime, sRemainTime);
	// ָ���ļ�ĩβ��д��
	mFile.SeekToEnd();
	mFile.Write(contents, wcslen(contents)*sizeof(wchar_t));
	// �ر��ļ�
	mFile.Close();
}

CString CRecordHelper::LoadRecords() {
	// ���ļ�
	CStdioFile mFile(this->mSaveFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeRead | CFile::typeUnicode);
	// ָ��ͷ��ѭ������
	mFile.SeekToBegin();
	CString contents, line, multiLine;
	contents.Empty();
	multiLine.Empty();
	//  ����ֶζ�ȡ
	while (mFile.ReadString(line)) {
		line.Trim();
		if (line.IsEmpty()) {
			contents = multiLine + _T("\r\n") + contents;
			multiLine.Empty();
		}
		else {
			multiLine += (line + _T("\r\n"));
		}
	}
	contents = multiLine + _T("\r\n") + contents;
	// �ر��ļ������ؽ��
	mFile.Close();
	return contents;
}

BOOL CRecordHelper::EmptyRecords() {
	// ����ļ�
	CFile mFile(this->mSaveFile, CFile::modeCreate | CFile::modeReadWrite);
	FileUnicodeEncode(mFile);
	mFile.Close();
	return true;
}