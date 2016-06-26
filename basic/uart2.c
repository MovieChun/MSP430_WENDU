/**********************************************************************
      �ļ�����uart2.c
      ˵��  ���������� , ����WIFIģ��
              �����ʣ�115200
              �жϽ���
              ���� ��TXD -> P5.6 -> Pin53
                     RXD -> P5.7 -> Pin54
      ʱ��  ��2016.05.23
      ����  ���Ѳ���
**********************************************************************/
#include "msp430x54x.h"
#include <stdlib.h>
#include <stdio.h>
#include "uart2.h"

/**********************************************************************
������UART2_init                                                         
˵�������ڳ�ʼ������
      ʹ��smclk, 16MHZ  
��������  
���䣺
      ʹ��16λУ������ߴ������ȷ��
      
**********************************************************************/

void UART2_init(void)
{ 
  P5SEL          |= BIT7 + BIT6                                    ; // ѡ�����Ź���
  P5DIR          |= BIT6                                           ; // ѡ�����Ź���
  UCA1CTL1        = UCSWRST                                        ; // ״̬����λ
  UCA1CTL1       |= UCSSEL_2                                       ; // CLK = smclk
  UCA1BR0         = 8                                              ; // 16mHz 
  UCA1BR1         = 0x00                                           ; 
  UCA1MCTL        = UCBRS_0 + UCBRF_11                              ; // UCBRSx=0, UCBRFx=11
  UCA1MCTL       |= UCOS16                                         ; //����16�β���
  UCA1CTL1       &= ~UCSWRST                                       ; // ����״̬��
  UCA1IE         |= UCRXIE                                         ; // ��������ж�
}

/***************************************************************************
                                                                           
������UART2_send                                                         
˵�������ڷ����ַ�������
������tx_buf -- �ַ����׵�ַ 
���磺UART_send("hallo word\n");
                                                                          
***************************************************************************/
void UART2_send(char *tx_buf)
{
  unsigned char i,length                                           ;
  length = strlen(tx_buf)                                          ;
  for(i=0;i<length && *tx_buf!= '\0';i++)
  {
    UCA1TXBUF = *tx_buf++                                          ; 
    while (!(UCA1IFG&UCTXIFG))                                     ; 
  }
}


/***************************************************************************
                                                                           
������UART2_send_char                                                       
˵��������һ���ַ�
������tx_buf �����ַ�
                                                                          
***************************************************************************/

void UART2_send_char(char tx_buf)
{
    UCA1TXBUF = tx_buf                                             ; 
    while (!(UCA1IFG&UCTXIFG))                                     ;
}


/***************************************************************************
                                                                           
������UART2_sendn                                                        
˵�������ڷ���n���ַ�
������tx_buf -- ������   num -- ���鳤��
                                                                          
***************************************************************************/

void UART2_sendn(char *tx_buf,char num)
{
  unsigned char i ;
  for(i=0;i<num;i++)
  {
    UCA1TXBUF = *tx_buf++                                          ; 
    while (!(UCA1IFG&UCTXIFG))                                     ; 
  }
}


/***************************************************************************
                                                                           
������UART2_send_num                                                         
˵�������ڷ�������
������num �޷�������
                                                                          
***************************************************************************/

void UART2_send_num(unsigned int num)
{
  unsigned char i;
  char n[5];
 
  for(i = 0;i < 5;i++){
    n[4-i] = num % 10 + '0';
    num /= 10;
  }
  for(i = 0;i < 4 && '0' == n[i];i++);
  
  UART2_sendn(n + i,5 - i);
  
}

/***************************************************************************
                                                                           
������UART2_send_f2                                                         
˵�������ڷ��͸���������ȷ��С�������λ
������������
                                                                          
***************************************************************************/

void UART2_send_float(float f2)
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
#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void)
{
  switch(__even_in_range(UCA1IV,4))
  {
  case 0:break                                                   ; // Vector 0 - no interrupt
  case 2:                                                            // Vector 2 - RXIFG
      RXBuffer_SCI   = UCA1RXBUF                                 ;
      event_SCI     |= 0x01                                      ;
      break                                                      ;
  case 4:break                                                     ;  // Vector 4 - TXIFG
  default: break                                                   ;  
  }  
}
*/
