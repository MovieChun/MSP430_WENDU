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

  
char  event_SCI, RXBuffer_SCI[2]                                           ;

//***************************************************************************//
//                                                                           //
//  USB�����жϷ������                                                      //
//                                                                           //
//***************************************************************************//
#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void)
{
  switch(__even_in_range(UCA1IV,4))
  {
  case 0:break                                                     ; // Vector 0 - no interrupt
  case 2:                                                            // Vector 2 - RXIFG
      RXBuffer_SCI[0]      = UCA1RXBUF                                 ;
      event_SCI           |= 0x01                                      ;
      break                                                        ;
  case 4:break                                                     ;  // Vector 4 - TXIFG
  default: break                                                   ;  
  }  
}

//***************************************************************************//
//                                                                           //
//  RS232/485�����жϷ������                                                //
//                                                                           //
//***************************************************************************//
#pragma vector=USCI_A3_VECTOR
__interrupt void USCI_A3_ISR(void)
{
  switch(__even_in_range(UCA3IV,4))
  {
  case 0:break                                                     ; // Vector 0 - no interrupt
  case 2:                                                            // Vector 2 - RXIFG
      RXBuffer_SCI[0]      = UCA3RXBUF                                 ;
      event_SCI           |= 0x01                                      ;
      break                                                        ;
  case 4:break                                                     ;  // Vector 4 - TXIFG
  default: break                                                   ;  
  }  
}

//***************************************************************************//
//                                                                           //
//  Init_UART(void): ��ʼ��USB�˿�                                           //
//                                                                           //
//***************************************************************************//
void Init_UART(void)
{ 
//  USB_PORT_SEL   |= TXD_U + RXD_U                                  ; // ѡ�����Ź���
//  USB_PORT_DIR   |= TXD_U                                          ; // ѡ�����Ź���
  UCA1CTL1        = UCSWRST                                        ; // ״̬����λ
  UCA1CTL1       |= UCSSEL_1                                       ; // CLK = ACLK
  UCA1BR0         = 0x03                                           ; // 32kHz/9600=3.41 
  UCA1BR1         = 0x00                                           ; 
  UCA1MCTL        = UCBRS_3 + UCBRF_0                              ; // UCBRSx=3, UCBRFx=0
  UCA1CTL1       &= ~UCSWRST                                       ; // ����״̬��
  UCA1IE         |= UCRXIE                                         ; // ��������ж�
}


//***************************************************************************//
//                                                                           //
//  Init_RSUART(void): ��ʼ��RS232/485�˿�                                   //
//                                                                           //
//***************************************************************************//
void Init_RSUART(void)
{ 
//  RS_PORT_SEL    |= TXD + RXD                                      ; // ѡ�����Ź���
//  RS_PORT_DIR    |= TXD                                            ; // ѡ�����Ź���
  UCA3CTL1        = UCSWRST                                        ; // ״̬����λ
  UCA3CTL1       |= UCSSEL_1                                       ; // CLK = ACLK
  UCA3BR0         = 0x03                                           ; // 32kHz/9600=3.41 
  UCA3BR1         = 0x00                                           ; 
  UCA3MCTL        = UCBRS_3 + UCBRF_0                              ; // UCBRSx=3, UCBRFx=0
  UCA3CTL1       &= ~UCSWRST                                       ; // ����״̬��
  UCA3IE         |= UCRXIE                                         ; // ��������ж�
//  RS485_IN                                                         ;
}

//***************************************************************************//
//                                                                           //
//  UTX_PROC(void): USB�˿ڷ��ͳ���                                          //
//                                                                           //
//***************************************************************************//
void UTX_PROC(char *tx_buf)
{
  unsigned char i,length                                           ;
  length = strlen(tx_buf)                                          ;
  for(i=0;i<length;i++)
  {
    UCA1TXBUF = *tx_buf++                                          ; 
//    __delay_cycles(5000)                                         ;
    while (!(UCA1IFG&UCTXIFG))                                     ; 
  }
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

//***************************************************************************//
//                                                                           //
//  RS485TX_PROC(void): RS485�˿ڷ��ͳ���                                    //
//                                                                           //
//***************************************************************************//
void RS485TX_PROC(char *tx_buf)
{
  unsigned char i,length                                           ;
  length = strlen(tx_buf)                                          ;
//  RS485_OUT                                                        ;
  for(i=0;i<length;i++)
  {
    UCA3TXBUF = *tx_buf++                                          ; 
    while (!(UCA3IFG&UCTXIFG))                                     ; 
  }
  __delay_cycles(100000)                                           ;
//  RS485_IN                                                         ;
}

