/**********************************************************************
      文件名：uart.h
      说明  ：串口驱动
              波特率：115200
              中断接收
              引脚 ：TXD -> P10.4 -> Pin80
                     RXD -> P10.5 -> Pin81
      时间  ：2016.05.23
      补充  ：已测试
**********************************************************************/
#ifndef _uart_h_
#define _uart_h_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define UART_PORT_SEL   P10SEL                       //定义IO口
#define UART_PORT_DIR   P10DIR

#define UART_TXD   BIT4
#define UART_RXD   BIT5


extern void UART_init(void);                       //串口初始化  高速精确模式
extern void UART_send(char *tx_buf);               //发送字符串函数
extern void UART_send2(char *tx_buf,char num);     //发送数组函数
extern void UART_send_num(unsigned int num);       //发送数组函数
extern void UART_send_char(char tx_buf);            //发送一个字符
extern void UART_send_float(float f2);

#endif