/*********************************************
    文件名：Fstring.c
    说明：字符串处理
***********************************************/

#include "msp430x54x.h"
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

