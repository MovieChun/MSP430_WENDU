//===========================================================================//
//                                                                           //
// 文件：  MAIN.C                                                            //
// 说明：  BW-DK5438开发板单片机片内FLASH擦除/读/写实验程序                  //
//         程序运行后，每次按下第4行任一按键，则依次读出片内FLASH起始        //
//         地址为0xE200的512字节数据后加1暂存，擦除0xE200段后将暂存数        //
//         据写会FLASH                                                       //
// 编译：  IAR Embedded Workbench IDE for MSP430 v4.21                       //
// 版本：  v1.1                                                              //
// 编写：  JASON.ZHANG                                                       //
// 版权：  北京拓普博维电子技术有限公司                                      //
//                                                                           //
//===========================================================================//

#include "msp430x54x.h"
#include <stdlib.h>
#include <stdio.h>



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



void Flash_write(unsigned int address, unsigned char* data,unsigned int length)
{
  unsigned int   i                                        ;
  unsigned char* Wr_Addr                                  ;
  Wr_Addr =(unsigned char*)address                        ;
  FCTL1   = FWKEY + WRT                                   ; // Set WRT bit for write operation
  FCTL3   = FWKEY                                         ;
  for(i=0;i<length;i++)
  {
    *Wr_Addr= data[i]                                     ;
    Wr_Addr++                                             ;
  }
  FCTL1   = FWKEY                                         ;
  FCTL3   = FWKEY + LOCK                                  ;  
}

/**********************************************************
      FLASH 存储
**********************************************************/
void Flash_read(unsigned int address, unsigned char* data,unsigned int length)
{
  unsigned int   i                                        ;
  unsigned char* Wr_Addr                                  ;
  Wr_Addr =(unsigned char*)address                        ;
  //FCTL1   = FWKEY + WRT                                   ; // Set WRT bit for write operation
  //FCTL3   = FWKEY                                         ;
  for(i=0;i<length;i++)
  {
    data[i] =  *Wr_Addr                                   ;
    Wr_Addr++                                             ;
  }
  //FCTL1   = FWKEY                                         ;
  //FCTL3   = FWKEY + LOCK                                  ;  
}


/*
int main( void )
{
  unsigned char temp[1000]                                ;
  unsigned int  i                                         ;
  char *addr                                              ;
  WDTCTL = WDTPW + WDTHOLD                                ;
  Init_CLK()                                              ;
  Init_Port()                                             ;
  for(;;)
  {
    if(!(P6IN&KPR0))
    {
      addr   = (char *)0xE200                             ;
      for(i=0;i<512;i++)
      {
        temp[i]  = *addr++                                ; 
        temp[i]++                                         ;
      }
      FERASE(0xE200)                                      ;
      FWRITE(0xE200,temp,512)                             ;
    }
  }
}
*/