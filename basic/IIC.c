/**********************************************************************
      �ļ�����IIC.c
      ˵��  ��
              ���� ��SCL -> P5.4 -> Pin51
                     SDA -> P3.7 -> Pin42
      ʱ��  ��2016.05.24
      ����  ��������
**********************************************************************/

#include "msp430x54x.h"
#include <stdlib.h>
#include <stdio.h>
#include "IIC.h"

unsigned char *PRxData;                     // Pointer to RX data
unsigned char RXByteCtr;
unsigned char *PTxData;                     // Pointer to TX data
unsigned char TXByteCtr;

//IIC_flag IIC״̬��־ 0������  1������  2������
unsigned char IIC_flag;     

#define  RXBUF    UCB1RXBUF
#define  TXBUF    UCB1TXBUF
#define  IICCTL1  UCB1CTL1

#define  IIC_send  P3DIR |= BIT7
#define  IIC_get   P3DIR &= ~BIT7
/**********************************************************
      ��������IIC_INIT
      ˵��  ������ģʽ��//�жϽ��գ����� 100kHZ
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
  //UCB1IE |= UCTXIE + UCRXIE;                // Enable TX and RX interrupt  �����ж�
 
}

void IIC_start(unsigned char flag)
{
   IIC_send;
   if(0 == flag)IICCTL1 |= UCTR;
   else IICCTL1 &= ~UCTR;         	           // I2C RX
   IICCTL1 |= UCTXSTT;
   
}

/**********************************************************
      ��������IIC_write
      ˵��  �������жϷ���
      ����  ��data -- д��������  num -- ��������
              IIC_flag == 1�����У������긴λΪ0
***********************************************************/
void IIC_write(unsigned char data ,unsigned char flag){
    IIC_flag = 1;                          //��־λ�ڶ�λ��λ
    IIC_send;
    //TXBUF = data ;
   // IICCTL1 |= UCTR;                       // ����ģʽ
    if(2 == flag)IICCTL1 |= UCTXSTP; 
    //while(!(UCB1IFG & 0x04));
    while(IIC_flag);

} 


/**********************************************************
      ��������IIC_read
      ˵��  �������жϽ���
      ����  ��data -- �洢������  num -- ��������
              IIC_flag == 2�����У������긴λΪ0
***********************************************************/

unsigned char IIC_read(unsigned char flag)
{   unsigned char data; 
    IIC_flag = 2;                          //��־λ���λ��λ
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
  UCB1CTL1 |= UCTR;                // дģʽ  
  UCB1CTL1 |= UCTXSTT;             // ��������λ��д�����ֽ�  
   
  UCB1TXBUF = word_addr;           // �����ֽڵ�ַ  
  // �ȴ�UCTXIFG=1��UCTXSTT=0 ͬʱ�仯�ȴ�һ����־λ����  
  while(!(UCB1IFG& UCTXIFG))  
  {  
    if( UCB1IFG& UCNACKIFG )      // ����Ӧ�� UCNACKIFG=1  
    {  
      return 1;  
    }  
  }    
   
  UCB1CTL1 &= ~UCTR;               // ��ģʽ  
  UCB1CTL1 |= UCTXSTT;             // ��������λ�Ͷ������ֽ�  
   
  while(UCB1CTL1& UCTXSTT);       // �ȴ�UCTXSTT=0  
  // ����Ӧ�� UCNACKIFG = 1  
   
  for( char i= 0; i< len -1 ; i++)  
  {  
    while(!(UCB1IFG& UCRXIFG));   // ��ȡ�ֽ����ݣ����������һ���ֽ�����  
    *pword_buf++= UCB1RXBUF;  
  }  
   
  UCB1CTL1 |= UCTXSTP;             // �ڽ������һ���ֽ�֮ǰ����ֹͣλ  
   
  while(!(UCB1IFG& UCRXIFG));     // ��ȡ���һ���ֽ�����  
  *pword_buf = UCB1RXBUF;  
   
  while( UCB1CTL1& UCTXSTP );  
   
  return 0;  
}  


/*************************************************************
      �жϴ����������IIC_flage = 0
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
      IIC_flag = 0;                        //������ϣ����б�־ 
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
      IIC_flag = 0;                        //������ϣ����б�־ 
    }
  default: break;
  }
}
