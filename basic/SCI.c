/**********************************************************************
      文件名：sci.c
      说明  ：串口驱动
              波特率：9600
              中断接收
              引脚 ：TXD -> P9.4 -> Pin72
                     RXD -> P9.5 -> Pin73
      时间  ：2016.05.23
      补充  : 用于上位机调试
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
                                                                           
函数：SCI_init                                                         
说明：串口初始化函数
      使用ACLK,外部低速时钟，32KHZ
      比较省电
参数：无                                        
                                                                          
***************************************************************************/

void SCI_init(void)
{ 
  SCI_PORT_SEL    |= SCI_TXD + SCI_RXD                             ; // 选择引脚功能
  SCI_PORT_DIR    |= SCI_TXD                                       ; // 选择引脚功能
  
  UCA2CTL1        = UCSWRST                                        ; // 状态机复位
  UCA2CTL1       |= UCSSEL_2                                       ; // CLK = smclk
  UCA2BR0         = 8                                              ; // 16mHz 
  UCA2BR1         = 0x00                                           ; 
  UCA2MCTL        = UCBRS_0 + UCBRF_11                              ; // UCBRSx=0, UCBRFx=11
  UCA2MCTL       |= UCOS16                                         ; //开启16次采样
  UCA2CTL1       &= ~UCSWRST                                       ; // 启动状态机
  UCA2IE         |= UCRXIE                                         ; // 允许接收中断
}

/***************************************************************************
                                                                           
函数：SCI_send                                                         
说明：串口发送字符串函数
参数：tx_buf -- 字符串首地址 
例如：SCI_send("hallo word\n");
                                                                          
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
                                                                           
函数：SCI_send2                                                        
说明：串口发送数组
参数：tx_buf -- 数组名   num -- 数组长度
                                                                          
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
                                                                           
函数：SCI_send_num                                                         
说明：串口发送整数
参数：num 无符号整数
                                                                          
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
                                                                           
函数：SCI_send_f2                                                         
说明：串口发送浮点数，精确到小数点后两位
参数：浮点数
                                                                          
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
    中断接收函数
    输出：event_SCI  接收标志位的最低位置位
          UCA2RXBUF  接收到的一个字节       
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






