/**********************************************************************
      �ļ�����IIC.h
      ˵��  ��
              ���� ��CLK -> P3.6 -> Pin41
                     SDA -> P3.7 -> Pin42
      ʱ��  ��2016.05.24
      ����  ��������
**********************************************************************/

#ifndef _IIC_h_
#define _IIC_h_

#define SLAVE_ADD 0X48     //�ӻ���ַ
//IIC������־  0������  1��������  2��������
extern unsigned char IIC_flag;

void IIC_INIT(void);       //IIC��ʼ������
void IIC_write(unsigned char *data ,unsigned char num);  //IIC�жϷ��Ϳ���
void IIC_read(unsigned char *data ,unsigned char num);   //IIC�жϽ��տ���

#endif