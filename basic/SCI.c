/**********************************************************************
      �ļ�����sci.c
      ˵��  ����������
              �����ʣ�9600
              �жϽ���
              ���� ��TXD -> P9.4 -> Pin72
                     RXD -> P9.5 -> Pin73
      ʱ��  ��2016.05.23
      ����  : ������λ������
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
                                                                           
������SCI_init                                                         
˵�������ڳ�ʼ������
      ʹ��ACLK,�ⲿ����ʱ�ӣ�32KHZ
      �Ƚ�ʡ��
��������                                        
                                                                          
***************************************************************************/

void SCI_init(void)
{ 
  SCI_PORT_SEL    |= SCI_TXD + SCI_RXD                             ; // ѡ�����Ź���
  SCI_PORT_DIR    |= SCI_TXD                                       ; // ѡ�����Ź���
  
  UCA2CTL1        = UCSWRST                                        ; // ״̬����λ
  UCA2CTL1       |= UCSSEL_2                                       ; // CLK = smclk
  UCA2BR0         = 8                                              ; // 16mHz 
  UCA2BR1         = 0x00                                           ; 
  UCA2MCTL        = UCBRS_0 + UCBRF_11                              ; // UCBRSx=0, UCBRFx=11
  UCA2MCTL       |= UCOS16                                         ; //����16�β���
  UCA2CTL1       &= ~UCSWRST                                       ; // ����״̬��
  UCA2IE         |= UCRXIE                                         ; // ��������ж�
}

/***************************************************************************
                                                                           
������SCI_send                                                         
˵�������ڷ����ַ�������
������tx_buf -- �ַ����׵�ַ 
���磺SCI_send("hallo word\n");
                                                                          
***************************************************************************/
void SCI_send(char *tx_buf)
{
  unsigned char i,length                                           ;
  length = strlen(tx_buf)                                          ;
  for(i=0;i<length && *tx_buf!= '\0';i++)
  {
    UCA2TXBUF = *tx_buf++                                          ; 
    while (!(UCA2IFG&UCTXIFG))                                     ; 
  }
}

/***************************************************************************
                                                                           
������SCI_send2                                                        
˵�������ڷ�������
������tx_buf -- ������   num -- ���鳤��
                                                                          
***************************************************************************/

void SCI_send2(char *tx_buf,char num)
{
  unsigned char i ;
  for(i=0;i<num;i++)
  {
    UCA2TXBUF = *tx_buf++                                          ; 
    while (!(UCA2IFG&UCTXIFG))                                     ; 
  }
}


/***************************************************************************
                                                                           
������SCI_send_num                                                         
˵�������ڷ�������
������num �޷�������
                                                                          
***************************************************************************/

void SCI_send_num(unsigned int num)
{
  unsigned char i;
  char n[5];
 
  for(i = 0;i < 5;i++){
    n[4-i] = num % 10 + '0';
    num /= 10;
  }
  for(i = 0;i < 4 && '0' == n[i];i++);
  
  SCI_send2(n + i,5 - i);
  
}

/***************************************************************************
                                                                           
������SCI_send_f2                                                         
˵�������ڷ��͸���������ȷ��С�������λ
������������
                                                                          
***************************************************************************/

void SCI_send_float(float f2)
{
  unsigned int num;
  
  if(f2 < 0){
     SCI_send("-");
     f2 = -f2;
  }
  
  num = (unsigned int)f2;
  SCI_send_num(num);
  SCI_send(".");
  num = (unsigned int)(100 * f2) % 100;
  if(num < 10)SCI_send("0");
  SCI_send_num(num);
}

/***************************************************************************
    �жϽ��պ���
    �����event_SCI  ���ձ�־λ�����λ��λ
          UCA2RXBUF  ���յ���һ���ֽ�       
***************************************************************************/

#pragma vector=USCI_A2_VECTOR
__interrupt void USCI_A2_ISR(void)
{
  switch(__even_in_range(UCA2IV,4))
  {
  case 0:break                                                   ; // Vector 0 - no interrupt
  case 2:                                                            // Vector 2 - RXIFG
      RXBuffer_SCI   = UCA2RXBUF                                 ;
      event_SCI     |= 0x01                                      ;
      break                                                      ;
  case 4:break                                                     ;  // Vector 4 - TXIFG
  default: break                                                   ;  
  }  
}






