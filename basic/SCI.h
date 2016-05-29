/**********************************************************************
      �ļ�����sci.h
      ˵��  ����������
              �����ʣ�9600
              �жϽ���
              ���� ��TXD -> P10.4 -> Pin80
                     RXD -> P10.5 -> Pin81
      ʱ��  ��2016.05.23
      ����  ���Ѳ���
**********************************************************************/
#ifndef _sci_h_
#define _sci_h_

#define RS_PORT_SEL   P10SEL                       //����IO��
#define RS_PORT_DIR   P10DIR

#define TXD   BIT4
#define RXD   BIT5

extern char  event_SCI, RXBuffer_SCI;              //��ȡ��־λ�����յ�����
extern void UART_init_L(void);                     //���ڳ�ʼ��  ����ʡ��ģʽ
extern void UART_init_H(void);                     //���ڳ�ʼ��  ���پ�ȷģʽ
extern void UART_send(char *tx_buf);               //�����ַ�������
extern void UART_send2(char *tx_buf,char num);     //�������麯��
extern void UART_send_num(unsigned int num);       //�������麯��
#endif