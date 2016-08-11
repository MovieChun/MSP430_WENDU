/**********************************************************************
      文件名：uart.c
      说明  ：串口驱动 ， 用于SIM模块
              波特率：115200
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
#include "uart.h"

/**********************************************
 输出：event_SCI  接收标志位的最低位置位
       UCA3RXBUF  接收到的一个字节         
***********************************************/




/**********************************************************************
函数：UART_init                                                         
说明：串口初始化函数
      使用smclk, 16MHZ  
参数：无  
补充：
      使用16位校验能提高传输的正确率
      
**********************************************************************/
//SIM模块只接受单片机115200波特率
void UART_init(void)
{ 
  UART_PORT_SEL    |= UART_TXD + UART_RXD                          ; // 选择引脚功能
  UART_PORT_DIR    |= UART_TXD                                     ; // 选择引脚功能
  UCA3CTL1        = UCSWRST                                        ; // 状态机复位
  UCA3CTL1       |= UCSSEL_2                                       ; // CLK = smclk 
  //UCA3CTL1       |= UCSSEL_1;           
  UCA3BR0         = 8                                              ; //  
  //UCA3BR0         = 3 ;   
  UCA3BR1         = 0x00                                           ; 
  UCA3MCTL        = UCBRS_0 + UCBRF_11                              ; // UCBRSx=3, UCBRFx=0
  //UCA3MCTL        = UCBRS_3 + UCBRF_0;    
  UCA3MCTL       |= UCOS16                                         ; //
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
  for(i=0;i<length && *tx_buf!= '\0';i++)
  {
    UCA3TXBUF = *tx_buf++                                          ; 
    while (!(UCA3IFG&UCTXIFG))                                     ; 
  }
}


/***************************************************************************
                                                                           
函数：UART_send_char                                                       
说明：发送一个字符
参数：tx_buf ——字符
                                                                          
***************************************************************************/

void UART_send_char(char tx_buf)
{
    UCA3TXBUF = tx_buf                                             ; 
    while (!(UCA3IFG&UCTXIFG))                                     ;
}


/***************************************************************************
                                                                           
函数：UART_send2                                                        
说明：串口发送数组
参数：tx_buf -- 数组名   num -- 数组长度
                                                                          
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
                                                                           
函数：UART_send_num                                                         
说明：串口发送整数
参数：num 无符号整数
                                                                          
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
                                                                           
函数：UART_send_f2                                                         
说明：串口发送浮点数，精确到小数点后两位
参数：浮点数
                                                                          
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


