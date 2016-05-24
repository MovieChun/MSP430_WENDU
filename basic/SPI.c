/**********************************************************************
      文件名：SPI.c
      说明  ：
              引脚 ：SPI_MISO -> P3.1 -> Pin34
                     SPI_MOSI -> P3.2 -> Pin35
                     SPI_CLK  -> P3.3 -> Pin36 
      时间  ：2016.05.24
      补充  ：待测试
**********************************************************************/

#include "msp430x54x.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "SPI.h"


//***************************************************************************//
//                                                                           //
//  Init_SPI(void): 设置SPI端口                                              //
//                                                                           //
//***************************************************************************//


/******************************************************
      初始化SPI
*****************************************************/
void Init_SPI(void)
{  

  P3SEL    &= 0xF0                                        ;
  P3SEL    |= 0x0E                                        ; // // P3.1/2/3功能选择为SPI
  UCB0CTL1 |= UCSWRST                                     ; // 复位SPI状态机
  UCB0CTL0 |= UCMST+UCSYNC+UCCKPL+UCMSB                   ; // 3-pin, 8-bit SPI master, Clock polarity high, MSB
  UCB0CTL1 |= UCSSEL_2                                    ; // 选择SCK参考源为SMCLK
  UCB0BR0   = 0x02                                        ; // SCK = SMCK/2
  UCB0BR1   = 0                                           ; 
  UCB0CTL1 &=~UCSWRST                                     ; // SPI状态机使能
}

/****************************************************
      往SPI写入一个字节
*****************************************************/
inline void SPI_write(unsigned char data){
  while (!(UCB0IFG&UCTXIFG));               // USCI_B0 TX buffer ready?    
  UCB0TXBUF = data;
}

/*****************************************************
      从SPI读出一个字节
*******************************************************/
inline unsigned char SPI_read(void){
  while (!(UCB0IFG&UCRXIFG));               // USCI_B0 RX buffer ready?    
  return UCB0RXBUF;
}