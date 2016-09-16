

#ifndef _WIFI_INIT_h_
#define _WIFI_INIT_h_

#include "msp430x54x.h"
#include "Clock.h"
#include "uart2.h"
#include "SCI.h"
#include "Fstring.h"


#define WIFI_TX_UP       P5SEL &= ~BIT6;\
                         P5OUT |= BIT6

#define WIFI_TX_DOWN     P5SEL &= ~BIT6;\
                         P5OUT &= ~BIT6
                                         //�͵�ƽ���ѣ��ر�ģ��Ҳ���ֵ͵�ƽ��
#define WIFI_TX_ON       P5OUT |= BIT6;\
                         P5SEL |= BIT6  //�͵�ƽ���ѣ��ر�ģ��Ҳ���ֵ͵�ƽ��

#define WIFI_RX_DOWN       //�رպ󱣳ֵ͵�ƽ��

#define WIFI_UART_ON     UCA1CTL1 &= ~UCSWRST;\
                         P5DIR |= BIT6

#define WIFI_UART_OFF    UCA1CTL1 |= UCSWRST;\
                         P5DIR &= ~BIT6


#define wifi_DEBUG               //������Ϸģʽ
#define REMAX        128          //����������

//��������ģʽ
char  wifi_start(void);
//�˳�����ģʽ��д��1����wifiģ��
char  wifi_end(char mode);
//д������
char  wifi_command(char *command , char* data,char mode);  
//д��ip��ַ�Ͷ˿ں�
char  wifi_IP(unsigned char *ip,unsigned int port);  
//д��·���������ֺ�����
char  wifi_AP(char *name,char *password);   
//����Ƿ�������PTC��1 ���ӣ�0�Ͽ�
char  wifi_TCPtest(void);      
//�����������Ƿ������Ҫ��AP
char  SearchAP(char* name);     

extern int  uart2_num;        //���ڽ�������
extern char uart2_error;      //�������������

extern char wifi_getf;       //WIFIģ����յ����ݱ�־
extern char wifi_data[REMAX];//WIFIģ�����ݽ�������

#endif