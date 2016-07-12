/**********************************************************************
      文件名：uart2.c
      说明  ：串口驱动 , 用于WIFI模块
              波特率：115200
              中断接收
              引脚 ：TXD -> P5.6 -> Pin53
                     RXD -> P5.7 -> Pin54
      时间  ：2016.05.23
      补充  ：已测试
**********************************************************************/
#include "msp430x54x.h"
#include <stdlib.h>
#include <stdio.h>
#include "uart2.h"

/**********************************************************************
函数：UART2_init                                                         
说明：串口初始化函数
      使用smclk, 16MHZ  
参数：无  
补充：
      使用16位校验能提高传输的正确率
      
**********************************************************************/

void UART2_init(void)
{ 
  P5SEL          |= BIT7 + BIT6                                    ; // 选择引脚功能
  P5DIR          |= BIT6                                           ; // 选择引脚功能
  UCA1CTL1        = UCSWRST                                        ; // 状态机复位
  UCA1CTL1       |= UCSSEL_1                                       ; // CLK = ACLK  32K
  UCA1BR0         = 3                                              ; // 16mHz 
  UCA1BR1         = 0x00                                           ; 
  UCA1MCTL        = UCBRS_3 + UCBRF_0                               ; // UCBRSx=3, UCBRFx=0
  //UCA1MCTL       |= UCOS16                                         ; //开启16次采样
  UCA1CTL1       &= ~UCSWRST                                       ; // 启动状态机
  UCA1IE         |= UCRXIE                                         ; // 允许接收中断
}

/***************************************************************************
                                                                           
函数：UART2_send                                                         
说明：串口发送字符串函数
参数：tx_buf -- 字符串首地址 
例如：UART_send("hallo word\n");
                                                                          
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
                                                                           
函数：UART2_send_char                                                       
说明：发送一个字符
参数：tx_buf ——字符
                                                                          
***************************************************************************/

void UART2_send_char(char tx_buf)
{
    UCA1TXBUF = tx_buf                                             ; 
    while (!(UCA1IFG&UCTXIFG))                                     ;
}


/***************************************************************************
                                                                           
函数：UART2_sendn                                                        
说明：串口发送n个字符
参数：tx_buf -- 数组名   num -- 数组长度
                                                                          
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
                                                                           
函数：UART2_send_num                                                         
说明：串口发送整数
参数：num 无符号整数
                                                                          
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
                                                                           
函数：UART2_send_f2                                                         
说明：串口发送浮点数，精确到小数点后两位
参数：浮点数
                                                                          
***************************************************************************/

void UART2_send_float(float f2)
{
  unsigned int num;
  
  if(f2 < 0){
     UART2_send("-");
     f2 = -f2;
  }
  
  num = (unsigned int)f2;
  UART2_send_num(num);
  UART2_send(".");
  num = (unsigned int)(100 * f2) % 100;
  if(num < 10)UART2_send("0");
  UART2_send_num(num);
}


/***************************************************************************
    中断接收函数
    输出：event_SCI  接收标志位的最低位置位
          UCA3RXBUF  接收到的一个字节       
    放在主函数中
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
