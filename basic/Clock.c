//===========================================================================//
//                                                                           //
// �ļ���  Clock.c                                                           //
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
//                                                                           //
//                                                                           //
//===========================================================================//

#include "msp430x54x.h"
#include <stdlib.h>
#include <stdio.h>
#include "Clock.h"


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
  P7SEL     |= 0x03                                       ; // �˿�ѡ���ⲿ��Ƶ����XT1
  UCSCTL6   &= ~(XT2OFF + XT1OFF)                         ; // ����ʹ��
  UCSCTL6   |= XCAP_3                                     ; // �����ڲ����ص���
  UCSCTL3   |= SELREF_2                                   ; // FLLref = REFO
  UCSCTL4   |= SELA_0                                     ; // ACLK=XT1,SMCLK=DCO,MCLK=DCO
  //UCSCTL5   |= DIVA2                                    ;  //ACLK����Ƶ 32k
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
//  Init_TimerA0(void): ����TimerA0                                          //
//                                                                           //
//***************************************************************************//
void Init_Timer0_A5(void)
{  
  TA0CTL   = TASSEL0 + ID1 + ID0 + TACLR                   ; // ��λTimer0_A5, ��Ƶϵ������Ϊ8
                                                          // ��������0
                                                          // ����ʱ����ΪACLK  32K                                                        ;
  TA0CCR0  =  TIME                                         ; 
  TA0CCTL0 = 0                                               // ��ʼ���������
           | (1 << 4)                                      ; // ʹ�ܱȽ��ж�
  TA0CTL  |= MC0                                           ; // ���ü�����Ϊ�Ӽ���������*/
   
}

//***************************************************************************//
//                                                                           //
//  Init_TimerA1(void): ����TimerA1                                          //
//  1MS�ľ�ȷ��ʱ                                                                         //
//***************************************************************************//
unsigned int delay_ms = 0;   //��ȷ��ʱ
unsigned int uart1_ms = 0;   //����1�Ľ�����ʱ
unsigned int uart2_ms = 0;   //����2�Ľ�����ʱ
unsigned int SCI_ms = 0;
char uart1_flag = 0;         //����1��ʱ��־��0��ʾ������ʱ 
char uart2_flag = 0;         //����2��ʱ��־��0��ʾ������ʱ
char SCI_flag = 0;

void Init_Timer1_A3(void)
{  
  TA1CTL   = TASSEL0 + TACLR                               ; // ��λTimer1_A3, ��Ƶϵ������Ϊ1
                                                             // ��������0
                                                             // ����ʱ����ΪACLK                                                          ;
  TA1CCR0  =  31                                            ; // ���ü��������ʱ��Ϊ1ms  
  TA1CCTL0 = 0                                               // ��ʼ���������
           | (1 << 4)                                      ; // ʹ�ܱȽ��ж�
  TA1CTL  |= MC0                                           ; // ���ü�����Ϊ�Ӽ���������*/
   
}



char* UART1_delay(unsigned int ms){
    uart1_ms = ms;
    uart1_flag = 1;
    TA1CTL  |= MC0;      // ������ʱ��
    return &uart1_flag;
}

char* UART2_delay(unsigned int ms){
    uart2_ms = ms;
    uart2_flag = 1;
    TA1CTL  |= MC0;      // ������ʱ��
    return &uart2_flag;
}


char* SCI_delay(unsigned int ms){
    SCI_ms = ms;
    SCI_flag = 1;
    TA1CTL  |= MC0;      // ������ʱ��
    return &SCI_flag;
}


void Delay_ms(unsigned int ms){  //1ms��ʱ����
   delay_ms = ms;
   TA1CTL  |= MC0;       // ������ʱ��
   while(delay_ms);
}



#include "uart.h"

#pragma vector=TIMER1_A0_VECTOR                             
__interrupt void Timer1_A0 (void)
{
  if(delay_ms > 0)delay_ms--;
  
  if(uart1_ms > 0)uart1_ms--;
  else uart1_flag = 0;   //��ʱ��־��λ
      
  if(uart2_ms > 0)uart2_ms--;
  else uart2_flag = 0;   //��ʱ��־��λ
  
  if(SCI_ms > 0)SCI_ms--;
  else SCI_flag = 0;   //��ʱ��־��λ
  
  if((delay_ms + uart2_flag + uart1_flag + SCI_flag) == 0){//����Ҫ��ʱ����
      __bic_SR_register_on_exit(LPM0_bits);   // Exit active CPU
      TA1CTL  &= ~MC0;  //�رն�ʱ��
  } 
}
