/**********************************************************************
      文件名：Clock.h
      说明  ：时钟初始化，定时器初始化
              引脚 ：低速晶振 32.768k
                     XIN  -> P7.0 -> Pin13
                     XOUT -> P7.1 -> Pin14
                     高速晶振 16M
                     XT2IN  -> P5.2 -> Pin89
                     XT2OUT -> P5.3 -> Pin90
      时间  ：2016.05.23
      补充  ：已测试
**********************************************************************/
#ifndef _clock_h_
#define _clock_h_

//#define  TIME   (15000 - 1)         //1min定时 
#define  TIME   (2500*8 - 1)         //10s定时  
//#define  TIME   (250 - 1)           //1s定时  

// 主时钟设置选项，可选择内部DCO或外部高频晶振
#define  EXTERNAL_HF_OSC           //使用外部时钟源  

extern void Init_CLK(void);        //时钟初始化函数
extern void Init_Timer0_A5(void);  //定时器初始化
extern void Init_Timer1_A3(void);


extern char uart1_flag;         //串口1延时标志，0表示不再延时 
extern char uart2_flag;         //串口2延时标志，0表示不再延时 
extern char SCI_flag;          

extern char* UART1_delay(unsigned int ms);
extern char* UART2_delay(unsigned int ms);
char* SCI_delay(unsigned int ms);
extern void  Delay_ms(unsigned int ms);

#endif