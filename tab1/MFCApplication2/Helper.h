# ifndef HELPER_H_
# define HELPER_H_

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

#endif