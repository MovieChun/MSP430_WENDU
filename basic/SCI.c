/**********************************************************************
      文件名：sci.c
      说明  ：串口驱动
              波特率：9600
              中断接收
              引脚 ：TXD -> P10.4 -> Pin80
                     RXD -> P10.5 -> Pin81
      时间  ：2016.05.23
      补充  ：已测试
**********************************************************************/
#include "msp430x54x.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "SCI.h"

/**********************************************
 输出：event_SCI  接收标志位的最低位置位
       UCA3RXBUF  接收到的一个字节         
***********************************************/

char  event_SCI, RXBuffer_SCI;

/***************************************************************************
                                                                           
函数：UART_init                                                         
说明：串口初始化函数
      使用ACLK,外部低速时钟，32KHZ
      比较省电
参数：无                                        
                                                                          
***************************************************************************/

void UART_init_L(void)
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



/**********************************************************************
函数：UART_init                                                         
说明：串口初始化函数
      使用smclk, 16MHZ  
参数：无  
补充：
      使用16位校验能提高传输的正确率
      
**********************************************************************/

void UART_init_H(void)
{ 
  RS_PORT_SEL    |= TXD + RXD                                      ; // 选择引脚功能
  RS_PORT_DIR    |= TXD                                            ; // 选择引脚功能
  UCA3CTL1        = UCSWRST                                        ; // 状态机复位
  UCA3CTL1       |= UCSSEL_2                                       ; // CLK = smclk
  UCA3BR0         = 104                                            ; // 16mHz/9600/16=0x68 
  UCA3BR1         = 0x00                                           ; 
  UCA3MCTL        = UCBRS_0 + UCBRF_3                              ; // UCBRSx=3, UCBRFx=0
  UCA3MCTL       |= UCOS16                                         ; //开启16次采样
  UCA3CTL1       &= ~UCSWRST                                       ; // 启动状态机
  UCA3IE         |= UCRXIE                                         ; // 允许接收中断
}

/***************************************************************************
                                                                           
函数：UART_send                                                         
说明：串口发送字符串函数
参数：tx_buf -- 字符串首地址 
例如：UART_send("hallo word\n");
                                                                          
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
                                                                           
函数：UART_send_num                                                         
说明：串口发送数组
参数：tx_buf -- 数组名   num -- 数组长度
                                                                          
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
    中断接收函数
    输出：event_SCI  接收标志位的最低位置位
          UCA3RXBUF  接收到的一个字节       
    放在主函数中
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





