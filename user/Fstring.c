/*********************************************
    文件名：Fstring.c
    说明：字符串处理
***********************************************/

#include "Fstring.h"
/***********************************************************************
*  函数名：str_getchar(char str[],const char compare[] ,char get[])    *
*  功能：提取字符串中对应位置的字符                                    *
*  参数： str ——待处理字符串                                         *
*         compare  --  比较字符串 ，?将对应位置的字符提取出来          *
*         get  -- 提取字符串                                           *
*                                                                      *
*  输出：-1 -- 比较数组不在带处理字符串中                              *
*        其他 -- 比较完成，指针与处理数组开始的距离                    *
*                                                                      *
*  注意：可用多个'?'，遇上第一个'?'后只比较位置，不比较字符            *           
*  测试时间： 2016.06.18                                               *
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
*  函数名：str_include(char str[],const char compare[])                  *
*  功能：差看str,是否包含compare                                         *
*  参数： str ——待处理字符串                                           *
*         compare  --  比较字符串 ，?可跳过不确定的字符                  *
*                                                                        *
*  输出：-1 -- 比较数组不在带处理字符串中                                *
*        其他 -- 比较完成，指针与处理数组开始的距离                      *
*                                                                        *
*  测试时间： 2016.06.18                                                 *
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
   函数名：  getIP(char * str, unsigned char *ip, unsigned int * port)
   说明  ：  获取IP地址，带有ip地址校验
			 字符串格式为 IP:255.255.255.255,500
			 所有符号都采用英文半角符号，不留有空格
   参数  ：  str ——命令字符串
	     ip  ——ip地址数组
	     port——端口号地址

   输出  ： 

***************************************************************************/

char getIP(char * str, const char* command,unsigned char *ip, unsigned int * port){
	char ok = 0;
	int a = 0;
	unsigned int n = 0;
	unsigned data[4];

	a = str_include(str, command);                        //查看帧头
	if (a != -1){                                       //接收到修改ip指令
		str += a;
		for (int i = 0; i < 4;i++){                        //前四个数字是ip地址，第五个是端口号
			n = 0;
                        ok = 1;
			while (*str >= '0' && *str <= '9'){         //将字符串转化成数字
				n = n * 10 + *str - '0';
				str++;
			}
			if (n < 255){ data[i] =(unsigned char) n; }  //如果大于255，则说明是无效的ip地址
			else{
				ok = 0;                               //地址不对
				break;
			}
			str++;
		}
		if (1 == ok){                                //ip地址无误，接收端口号
			n = 0;
			while (*str >= '0' && *str <= '9'){
				n = n * 10 + *str - '0';
				str++;
			}
			*port = n;                                 //更改端口号
			for (int i = 0; i < 4; i++){               //更改ip地址
				ip[i] = data[i];
			}
		}
	}
	
        
	return ok;
}


/***************************************************************************
   函数名：  char getAP(char * str, const char* command,unsigned char *name, unsigned char *key)
   说明  ：  获取AP名和密码
   参数  ：  str ——命令字符串
	

   输出  ：

***************************************************************************/

char getAP(char * str, const char* command,char *name,char *key){
	char ok = 0;
	int a = 0;

	a = str_include(str, command);                        //查看帧头
	if (a != -1){                                       //接收到修改ip指令
		str += a;
		while (*str != ',' && *str != '\0'){         //将字符串转化成数字
		      *name++ = *str++;
		}
                *name = '\0';
                if(*str!='\0')str++;
                while (*str != ',' && *str != '\0'){         //将字符串转化成数字
		      *key++ = *str++;
		}
                *key = '\0';
                ok = 1;
	}        
	return ok;
}

/***************************************************************************
   函数名：  char getphone(char *str ,char *phone,char *command)
   说明  ：  获取IP地址，带有ip地址校验
			 字符串格式为 IP:255.255.255.255,500
			 所有符号都采用英文半角符号，不留有空格
   参数  ：  str ——命令字符串
			 ip  ——ip地址数组
			 port——端口号地址

   输出  ： 错误类型
            error = 0  : 输入无误，ip地址，端口号已更改
			errer = 1  : ip地址无效
			error = 2  : 找不到ip命令

***************************************************************************/
char getphone(char *str ,char *phone,char *command){
  char cnt = 0;
  int a;
  a = str_include(str, command);                        //查看帧头
    if (a != -1){                                       //接收到修改ip指令
        str += a;
        for(cnt = 0;cnt < 11 && *str >= '0' && *str <= '9';cnt++){
           *phone++ = *str++;
        }
    }
  return 0;
}



