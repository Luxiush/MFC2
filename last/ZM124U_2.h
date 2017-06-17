#ifndef _ZM124U_2_H_
#define _ZM124U_2_H_

//函数接口返回值，<0为错误码
enum
{
 IFD_OK = 0,            // 执行成功
 IFD_ICC_TypeError= -1, // 卡片类型不对
 IFD_ICC_NoExist=  -2,  // 无卡
 IFD_ICC_NoPower=  -3,  // 有卡未上电
 IFD_ICC_NoResponse= -4,// 卡片无应答
 IFD_ICC_BCCError=  -5, // BCC校验错误
 IFD_ICC_TimeOut=   -6, // 接收超时
 IFD_ICC_RunFail=   -7, // 执行失败
 IFD_ICC_SiteFail=  -8, // 卡片位置错误
 IFD_ICC_SetFail=   -9, // 设置失败
 IFD_NoSlot=        -10,// 无卡
 IFD_ConnectError= -11, // 读卡器连接错
 IFD_UnConnected= -12,  // 未建立连接(没有执行打开设备函数)
 IFD_BadCommand= -13,   // (动态库)不支持该命令
 IFD_ParameterError= -14,//(发给动态库的)命令参数错
 IFD_CheckSumError= -15, // 信息校验和出错
 IFD_ICC_PowerFail= -16, // 卡片上电失败
 IFD_ICC_ResetFail= -17, // 卡片复位失败
 IFD_ICC_PowerOffFail= -18,//卡片下电失败
};

int IDD_PowerOn(void);

int find_14443(unsigned char* uid,int* uid_len);

int Reader_Version(unsigned char *resp,int *resp_len);

int LED(char *data,int data_len,unsigned char point);


/*********************************************************************************
函数功能：读取M1卡块信息
参数说明：page：M1卡扇区号
block：M1卡块号
pswtype：密钥类型
psw：密钥值
des_data：读取的信息
des_len： 信息的长度
**********************************************************************************/
int read_block(int page,
	int block,
	unsigned char pswtype,
	unsigned char *psw,
	unsigned char *des_data,
	int* des_len);

/*********************************************************************************
函数功能：向M1卡写信息
参数说明：block：M1卡块号
page： M1卡扇区号
pswtype：密钥类型
psw：密钥值
src_data：写入的信息数据
src_len： 信息的长度
**********************************************************************************/
int write_block(int block,
	int page,
	unsigned char pswtype,
	unsigned char *psw,
	unsigned char *src_data,
	int src_len);


#endif