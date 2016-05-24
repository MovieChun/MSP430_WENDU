/**********************************************************************
      文件名：sci.h
      说明  ：串口驱动
              波特率：9600
              中断接收
              引脚 ：TXD -> P10.4 -> Pin80
                     RXD -> P10.5 -> Pin81
      时间  ：2016.05.23
      补充  ：已测试
**********************************************************************/
#ifndef _sci_h_
#define _sci_h_

#define RS_PORT_SEL   P10SEL                       //定义IO口
#define RS_PORT_DIR   P10DIR

#define TXD   BIT4
#define RXD   BIT5

extern char  event_SCI, RXBuffer_SCI;              //获取标志位，接收的数据
extern void UART_init_L(void);                     //串口初始化  低速省电模式
extern void UART_init_H(void);                     //串口初始化  高速精确模式
extern void UART_send(char *tx_buf);               //发送字符串函数
extern void UART_send_num(char *tx_buf,char num);  //发送数组函数

#endif