

#ifndef _Fstring_h_
#define _Fstring_h_

#include "Fstring.h"

//��'?'��str����ȡcompare�ж�Ӧ���ַ������ص�get��
//���أ� -1  ������
//       x   �Ƚ��߹����ַ��� 
//ע�⣺ compare�е�һ��'?'��ֻ��λ�ã������ַ��Ƚ�
//       һ��'?'��ȡһ���ַ�
//       compare����'?'ʱ��������ĺ�����ͬ
extern int str_getchar(char str[],const char compare[] ,char get[]);



//str �Ƿ���� compare
//���أ� -1  ������
//       x   �Ƚ��߹����ַ��� 
//compare�е�'?'��������ȷ���ַ�
extern int str_include(char str[],const char compare[]);


/***************************************************************************
   ��������  getIP(char * str, unsigned char *ip, unsigned int * port)
   ˵��  ��  ��ȡIP��ַ������ip��ַУ��
			 �ַ�����ʽΪ IP:255.255.255.255,500
			 ���з��Ŷ�����Ӣ�İ�Ƿ��ţ������пո�
   ����  ��  str ���������ַ���
             command ������ȡ֡ͷ����	 
             ip  ����ip��ַ����
             port�����˿ںŵ�ַ

   ���  �� ��������
            error = 0  : ��������ip��ַ���˿ں��Ѹ���
			errer = 1  : ip��ַ��Ч
			error = 2  : �Ҳ���ip����

***************************************************************************/

extern char getIP(char * str, const char* command,unsigned char *ip, unsigned int * port);


extern char getphone(char *str ,char *phone,char *command);

extern char getAP(char * str, const char* command,char *name,char *key);
extern char getUI(char * str, const char* command,unsigned int * num);
#endif