//===========================================================================//
//                                                                           //
// �ļ���  MAIN.C                                                            //
// ˵����  BW-DK5438�����嵥Ƭ��ʱ������ʵ�����                             //
//         ���õ�Ƭ��ʱ��Ϊ�ⲿ��Ƶ������ⲿ��Ƶ����+DCO��                  //
//         ��������ʱ�ӡ���ʱ�Ӻ�����ʱ��ͨ��IO�������                      //
//                                                                           //
//                      MSP430F5438                                          //
//                  -----------------                                        //
//              /|\|              XIN|-                                      //
//               | |                 | XTAL1 = 32.768k                       //
//               --|RST          XOUT|-                                      //
//                 |                 |                                       //
//                 |            XT2IN|-                                      //
//                 |                 | XTAL2 = 16Mhz                         //
//                 |RST        XT2OUT|-                                      //
//                 |                 |                                       //
//                 |            P11.0|-->ACLK                                //
//                 |            P11.1|-->MCLK                                //
//                 |            P11.2|-->SMCLK                               //
//                                                                           //
// ���룺  IAR Embedded Workbench IDE for MSP430 v4.21                       //
// �汾��  v1.1                                                              //
// ��д��  JASON.ZHANG                                                       //
// ��Ȩ��  �������ղ�ά���Ӽ������޹�˾                                      //
//                                                                           //
//===========================================================================//

#include "msp430x54x.h"
#include <stdlib.h>
#include <stdio.h>
#include "Clock.h"


// ��ʱ������ѡ���ѡ���ڲ�DCO���ⲿ��Ƶ����


#define  FLL_FACTOR     749    // FLL_FACTOR: DCO��Ƶϵ��    


//***************************************************************************//
//                                                                           //
//                       ��ʼ����ʱ��: MCLK = XT2                            //
//                                                                           //
//***************************************************************************//
#ifdef   EXTERNAL_HF_OSC
void Init_CLK(void)
{
  WDTCTL     = WDTPW + WDTHOLD                            ; // �ؿ��Ź�
  P5SEL     |= 0x0C                                       ; // �˿ڹ���ѡ������ P5.2��P5.3ѡ��Ϊ����XT2����
  UCSCTL6   &= ~XT2OFF                                    ; // ����ʹ��
  UCSCTL3   |= SELREF_2                                   ; // FLLref = REFO
  UCSCTL4   |= SELA_2                                     ; // ACLK=REFO,SMCLK=DCO,MCLK=DCO
  do
  {
    UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + XT1HFOFFG + DCOFFG); // ��� XT2,XT1,DCO �����־                                                          
    SFRIFG1 &= ~OFIFG                                     ; 
  }while(SFRIFG1&OFIFG)                                   ; // ������������־
  UCSCTL6   |= XT2DRIVE0 + XT2DRIVE1                      ; // XT2 ����ģʽ 24~32MHz                                            
  UCSCTL4   |= SELS_5 + SELM_5                            ; // SMCLK = MCLK = XT2
}


//***************************************************************************//
//                                                                           //
//                 ��ʼ����ʱ��: MCLK = XT1��(FLL_FACTOR+1)                  //
//                                                                           //
//***************************************************************************//
#else   
void Init_CLK(void)
{
  WDTCTL     = WDTPW + WDTHOLD                            ; // �ؿ��Ź�
  P7SEL     |= 0x03                                       ; // �˿�ѡ���ⲿ��Ƶ����XT1
  UCSCTL6   &=~XT1OFF                                     ; // ʹ���ⲿ���� 
  UCSCTL6   |= XCAP_3                                     ; // �����ڲ����ص���
  UCSCTL3   |= SELREF_2                                   ; // DCOref = REFO
  UCSCTL4   |= SELA_0                                     ; // ACLK   = XT1  
  __bis_SR_register(SCG0)                                 ; // �ر�FLL���ƻ�·
  UCSCTL0    = 0x0000                                     ; // ����DCOx, MODx
  UCSCTL1    = DCORSEL_7                                  ; // ����DCO�񵴷�Χ
  UCSCTL2    = FLLD__1 + FLL_FACTOR                       ; // Fdco = ( FLL_FACTOR + 1)��FLLRef = (649 + 1) * 32768 = 21.2992MHz
  __bic_SR_register(SCG0)                                 ; // ��FLL���ƻ�·                                                            
  __delay_cycles(1024000)                                 ; 
  do
  {
    UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + XT1HFOFFG + DCOFFG); // ��� XT2,XT1,DCO �����־                                                            
    SFRIFG1 &= ~OFIFG                                     ; 
  }while(SFRIFG1&OFIFG)                                   ; // ������������־ 
}
#endif




//***************************************************************************//
//                                                                           //
//  Init_TimerA0(void): ����TimerA0                                              //
//                                                                           //
//***************************************************************************//
void Init_Timer0_A5(void)
{
  TA0CTL   = 0                                               // ��λTimer0_A5, ��Ƶϵ������Ϊ1
           | (1 << 2)                                        // ��������0
           | (2 << 8)                                      ; // ����ʱ����ΪSMCLK                                                          ;
  TA0CCR0  =  TIME                                         ; // SMCK=EX2=16MHz�����ü��������ʱ��Ϊ1ms  16λ
  TA0CCTL0 = 0                                               // ��ʼ���������
           | (1 << 4)                                      ; // ʹ�ܱȽ��ж�
  TA0CTL  |= (1 << 4)                                      ; // ���ü�����Ϊ�Ӽ���������
}


/*
#pragma vector=TIMER0_A0_VECTOR                             
__interrupt void Timer0_A0 (void)
{
  P1OUT   ^= 0xF0                                         ; 
  P9OUT   ^= 0x0F                                         ; 
  TA0CCR0  = (16000) - 1                                  ;
}
*/