
#ifndef _WIFI_RUN_H_
#define _WIFI_RUN_H_

#include "msp430x54x.h"
#include "WIFI_INIT.h"

extern char APname[20];
extern char APkey[20];
extern unsigned char WIFI_IP[4];
extern unsigned int  WIFI_PORT;

//�����
#define WIFI_SLEEP    (P4OUT & BIT6)//wifi˯������,˯��״̬Ϊ��
#define WIFI_READY    (P4OUT & BIT5)//wifi׼�����ţ���ʱ��ͨ��

//�����
#define WIFI_REST_ON
#define WIFI_REST_OFF


#define WIFI_LED_INIT   P4DIR |= BIT7
#define WIFI_LED_ON     P4OUT |= BIT7
#define WIFI_LED_OFF    P4OUT &= ~BIT7
#define WIFI_LED_TURN 

void  wifi_init(void);        //wifi��ʼ��
void  wifi_sleep(int mode);  //����ģʽѡ��
void  wifi_wake(int mode);   //����
int   wifi_send_data(int num ,float *data,int n,int flag); //�������ݰ������������������쳣��־
void  wifi_close(void);      //�ر�wifiģ��
char* wifi_SR(char*s);  //���ڵ���ģ��
void  wifi_gets(void);

#endif