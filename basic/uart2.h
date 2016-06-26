/**********************************************************************
      �ļ�����uart2.h
      ˵��  ����������, ����WIFIģ��
              �����ʣ�115200
              �жϽ���
              ���� ��TXD -> P5.6 -> Pin53
                     RXD -> P5.7 -> Pin54
      ʱ��  ��2016.05.23
      ����  ���Ѳ���
**********************************************************************/
#ifndef _uart_h_
#define _uart_h_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

extern void UART2_init(void);                       //���ڳ�ʼ��  ���پ�ȷģʽ
extern void UART2_send(char *tx_buf);               //�����ַ�������
extern void UART2_sendn(char *tx_buf,char num);     //����n���ַ�
extern void UART2_send_num(unsigned int num);       //�������麯��
extern void UART2_send_char(char tx_buf);            //����һ���ַ�

#endif