/**********************************************************************
      文件名：IIC.c
      说明  ：
              引脚 ：SCL -> P5.4 -> Pin51
                     SDA -> P3.7 -> Pin42
      时间  ：2016.05.24
      补充  ：待测试
**********************************************************************/

#include "msp430x54x.h"
#include <stdlib.h>
#include <stdio.h>
#include "IIC.h"

unsigned char *PRxData;                     // Pointer to RX data
unsigned char RXByteCtr;
unsigned char *PTxData;                     // Pointer to TX data
unsigned char TXByteCtr;

//IIC_flag IIC状态标志 0：空闲  1：发送  2：接收
unsigned char IIC_flag;     

#define  RXBUF    UCB1RXBUF
#define  TXBUF    UCB1TXBUF
#define  IICCTL1  UCB1CTL1

#define  IIC_send  P3DIR |= BIT7
#define  IIC_get   P3DIR &= ~BIT7
/**********************************************************
      函数名：IIC_INIT
      说明  ：主机模式，//中断接收，发送 100kHZ
***********************************************************/
void IIC_INIT(void){
  P5SEL |= BIT4;
  P3SEL |= BIT7;                     // Assign I2C pins to USCI_B1
  //P3DIR |= BIT7;
  //P5DIR |= BIT4;
  UCB1CTL1 = UCSWRST;                       // Enable SW reset
  UCB1CTL0 = UCMST + UCMODE_3 + UCSYNC;     // I2C Master, synchronous mode
  UCB1CTL1 = UCSSEL_2 + UCSWRST;            // Use SMCLK, keep SW reset
  UCB1BR0 = 160;                            // fSCL = SMCLK/160= 100kHz
  UCB1BR1 = 0;
  UCB1I2CSA = SLAVE_ADD;                    // Slave Address is 048h
  UCB1CTL1 &= ~UCSWRST;                     // Clear SW reset, resume operation 
  //UCB1IE |= UCTXIE + UCRXIE;                // Enable TX and RX interrupt  不用中断
 
}

void IIC_start(unsigned char flag)
{
   IIC_send;
   if(0 == flag)IICCTL1 |= UCTR;
   else IICCTL1 &= ~UCTR;         	           // I2C RX
   IICCTL1 |= UCTXSTT;
   
}

/**********************************************************
      函数名：IIC_write
      说明  ：采用中断发送
      参数  ：data -- 写入数组名  num -- 数据数量
              IIC_flag == 1发送中，发送完复位为0
***********************************************************/
void IIC_write(unsigned char data ,unsigned char flag){
    IIC_flag = 1;                          //标志位第二位复位
    IIC_send;
    //TXBUF = data ;
   // IICCTL1 |= UCTR;                       // 发送模式
    if(2 == flag)IICCTL1 |= UCTXSTP; 
    //while(!(UCB1IFG & 0x04));
    while(IIC_flag);

} 


/**********************************************************
      函数名：IIC_read
      说明  ：采用中断接收
      参数  ：data -- 存储数组名  num -- 数据数量
              IIC_flag == 2接收中，接收完复位为0
***********************************************************/

unsigned char IIC_read(unsigned char flag)
{   unsigned char data; 
    IIC_flag = 2;                          //标志位最低位复位
    IIC_get;
    //IICCTL1 &= ~UCTR;
    if(1 == flag)IICCTL1 |= UCTXNACK;
    //while(!(UCB1IFG & UCRXIFG));
    data = RXBUF;
    return data;
}

///-----------------------------------------------------------------

char mlx_readpage(char word_addr, char *pword_buf, char len )  
{  
  while( UCB1CTL1& UCTXSTP );  
  UCB1CTL1 |= UCTR;                // 写模式  
  UCB1CTL1 |= UCTXSTT;             // 发送启动位和写控制字节  
   
  UCB1TXBUF = word_addr;           // 发送字节地址  
  // 等待UCTXIFG=1与UCTXSTT=0 同时变化等待一个标志位即可  
  while(!(UCB1IFG& UCTXIFG))  
  {  
    if( UCB1IFG& UCNACKIFG )      // 若无应答 UCNACKIFG=1  
    {  
      return 1;  
    }  
  }    
   
  UCB1CTL1 &= ~UCTR;               // 读模式  
  UCB1CTL1 |= UCTXSTT;             // 发送启动位和读控制字节  
   
  while(UCB1CTL1& UCTXSTT);       // 等待UCTXSTT=0  
  // 若无应答 UCNACKIFG = 1  
   
  for( char i= 0; i< len -1 ; i++)  
  {  
    while(!(UCB1IFG& UCRXIFG));   // 读取字节内容，不包括最后一个字节内容  
    *pword_buf++= UCB1RXBUF;  
  }  
   
  UCB1CTL1 |= UCTXSTP;             // 在接收最后一个字节之前发送停止位  
   
  while(!(UCB1IFG& UCRXIFG));     // 读取最后一个字节内容  
  *pword_buf = UCB1RXBUF;  
   
  while( UCB1CTL1& UCTXSTP );  
   
  return 0;  
}  


/*************************************************************
      中断处理，接收完后IIC_flage = 0
*************************************************************/
#pragma vector = USCI_B1_VECTOR
__interrupt void USCI_B1_ISR(void)
{
  switch(__even_in_range(UCB1IV,12))
  {
  case  0: break;                           // Vector  0: No interrupts
  case  2: break;                           // Vector  2: ALIFG
  case  4: break;                           // Vector  4: NACKIFG
  case  6: break;   		            // Vector  6: STTIFG
  case  8: break;                           // Vector  8: STPIFG
  case 10:                                  // Vector 10: RXIFG
    RXByteCtr--;                            // Decrement RX byte counter
    if (RXByteCtr)
    {
      *PRxData++ = RXBUF;                   // Move RX data to address PRxData
    }
    else
    {
      *PRxData = RXBUF;                    // Move final RX data to PRxData
      IIC_flag = 0;                        //接收完毕，空闲标志 
    }
    break;
  case 12:                                  // Vector 12: TXIFG
    IIC_flag = 0;   
    //TXByteCtr--;                           // Decrement TX byte counter
    if (TXByteCtr)                          // Check TX byte counter
    {
      TXBUF = 100;                   // Load TX buffer
     
    }
    else
    {
      TXBUF = 100;                   // Load TX buffer
      IIC_flag = 0;                        //发送完毕，空闲标志 
    }
  default: break;
  }
}
