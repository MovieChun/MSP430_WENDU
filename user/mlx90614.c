
#include "msp430x54x.h"
#include <stdlib.h>
#include <stdio.h>
#include "IIC.h"
#include "mlx90614.h"

#define MLX_ADD  0x00     //7λ��ַ




/**********************************************************
      ��������mlx90614_init
      ˵��  ��mlx��ʼ������
***********************************************************/

void mlx90614_INIT(void){
     IIC_INIT(MLX_ADD); 
}

/**********************************************************
      ��������MLX_Write
      ˵��  �����Ĵ���д�룬һ��ֻ��д��һ���ֽ�
      ����  ��
              command �Ĵ�����ַ
              data д��ĵ�ַ
              len  д�볤��
      ���  ����־  ������������1  �쳣��������0
***********************************************************/
char MLX_Write(unsigned char command , unsigned int  data){
  char c[3];
  c[0] = data & 0x00ff;
  c[1] = data >> 8;
  c[2] = 0;
  return IIC_writepage(command,c,3);
}

/**********************************************************
      ��������MLX_WriteT
      ˵��  ���¶�д��
      ����  ��д��ĵ�ַ
      ���  ����־  ������������1  �쳣��������0
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
      ��������MLX_Read
      ˵��  ��mlxһ�οɶ���3���ֽڣ���ʾ��λ����λ��flag
      ����  �������ĵ�ַ
      ���  ����־  ������������1  �쳣��������0
***********************************************************/
char MLX_Read(unsigned char command ,unsigned int *data){
   char c[3];
   IIC_readpage(command , c , 3 );
   *data = (unsigned int)c[1]<< 8 | c[0];
   return c[2];
}


/**********************************************************
      ��������MLX_ReadT
      ˵��  ���¶ȶ�ȡ
      ����  �������ĵ�ַ
      ���  ����־  ������������1  �쳣��������0
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
      ��������MLX_RT
      ˵��  ���¶ȶ�ȡ
      ����  ����
      ���  ���ⲿ�¶�ֵ
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
