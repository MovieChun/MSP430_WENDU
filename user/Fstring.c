/*********************************************
    �ļ�����Fstring.c
    ˵�����ַ�������
***********************************************/

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


/***************************************************************************
   ��������  getIP(char * str, unsigned char *ip, unsigned int * port)
   ˵��  ��  ��ȡIP��ַ������ip��ַУ��
			 �ַ�����ʽΪ IP:255.255.255.255,500
			 ���з��Ŷ�����Ӣ�İ�Ƿ��ţ������пո�
   ����  ��  str ���������ַ���
	     ip  ����ip��ַ����
	     port�����˿ںŵ�ַ

   ���  �� 

***************************************************************************/

char getIP(char * str, const char* command,unsigned char *ip, unsigned int * port){
	char ok = 0;
	int a = 0;
	unsigned int n = 0;
	unsigned data[4];

	a = str_include(str, command);                        //�鿴֡ͷ
	if (a != -1){                                       //���յ��޸�ipָ��
		str += a;
		for (int i = 0; i < 4;i++){                        //ǰ�ĸ�������ip��ַ��������Ƕ˿ں�
			n = 0;
                        ok = 1;
			while (*str >= '0' && *str <= '9'){         //���ַ���ת��������
				n = n * 10 + *str - '0';
				str++;
			}
			if (n < 255){ data[i] =(unsigned char) n; }  //�������255����˵������Ч��ip��ַ
			else{
				ok = 0;                               //��ַ����
				break;
			}
			str++;
		}
		if (1 == ok){                                //ip��ַ���󣬽��ն˿ں�
			n = 0;
			while (*str >= '0' && *str <= '9'){
				n = n * 10 + *str - '0';
				str++;
			}
			*port = n;                                 //���Ķ˿ں�
			for (int i = 0; i < 4; i++){               //����ip��ַ
				ip[i] = data[i];
			}
		}
	}
	
        
	return ok;
}


/***************************************************************************
   ��������  char getAP(char * str, const char* command,unsigned char *name, unsigned char *key)
   ˵��  ��  ��ȡAP��������
   ����  ��  str ���������ַ���
	

   ���  ��

***************************************************************************/

char getAP(char * str, const char* command,char *name,char *key){
	char ok = 0;
	int a = 0;

	a = str_include(str, command);                        //�鿴֡ͷ
	if (a != -1){                                       //���յ��޸�ipָ��
		str += a;
		while (*str != ',' && *str != '\0'){         //���ַ���ת��������
		      *name++ = *str++;
		}
                *name = '\0';
                if(*str!='\0')str++;
                while (*str != ',' && *str != '\0'){         //���ַ���ת��������
		      *key++ = *str++;
		}
                *key = '\0';
                ok = 1;
	}        
	return ok;
}

/***************************************************************************
   ��������  char getphone(char *str ,char *phone,char *command)
   ˵��  ��  ��ȡIP��ַ������ip��ַУ��
			 �ַ�����ʽΪ IP:255.255.255.255,500
			 ���з��Ŷ�����Ӣ�İ�Ƿ��ţ������пո�
   ����  ��  str ���������ַ���
			 ip  ����ip��ַ����
			 port�����˿ںŵ�ַ

   ���  �� ��������
            error = 0  : ��������ip��ַ���˿ں��Ѹ���
			errer = 1  : ip��ַ��Ч
			error = 2  : �Ҳ���ip����

***************************************************************************/
char getphone(char *str ,char *phone,char *command){
  char cnt = 0;
  int a;
  a = str_include(str, command);                        //�鿴֡ͷ
    if (a != -1){                                       //���յ��޸�ipָ��
        str += a;
        for(cnt = 0;cnt < 11 && *str >= '0' && *str <= '9';cnt++){
           *phone++ = *str++;
        }
    }
  return 0;
}



