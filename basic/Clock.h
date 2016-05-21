
#ifndef _clock_h_
#define _clock_h_

#define  TIME  ( 16000 - 1)          //
#define  EXTERNAL_HF_OSC     //使用外部时钟源
extern void Init_CLK(void);  //时钟初始化函数
extern void Init_Timer0_A5(void);

#endif