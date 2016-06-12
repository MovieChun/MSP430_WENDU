/**********************************************************************
      �ļ�����uart.c
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
#include "uart.h"

/**********************************************
 �����event_SCI  ���ձ�־λ�����λ��λ
       UCA3RXBUF  ���յ���һ���ֽ�         
***********************************************/




/**********************************************************************
������UART_init                                                         
˵�������ڳ�ʼ������
      ʹ��smclk, 16MHZ  
��������  
���䣺
      ʹ��16λУ������ߴ������ȷ��
      
**********************************************************************/

void UART_init(void)
{ 
  UART_PORT_SEL    |= UART_TXD + UART_RXD                          ; // ѡ�����Ź���
  UART_PORT_DIR    |= UART_TXD                                     ; // ѡ�����Ź���
  UCA3CTL1        = UCSWRST                                        ; // ״̬����λ
  UCA3CTL1       |= UCSSEL_2                                       ; // CLK = smclk
  UCA3BR0         = 8                                              ; // 16mHz 
  UCA3BR1         = 0x00                                           ; 
  UCA3MCTL        = UCBRS_0 + UCBRF_11                              ; // UCBRSx=0, UCBRFx=11
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
  for(i=0;i<length && *tx_buf!= '\0';i++)
  {
    UCA3TXBUF = *tx_buf++                                          ; 
    while (!(UCA3IFG&UCTXIFG))                                     ; 
  }
}

/***************************************************************************
                                                                           
������UART_send2                                                        
˵�������ڷ�������
������tx_buf -- ������   num -- ���鳤��
                                                                          
***************************************************************************/

void UART_send2(char *tx_buf,char num)
{
  unsigned char i ;
  for(i=0;i<num;i++)
  {
    UCA3TXBUF = *tx_buf++                                          ; 
    while (!(UCA3IFG&UCTXIFG))                                     ; 
  }
}


/***************************************************************************
                                                                           
������UART_send_num                                                         
˵�������ڷ�������
������num �޷�������
                                                                          
***************************************************************************/

void UART_send_num(unsigned int num)
{
  unsigned char i;
  char n[5];
 
  for(i = 0;i < 5;i++){
    n[4-i] = num % 10 + '0';
    num /= 10;
  }
  for(i = 0;i < 4 && '0' == n[i];i++);
  
  UART_send2(n + i,5 - i);
  
}

/***************************************************************************
                                                                           
������UART_send_f2                                                         
˵�������ڷ��͸���������ȷ��С�������λ
������������
                                                                          
***************************************************************************/

void UART_send_float(float f2)
{
  unsigned int num;
  
  if(f2 < 0){
     UART_send("-");
     f2 = -f2;
  }
  
  num = (unsigned int)f2;
  UART_send_num(num);
  UART_send(".");
  num = (unsigned int)(100 * f2) % 100;
  if(num < 10)UART_send("0");
  UART_send_num(num);
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


