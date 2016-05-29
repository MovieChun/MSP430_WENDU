/**********************************************************************
      文件名：IIC.h
      说明  ：
              引脚 ：CLK -> P3.6 -> Pin41
                     SDA -> P3.7 -> Pin42
      时间  ：2016.05.24
      补充  ：待测试
**********************************************************************/

#ifndef _IIC_h_
#define _IIC_h_

#define SLAVE_ADD 0X00     //从机地址
//IIC工作标志  0：空闲  1：发送中  2：接收中
extern unsigned char IIC_flag;

void IIC_INIT(void);       //IIC初始化函数
void IIC_write(unsigned char data ,unsigned char flag);  //IIC中断发送开启
unsigned char IIC_read(unsigned char flag);              //IIC中断接收开启
void IIC_start(unsigned char flag);
char mlx_readpage(char word_addr, char *pword_buf, char len );

#endif