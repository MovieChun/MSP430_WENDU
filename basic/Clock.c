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
  P7SEL     |= 0x03                                       ; // 端口选择外部低频晶振XT1
  UCSCTL6   &= ~(XT2OFF + XT1OFF)                         ; // 振荡器使能
  UCSCTL6   |= XCAP_3                                     ; // 设置内部负载电容
  UCSCTL3   |= SELREF_2                                   ; // FLLref = REFO
  UCSCTL4   |= SELA_0                                     ; // ACLK=XT1,SMCLK=DCO,MCLK=DCO
  //UCSCTL5   |= DIVA2                                      ;  //ACLK不分频 32k
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
//  Init_TimerA0(void): 设置TimerA0                                          //
//                                                                           //
//***************************************************************************//
void Init_Timer0_A5(void)
{  
  TA0CTL   = TASSEL0 + ID1 + ID0 + TACLR                   ; // 复位Timer0_A5, 分频系数设置为8
                                                          // 计数器清0
                                                          // 计数时钟设为ACLK                                                          ;
  TA0CCR0  =  TIME                                         ; 
  TA0CCTL0 = 0                                               // 初始化捕获控制
           | (1 << 4)                                      ; // 使能比较中断
  TA0CTL  |= MC0                                           ; // 设置计数器为加计数，启动*/
   
}

//***************************************************************************//
//                                                                           //
//  Init_TimerA1(void): 设置TimerA1                                          //
//  1MS的精确定时                                                                         //
//***************************************************************************//
unsigned int delay_ms = 0;   //精确延时
unsigned int uart1_ms = 0;   //串口1的接收延时
unsigned int uart2_ms = 0;   //串口2的接收延时
unsigned int SCI_ms = 0;
char uart1_flag = 0;         //串口1延时标志，0表示不再延时 
char uart2_flag = 0;         //串口2延时标志，0表示不再延时
char SCI_flag = 0;

void Init_Timer1_A3(void)
{  
  TA1CTL   = TASSEL0 + TACLR                               ; // 复位Timer1_A3, 分频系数设置为1
                                                             // 计数器清0
                                                             // 计数时钟设为ACLK                                                          ;
  TA1CCR0  =  31                                            ; // 设置计数器溢出时间为1ms  
  TA1CCTL0 = 0                                               // 初始化捕获控制
           | (1 << 4)                                      ; // 使能比较中断
  TA1CTL  |= MC0                                           ; // 设置计数器为加计数，启动*/
   
}



char* UART1_delay(unsigned int ms){
    uart1_ms = ms;
    uart1_flag = 1;
    TA1CTL  |= MC0;      // 启动定时器
    return &uart1_flag;
}

char* UART2_delay(unsigned int ms){
    uart2_ms = ms;
    uart2_flag = 1;
    TA1CTL  |= MC0;      // 启动定时器
    return &uart2_flag;
}


char* SCI_delay(unsigned int ms){
    SCI_ms = ms;
    SCI_flag = 1;
    TA1CTL  |= MC0;      // 启动定时器
    return &SCI_flag;
}


void Delay_ms(unsigned int ms){  //1ms延时函数
   delay_ms = ms;
   TA1CTL  |= MC0;       // 启动定时器
   while(delay_ms);
}



#include "uart.h"

#pragma vector=TIMER1_A0_VECTOR                             
__interrupt void Timer1_A0 (void)
{
  if(delay_ms > 0)delay_ms--;
  
  if(uart1_ms > 0)uart1_ms--;
  else uart1_flag = 0;   //延时标志复位
      
  if(uart2_ms > 0)uart2_ms--;
  else uart2_flag = 0;   //延时标志复位
  
  if(SCI_ms > 0)SCI_ms--;
  else SCI_flag = 0;   //延时标志复位
  
  if((delay_ms + uart2_flag + uart1_flag + SCI_flag) == 0){//不需要延时功能
      TA1CTL  &= ~MC0;  //关闭定时器
  } 
}
