# ifndef HELPER_H_
# define HELPER_H_
#pragma comment(lib, "./lib/ZM124U.lib")
#include "./lib/ZM124U.h"

static CString GetIFDStatusString(int val){
	const static char str[20][20] = {
		"OK",
		"TypeError",
		"NoExist",
		"NoPower",
		"NoResponse",
		"BCCError",
		"TimeOut",
		"RunFail",
		"SiteFail",
		"etFail",
		"NoSlot",
		"ConnectError",
		"UnConnected",
		"BadCommand",
		"ParameterError",
		"CheckSumError",
		"PowerFail",
		"ResetFail",
		"PowerOffFail"
	};
	return CString(str[-val]);
}

// CString转为16进制串
static void CStr2HexStr(CString src, unsigned char* des, int* des_len){
	src.MakeUpper();
	if (src.GetLength() % 2 == 1){
		src.Insert(0, '0');
	}

	unsigned char tmp(0), t(0);
	for (int i = 0; i < src.GetLength(); ++i){
		if ('0' <= src[i] && src[i] <= '9'){
			tmp = src[i] - '0';
		}
		else if ('A' <= src[i] && src[i] <= 'F'){
			tmp = src[i] - 'A' + 10;
		}

		t = (t << 4) + tmp;
		if (i % 2 == 1){
			des[i/2] = t;
			t = 0;
		}
	}
	if (des_len) *des_len = src.GetLength() / 2;
}

// 16进制串转CString
static CString HexStr2CStr(unsigned char* data, int data_len){
	CString res("");
	for (int i = 0; i < data_len; ++i){
		res.AppendFormat(_T("%02x "), data[i]);

	}
	return res;
}

static void CString2CharStar(const CString& s, char* ch, int len) {
	int i;
	for (i = 0; i < len; i++) {
		ch[i] = s[i];
	}
	ch[i] = '\0';
	return;
}

static CString UnsignedChar2CStr(unsigned char* str, int str_len){
	CString res("");

	for (int i = 0; i < str_len; ++i)
		res += str[i];

	return res;
}

static void CStr2UnsignedChar(CString src, unsigned char* des, int* des_len){
	int len = src.GetAllocLength();
	memcpy(des, src, len);

	if (des_len != NULL){
		*des_len = len;
	}
}

static void HexCString2UnsignedCharStar(const CString& hexStr, unsigned char* asc, int* asc_len) {
	*asc_len = 0;
	int len = hexStr.GetLength();

	char temp[200];
	char tmp[3] = { 0 };
	char* Hex;
	unsigned char* p;

	CString2CharStar(hexStr, temp, len);
	Hex = temp;
	p = asc;

	while (*Hex != '\0') {
		tmp[0] = *Hex;
		Hex++;
		tmp[1] = *Hex;
		Hex++;
		tmp[2] = '\0';
		*p = (unsigned char)strtol(tmp, NULL, 16);
		p++;
		(*asc_len)++;
	}
	*p = '\0';
	return;
}


static bool LEDSet(long num) {
	// 设置LED
	// 最高支持6位数字
	if (num >= 0 && num <= 999999) {
		unsigned char point = 0x00; // 无小数
		CString strNum;
		strNum.Format(_T("%d"), num);
		int len_info = strNum.GetLength();
		char chinfo[7];
		CString2CharStar(strNum, chinfo, len_info);
		if (LED(chinfo, len_info, point) == IFD_OK) {
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}

#endif