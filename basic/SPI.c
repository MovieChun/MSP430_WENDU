//===========================================================================//
//                                                                           //
// �ļ���  MAIN.C                                                            //
// ˵����  BW-DK5438�����嵥Ƭ��SPI�˿�ʵ�����                              //
//         ����SPI�˿ڹ���ģʽ��ͨ��SPI�˿ڷ�������                          //
//         ����P3.1/P3.3�˿ڹ۲����ݡ�ʱ�Ӳ���                               //
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
#include "SPI.h"


#define  FLL_FACTOR     649                                 // FLL_FACTOR: DCO��Ƶϵ��    
char  event_SPI, RXBuffer_SPI[2]                                  ;


//***************************************************************************//
//                                                                           //
//  Init_SPI(void): ����SPI�˿�                                              //
//                                                                           //
//***************************************************************************//
void Init_SPI(void)
{  
  //P8OUT    |= LCD_NCS                                     ; // SPI�˿ڸ��ã�DisableTFTҺ���˿�
  //P8DIR    |= LCD_NCS                                     ;
  //P3OUT    |= NCS25                                       ; 
  //P3DIR    |= NCS25                                       ;
  P3SEL    &= 0xF0                                        ;
  P3SEL    |= 0x0E                                        ; // // P3.1/2/3����ѡ��ΪSPI
  UCB0CTL1 |= UCSWRST                                     ; // ��λSPI״̬��
  UCB0CTL0 |= UCMST+UCSYNC+UCCKPL+UCMSB                   ; // 3-pin, 8-bit SPI master, Clock polarity high, MSB
  UCB0CTL1 |= UCSSEL_2                                    ; // ѡ��SCK�ο�ԴΪSMCLK
  UCB0BR0   = 0x02                                        ; // SCK = SMCK/2
  UCB0BR1   = 0                                           ; 
  UCB0CTL1 &=~UCSWRST                                     ; // SPI״̬��ʹ��
}

