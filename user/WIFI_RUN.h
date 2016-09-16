
#ifndef _WIFI_RUN_H_
#define _WIFI_RUN_H_

#include "msp430x54x.h"
#include "WIFI_INIT.h"

extern char APname[20];
extern char APkey[20];
extern unsigned char WIFI_IP[4];
extern unsigned int  WIFI_PORT;

//输入口
#define WIFI_SLEEP    (P4OUT & BIT6)//wifi睡眠引脚,睡眠状态为低
#define WIFI_READY    (P4OUT & BIT5)//wifi准备引脚，低时可通信

//输出口
#define WIFI_REST_ON
#define WIFI_REST_OFF


#define WIFI_LED_INIT   P4DIR |= BIT7
#define WIFI_LED_ON     P4OUT |= BIT7
#define WIFI_LED_OFF    P4OUT &= ~BIT7
#define WIFI_LED_TURN 

void  wifi_init(void);        //wifi初始化
void  wifi_sleep(int mode);  //休眠模式选择
void  wifi_wake(int mode);   //唤醒
int   wifi_send_data(int num ,float *data,int n,int flag); //发送数据包，数量，数组名，异常标志
void  wifi_close(void);      //关闭wifi模块
char* wifi_SR(char*s);  //用于调试模块
void  wifi_gets(void);

#endif