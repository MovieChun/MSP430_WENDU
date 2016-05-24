/**********************************************************************
      �ļ�����flash.c
      ˵��  ��д�롢��ȡflash�е�����
      ʱ��  ��2016.05.23
      ����  ���Ѳ���
**********************************************************************/

#include "msp430x54x.h"
#include <stdlib.h>
#include <stdio.h>
#include "flash.h"


//***************************************************************************//
//                                                                           //
//������void ERASE(unsigned int)                                             //
//˵��������Ƭ��FLASH���ݶ�                                                  //
//������address �����������ݶε�ַ                                           //
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
//������Flash_write                                                          
//˵���������ݴ���flash                                                      
//������address --��ʼ��ַ  data--��������  length--���ݳ���                                          
//                                                                          
//***************************************************************************//

void Flash_write(unsigned int address, unsigned char* data,unsigned int length)
{
  unsigned int   i;
  unsigned char* Wr_Addr;
  FERASE(address);  //����ǰ�ѵ�һ�����㣬�����ڴ洢�ȶ�
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
//������Flash_write                                                          
//˵������ȡflash�е�����                                                     
//������address --��ʼ��ַ  data--��������  length--���ݳ���                                          
//                                                                          
//***************************************************************************//
void Flash_read(unsigned int address, unsigned char* data,unsigned int length)
{
  unsigned int   i                                        ;                    ;
  unsigned char* Wr_Addr;
  Wr_Addr =(unsigned char*)address;  
  for(i=0;i<length;i++)
  {
    data[i] = *Wr_Addr ;     //��ȡ����
    Wr_Addr++ ;
  }
}

