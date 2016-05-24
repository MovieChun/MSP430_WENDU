/**********************************************************************
      文件名：flash.h
      说明  ：写入、读取flash中的数据
      时间  ：2016.05.23
      补充  ：已测试
**********************************************************************/
#ifndef _flash_h_
#define _flash_h_

#define SAVE_ADD  0XE200    //存储地址

extern void FERASE(unsigned int address);  //擦除地址
//读取flash中的数据  参数：储存首地址  存入数组  存入长度
extern void Flash_read(unsigned int address, unsigned char* data,unsigned int length);
//将数据存入flash中  参数：读取首地址  读取数组  读取长度
extern void Flash_write(unsigned int address, unsigned char* data,unsigned int length);


#endif