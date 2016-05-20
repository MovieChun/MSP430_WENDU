//===========================================================================//
//                                                                           //
// �ļ���  MAIN.C                                                            //
// ˵����  BW-DK5438�����嵥Ƭ��Ƭ��FLASH����/��/дʵ�����                  //
//         �������к�ÿ�ΰ��µ�4����һ�����������ζ���Ƭ��FLASH��ʼ        //
//         ��ַΪ0xE200��512�ֽ����ݺ��1�ݴ棬����0xE200�κ��ݴ���        //
//         ��д��FLASH                                                       //
// ���룺  IAR Embedded Workbench IDE for MSP430 v4.21                       //
// �汾��  v1.1                                                              //
// ��д��  JASON.ZHANG                                                       //
// ��Ȩ��  �������ղ�ά���Ӽ������޹�˾                                      //
//                                                                           //
//===========================================================================//

#include "msp430x54x.h"
#include <stdlib.h>
#include <stdio.h>

#define  FLL_FACTOR                 649                     // FLL_FACTOR: DCO��Ƶϵ��    


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
//                                                                           //
//������void ERASE(unsigned int)                                             //
//˵��������Ƭ��FLASH����                                                    //
//������address ����������ʼ��ַ                                             //
//                                                                           //
//***************************************************************************//
void FWRITE(unsigned int address, unsigned char* data,unsigned int length)
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