
#include "msp430x54x.h"
#include <stdlib.h>
#include <stdio.h>
#include "IIC.h"
#include"mlx90614.h"

unsigned char MLS_ADD = 0x5A;     //7Œªµÿ÷∑
#define WRITE   (MLS_ADD << 1 | 0)
#define READ    (MLS_ADD << 1 | 1)
//-------------ROM---------------------

unsigned int  Read_in(void){
   unsigned int data;
   unsigned char c[3];
   //unsigned char flag;
   mlx_readpage(0x07,c, 3 );
  
   
   data = (unsigned int)c[1]<< 8 | c[0];
   data = data * 2 - 27315;
   return data;
}

unsigned int  Read_out(void){
   unsigned int data;
   unsigned int c[2];
   unsigned char flag;
   IIC_start(0);
   IIC_write( 0x06,0);
  
   IIC_start(0);
   c[0] = IIC_read(0);
   c[1] = IIC_read(0);
   flag = IIC_read(1);
   
   data = c[1]<< 8 | c[0];
   return data;
}