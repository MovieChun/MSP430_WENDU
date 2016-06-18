

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

#endif