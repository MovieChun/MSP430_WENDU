
#include "msp430x54x.h"
#include <stdlib.h>
#include <stdio.h>
#include "IIC.h"
#include "mlx90614.h"

#define MLX_ADD  0x00     //7位地址




/**********************************************************
      函数名：mlx90614_init
      说明  ：mlx初始化函数
***********************************************************/

void mlx90614_INIT(void){
     IIC_INIT(MLX_ADD); 
}

/**********************************************************
      函数名：MLX_Write
      说明  ：往寄存器写入，一次只能写入一个字节
      参数  ：
              command 寄存器地址
              data 写入的地址
              len  写入长度
      输出  ：标志  正常结束返回1  异常结束返回0
***********************************************************/
char MLX_Write(unsigned char command , unsigned int  data){
  char c[3];
  c[0] = data & 0x00ff;
  c[1] = data >> 8;
  c[2] = 0;
  return IIC_writepage(command,c,3);
}

/**********************************************************
      函数名：MLX_WriteT
      说明  ：温度写入
      参数  ：写入的地址
      输出  ：标志  正常结束返回1  异常结束返回0
***********************************************************/
char MLX_WriteT(unsigned char command ,float Tdata){
    unsigned int data;
    char c[3];
    data = (unsigned int)(Tdata * 50);
    c[0] = data & 0x00ff;
    c[1] = data >> 8;
    c[2] = 0;
    return IIC_writepage(command,c,3);
}

/**********************************************************
      函数名：MLX_Read
      说明  ：mlx一次可读出3个字节，显示高位，低位，flag
      参数  ：读出的地址
      输出  ：标志  正常结束返回1  异常结束返回0
***********************************************************/
char MLX_Read(unsigned char command ,unsigned int *data){
   char c[3];
   IIC_readpage(command , c , 3 );
   *data = (unsigned int)c[1]<< 8 | c[0];
   return c[2];
}


/**********************************************************
      函数名：MLX_ReadT
      说明  ：温度读取
      参数  ：读出的地址
      输出  ：标志  正常结束返回1  异常结束返回0
***********************************************************/
char MLX_ReadT(unsigned char command ,float *Tdata){
    unsigned int data;
    char c[3];
    IIC_readpage(command , c , 3 );
    data = (unsigned int)c[1]<< 8 | c[0];
    *Tdata =((float)data * 2 - 27315)/100;  
    return c[2];
}


/**********************************************************
      函数名：MLX_RT
      说明  ：温度读取
      参数  ：无
      输出  ：外部温度值
***********************************************************/

float MLX_RT(void){
    unsigned int data;
    float Tdata = 0;
    char c[3];
    IIC_readpage(TOBJ1 , c , 3 );
    data = (unsigned int)c[1]<< 8 | c[0];
    Tdata =((float)data * 2 - 27315)/100;  
    return Tdata;
}
