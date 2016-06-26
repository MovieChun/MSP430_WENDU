/**********************************************************************
      文件名：sci.c
      说明  ：串口驱动
              波特率：115200
              中断接收
              引脚 ：TXD -> P9.4 -> Pin72
                     RXD -> P9.5 -> Pin73
      时间  ：2016.05.23
      补充  : 用于上位机调试
**********************************************************************/
#ifndef _sci_h_
#define _sci_h_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SCI_PORT_SEL   P9SEL                       //定义IO口
#define SCI_PORT_DIR   P9DIR

#define SCI_TXD   BIT4
#define SCI_RXD   BIT5

extern char event_SCI, RXBuffer_SCI;              //获取标志位，接收的数据
extern void SCI_init(void);                     //串口初始化  低速省电模式
extern void SCI_send(char *tx_buf);               //发送字符串函数
extern void SCI_send2(char *tx_buf,char num);     //发送数组函数
extern void SCI_send_num(unsigned int num);       //发送整数
extern void SCI_send_float(float f2);       //发送小数点后两位精度的浮点数

#endif