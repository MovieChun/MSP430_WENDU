
#include "msp430x54x.h"
#include "SIM800.h"
#include "Clock.h"
#include "uart.h"
#include "SCI.h"
#include "Fstring.h"
#include <stdlib.h>
#include <stdio.h>
#define RECMAX  512   //����������
#define SIM_DEBUG   //������Ϸģʽ

int  uart_num = 0;      //���ڽ�������
char uart_error = 0;    //�������������

char SIM_getf = 0;      //SIMģ����յ����ݱ�־
char SIM_data[RECMAX];  //SIMģ�����ݽ�������



//����ͽ������
void com_end(void){
     UART_send("\n");        //���ͽ�ֹ��
     SIM_getf = 0;           //���������λ
     UART1_delay(10);  //��ȴ�10ms
     while(uart1_flag);      //�ȴ����ݽ�����ɣ����޷�����10ms���˳�      
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
      
      com_end();      //�Ȱ��ϸ��������
      com_start();
      com_send((char*)command);
      com_end();
      if(1 == SIM_getf){
            error = 1;           //�յ��źŵ�����OK ˵������ָ�����
            length = str_include(SIM_data,recomm);
            if( -1 != length){
                 error = 0;      //���յ�OK ˵��״̬���ز���ȷ
            }
      }      
      SIM_getf = 0;      

#ifdef SIM_DEBUG
        SCI_send(SIM_data);
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
      if(1 == SIM_getf){
            error = 1;           //�յ��źŵ�����OK ˵������ָ�����
            length = str_getchar(SIM_data, recomm, get);
            if( -1 != length){
                 error = 0;      //���յ�OK ˵��״̬���ز���ȷ
            }
      }
         
      SIM_getf = 0;      

#ifdef SIM_DEBUG
        SCI_send(SIM_data);
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
     char error = 2;
     
     UART1_delay(ms);
     while(uart1_flag);
     if(1 == SIM_getf){          //�Ѿ��յ�����
       error = 1;
       if(-1 != str_include(SIM_data,recomm)){
         error = 0;
         //SIM_getf = 0;
       }
     }    
     
     
#ifdef SIM_DEBUG
       SCI_send(SIM_data);
       if(error == 0)SCI_send("  test ok\n");
       else SCI_send("  test no\n");
#endif     
     
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
  
     if(1 == SIM_getf){
       error = 1;
       if(-1 != str_include(SIM_data,"OK"))error = 0;
     }

#ifdef SIM_DEBUG
        SCI_send(SIM_data);
#endif     

     return error;
}






char SIM800_init(void){
     char ok = 0;
 
     char get[10] = "ab";
     
       //while(SIM_command( "CGREG=1","\0"));
       //while(SIM_comGet( "CGREG?","+CGREG: 1,?",get)); //? 1��ʾ�Ѿ�����  5��ʾ����
       //while(SIM_command( "CIPSTATUS","OK"));  //�鿴����״̬

      SIM_command( "CGREG=1","\0");
      SIM_comGet( "CGREG?","+CGREG: 1,?",get);
      SIM_command( "CIPSTATUS","OK");
     return ok;
}






char SIM800_Getip(unsigned char *ip,unsigned int port ){
     char ok = 0;
     
     UART_send("AT+CIPSTART = \"TCP\",\"");
     
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
     UART1_delay(100);  //��ȴ�10ms
     while(uart1_flag);      //�ȴ����ݽ�����ɣ����޷�����10ms���˳�    
     
#ifdef SIM_DEBUG
        SCI_send(SIM_data);
#endif
     
     return ok;
}


//------------GPRS���ͺ���------------
char GPRS_Start(void){
  char error = 3;
  error = SIM_command( "CIPSEND",">");

#ifdef SIM_DEBUG
        SCI_send(SIM_data);
#endif
  
  return error;
}

char GPRS_SendEnd(void){
    UART_send_char(0x1a); //���ͽ�ֹ��
     
    SIM_getf = 0;
    UART1_delay(10);  //��ȴ�10ms
    while(uart1_flag == 1);      //�ȴ����ݽ�����ɣ����޷�����10ms���˳�    
     
#ifdef SIM_DEBUG
        SCI_send(SIM_data);
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
        if(0 == SIM_getf)uart_num = 0;
        SIM_data[uart_num++] = UCA3RXBUF;
        SIM_data[uart_num] = '\0';
        SIM_getf = 1;
        if(uart_num < RECMAX - 1){
             uart_error = 0;
             UART1_delay(10);                                         //10ms��û���յ���������Ϊͨ�Ž���
        }else {
            uart_num = RECMAX - 2;
            uart_error = 1;
       }
   
      break                                                        ;
  case 4:break                                                     ;  // Vector 4 - TXIFG
  default: break                                                   ;  
  }  
}


