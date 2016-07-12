
#include "msp430x54x.h"
#include "SIM800.h"
#include "Clock.h"
#include "uart.h"
#include "SCI.h"
#include "Fstring.h"
#include <stdlib.h>
#include <stdio.h>


int  uart_num = 0;      //串口接收数量
char uart_error = 0;    //数据溢出最大错误

char SIM_getf = 0;      //SIM模块接收到数据标志
char SIM_data[RECMAX];  //SIM模块数据接收数组



//命令发送结束语句
void com_end(void){
     UART_send("\r\n");        //发送截止符
     SIM_getf = 0;           //接收数组归位
     UART1_delay(10);  //最长等待10ms
     while(uart1_flag);      //等待数据接收完成，若无返回则10ms后退出      
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
      
      //com_end();      //先把上个命令清空
      com_start();
      com_send((char*)command);
      com_end();
      if(1 == SIM_getf){
            error = 1;           //收到信号但不是OK 说明命令指令错误
            length = str_include(SIM_data,recomm);
            if( -1 != length){
                 error = 0;      //能收到OK 说明状态返回不正确
            }
      }      
      SIM_getf = 0;      

#ifdef SIM_DEBUG
        SCI_send(SIM_data);
#endif
      
      //return error;
        if(0 == error)return 1;
        else return 0;
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
      if(1 == SIM_getf){
            error = 1;           //收到信号但不是OK 说明命令指令错误
            length = str_getchar(SIM_data, recomm, get);
            if( -1 != length){
                 error = 0;      //能收到OK 说明状态返回不正确
            }
      }
         
      SIM_getf = 0;      

#ifdef SIM_DEBUG
        SCI_send(SIM_data);
#endif
      //return error;
        if(0 == error)return 1;
        else return 0;
}


/******************************************************************
    函数名：SIM_DelayRecom(unsigned int ms ,const char *recomm)
    功能： 延时查看返回的字符串
    参数：
           recomm  比对字符串
    返回： 0  错误
           1  正确
******************************************************************/
char SIM_DelayRecom(const char *recomm){
     char ok = 0;
    
       if(-1 != str_include(SIM_data,recomm)){
         ok = 1;
       }
         
#ifdef SIM_DEBUG
       SCI_send(SIM_data);
#endif     
     
     return ok;
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
  
     if(1 == SIM_getf){
       error = 1;
       if(-1 != str_include(SIM_data,"OK"))error = 0;
     }
       SIM_getf = 0;

#ifdef SIM_DEBUG
        SCI_send(SIM_data);
#endif     

     return error;
}






char SIM800_init(void){
     char ok = 0; 
     char get[10] = "ab";      
     SIM_comGet( "CGREG?","+CGREG: 1,?",get);
      
     return ok;
}






char SIM800_Getip(unsigned char *ip,unsigned int port ){
     char ok = 0;
     
     UART_send("AT+CIPSTART=\"TCP\",\"");
     
     UART_send_num(ip[0]);
     UART_send(".");
     UART_send_num(ip[1]);
     UART_send(".");
     UART_send_num(ip[2]);
     UART_send(".");
     UART_send_num(ip[3]);
    
     UART_send("\",");
     UART_send_num(port);
     UART_send("\n");

     SIM_getf = 0;
     UART1_delay(100);  //最长等待10ms
     while(uart1_flag);      //等待数据接收完成，若无返回则10ms后退出    
     
#ifdef SIM_DEBUG
        SCI_send(SIM_data);
#endif
     
     return ok;
}


//------------GPRS发送函数------------
char GPRS_Start(void){
  char error = 3;
  error = SIM_command( "CIPSEND",">");

#ifdef SIM_DEBUG
        SCI_send(SIM_data);
#endif
  
  return error;
}


char GPRS_SendEnd(void){
    UART_send_char(0x1a); //发送截止符
     
    SIM_getf = 0;
    UART1_delay(10);  //最长等待10ms
    while(uart1_flag == 1);      //等待数据接收完成，若无返回则10ms后退出    
     
#ifdef SIM_DEBUG
        SCI_send(SIM_data);
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
        if(0 == SIM_getf)uart_num = 0;
        SIM_data[uart_num++] = UCA3RXBUF;
        SIM_data[uart_num] = '\0';
        SIM_getf = 1;
        if(uart_num < RECMAX - 1){
             uart_error = 0;
             UART1_delay(10);                                         //10ms内没接收到新数据视为通信结束
        }else {
            uart_num = RECMAX - 2;
            uart_error = 1;
       }
   
      break                                                        ;
  case 4:break                                                     ;  // Vector 4 - TXIFG
  default: break                                                   ;  
  }  
}


//--------------短信--------------------------
char note_send(char *phone , char*str){   //AT+CMGS="+8618712760783"
      //com_end();      //先把上个命令清空
      com_start();
      com_send("AT+CMGS=\"");
      com_send(phone); 
      com_send("\"");
      com_end();
      
      com_send(str);
      UART_send_char(0x1a); //发送截止符
      
      return 0; 
}