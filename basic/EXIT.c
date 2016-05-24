/**********************************************************************
      文件名：EXTI.c
      说明  ：P1口下降沿触发中断
      时间  ：2016.05.24
**********************************************************************/
#include "msp430x54x.h"
#include <stdlib.h>
#include <stdio.h>
#include "EXIT.h"

/*  临时注释 ，使用时删
1：关于 __bis_SR_register(LPM4_bits + GIE)

　 为了使单片机进入中断，在主函数中必须添加一句__bis_SR_register(LPM4_bits + GIE)"__bis_SR_register (LPM4_bits + GIE)（means go to sleep mode Low_Power_Mode_4 and with General_Interrupt_Enabled.）;其中LPM4_bits可以根据实际情况而改变，而GIE是系统中断使能位，必须置高才有效；而这也是其低能耗的一个重要体现。

2：关于__no_operation();  


Description

__no_operation inserts a NOP opcode into the code stream.

__no_operation is an intrinsic function and produces inline code.

*/

/***************************************************************
    函数名：EXIT_init1
    说明  ：P1口中断触发
           下降沿触发
**************************************************************/

void EXIT_init1(void)
{
  P1REN |= EXTI_IO;                            // Enable P1. internal resistance
  P1OUT |= EXTI_IO;                            // Set P1. as pull-Up resistance
  P1IE  |= EXTI_IO;                            // P1. interrupt enabled
  P1IES |= EXTI_IO;                            // P1. Hi/Lo edge
  P1IFG &= ~EXTI_IO;                           // P1. IFG cleared
  __bis_SR_register(LPM4_bits + GIE);          // Enter LPM4 w/interrupt
  __no_operation();                            // For debugger
  
}

/***********************************************
中断函数
***********************************************/
/*
// Port 1 interrupt service routine
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
 
  P1IFG &= ~EXTI_IO;                          // P1. IFG cleared
}
*/