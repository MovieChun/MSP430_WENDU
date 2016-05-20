//===========================================================================//
//                                                                           //
// 文件：  MAIN.C                                                            //
// 说明：  BW-DK5438开发板单片机时钟设置实验程序                             //
//         设置单片机时钟为外部高频晶振或外部低频晶振+DCO，                  //
//         并将辅助时钟、主时钟和子主时钟通过IO引脚输出                      //
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
// 编译：  IAR Embedded Workbench IDE for MSP430 v4.21                       //
// 版本：  v1.1                                                              //
// 编写：  JASON.ZHANG                                                       //
// 版权：  北京拓普博维电子技术有限公司                                      //
//                                                                           //
//===========================================================================//

#include "msp430x54x.h"
#include <stdlib.h>
#include <stdio.h>


// 主时钟设置选项，可选择内部DCO或外部高频晶振
#define  INTERNAL_HF_OSC
//#define  EXTERNAL_HF_OSC

#ifdef   INTERNAL_HF_OSC
#define  INTERNAL_HF_OSC
#endif

#ifdef   EXTERNAL_HF_OSC
#define  EXTERNAL_HF_OSC
#endif


#define  FLL_FACTOR     749    // FLL_FACTOR: DCO倍频系数    


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
#endif

//***************************************************************************//
//                                                                           //
//                 初始化主时钟: MCLK = XT1×(FLL_FACTOR+1)                  //
//                                                                           //
//***************************************************************************//
#ifdef   INTERNAL_HF_OSC
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
//                 主程序: 设置时钟并输出至P11.0~2                           //
//                                                                           //
//***************************************************************************//
/*
int main( void )
{
  
  WDTCTL = WDTPW + WDTHOLD                                ; // 关闭看门狗
  Init_CLK()                                              ;
  P11DS  = TACK + TMCK + TSMCK                            ; // ACK、MCK、SMCK输出至P11.0/1/2
  P11SEL = TACK + TMCK + TSMCK                            ;
  P11DIR = TACK + TMCK + TSMCK                            ;
  MAIN_POWER_ON                                           ; // 指示时钟初始化成功
  while(1)                                                ;
}
*/