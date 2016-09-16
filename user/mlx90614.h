#ifndef _mlx90614_h_
#define _mlx90614_h_

#include "Clock.h"

//-------------EEPROM---------------------
#define  TOMAX     0X20    //�¶����ֵ ��д
#define  TOMIN     0X21    //�¶���Сֵ ��д
#define  TA_RANG   0X23    //�¶ȷ�Χ   ��д
//-------------ROM-------------------------
#define  TA        0X06    //�����¶�    ֻ��
#define  TOBJ1     0X07    //�ⲿ�¶�1   ֻ��
#define  TOBJ2     0X08    //�ⲿ�¶�2   ֻ��
//-------------other-----------------------
#define  MLX_SLEEP  0XFF   //��������ģʽ

#define  MLX_sleep()  IIC_Command(MLX_SLEEP)   //�������������

extern void mlx90614_INIT(void);
extern char MLX_WriteT(unsigned char command ,float Tdata);
extern char MLX_Write(unsigned char command , unsigned int data);

extern char MLX_Read(unsigned char command ,unsigned int *data);
extern char MLX_ReadT(unsigned char command ,float *Tdata);

extern float MLX_RT(void); //�ⲿ�¶ȶ�ȡ
extern float TEM_AVE(void);
#endif