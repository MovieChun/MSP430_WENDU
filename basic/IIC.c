/**********************************************************************
      �ļ�����IIC.c
      ˵��  ��
              ���� ��SCL -> P5.4 -> Pin51
                     SDA -> P3.7 -> Pin42
      ʱ��  ��2016.05.30
      
**********************************************************************/

#include "msp430x54x.h"
#include <stdlib.h>
#include <stdio.h>
#include "IIC.h"


/**********************************************************
      ��������IIC_INIT
      ˵��  ������ģʽ������ 100kHZ
***********************************************************/
void IIC_INIT(unsigned char slave_add){
  P5SEL |= BIT4;
  P3SEL |= BIT7;                            // Assign I2C pins to USCI_B1
  UCB1CTL1 = UCSWRST;                       // Enable SW reset
  UCB1CTL0 = UCMST + UCMODE_3 + UCSYNC;     // I2C Master, synchronous mode ���ӻ�ģʽ
  UCB1CTL1 = UCSSEL_2 + UCSWRST;            // Use SMCLK, keep SW reset
  UCB1BR0 = 160;                            // fSCL = SMCLK/160= 100kHz
  UCB1BR1 = 0;
  UCB1I2CSA = slave_add;                    
  UCB1CTL1 &= ~UCSWRST;                     // Clear SW reset, resume operation  
}


/**********************************************************
      ��������IIC_SLAVE_ADD
      ˵��  ����ȡ�ӻ���ַ����ӻ��ǿ����õ�
***********************************************************/
void IIC_SLAVE_ADD(unsigned char add)
{
   UCB1I2CSA = add;
}


/**********************************************************
      ��������IIC_Commend
      ˵��  ��д������
      ����  ��command  �Ĵ���������
      ����  ��0�쳣�ж�  1����ֹͣ
***********************************************************/

char  IIC_Command(char command)  
{  
  while( UCB1CTL1& UCTXSTP );  
  UCB1CTL1 |= UCTR;                // дģʽ  
  UCB1CTL1 |= UCTXSTT;             // ��������λ  

  UCB1TXBUF = command;           // �����ֽڵ�ַ  
  // �ȴ�UCTXIFG=1��UCTXSTT=0 ͬʱ�仯�ȴ�һ����־λ����  
  while(!(UCB1IFG& UCTXIFG))  
  {  
    if( UCB1IFG& UCNACKIFG )      // ����Ӧ�� UCNACKIFG=1  
    {  
      return 0;  
    }  
  }    
  UCB1CTL1 |= UCTXSTP;   
  while(UCB1CTL1& UCTXSTP);       // �ȴ��������  
   
  return 1;  
} 


/**********************************************************
      ��������IIC_writepage
      ˵��  ������д������
      ����  ��command  �Ĵ���������
              pword_buf  д��������
              len  ���ݳ���
      ����  ��0�쳣�ж�  1����ֹͣ
***********************************************************/

char  IIC_writepage( char command, char *pword_buf, char len)  
{  
  while( UCB1CTL1& UCTXSTP );  
  UCB1CTL1 |= UCTR;                // дģʽ  
  UCB1CTL1 |= UCTXSTT;             // ��������λ  
   
  UCB1TXBUF = command;           // �����ֽڵ�ַ  
  // �ȴ�UCTXIFG=1��UCTXSTT=0 ͬʱ�仯�ȴ�һ����־λ����  
  while(!(UCB1IFG& UCTXIFG))  
  {  
    if( UCB1IFG& UCNACKIFG )      // ����Ӧ�� UCNACKIFG=1  
    {  
      return 0;  
    }  
  }    
   
  for( char i= 0; i < len; i++)  
  {  
    UCB1TXBUF = *pword_buf++;      // ���ͼĴ�������  
    while(!(UCB1IFG& UCTXIFG))     // �ȴ�UCTXIFG=1  
        if( UCB1IFG& UCNACKIFG )      // ����Ӧ�� UCNACKIFG=1  
          return 0;       
  }  
   
  UCB1CTL1 |= UCTXSTP;  
  while(UCB1CTL1& UCTXSTP);       // �ȴ��������  
   
  return 1;  
} 

/**********************************************************
      ��������IIC_readpage
      ˵��  ��������������
      ����  ��command  �Ĵ���������
              pword_buf  ����������
              len  ���ݳ���
      ����  ��0�쳣�ж�  1����ֹͣ
***********************************************************/

char IIC_readpage(char command, char *pword_buf, char len )  
{  
  while( UCB1CTL1 & UCTXSTP );  
  UCB1CTL1 |= UCTR;                // дģʽ  
  UCB1CTL1 |= UCTXSTT;             // ��������λ��д�����ֽ�  
   
  UCB1TXBUF = command;           // �����ֽڵ�ַ  
  // �ȴ�UCTXIFG=1��UCTXSTT=0 ͬʱ�仯�ȴ�һ����־λ����  
  while(!(UCB1IFG& UCTXIFG))  
  {  
    if( UCB1IFG& UCNACKIFG )      // ����Ӧ�� UCNACKIFG=1  
    {  
      return 0;  
    }  
  }    
   
  UCB1CTL1 &= ~UCTR;               // ��ģʽ  
  UCB1CTL1 |= UCTXSTT;             // ��������λ�Ͷ������ֽ�  
   
  while(UCB1CTL1& UCTXSTT);       // �ȴ�UCTXSTT=0  
  // ����Ӧ�� UCNACKIFG = 1  
   
  for( char i= 0; i< len -1 ; i++)  
  {  
    while(!(UCB1IFG& UCRXIFG))  // ��ȡ�ֽ����ݣ����������һ���ֽ�����  
       if( UCB1IFG& UCNACKIFG )      // ����Ӧ�� UCNACKIFG=1  
    {  
      return 0;  
    }  
    
    *pword_buf++= UCB1RXBUF;  
  }  
   
  UCB1CTL1 |= UCTXSTP;             // �ڽ������һ���ֽ�֮ǰ����ֹͣλ  
   
  while(!(UCB1IFG& UCRXIFG))     // ��ȡ���һ���ֽ�����  
     if( UCB1IFG& UCNACKIFG )      // ����Ӧ�� UCNACKIFG=1  
    {  
      return 0;  
    }  
  *pword_buf = UCB1RXBUF;  
   
  while( UCB1CTL1& UCTXSTP );  
   
  return 1;  
}  


