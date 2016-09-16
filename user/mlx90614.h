#ifndef _mlx90614_h_
#define _mlx90614_h_

#include "Clock.h"

//-------------EEPROM---------------------
#define  TOMAX     0X20    //温度最大值 读写
#define  TOMIN     0X21    //温度最小值 读写
#define  TA_RANG   0X23    //温度范围   读写
//-------------ROM-------------------------
#define  TA        0X06    //本身温度    只读
#define  TOBJ1     0X07    //外部温度1   只读
#define  TOBJ2     0X08    //外部温度2   只读
//-------------other-----------------------
#define  MLX_SLEEP  0XFF   //进入休眠模式

#define  MLX_sleep()  IIC_Command(MLX_SLEEP)   //这个函数有问题

extern void mlx90614_INIT(void);
extern char MLX_WriteT(unsigned char command ,float Tdata);
extern char MLX_Write(unsigned char command , unsigned int data);

extern char MLX_Read(unsigned char command ,unsigned int *data);
extern char MLX_ReadT(unsigned char command ,float *Tdata);

extern float MLX_RT(void); //外部温度读取
extern float TEM_AVE(void);
#endif