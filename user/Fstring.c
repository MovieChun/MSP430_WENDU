/*********************************************
    �ļ�����Fstring.c
    ˵�����ַ�������
***********************************************/

#include "msp430x54x.h"
#include "Fstring.h"
/***********************************************************************
*  ��������str_getchar(char str[],const char compare[] ,char get[])    *
*  ���ܣ���ȡ�ַ����ж�Ӧλ�õ��ַ�                                    *
*  ������ str �����������ַ���                                         *
*         compare  --  �Ƚ��ַ��� ��?����Ӧλ�õ��ַ���ȡ����          *
*         get  -- ��ȡ�ַ���                                           *
*                                                                      *
*  �����-1 -- �Ƚ����鲻�ڴ������ַ�����                              *
*        ���� -- �Ƚ���ɣ�ָ���봦�����鿪ʼ�ľ���                    *
*                                                                      *
*  ע�⣺���ö��'?'�����ϵ�һ��'?'��ֻ�Ƚ�λ�ã����Ƚ��ַ�            *           
*  ����ʱ�䣺 2016.06.18                                               *
************************************************************************/

int str_getchar(char str[],const char compare[] ,char get[]){
      char flag = 0;
      int cnt = 0;
      int i = 0;
      
      if(*compare == '\0') return 0;
     
       while(str[cnt] != '\0'){
         if('?' == compare[i] || '\0'== compare[i]){
           flag =1;
           break;
         }
       
         if(str[cnt] == compare[i] )i++;
         else if(str[cnt] == compare[0])i = 1;
         else i = 0;
       
         cnt++;
      }
      
   
      if(flag == 0){ cnt = -1;}
      else{
        while(compare[i] != '\0' && str[cnt] != '\0'){
          if(compare[i] == '?')*get++ = str[cnt];
          i++;
          cnt++;
        }
      }      
      return cnt;
}
/*************************************************************************
*  ��������str_include(char str[],const char compare[])                  *
*  ���ܣ��str,�Ƿ����compare                                         *
*  ������ str �����������ַ���                                           *
*         compare  --  �Ƚ��ַ��� ��?��������ȷ�����ַ�                  *
*                                                                        *
*  �����-1 -- �Ƚ����鲻�ڴ������ַ�����                                *
*        ���� -- �Ƚ���ɣ�ָ���봦�����鿪ʼ�ľ���                      *
*                                                                        *
*  ����ʱ�䣺 2016.06.18                                                 *
**************************************************************************/
int str_include(char str[],const char compare[]){
   char flag = 0; 
   int i = 0;
   int cnt = 0;
   
   if(*compare == '\0') return 0;
   while(str[cnt] != '\0'){
       
       if(compare[i] == '?')i++;
       else if(str[cnt] == compare[i] )i++;
       else if(str[cnt] == compare[0])i = 1;
       else i = 0;
       
       cnt++;
       if(compare[i] == '\0'){
         flag = 1;
         break;
       }    
   }
   if(0 == flag)cnt= -1;
   
   return cnt;  
}

