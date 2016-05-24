/**********************************************************************
      �ļ�����IIC.c
      ˵��  ��
              ���� ��CLK -> P3.6 -> Pin41
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
 
/**********************************************************
      ��������IIC_INIT
      ˵��  ������ģʽ���жϽ��գ����� 100kHZ
***********************************************************/
void IIC_INIT(void){
  P3SEL |= BIT6 + BIT7;                     // Assign I2C pins to USCI_B1
  UCB1CTL1 |= UCSWRST;                      // Enable SW reset
  UCB1CTL0 = UCMST + UCMODE_3 + UCSYNC;     // I2C Master, synchronous mode
  UCB1CTL1 = UCSSEL_2 + UCSWRST;            // Use SMCLK, keep SW reset
  UCB1BR0 = 160;                            // fSCL = SMCLK/160= 100kHz
  UCB1BR1 = 0;
  UCB1I2CSA = SLAVE_ADD;                    // Slave Address is 048h
  UCB1CTL1 &= ~UCSWRST;                     // Clear SW reset, resume operation 
  UCB1IE |= UCTXIE + UCRXIE;                // Enable TX and RX interrupt
 
}

/**********************************************************
      ��������IIC_write
      ˵��  �������жϷ���
      ����  ��data -- д��������  num -- ��������
              IIC_flag == 1�����У������긴λΪ0
***********************************************************/
void IIC_write(unsigned char *data ,unsigned char num){
    IIC_flag = 1;                       //��־λ�ڶ�λ��λ
    PTxData = (unsigned char *)data;         // Start of RX buffer
    TXByteCtr = num;                         // Load RX byte counter
    IICCTL1 |= UCTR + UCTXSTT;           // I2C TX, start condition
}


/**********************************************************
      ��������IIC_read
      ˵��  �������жϽ���
      ����  ��data -- �洢������  num -- ��������
              IIC_flag == 2�����У������긴λΪ0
***********************************************************/

void IIC_read(unsigned char *data ,unsigned char num)
{
    IIC_flag = 2;                          //��־λ���λ��λ
    PRxData = (unsigned char *)data;       // Start of RX buffer
    RXByteCtr = num;                       // Load RX byte counter
    //while (IICCTL1 & UCTXSTP);           // Ensure stop condition got sent
    IICCTL1 &= ~UCTR;         	           // I2C RX
    IICCTL1 |= UCTXSTT;                    // I2C start condition
}

/*************************************************************
      �жϴ����������IIC_flage = 0
*************************************************************/
#pragma vector = USCI_B0_VECTOR
__interrupt void USCI_B0_ISR(void)
{
  switch(__even_in_range(UCB0IV,12))
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
     TXByteCtr--;                           // Decrement TX byte counter
    if (TXByteCtr)                          // Check TX byte counter
    {
      TXBUF = *PTxData++;                   // Load TX buffer
     
    }
    else
    {
      TXBUF = *PTxData++;                   // Load TX buffer
      IIC_flag = 0;                        //������ϣ����б�־ 
    }
  default: break;
  }
}
