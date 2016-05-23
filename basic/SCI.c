//===========================================================================//
//                                                                           //
// 文件：  MAIN.C                                                            //
// 说明：  BW-DK5438开发板单片机USB端口实验程序                              //
//         主程序检测有按键按下则通过USB端口发送"This is BW-DK5483" ,        //
//         可将开发板USB接口与PC连接，PC上运行串口大师等串口调试软件，       //
//         打开USB串口接收并观察开发板发送数据，也可由PC端发送数据至         //
//         开发板,开发板将接收数据发回PC                                     //
// 注意：  开发板主电源上电后，PC端串口调试软件应重新关闭/打开一次串口       //
// 编译：  IAR Embedded Workbench IDE for MSP430 v4.21                       //
// 版本：  v1.1                                                              //
// 编写：  JASON.ZHANG                                                       //
// 版权：  北京拓普博维电子技术有限公司                                      //
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
//  RS232/485接收中断服务程序                                                //
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
//  Init_RSUART(void): 初始化RS232/485端口                                   //
//       能用                                                                    //
//  使用低频时钟（外部低频晶振），不采样，（正确率低），无奇偶校验  9600
//***************************************************************************//

void Init_RSUART(void)
{ 
  RS_PORT_SEL    |= TXD + RXD                                      ; // 选择引脚功能
  RS_PORT_DIR    |= TXD                                            ; // 选择引脚功能
  UCA3CTL1        = UCSWRST                                        ; // 状态机复位
  UCA3CTL1       |= UCSSEL_1                                       ; // CLK = ACLK
  UCA3BR0         = 0x03                                           ; // 32kHz/9600=3.41 
  UCA3BR1         = 0x00                                           ; 
  UCA3MCTL        = UCBRS_3 + UCBRF_0                              ; // UCBRSx=3, UCBRFx=0
  UCA3CTL1       &= ~UCSWRST                                       ; // 启动状态机
  UCA3IE         |= UCRXIE                                         ; // 允许接收中断

}

//***************************************************************************//
//                                                                           //
//  RS232TX_PROC(void): RS232端口发送程序                                    //
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
//  Init_RSUART(void): 初始化RS232/485端口                                   //
//        待改                                                                   //
//  
//***************************************************************************//
/*
void Init_RSUART(void)
{ 
  RS_PORT_SEL    |= TXD + RXD                                      ; // 选择引脚功能
  RS_PORT_DIR    |= TXD                                            ; // 选择引脚功能
  UCA3CTL1        = UCSWRST                                        ; // 状态机复位
  UCA3CTL1       |= UCSSEL_2                                       ; // CLK = smclk
  UCA3BR0         = 0x68                                           ; // 16mHz/9600/16=0x68 
  UCA3BR1         = 0x00                                           ; 
  UCA3MCTL        = UCBRS_0 + UCBRF_3                              ; // UCBRSx=3, UCBRFx=0
  //UCA3CTL1       &= ~UCSWRST                                       ; // 启动状态机
  //UCA3CTL1       |= UCSWRST                                       ; // 启动状态机
  UCA3IE         |= UCRXIE                                         ; // 允许接收中断
}*/