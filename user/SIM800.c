
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



//-------------GSM指令表---------------------

//----------------操作函数---------------------
char  re_commend[RECMAX];

//查看状态
char* SIM800_START(char*commend){
     char* flag;
     int i = 0;
     UART_send(commend);
     uart_get = 0;
     flag = TIM1_delay(100);
     while(*flag == 1);
     if(uart_get == 1){
        for(i = 0;i < RECMAX && recive_data[i] != '\0';i++)
        re_commend[i] = recive_data[i];
      }
     else {
       for(i = 0;i < 5;i++)
         re_commend[i] = 'X';
       re_commend[i] = '\0';
     }
     //SCI_send(re_commend); 
     return re_commend;  
}

//设置参数
char* SIM800_SET(char*commend,unsigned int data){
     char* flag;
     UART_send(commend);
     UART_send("=");
     UART_send_num(data);
     uart_get = 0;
     flag = TIM1_delay(100);
     while(*flag == 1);
     for(int i = 0;i < RECMAX && recive_data[i] != '\0';i++)
        re_commend[i] = recive_data[i];
     //SCI_send(recive_data); 
     return re_commend;  
}


//测试是否连接上模块
char SIM800_test(void){
     char* flag;
     char ok = 0;
     char i = 0;
     UART_send("AT\n");
     uart_get = 0;
     flag = TIM1_delay(100);
     
     while(*flag == 1);
     if(1 == uart_get){
       if(recive_data[0] == 'O'){
          if(recive_data[1] == 'K')ok = 1;
       }
       else{
          for(i = 0;recive_data[i] != '\n' && i< 6;i++);    //跳到第二行，即跳过 AT
          if('O'== recive_data[i+1] && 'K'==recive_data[i+2]) ok = 1;
       }
     }
     uart_get = 0;
     
     SCI_send(recive_data);     
     return ok;
}


char SIM800_init(void){
     char* flag;
     char ok = 0;
     char i = 0;
     
     UART_send("AT+CGREG=1\n");   //使用网络
     uart_get = 0;                //接收标志复位
     flag = TIM1_delay(10);       //最长等待10ms
     while(*flag);                //等待数据接收完成
     SCI_send(recive_data);       //发送给电脑查看返回值 第二行返回OK
     

     UART_send("AT+CGREG?\n");                         //发送指令
     uart_get = 0;                                     //接收标志复位
     flag = TIM1_delay(10);                            //最长等待时间10ms
     while(*flag);                                     //等待接收完成
     if(1 == uart_get){                                //有数据输入
       for(i = 0;recive_data[i] != ',' && i< 20;i++);  //寻找“，” 
       if('1' == recive_data[i+1] || '5'==recive_data[i+1]) ok = 1; //1 表示已连接网络 ； 5表示漫游状态
     }
     uart_get = 0;
     SCI_send(recive_data);  //发送给电脑 查看
     
     return ok;
}

char SIM800_Getip(char *ip,unsigned int port ){
     char* flag;
     char i =0;
     char ok = 0;
     uart_get = 0;
     UART_send("AT+CIPSTART = \"TCP\",\"");
     UART_send(ip);
     UART_send("\",");
     UART_send_num(port);
     UART_send("\n");
     
     flag = TIM1_delay(100);
     while(*flag);
     if(1 == uart_get){
       for(i = 0;recive_data[i] != '\n' && i< 6;i++);    //跳到第二行，即跳过 AT
       if('O'== recive_data[i+1] && 'K'==recive_data[i+2]) ok = 1;
     }
     uart_get = 0;
     
     SCI_send(recive_data);   //发送给电脑
     uart_get = 0;  
     
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


