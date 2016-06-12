
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



//-------------GSMָ���---------------------

//----------------��������---------------------
char  re_commend[RECMAX];

//�鿴״̬
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

//���ò���
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


//�����Ƿ�������ģ��
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
          for(i = 0;recive_data[i] != '\n' && i< 6;i++);    //�����ڶ��У������� AT
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
     
     UART_send("AT+CGREG=1\n");   //ʹ������
     uart_get = 0;                //���ձ�־��λ
     flag = TIM1_delay(10);       //��ȴ�10ms
     while(*flag);                //�ȴ����ݽ������
     SCI_send(recive_data);       //���͸����Բ鿴����ֵ �ڶ��з���OK
     

     UART_send("AT+CGREG?\n");                         //����ָ��
     uart_get = 0;                                     //���ձ�־��λ
     flag = TIM1_delay(10);                            //��ȴ�ʱ��10ms
     while(*flag);                                     //�ȴ��������
     if(1 == uart_get){                                //����������
       for(i = 0;recive_data[i] != ',' && i< 20;i++);  //Ѱ�ҡ����� 
       if('1' == recive_data[i+1] || '5'==recive_data[i+1]) ok = 1; //1 ��ʾ���������� �� 5��ʾ����״̬
     }
     uart_get = 0;
     SCI_send(recive_data);  //���͸����� �鿴
     
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
       for(i = 0;recive_data[i] != '\n' && i< 6;i++);    //�����ڶ��У������� AT
       if('O'== recive_data[i+1] && 'K'==recive_data[i+2]) ok = 1;
     }
     uart_get = 0;
     
     SCI_send(recive_data);   //���͸�����
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


