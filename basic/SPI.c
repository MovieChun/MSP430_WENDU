//===========================================================================//
//                                                                           //
// 文件：  MAIN.C                                                            //
// 说明：  BW-DK5438开发板单片机SPI端口实验程序                              //
//         设置SPI端口工作模式，通过SPI端口发送数据                          //
//         可在P3.1/P3.3端口观察数据、时钟波形                               //
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
#include "SPI.h"


#define  FLL_FACTOR     649                                 // FLL_FACTOR: DCO倍频系数    
char  event_SPI, RXBuffer_SPI[2]                                  ;


//***************************************************************************//
//                                                                           //
//  Init_SPI(void): 设置SPI端口                                              //
//                                                                           //
//***************************************************************************//
void Init_SPI(void)
{  
  //P8OUT    |= LCD_NCS                                     ; // SPI端口复用，DisableTFT液晶端口
  //P8DIR    |= LCD_NCS                                     ;
  //P3OUT    |= NCS25                                       ; 
  //P3DIR    |= NCS25                                       ;
  P3SEL    &= 0xF0                                        ;
  P3SEL    |= 0x0E                                        ; // // P3.1/2/3功能选择为SPI
  UCB0CTL1 |= UCSWRST                                     ; // 复位SPI状态机
  UCB0CTL0 |= UCMST+UCSYNC+UCCKPL+UCMSB                   ; // 3-pin, 8-bit SPI master, Clock polarity high, MSB
  UCB0CTL1 |= UCSSEL_2                                    ; // 选择SCK参考源为SMCLK
  UCB0BR0   = 0x02                                        ; // SCK = SMCK/2
  UCB0BR1   = 0                                           ; 
  UCB0CTL1 &=~UCSWRST                                     ; // SPI状态机使能
}

