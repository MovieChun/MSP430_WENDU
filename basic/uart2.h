/**********************************************************************
      文件名：uart2.h
      说明  ：串口驱动, 用于WIFI模块
              波特率：115200
              中断接收
              引脚 ：TXD -> P5.6 -> Pin53
                     RXD -> P5.7 -> Pin54
      时间  ：2016.05.23
      补充  ：已测试
**********************************************************************/
#ifndef _uart2_h_
#define _uart2_h_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

extern void UART2_init(void);                       //串口初始化  高速精确模式
extern void UART2_send(char *tx_buf);               //发送字符串函数
extern void UART2_sendn(char *tx_buf,char num);     //发送n个字符
extern void UART2_send_num(unsigned int num);       //发送数组函数
extern void UART2_send_char(char tx_buf);            //发送一个字符

#endif