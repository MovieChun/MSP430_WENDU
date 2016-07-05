/**********************************************************************
      �ļ�����uart.h
      ˵��  ����������
              �����ʣ�115200
              �жϽ���
              ���� ��TXD -> P10.4 -> Pin80
                     RXD -> P10.5 -> Pin81
      ʱ��  ��2016.05.23
      ����  ���Ѳ���
**********************************************************************/
#ifndef _uart_h_
#define _uart_h_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define UART_PORT_SEL   P10SEL                       //����IO��
#define UART_PORT_DIR   P10DIR

#define UART_TXD   BIT4
#define UART_RXD   BIT5


extern void UART_init(void);                       //���ڳ�ʼ��  ���پ�ȷģʽ
extern void UART_send(char *tx_buf);               //�����ַ�������
extern void UART_send2(char *tx_buf,char num);     //�������麯��
extern void UART_send_num(unsigned int num);       //�������麯��
extern void UART_send_char(char tx_buf);            //����һ���ַ�
extern void UART_send_float(float f2);

#endif