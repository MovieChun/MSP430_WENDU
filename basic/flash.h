/**********************************************************************
      �ļ�����flash.h
      ˵��  ��д�롢��ȡflash�е�����
      ʱ��  ��2016.05.23
      ����  ���Ѳ���
**********************************************************************/
#ifndef _flash_h_
#define _flash_h_

#define SAVE_ADD  0XE200    //�洢��ַ

extern void FERASE(unsigned int address);  //������ַ
//��ȡflash�е�����  �����������׵�ַ  ��������  ���볤��
extern void Flash_read(unsigned int address, unsigned char* data,unsigned int length);
//�����ݴ���flash��  ��������ȡ�׵�ַ  ��ȡ����  ��ȡ����
extern void Flash_write(unsigned int address, unsigned char* data,unsigned int length);


#endif