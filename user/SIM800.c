
#include "msp430x54x.h"
#include "Clock.h"
#include "uart.h"
#include "SCI.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
extern char event_SCI, *RXBuffer_uart;              //获取标志位，接收的数据
char  event_uart, *RXBuffer_uart;
int uart_num = 0;
char uart_error = 0,uart_get = 0;

#define RECMAX  512
char recive_data[RECMAX];
char com_num = 0;  //命令返回的字符串
#define RECIVE_RES()   uart_num = 0
//-------------GPRS指令表---------------------

//strcspn

//-------------GSM指令表---------------------

//----------------操作函数---------------------

/**************************************************
      接收比较
      ？或* 可以跳过不确定字符
*****************************************************/
char str_compare(char* str,const char* command ,char num){
      char flag = 0;
      int length = sizeof(command);
      int cnt = 0;
      while(*str != '\n' && *str != '\0')str++;  //跳过该行
      
      if(2 == num){                              //跳过第一个状态，返回第二个
         while(*str != '\n' && *str != '\0')str++;
         while(*str != '\n' && *str != '\0')str++;
      }
      
      if(*str == '\n')str++;
      
      for(cnt = 0;*str != '\0' && cnt < length;cnt++){
        if(*command == '?' || *command == '*'){
            command++;
            str++;
        }
        
        if(*str++ != *command++)break;
      }
      
      if(cnt == length )flag = 1;
     
      return flag;
}

/***********************************************
提取字符或字符串  ?提取一个字符  *提取往后的字符
************************************************/

char str_getchar(char* str,const char* command ,char num ,char *get){
      char flag = 0;
      int length = sizeof(command);
      int cnt = 0;
      
      while(*str != '\n' && *str != '\0')str++;  //跳过该行
      if(2 == num){                           //跳过第一个状态，返回第二个
         while(*str != '\n' && *str != '\0')str++;
         while(*str != '\n' && *str != '\0')str++;
      }
      
      if(*str == '\n')str++;
      for(cnt = 0;*str != '\0' && cnt < length;cnt++){
        if(*command == '?'){
            command++;
            *get = *str++;
        }
        else if(*command == '*'){
           while(*str != '\r') *get++ = *str++;
           break;
        }
        
        if(*str++ != *command++)break;
      }
      
      
      if(cnt == length )flag = 1;
        
      return flag;
}
/****************************************************
查看str1 是否包含 str2
若包含字符串2，返回str2在str1轴的后一个字符
否则返回-1
******************************************************/
int str_include(char* str,const char* command){
   char flag = 0; 
   int length = sizeof(command);
   int i = 0;
   int cnt = 0;
   
   if(*command == '\0') return 0;
   
   while(str[cnt] != '\0'){
       if(str[cnt] == command[i] )i++;
       else if(str[cnt] == command[0])i = 1;
       else i = 0;
       
       cnt++;
       if(i >= length){
         flag = 1;
         break;
       }    
   }
   if(0 == flag)cnt= -1;
   
   return cnt;  
}


/*****************************************************
    发送指令

*******************************************************/
#define  com_start()  UART_send("AT+")
#define  com_send     UART_send 
#define  com_set()    UART_send("=")
#define  com_state()  UART_send("?")
#define  com_quote()  UART_send("\"")


void com_end(void){
     char* flag;
     UART_send("\n");        //发送截止符
     uart_get = 0;           //接收数组归位
     flag = TIM1_delay(10);  //最长等待10ms
     while(*flag == 1);      //等待数据接收完成，若无返回则10ms后退出  
     
}

//向SIM发送指令  命令  需要回馈的状态  状态所在位置
//ip而外处理
//0 无问题  1 命令不符合  2 链接不上
char  SIM_command(const char*command ,const char* recomm){
      char error = 2;
      int length = 0;
      
      com_start();
      com_send((char*)command);
      com_end();
      if(1 == uart_get){
            error = 1;           //收到信号但不是OK 说明命令指令错误
            length = str_include(recive_data + length,recomm);
            if( -1 != length){
                 error = 0;      //能收到OK 说明状态返回不正确
            }
      }
         
      uart_get = 0;      
      
      SCI_send(recive_data);
      
      return error;
}

/****************************************************



******************************************************/
void send_data(void){  //查看
    while(delay_flag == 1);
    if(1 == uart_get){
       uart_get = 0;
       SCI_send(recive_data);   //发送给电脑
    }
}


