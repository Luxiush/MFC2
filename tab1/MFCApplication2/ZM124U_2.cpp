#include "stdafx.h"
#include "afxdialogex.h"
#include <stdlib.h>
#include <stdio.h>

#include "ZM124U_2.h"


int IDD_PowerOn(void){

	return 0;
}

int find_14443(unsigned char* uid, int* uid_len){
	const int len = 6;
	unsigned char uid_tmp[len] = {0x12,0x34,0x56,0x78,0x9a,0xbc};

	memcpy(uid, uid_tmp, len);
	*uid_len = len;
	printf("find_14443: uid: %s, uid_len: %s\n", uid, uid_len);
	return 0;
}

int Reader_Version(unsigned char *resp,int *resp_len){
	const int len = 8;
	unsigned char tmp[len] = "v12.3.4";

	memcpy(resp, tmp, len);
	*resp_len = len;

	printf("Reader_Version: resp: %s, resp_len: %s\n", resp, resp_len);
	return 0;
}

int LED(char *data,int data_len,unsigned char point){
	printf("LED: data: %s, data_len: %d, point: %c\n", data, data_len, point);
	return 0;
}


int read_block(int page, int block, 
	unsigned char pswtype, unsigned char *psw,
	unsigned char *des_data, int* des_len)
{
	//参数合法性检查
	if (page < 0 || page>15) return -14;
	if (block < 0 || block>3) return -13;
	if (pswtype != 10 && pswtype != 11) return -12;

	des_data[0] = (page<<4) | (block&0xf);
	des_data[1] = pswtype<<4;

	for (int i = 0; i < 6; ++i){
		des_data[2 + i] = psw[i];
	}
	if (des_len) *des_len = 16;

	return 0;
}

int write_block(int block, int page,
	unsigned char pswtype, unsigned char *psw,
	unsigned char *src_data, int src_len)
{
	//参数合法性检查
	if (page < 0 || page>15) return -14;
	if (block < 0 || block>3) return -13;
	if (pswtype != 10 && pswtype != 11) return -12;
	if (src_len != 16) return -11;

	return 0;
}







