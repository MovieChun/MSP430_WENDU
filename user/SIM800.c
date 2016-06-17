
#include "msp430x54x.h"
#include "Clock.h"
#include "uart.h"
#include "SCI.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
extern char event_SCI, *RXBuffer_uart;              //��ȡ��־λ�����յ�����
char  event_uart, *RXBuffer_uart;
int uart_num = 0;
char uart_error = 0,uart_get = 0;

#define RECMAX  512
char recive_data[RECMAX];
char com_num = 0;  //����ص��ַ���
#define RECIVE_RES()   uart_num = 0
//-------------GPRSָ���---------------------

//strcspn

//-------------GSMָ���---------------------

//----------------��������---------------------

/**************************************************
      ���ձȽ�
      ����* ����������ȷ���ַ�
*****************************************************/
char str_compare(char* str,const char* command ,char num){
      char flag = 0;
      int length = sizeof(command);
      int cnt = 0;
      while(*str != '\n' && *str != '\0')str++;  //��������
      
      if(2 == num){                              //������һ��״̬�����صڶ���
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
��ȡ�ַ����ַ���  ?��ȡһ���ַ�  *��ȡ������ַ�
************************************************/

char str_getchar(char* str,const char* command ,char num ,char *get){
      char flag = 0;
      int length = sizeof(command);
      int cnt = 0;
      
      while(*str != '\n' && *str != '\0')str++;  //��������
      if(2 == num){                           //������һ��״̬�����صڶ���
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
�鿴str1 �Ƿ���� str2
�������ַ���2������str2��str1��ĺ�һ���ַ�
���򷵻�-1
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
    ����ָ��

*******************************************************/
#define  com_start()  UART_send("AT+")
#define  com_send     UART_send 
#define  com_set()    UART_send("=")
#define  com_state()  UART_send("?")
#define  com_quote()  UART_send("\"")


void com_end(void){
     char* flag;
     UART_send("\n");        //���ͽ�ֹ��
     uart_get = 0;           //���������λ
     flag = TIM1_delay(10);  //��ȴ�10ms
     while(*flag == 1);      //�ȴ����ݽ�����ɣ����޷�����10ms���˳�  
     
}

//��SIM����ָ��  ����  ��Ҫ������״̬  ״̬����λ��
//ip���⴦��
//0 ������  1 �������  2 ���Ӳ���
char  SIM_command(const char*command ,const char* recomm){
      char error = 2;
      int length = 0;
      
      com_start();
      com_send((char*)command);
      com_end();
      if(1 == uart_get){
            error = 1;           //�յ��źŵ�����OK ˵������ָ�����
            length = str_include(recive_data + length,recomm);
            if( -1 != length){
                 error = 0;      //���յ�OK ˵��״̬���ز���ȷ
            }
      }
         
      uart_get = 0;      
      
      SCI_send(recive_data);
      
      return error;
}

/****************************************************



******************************************************/
void send_data(void){  //�鿴
    while(delay_flag == 1);
    if(1 == uart_get){
       uart_get = 0;
       SCI_send(recive_data);   //���͸�����
    }
}


//�����Ƿ�������ģ��
char SIM800_test(void){
     char error = 3;
     
     com_send("AT");
     com_end();
     //�յ����ݲ��ҷ���ok
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
char SIM800_status(void){  //�鿴����״̬
     char* flag;
     char i =0;
     char ok = 0;
     UART_send("AT+CIPSTATUS\n");
     uart_get = 0;
     flag = TIM1_delay(100);
     while(*flag);
     if(1 == uart_get){
       for(i = 0;recive_data[i] != '\n' && i< 6;i++);    //�����ڶ��У������� AT
       if('O'== recive_data[i+1] && 'K'==recive_data[i+2]) ok = 1;    //�ѽ��ܵ�����
       if(ok == 1){
          //flag = TIM1_delay(100);                            //�ٵ�100ms�� 
          //while(*flag);                                     //�ȴ��������
          for(;recive_data[i] != '\n' && i< 100;i++);    //�����ڶ��У������� AT
          for(;recive_data[i] == 'S' && i< 100;i++);     //Ѱ��closed��s
          if(recive_data[i + 1] == 'E')ok=2;
       }
     }
     uart_get = 0;
     
     SCI_send(recive_data);   //���͸�����
     
     return ok;
}

//AT+CIPCLOSE  �Ͽ�����
char SIM800_close(void){  //�鿴����״̬
     char* flag;
     char i =0;
     char ok = 0;
     UART_send("AT+CIPCLOSE\n");
     uart_get = 0;
     flag = TIM1_delay(100);
     while(*flag);
     if(1 == uart_get){
       for(i = 0;recive_data[i] != '\n' && i< 6;i++);    //�����ڶ��У������� AT
       if('O'== recive_data[i+1] && 'K'==recive_data[i+2]) ok = 1;
     }
     uart_get = 0;
     
     SCI_send(recive_data);   //���͸�����
     
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
       for(i = 0;recive_data[i] != '\n' && i< 6;i++);    //�����ڶ��У������� AT
       if('O'== recive_data[i+1] && 'K'==recive_data[i+2]) ok = 1;
     }
     uart_get = 0;
     
     SCI_send(recive_data);   //���͸�����
     
     
     return ok;
}


char STM800_SEND(char *data){
  char* flag;
  char end[1]={0x1a};
  UART_send("AT+CIPSEND\n");
  
  flag = TIM1_delay(100);
  uart_get = 0;
  while(*flag);  
  SCI_send(recive_data);   //���͸�����
  
  UART_send(data);
  UART_send(end);
  uart_get = 0;
  while(*flag);  
  SCI_send(recive_data);   //���͸�����
  
  return 0;
}

//-------------------�жϽ���-------------------------
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
             TIM1_delay(10);     //100ms��û���յ���������Ϊͨ�Ž���
        }else {
            uart_num = RECMAX - 2;
            uart_error = 1;
       }
   
      break                                                        ;
  case 4:break                                                     ;  // Vector 4 - TXIFG
  default: break                                                   ;  
  }  
}


