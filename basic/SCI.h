/**********************************************************************
      �ļ�����sci.c
      ˵��  ����������
              �����ʣ�115200
              �жϽ���
              ���� ��TXD -> P9.4 -> Pin72
                     RXD -> P9.5 -> Pin73
      ʱ��  ��2016.05.23
      ����  : ������λ������
**********************************************************************/
#ifndef _sci_h_
#define _sci_h_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SCI_PORT_SEL   P9SEL                       //����IO��
#define SCI_PORT_DIR   P9DIR

#define SCI_TXD   BIT4
#define SCI_RXD   BIT5

#define SCI_MAX  512

extern int SCI_num;
extern char SCI_getf;
extern char SCI_data[SCI_MAX];
extern char SCI_error;

extern char event_SCI, RXBuffer_SCI;              //��ȡ��־λ�����յ�����
extern void SCI_init(void);                     //���ڳ�ʼ��  ����ʡ��ģʽ
extern void SCI_send(char *tx_buf);               //�����ַ�������
extern void SCI_send2(char *tx_buf,char num);     //�������麯��
extern void SCI_send_num(unsigned int num);       //��������
extern void SCI_send_float(float f2);       //����С�������λ���ȵĸ�����

#endif