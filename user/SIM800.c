
#include "msp430x54x.h"
#include "SIM800.h"
#include "Clock.h"
#include "uart.h"
#include "SCI.h"
#include "Fstring.h"
#include <stdlib.h>
#include <stdio.h>

#define SIM_DEBUG   //������Ϸģʽ

int  uart_num = 0;
char uart_error = 0,uart_get = 0;
char recive_data[RECMAX];


//����ͽ������
void com_end(void){
     char* flag;
     UART_send("\n");        //���ͽ�ֹ��
     uart_get = 0;           //���������λ
     flag = TIM1_delay(10);  //��ȴ�10ms
     while(*flag == 1);      //�ȴ����ݽ�����ɣ����޷�����10ms���˳�      
}

/******************************************************************
    ��������SIM_command(const char*command ,const char* recomm)
    ���ܣ�д��������ȽϷ���״̬
    ������ command  �����ַ���������"AT+"
           recomm   ���ص�״̬�Ƚϣ��õ��ǰ�������
    ���أ���������  0  û�д���
                    1  ���ز���
                    2  û�з���
******************************************************************/
char  SIM_command(const char*command ,const char* recomm){
      char error = 2;
      int length = 0;
      
      com_start();
      com_send((char*)command);
      com_end();
      if(1 == uart_get){
            error = 1;           //�յ��źŵ�����OK ˵������ָ�����
            length = str_include(recive_data,recomm);
            if( -1 != length){
                 error = 0;      //���յ�OK ˵��״̬���ز���ȷ
            }
      }      
      uart_get = 0;      

#ifdef SIM_DEBUG
        SCI_send(recive_data);
#endif
      
      return error;
}


/******************************************************************
    ��������SIM_comGet(const char*command ,const char* recomm,char *get)
    ���ܣ� ��'?'��ȡ״̬�ַ������ȽϷ���״̬
    ������ command  �����ַ���������"AT+"
           recomm   ���ص�״̬�Ƚϣ��õ��ǰ�������
    ���أ���������  0  û�д���
                    1  ���ز���
                    2  û�з���
******************************************************************/
char  SIM_comGet(const char*command ,const char* recomm,char *get){
      char error = 2;
      int length = 0;
      
      com_start();
      com_send((char*)command);
      com_end();
      if(1 == uart_get){
            error = 1;           //�յ��źŵ�����OK ˵������ָ�����
            length = str_getchar(recive_data, recomm, get);
            if( -1 != length){
                 error = 0;      //���յ�OK ˵��״̬���ز���ȷ
            }
      }
         
      uart_get = 0;      

#ifdef SIM_DEBUG
        SCI_send(recive_data);
#endif
      return error;
}


/******************************************************************
    ��������SIM_DelayRecom(unsigned int ms ,const char *recomm)
    ���ܣ� ��ʱ�鿴���ص��ַ���
    ������ ms   �ȴ�ʱ��
           recomm  �ȶ��ַ���
    ���أ���������  0  û�д���
                    1  ���ز���
                    2  û�з���
******************************************************************/
char SIM_DelayRecom(unsigned int ms ,const char *recomm){
     char *flag;
     char error = 2;
     
     flag = TIM1_delay(ms);
     while(*flag == 1);
     if(1 == uart_get){          //�Ѿ��յ�����
       error = 1;
       if(-1 != str_include(recive_data,recomm)){
         error = 0;
         uart_get = 0;
       }
     }    
     return error;
}
/******************************************************************
    ��������SIM800_test(void)
    ���ܣ� �����Ƿ�������ģ��
    ���أ� ��������  0  û�д���
                    1  ���ز���
                    2  û�з���
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
       //while(SIM_comGet( "CGREG?","+CGREG: 1,?",get)); //? 1��ʾ�Ѿ�����  5��ʾ����
       //while(SIM_command( "CIPCLOSE", "CLOSED"));  //�Ͽ�����
       while(SIM_command( "CIPSTATUS","OK"));  //�鿴����״̬
       //flag = TIM1_delay(100);  //��ȴ�10ms
       //while(*flag == 1);      //�ȴ����ݽ�����ɣ����޷�����10ms���˳� 
      // while(-1 == str_include(recive_data,"CONNECTING"));
       
       //while(SIM_command( "AT+CIPSTATUS\n","CLOSED"));  //�鿴����״̬
       //while(SIM_command( "AT+CIPSTATUS\n","CLOSED"));  //�鿴����״̬
       
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
     flag = TIM1_delay(10);  //��ȴ�10ms
     while(*flag == 1);      //�ȴ����ݽ�����ɣ����޷�����10ms���˳�    
     
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




//-------------------�жϽ���-------------------------//
//---------------����SIM���ص�����--------------------//
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


