/**********************************************************************
      文件名：IIC.c
      说明  ：
              引脚 ：SCL -> P5.4 -> Pin51
                     SDA -> P3.7 -> Pin42
      时间  ：2016.05.30
      
**********************************************************************/

#include "msp430x54x.h"
#include <stdlib.h>
#include <stdio.h>
#include "IIC.h"


/**********************************************************
      函数名：IIC_INIT
      说明  ：主机模式，发送 100kHZ
***********************************************************/
void IIC_INIT(unsigned char slave_add){
  P5SEL |= BIT4;
  P3SEL |= BIT7;                            // Assign I2C pins to USCI_B1
  UCB1CTL1 = UCSWRST;                       // Enable SW reset
  UCB1CTL0 = UCMST + UCMODE_3 + UCSYNC;     // I2C Master, synchronous mode 单从机模式
  UCB1CTL1 = UCSSEL_2 + UCSWRST;            // Use SMCLK, keep SW reset
  UCB1BR0 = 160;                            // fSCL = SMCLK/160= 100kHz
  UCB1BR1 = 0;
  UCB1I2CSA = slave_add;                    
  UCB1CTL1 &= ~UCSWRST;                     // Clear SW reset, resume operation  
}


/**********************************************************
      函数名：IIC_SLAVE_ADD
      说明  ：获取从机地址，多从机是可能用到
***********************************************************/
void IIC_SLAVE_ADD(unsigned char add)
{
   UCB1I2CSA = add;
}


/**********************************************************
      函数名：IIC_Commend
      说明  ：写入命令
      参数  ：command  寄存器的命令
      返回  ：0异常中断  1正常停止
***********************************************************/

char  IIC_Command(char command)  
{  
  while( UCB1CTL1& UCTXSTP );  
  UCB1CTL1 |= UCTR;                // 写模式  
  UCB1CTL1 |= UCTXSTT;             // 发送启动位  

  UCB1TXBUF = command;           // 发送字节地址  
  // 等待UCTXIFG=1与UCTXSTT=0 同时变化等待一个标志位即可  
  while(!(UCB1IFG& UCTXIFG))  
  {  
    if( UCB1IFG& UCNACKIFG )      // 若无应答 UCNACKIFG=1  
    {  
      return 0;  
    }  
  }    
  UCB1CTL1 |= UCTXSTP;   
  while(UCB1CTL1& UCTXSTP);       // 等待发送完成  
   
  return 1;  
} 


/**********************************************************
      函数名：IIC_writepage
      说明  ：连续写入数据
      参数  ：command  寄存器的命令
              pword_buf  写入数组名
              len  数据长度
      返回  ：0异常中断  1正常停止
***********************************************************/

char  IIC_writepage( char command, char *pword_buf, char len)  
{  
  while( UCB1CTL1& UCTXSTP );  
  UCB1CTL1 |= UCTR;                // 写模式  
  UCB1CTL1 |= UCTXSTT;             // 发送启动位  
   
  UCB1TXBUF = command;           // 发送字节地址  
  // 等待UCTXIFG=1与UCTXSTT=0 同时变化等待一个标志位即可  
  while(!(UCB1IFG& UCTXIFG))  
  {  
    if( UCB1IFG& UCNACKIFG )      // 若无应答 UCNACKIFG=1  
    {  
      return 0;  
    }  
  }    
   
  for( char i= 0; i < len; i++)  
  {  
    UCB1TXBUF = *pword_buf++;      // 发送寄存器内容  
    while(!(UCB1IFG& UCTXIFG))     // 等待UCTXIFG=1  
        if( UCB1IFG& UCNACKIFG )      // 若无应答 UCNACKIFG=1  
          return 0;       
  }  
   
  UCB1CTL1 |= UCTXSTP;  
  while(UCB1CTL1& UCTXSTP);       // 等待发送完成  
   
  return 1;  
} 

/**********************************************************
      函数名：IIC_readpage
      说明  ：连续读出数据
      参数  ：command  寄存器的命令
              pword_buf  读出数组名
              len  数据长度
      返回  ：0异常中断  1正常停止
***********************************************************/

char IIC_readpage(char command, char *pword_buf, char len )  
{  
  while( UCB1CTL1 & UCTXSTP );  
  UCB1CTL1 |= UCTR;                // 写模式  
  UCB1CTL1 |= UCTXSTT;             // 发送启动位和写控制字节  
   
  UCB1TXBUF = command;           // 发送字节地址  
  // 等待UCTXIFG=1与UCTXSTT=0 同时变化等待一个标志位即可  
  while(!(UCB1IFG& UCTXIFG))  
  {  
    if( UCB1IFG& UCNACKIFG )      // 若无应答 UCNACKIFG=1  
    {  
      return 0;  
    }  
  }    
   
  UCB1CTL1 &= ~UCTR;               // 读模式  
  UCB1CTL1 |= UCTXSTT;             // 发送启动位和读控制字节  
   
  while(UCB1CTL1& UCTXSTT);       // 等待UCTXSTT=0  
  // 若无应答 UCNACKIFG = 1  
   
  for( char i= 0; i< len -1 ; i++)  
  {  
    while(!(UCB1IFG& UCRXIFG))  // 读取字节内容，不包括最后一个字节内容  
       if( UCB1IFG& UCNACKIFG )      // 若无应答 UCNACKIFG=1  
    {  
      return 0;  
    }  
    
    *pword_buf++= UCB1RXBUF;  
  }  
   
  UCB1CTL1 |= UCTXSTP;             // 在接收最后一个字节之前发送停止位  
   
  while(!(UCB1IFG& UCRXIFG))     // 读取最后一个字节内容  
     if( UCB1IFG& UCNACKIFG )      // 若无应答 UCNACKIFG=1  
    {  
      return 0;  
    }  
  *pword_buf = UCB1RXBUF;  
   
  while( UCB1CTL1& UCTXSTP );  
   
  return 1;  
}  


