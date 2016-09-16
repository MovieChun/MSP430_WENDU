/**********************************************************************
      文件名：flash.c
      说明  ：写入、读取flash中的数据
      时间  ：2016.05.23
      补充  ：已测试
**********************************************************************/

#include "msp430x54x.h"
#include <stdlib.h>
#include <stdio.h>
#include "flash.h"


//***************************************************************************//
//                                                                           //
//函数：void ERASE(unsigned int)                                             //
//说明：擦除片内FLASH数据段                                                  //
//参数：address ——擦除数据段地址                                           //
//                                                                           //
//***************************************************************************//
void FERASE(unsigned int address)
{
  unsigned int *Erase_address                             ;
  Erase_address  = (unsigned int *)address                ;
  FCTL1          = FWKEY + ERASE                          ;
  FCTL3          = FWKEY                                  ;
  *Erase_address = 0                                      ;
  FCTL1          = FWKEY                                  ;
  FCTL3          = FWKEY + LOCK                           ;
}

//***************************************************************************//
//                                                                           
//函数：Flash_write                                                          
//说明：将数据存入flash                                                      
//参数：address --起始地址  data--数据数组  length--数据长度                                          
//                                                                          
//***************************************************************************//

void Flash_write(unsigned int address, unsigned char* data,unsigned int length)
{
  unsigned int   i;
  unsigned char* Wr_Addr;
  Wr_Addr =(unsigned char*)address;  
  FCTL1   = FWKEY + WRT; // Set WRT bit for write operation
  FCTL3   = FWKEY;
  for(i=0;i<length;i++)
  {
    *Wr_Addr= data[i] ;
    Wr_Addr++ ;
  }
  FCTL1   = FWKEY;
  FCTL3   = FWKEY + LOCK ;  
}

//***************************************************************************//
//                                                                           
//函数：Flash_write                                                          
//说明：读取flash中的数据                                                     
//参数：address --起始地址  data--数据数组  length--数据长度                                          
//                                                                          
//***************************************************************************//
void Flash_read(unsigned int address, unsigned char* data,unsigned int length)
{
  unsigned int   i;                  
  unsigned char* Wr_Addr;
  Wr_Addr =(unsigned char*)address;  
  for(i=0;i<length;i++)
  {
    data[i] = *Wr_Addr ;     //读取数据
    Wr_Addr++ ;
  }
}