//测试是否连接上模块
char SIM800_test(void){
     char error = 3;
     
     com_send("AT");
     com_end();
     //收到数据并且返回ok
     if(1 == uart_get){
       error = 2;
       if(-1 != str_include(recive_data,"OK"))error = 0;
     }
     
     
     SCI_send(recive_data);
     return error;
}




char SIM800_init(void){
     char ok = 0;
    
     
       while(SIM_command( "CGREG=1","\0"));
       while(SIM_command( "CGREG?","+CGREG: 1,1"));
    // SCI_send_num(100);
     
     return ok;
}

//----------------------------------------------------
//
//--------------------------------------------------------
char SIM800_status(void){  //查看链接状态
     char* flag;
     char i =0;
     char ok = 0;
     UART_send("AT+CIPSTATUS\n");
     uart_get = 0;
     flag = TIM1_delay(100);
     while(*flag);
     if(1 == uart_get){
       for(i = 0;recive_data[i] != '\n' && i< 6;i++);    //跳到第二行，即跳过 AT
       if('O'== recive_data[i+1] && 'K'==recive_data[i+2]) ok = 1;    //已接受到命令
       if(ok == 1){
          //flag = TIM1_delay(100);                            //再等100ms？ 
          //while(*flag);                                     //等待接收完成
          for(;recive_data[i] != '\n' && i< 100;i++);    //跳到第二行，即跳过 AT
          for(;recive_data[i] == 'S' && i< 100;i++);     //寻找closed的s
          if(recive_data[i + 1] == 'E')ok=2;
       }
     }
     uart_get = 0;
     
     SCI_send(recive_data);   //发送给电脑
     
     return ok;
}

//AT+CIPCLOSE  断开链接
char SIM800_close(void){  //查看链接状态
     char* flag;
     char i =0;
     char ok = 0;
     UART_send("AT+CIPCLOSE\n");
     uart_get = 0;
     flag = TIM1_delay(100);
     while(*flag);
     if(1 == uart_get){
       for(i = 0;recive_data[i] != '\n' && i< 6;i++);    //跳到第二行，即跳过 AT
       if('O'== recive_data[i+1] && 'K'==recive_data[i+2]) ok = 1;
     }
     uart_get = 0;
     
     SCI_send(recive_data);   //发送给电脑
     
     return ok;
}

char SIM800_Getip(char *ip,unsigned int port ){
     char* flag;
     char i =0;
     char ok = 0;
     
          
     UART_send("AT+CIPSTART = \"TCP\",\"");
     UART_send(ip);
     UART_send("\",");
     UART_send_num(port);
     UART_send("\n");
     uart_get = 0;
     flag = TIM1_delay(100);
     while(*flag);
     if(1 == uart_get){
       for(i = 0;recive_data[i] != '\n' && i< 6;i++);    //跳到第二行，即跳过 AT
       if('O'== recive_data[i+1] && 'K'==recive_data[i+2]) ok = 1;
     }
     uart_get = 0;
     
     SCI_send(recive_data);   //发送给电脑
     
     
     return ok;
}


char STM800_SEND(char *data){
  char* flag;
  char end[1]={0x1a};
  UART_send("AT+CIPSEND\n");
  
  flag = TIM1_delay(100);
  uart_get = 0;
  while(*flag);  
  SCI_send(recive_data);   //发送给电脑
  
  UART_send(data);
  UART_send(end);
  uart_get = 0;
  while(*flag);  
  SCI_send(recive_data);   //发送给电脑
  
  return 0;
}

//-------------------中断接收-------------------------
#pragma vector=USCI_A3_VECTOR
__interrupt void USCI_A3_ISR(void)
{
  switch(__even_in_range(UCA3IV,4))
  {
  case 0:break                                                     ; // Vector 0 - no interrupt
  case 2:                                                            // Vector 2 - RXIFG
        if(0 == uart_get)uart_num = 0;
        recive_data[uart_num++] = UCA3RXBUF;
        recive_data[uart_num] = '\0';
        uart_get = 1;
        if(uart_num < RECMAX - 1){
             uart_error = 0;
             TIM1_delay(10);     //100ms内没接收到新数据视为通信结束
        }else {
            uart_num = RECMAX - 2;
            uart_error = 1;
       }
   
      break                                                        ;
  case 4:break                                                     ;  // Vector 4 - TXIFG
  default: break                                                   ;  
  }  
}


