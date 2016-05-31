/**********************************************************************
      �ļ�����IIC.h
      ˵��  ��
              ���� ��SCL -> P5.4 -> Pin51
                     SDA -> P3.7 -> Pin42
      ʱ��  ��2016.05.30
      
**********************************************************************/
#ifndef _IIC_h_
#define _IIC_h_


void IIC_INIT(unsigned char slave_add);                       //IIC��ʼ������
char IIC_readpage(char command, char *pword_buf, char len );  //д������  0�쳣�ж� 1�������
char IIC_writepage(char command, char *pword_buf, char len);  //��������  0�쳣�ж� 1�������
void IIC_SLAVE_ADD(unsigned char add);                        //д��ӻ���ַ
char  IIC_Command(char command);                              //д����

#endif