

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

#endif