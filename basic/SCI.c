/**********************************************************************
      �ļ�����sci.c
      ˵��  ����������
              �����ʣ�9600
              �жϽ���
              ���� ��TXD -> P10.4 -> Pin80
                     RXD -> P10.5 -> Pin81
      ʱ��  ��2016.05.23
      ����  ���Ѳ���
**********************************************************************/
#include "msp430x54x.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "SCI.h"

/**********************************************
 �����event_SCI  ���ձ�־λ�����λ��λ
       UCA3RXBUF  ���յ���һ���ֽ�         
***********************************************/

char  event_SCI, RXBuffer_SCI;

/***************************************************************************
                                                                           
������UART_init                                                         
˵�������ڳ�ʼ������
      ʹ��ACLK,�ⲿ����ʱ�ӣ�32KHZ
      �Ƚ�ʡ��
��������                                        
                                                                          
***************************************************************************/

void UART_init_L(void)
{ 
  RS_PORT_SEL    |= TXD + RXD                                      ; // ѡ�����Ź���
  RS_PORT_DIR    |= TXD                                            ; // ѡ�����Ź���
  UCA3CTL1        = UCSWRST                                        ; // ״̬����λ
  UCA3CTL1       |= UCSSEL_1                                       ; // CLK = ACLK
  UCA3BR0         = 0x03                                           ; // 32kHz/9600=3.41 
  UCA3BR1         = 0x00                                           ; 
  UCA3MCTL        = UCBRS_3 + UCBRF_0                              ; // UCBRSx=3, UCBRFx=0
  UCA3CTL1       &= ~UCSWRST                                       ; // ����״̬��
  UCA3IE         |= UCRXIE                                         ; // ��������ж�

}



/**********************************************************************
������UART_init                                                         
˵�������ڳ�ʼ������
      ʹ��smclk, 16MHZ  
��������  
���䣺
      ʹ��16λУ������ߴ������ȷ��
      
**********************************************************************/

void UART_init_H(void)
{ 
  RS_PORT_SEL    |= TXD + RXD                                      ; // ѡ�����Ź���
  RS_PORT_DIR    |= TXD                                            ; // ѡ�����Ź���
  UCA3CTL1        = UCSWRST                                        ; // ״̬����λ
  UCA3CTL1       |= UCSSEL_2                                       ; // CLK = smclk
  UCA3BR0         = 104                                            ; // 16mHz/9600/16=0x68 
  UCA3BR1         = 0x00                                           ; 
  UCA3MCTL        = UCBRS_0 + UCBRF_3                              ; // UCBRSx=3, UCBRFx=0
  UCA3MCTL       |= UCOS16                                         ; //����16�β���
  UCA3CTL1       &= ~UCSWRST                                       ; // ����״̬��
  UCA3IE         |= UCRXIE                                         ; // ��������ж�
}

/***************************************************************************
                                                                           
������UART_send                                                         
˵�������ڷ����ַ�������
������tx_buf -- �ַ����׵�ַ 
���磺UART_send("hallo word\n");
                                                                          
***************************************************************************/
void UART_send(char *tx_buf)
{
  unsigned char i,length                                           ;
  length = strlen(tx_buf)                                          ;
  for(i=0;i<length;i++)
  {
    UCA3TXBUF = *tx_buf++                                          ; 
    while (!(UCA3IFG&UCTXIFG))                                     ; 
  }
}

/***************************************************************************
                                                                           
������UART_send_num                                                         
˵�������ڷ�������
������tx_buf -- ������   num -- ���鳤��
                                                                          
***************************************************************************/

void UART_send_num(char *tx_buf,char num)
{
  unsigned char i ;
  for(i=0;i<num;i++)
  {
    UCA3TXBUF = *tx_buf++                                          ; 
    while (!(UCA3IFG&UCTXIFG))                                     ; 
  }
}

/***************************************************************************
    �жϽ��պ���
    �����event_SCI  ���ձ�־λ�����λ��λ
          UCA3RXBUF  ���յ���һ���ֽ�       
    ������������
***************************************************************************/
/*
#pragma vector=USCI_A3_VECTOR
__interrupt void USCI_A3_ISR(void)
{
  switch(__even_in_range(UCA3IV,4))
  {
  case 0:break                                                   ; // Vector 0 - no interrupt
  case 2:                                                            // Vector 2 - RXIFG
      RXBuffer_SCI   = UCA3RXBUF                                 ;
      event_SCI     |= 0x01                                      ;
      break                                                      ;
  case 4:break                                                     ;  // Vector 4 - TXIFG
  default: break                                                   ;  
  }  
}
*/





