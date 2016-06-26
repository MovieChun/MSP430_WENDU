

#ifndef _Fstring_h_
#define _Fstring_h_

#include "Fstring.h"

//用'?'在str中提取compare中对应的字符，返回到get中
//返回： -1  不包含
//       x   比较走过的字符数 
//注意： compare中第一个'?'后，只看位置，不做字符比较
//       一个'?'提取一个字符
//       compare中无'?'时，与下面的函数相同
extern int str_getchar(char str[],const char compare[] ,char get[]);



//str 是否包含 compare
//返回： -1  不包含
//       x   比较走过的字符数 
//compare中的'?'可跳过不确定字符
extern int str_include(char str[],const char compare[]);


/***************************************************************************
   函数名：  getIP(char * str, unsigned char *ip, unsigned int * port)
   说明  ：  获取IP地址，带有ip地址校验
			 字符串格式为 IP:255.255.255.255,500
			 所有符号都采用英文半角符号，不留有空格
   参数  ：  str ——命令字符串
             command ——提取帧头命令	 
             ip  ——ip地址数组
             port——端口号地址

   输出  ： 错误类型
            error = 0  : 输入无误，ip地址，端口号已更改
			errer = 1  : ip地址无效
			error = 2  : 找不到ip命令

***************************************************************************/

extern char getIP(char * str, const char* command,unsigned char *ip, unsigned int * port);

#endif