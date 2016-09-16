

#ifndef _WIFI_INIT_h_
#define _WIFI_INIT_h_

#include "msp430x54x.h"
#include "Clock.h"
#include "uart2.h"
#include "SCI.h"
#include "Fstring.h"


#define WIFI_TX_UP       P5SEL &= ~BIT6;\
                         P5OUT |= BIT6

#define WIFI_TX_DOWN     P5SEL &= ~BIT6;\
                         P5OUT &= ~BIT6
                                         //低电平唤醒，关闭模块也保持低电平？
#define WIFI_TX_ON       P5OUT |= BIT6;\
                         P5SEL |= BIT6  //低电平唤醒，关闭模块也保持低电平？

#define WIFI_RX_DOWN       //关闭后保持低电平？

#define WIFI_UART_ON     UCA1CTL1 &= ~UCSWRST;\
                         P5DIR |= BIT6

#define WIFI_UART_OFF    UCA1CTL1 |= UCSWRST;\
                         P5DIR &= ~BIT6


#define wifi_DEBUG               //开启调戏模式
#define REMAX        128          //接收区容量

//进入命令模式
char  wifi_start(void);
//退出命令模式，写入1重启wifi模块
char  wifi_end(char mode);
//写入命令
char  wifi_command(char *command , char* data,char mode);  
//写入ip地址和端口号
char  wifi_IP(unsigned char *ip,unsigned int port);  
//写入路由器的名字和密码
char  wifi_AP(char *name,char *password);   
//检测是否连接上PTC，1 连接，0断开
char  wifi_TCPtest(void);      
//搜索附近的是否存在想要的AP
char  SearchAP(char* name);     

extern int  uart2_num;        //串口接收数量
extern char uart2_error;      //数据溢出最大错误

extern char wifi_getf;       //WIFI模块接收到数据标志
extern char wifi_data[REMAX];//WIFI模块数据接收数组

#endif