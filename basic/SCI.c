//===========================================================================//
//                                                                           //
// �ļ���  MAIN.C                                                            //
// ˵����  BW-DK5438�����嵥Ƭ��USB�˿�ʵ�����                              //
//         ���������а���������ͨ��USB�˿ڷ���"This is BW-DK5483" ,        //
//         �ɽ�������USB�ӿ���PC���ӣ�PC�����д��ڴ�ʦ�ȴ��ڵ��������       //
//         ��USB���ڽ��ղ��۲쿪���巢�����ݣ�Ҳ����PC�˷���������         //
//         ������,�����彫�������ݷ���PC                                     //
// ע�⣺  ����������Դ�ϵ��PC�˴��ڵ������Ӧ���¹ر�/��һ�δ���       //
// ���룺  IAR Embedded Workbench IDE for MSP430 v4.21                       //
// �汾��  v1.1                                                              //
// ��д��  JASON.ZHANG                                                       //
// ��Ȩ��  �������ղ�ά���Ӽ������޹�˾                                      //
//                                                                           //
//===========================================================================//

#include "msp430x54x.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "SCI.h"

  
char  event_SCI, RXBuffer_SCI;

//***************************************************************************//
//                                                                           //
//  RS232/485�����жϷ������                                                //
//                                                                           //
//***************************************************************************//
/*#pragma vector=USCI_A3_VECTOR
__interrupt void USCI_A3_ISR(void)
{
  switch(__even_in_range(UCA3IV,4))
  {
  case 0:break                                                     ; // Vector 0 - no interrupt
  case 2:                                                            // Vector 2 - RXIFG
      RXBuffer_SCI      = UCA3RXBUF                                 ;
      event_SCI           |= 0x01                                      ;
      break                                                        ;
  case 4:break                                                     ;  // Vector 4 - TXIFG
  default: break                                                   ;  
  }  
}
*/

//***************************************************************************//
//                                                                           //
//  Init_RSUART(void): ��ʼ��RS232/485�˿�                                   //
//       ����                                                                    //
//  ʹ�õ�Ƶʱ�ӣ��ⲿ��Ƶ���񣩣�������������ȷ�ʵͣ�������żУ��  9600
//***************************************************************************//

void Init_RSUART(void)
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

//***************************************************************************//
//                                                                           //
//  RS232TX_PROC(void): RS232�˿ڷ��ͳ���                                    //
//                                                                           //
//***************************************************************************//
void RS232TX_PROC(char *tx_buf)
{
  unsigned char i,length                                           ;
  length = strlen(tx_buf)                                          ;
  for(i=0;i<length;i++)
  {
    UCA3TXBUF = *tx_buf++                                          ; 
    while (!(UCA3IFG&UCTXIFG))                                     ; 
  }
}

void RS232TX_PROC2(char *tx_buf,char length)
{
  unsigned char i ;
  for(i=0;i<length;i++)
  {
    UCA3TXBUF = *tx_buf++                                          ; 
    while (!(UCA3IFG&UCTXIFG))                                     ; 
  }
}

//***************************************************************************//
//                                                                           //
//  Init_RSUART(void): ��ʼ��RS232/485�˿�                                   //
//        ����                                                                   //
//  
//***************************************************************************//
/*
void Init_RSUART(void)
{ 
  RS_PORT_SEL    |= TXD + RXD                                      ; // ѡ�����Ź���
  RS_PORT_DIR    |= TXD                                            ; // ѡ�����Ź���
  UCA3CTL1        = UCSWRST                                        ; // ״̬����λ
  UCA3CTL1       |= UCSSEL_2                                       ; // CLK = smclk
  UCA3BR0         = 0x68                                           ; // 16mHz/9600/16=0x68 
  UCA3BR1         = 0x00                                           ; 
  UCA3MCTL        = UCBRS_0 + UCBRF_3                              ; // UCBRSx=3, UCBRFx=0
  //UCA3CTL1       &= ~UCSWRST                                       ; // ����״̬��
  //UCA3CTL1       |= UCSWRST                                       ; // ����״̬��
  UCA3IE         |= UCRXIE                                         ; // ��������ж�
}*/