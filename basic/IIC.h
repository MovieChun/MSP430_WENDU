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

#define SLAVE_ADD 0X00     //�ӻ���ַ
//IIC������־  0������  1��������  2��������
extern unsigned char IIC_flag;

void IIC_INIT(void);       //IIC��ʼ������
void IIC_write(unsigned char data ,unsigned char flag);  //IIC�жϷ��Ϳ���
unsigned char IIC_read(unsigned char flag);              //IIC�жϽ��տ���
void IIC_start(unsigned char flag);
char mlx_readpage(char word_addr, char *pword_buf, char len );

#endif