//===========================================================================//
//                                                                           //
// 文件：  Clock.c                                                           //
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
//                       初始化主时钟: MCLK = XT2                            //
//                                                                           //
//***************************************************************************//
#ifdef   EXTERNAL_HF_OSC
void Init_CLK(void)
{
  WDTCTL     = WDTPW + WDTHOLD                            ; // 关看门狗
  P5SEL     |= 0x0C                                       ; // 端口功能选择振荡器 P5.2和P5.3选择为晶振XT2输入
  UCSCTL6   &= ~XT2OFF                                    ; // 振荡器使能
  UCSCTL3   |= SELREF_2                                   ; // FLLref = REFO
  UCSCTL4   |= SELA_2                                     ; // ACLK=REFO,SMCLK=DCO,MCLK=DCO
  do
  {
    UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + XT1HFOFFG + DCOFFG); // 清除 XT2,XT1,DCO 错误标志                                                          
    SFRIFG1 &= ~OFIFG                                     ; 
  }while(SFRIFG1&OFIFG)                                   ; // 检测振荡器错误标志
  UCSCTL6   |= XT2DRIVE0 + XT2DRIVE1                      ; // XT2 驱动模式 24~32MHz                                            
  UCSCTL4   |= SELS_5 + SELM_5                            ; // SMCLK = MCLK = XT2
}

//***************************************************************************//
//                                                                           //
//                 初始化主时钟: MCLK = XT1×(FLL_FACTOR+1)                  //
//                                                                           //
//***************************************************************************//
#else   
void Init_CLK(void)
{
  WDTCTL     = WDTPW + WDTHOLD                            ; // 关看门狗
  P7SEL     |= 0x03                                       ; // 端口选择外部低频晶振XT1
  UCSCTL6   &=~XT1OFF                                     ; // 使能外部晶振 
  UCSCTL6   |= XCAP_3                                     ; // 设置内部负载电容
  UCSCTL3   |= SELREF_2                                   ; // DCOref = REFO
  UCSCTL4   |= SELA_0                                     ; // ACLK   = XT1  
  __bis_SR_register(SCG0)                                 ; // 关闭FLL控制回路
  UCSCTL0    = 0x0000                                     ; // 设置DCOx, MODx
  UCSCTL1    = DCORSEL_7                                  ; // 设置DCO振荡范围
  UCSCTL2    = FLLD__1 + FLL_FACTOR                       ; // Fdco = ( FLL_FACTOR + 1)×FLLRef = (649 + 1) * 32768 = 21.2992MHz
  __bic_SR_register(SCG0)                                 ; // 打开FLL控制回路                                                            
  __delay_cycles(1024000)                                 ; 
  do
  {
    UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + XT1HFOFFG + DCOFFG); // 清除 XT2,XT1,DCO 错误标志                                                            
    SFRIFG1 &= ~OFIFG                                     ; 
  }while(SFRIFG1&OFIFG)                                   ; // 检测振荡器错误标志 
}
#endif





//***************************************************************************//
//                                                                           //
//  Init_TimerA0(void): 设置TimerA0                                              //
//                                                                           //
//***************************************************************************//
void Init_Timer0_A5(void)
{
  TA0CTL   = 0                                               // 复位Timer0_A5, 分频系数设置为1
           | (1 << 2)                                        // 计数器清0
           | (2 << 8)                                      ; // 计数时钟设为SMCLK                                                          ;
  TA0CCR0  =  TIME                                         ; // SMCK=EX2=16MHz，设置计数器溢出时间为1ms  16位
  TA0CCTL0 = 0                                               // 初始化捕获控制
           | (1 << 4)                                      ; // 使能比较中断
  TA0CTL  |= (1 << 4)                                      ; // 设置计数器为加计数，启动
}


/*********************************************
中断函数写到 main.c
********************************************/
/*
#pragma vector=TIMER0_A0_VECTOR                             
__interrupt void Timer0_A0 (void)
{
  P1OUT   ^= 0xF0                                         ; 
  P9OUT   ^= 0x0F                                         ; 
  TA0CCR0  = (16000) - 1                                  ;
}
*/