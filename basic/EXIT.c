/**********************************************************************
      �ļ�����EXTI.c
      ˵��  ��P1���½��ش����ж�
      ʱ��  ��2016.05.24
**********************************************************************/
#include "msp430x54x.h"
#include <stdlib.h>
#include <stdio.h>
#include "EXIT.h"

/*  ��ʱע�� ��ʹ��ʱɾ
1������ __bis_SR_register(LPM4_bits + GIE)

�� Ϊ��ʹ��Ƭ�������жϣ����������б������һ��__bis_SR_register(LPM4_bits + GIE)"__bis_SR_register (LPM4_bits + GIE)��means go to sleep mode Low_Power_Mode_4 and with General_Interrupt_Enabled.��;����LPM4_bits���Ը���ʵ��������ı䣬��GIE��ϵͳ�ж�ʹ��λ�������ø߲���Ч������Ҳ������ܺĵ�һ����Ҫ���֡�

2������__no_operation();  


Description

__no_operation inserts a NOP opcode into the code stream.

__no_operation is an intrinsic function and produces inline code.

*/

/***************************************************************
    ��������EXIT_init1
    ˵��  ��P1���жϴ���
           �½��ش���
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
�жϺ���
***********************************************/
/*
// Port 1 interrupt service routine
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
 
  P1IFG &= ~EXTI_IO;                          // P1. IFG cleared
}
*/