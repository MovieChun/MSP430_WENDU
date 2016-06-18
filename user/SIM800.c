
#include "msp430x54x.h"
#include "SIM800.h"
#include "Clock.h"
#include "uart.h"
#include "SCI.h"
#include "Fstring.h"
#include <stdlib.h>
#include <stdio.h>

#define SIM_DEBUG   //开启调戏模式

int  uart_num = 0;
char uart_error = 0,uart_get = 0;
char recive_data[RECMAX];


//命令发送结束语句
void com_end(void){
     char* flag;
     UART_send("\n");        //发送截止符
     uart_get = 0;           //接收数组归位
     flag = TIM1_delay(10);  //最长等待10ms
     while(*flag == 1);      //等待数据接收完成，若无返回则10ms后退出      
}

/******************************************************************
    函数名：SIM_command(const char*command ,const char* recomm)
    功能：写入命令，并比较返回状态
    参数： command  命令字符串，不用"AT+"
           recomm   返回的状态比较，用的是包含查找
    返回：错误类型  0  没有错误
                    1  返回不符
                    2  没有返回
******************************************************************/
char  SIM_command(const char*command ,const char* recomm){
      char error = 2;
      int length = 0;
      
      com_start();
      com_send((char*)command);
      com_end();
      if(1 == uart_get){
            error = 1;           //收到信号但不是OK 说明命令指令错误
            length = str_include(recive_data,recomm);
            if( -1 != length){
                 error = 0;      //能收到OK 说明状态返回不正确
            }
      }      
      uart_get = 0;      

#ifdef SIM_DEBUG
        SCI_send(recive_data);
#endif
      
      return error;
}


/******************************************************************
    函数名：SIM_comGet(const char*command ,const char* recomm,char *get)
    功能： 用'?'提取状态字符，并比较返回状态
    参数： command  命令字符串，不用"AT+"
           recomm   返回的状态比较，用的是包含查找
    返回：错误类型  0  没有错误
                    1  返回不符
                    2  没有返回
******************************************************************/
char  SIM_comGet(const char*command ,const char* recomm,char *get){
      char error = 2;
      int length = 0;
      
      com_start();
      com_send((char*)command);
      com_end();
      if(1 == uart_get){
            error = 1;           //收到信号但不是OK 说明命令指令错误
            length = str_getchar(recive_data, recomm, get);
            if( -1 != length){
                 error = 0;      //能收到OK 说明状态返回不正确
            }
      }
         
      uart_get = 0;      

#ifdef SIM_DEBUG
        SCI_send(recive_data);
#endif
      return error;
}


/******************************************************************
    函数名：SIM_DelayRecom(unsigned int ms ,const char *recomm)
    功能： 延时查看返回的字符串
    参数： ms   等待时间
           recomm  比对字符串
    返回：错误类型  0  没有错误
                    1  返回不符
                    2  没有返回
******************************************************************/
char SIM_DelayRecom(unsigned int ms ,const char *recomm){
     char *flag;
     char error = 2;
     
     flag = TIM1_delay(ms);
     while(*flag == 1);
     if(1 == uart_get){          //已经收到数据
       error = 1;
       if(-1 != str_include(recive_data,recomm)){
         error = 0;
         uart_get = 0;
       }
     }    
     return error;
}
/******************************************************************
    函数名：SIM800_test(void)
    功能： 测试是否连接上模块
    返回： 错误类型  0  没有错误
                    1  返回不符
                    2  没有返回
******************************************************************/
char SIM800_test(void){
     char error = 2;  
     com_send("AT");
     com_end();
     if(1 == uart_get){
       error = 1;
       if(-1 != str_include(recive_data,"OK"))error = 0;
     }

#ifdef SIM_DEBUG
        SCI_send(recive_data);
#endif     

     return error;
}






char SIM800_init(void){
     char ok = 0;
     //char *flag;
     //char get[10] = "ab";
       //SIM_command( "CIPSTATUS","CLOSED");
       //while(SIM_command( "CGREG=1","\0"));
       //while(SIM_comGet( "CGREG?","+CGREG: 1,?",get)); //? 1表示已经连接  5表示漫游
       //while(SIM_command( "CIPCLOSE", "CLOSED"));  //断开连接
       while(SIM_command( "CIPSTATUS","OK"));  //查看链接状态
       //flag = TIM1_delay(100);  //最长等待10ms
       //while(*flag == 1);      //等待数据接收完成，若无返回则10ms后退出 
      // while(-1 == str_include(recive_data,"CONNECTING"));
       
       //while(SIM_command( "AT+CIPSTATUS\n","CLOSED"));  //查看链接状态
       //while(SIM_command( "AT+CIPSTATUS\n","CLOSED"));  //查看链接状态
       
       // SCI_send_num(str_include("asdf qazwsx","qaz?s"));
       //SCI_send(get);
       //SCI_send("\n");
     
     return ok;
}






char SIM800_Getip(char *ip,unsigned int port ){
     char *flag ;
     char ok = 0;
     
     UART_send("AT+CIPSTART = \"TCP\",\"");
     UART_send(ip);
     UART_send("\",");
     UART_send_num(port);
     UART_send("\n");
     uart_get = 0; 
     uart_get = 0;
     flag = TIM1_delay(10);  //最长等待10ms
     while(*flag == 1);      //等待数据接收完成，若无返回则10ms后退出    
     
#ifdef SIM_DEBUG
        SCI_send(recive_data);
#endif
     
     return ok;
}


char STM800_SEND(char *data){
  char* flag;
  char end[1]={0x1a};
  UART_send("AT+CIPSEND\n");
  
  flag = TIM1_delay(100);
  uart_get = 0;
  while(*flag);  

#ifdef SIM_DEBUG
        SCI_send(recive_data);
#endif
  
  UART_send(data);
  UART_send(end);
  uart_get = 0;
  while(*flag);  

#ifdef SIM_DEBUG
        SCI_send(recive_data);
#endif
  
  return 0;
}




//-------------------中断接收-------------------------//
//---------------接收SIM返回的数据--------------------//
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


