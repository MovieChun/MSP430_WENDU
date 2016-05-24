/**********************************************************************
      文件名：SPI.h
      说明  ：
              引脚 ：SPI_MISO -> P3.1 -> Pin34
                     SPI_MOSI -> P3.2 -> Pin35
                     SPI_CLK  -> P3.3 -> Pin36 
      时间  ：2016.05.24
      补充  ：待测试
**********************************************************************/
#ifndef _spi_h_
#define _spi_h_

void Init_SPI(void);                        //SPI初始化
inline void SPI_write(unsigned char data);  //写入一个字节
inline unsigned char SPI_read(void);        //读出一个字节
                         
                        
#endif