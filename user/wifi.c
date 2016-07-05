
#include "msp430x54x.h"
#include "Clock.h"
#include "uart2.h"
#include "SCI.h"
#include "Fstring.h"
#include "wifi.h"

//#define wifi_DEBUG       //������Ϸģʽ
#define REMAX     512    //����������

int  uart2_num = 0;      //���ڽ�������
char uart2_error = 0;    //�������������

char wifi_getf = 0;      //SIMģ����յ����ݱ�־
char wifi_data[REMAX];   //SIMģ�����ݽ�������

/*==============================================================
wifiģ�鱸��¼��
    ��������"+++",����"a"��3s�ڷ���a,���ɽ�������ģʽ
    wifiģ��ʹ�õĵ�ַ�Ͷ˿ں��Ǿ�������ip��ַ�Ͷ˿ں�
    ����·������at+wsta=DC411,c411123456   //���AP�������룬AP��ֻ������ĸ������
    ����IP��ַ��at+socka=TCPC,192.168.31.102,8000  //TCP����
    �˳�����ģʽ��at+entm
    ���������� at+z  //�ı�IP��ַ��ʹ��
    �رջ��ԣ� at+e=off
    ��������AP: at+wscan
    �ָ��������ã�at+reld
    �������ߣ�at+slp
    ���ù���ģʽ��at+wmode=sta
    ʹ�ö�̬IP:at+wann=dhcp
================================================================*/

/******************************************************************
    ��������wifi_start
    ���ܣ���������ģʽ,ͬʱҲ�ǲ��Թ���
    ��������
    ���أ���������  0  û�д���
                    1  ���ز���
                    2  û�з���
******************************************************************/
char  wifi_start(void){
      char error = 2;
      int length = 0;
     
      UART2_send("+++");    //����"+++"��"a"��������ģʽ 
      wifi_getf = 0;
      UART2_delay(10);
      while(uart2_flag);
     
      UART2_send("a");       
      UART2_delay(10);
      while(uart2_flag);
      
      if(1 == wifi_getf){
            error = 1;           //�յ��źŵ�����OK ˵������ָ�����
            //SCI_send("\n--------");
            //SCI_send(wifi_data);
            //SCI_send("\n");
            length = str_include(wifi_data,"OK");
            if( -1 != length){              
                error = 0;      //���յ�OK ˵��״̬���ز���ȷ
            }
      }      
      wifi_getf = 0;      


#ifdef wifi_DEBUG
        SCI_send(wifi_data);
#endif
      
      return error;
}

/******************************************************************
    ��������wifi_end
    ���ܣ��˳�����ģʽ
    ��������
    ���أ���������  0  û�д���
                    1  ���ز���
                    2  û�з���
******************************************************************/
char  wifi_end(char mode){
      char error = 2;
      int length = 0;
      
      UART2_delay(1);
      if(mode == 0)UART2_send("AT+ENTM\n");    //�˳�����ģʽ
      else UART2_send("AT+Z\n");    //�˳�����ģʽ
      wifi_getf = 0;
      UART2_delay(10);
      while(uart2_flag);
      
      if(1 == wifi_getf){
            error = 1;           //�յ��źŵ�����OK ˵������ָ�����
            
            length = str_include(wifi_data,"OK");
            if( -1 != length){
                 error = 0;      //���յ�OK ˵��״̬���ز���ȷ
            }
      }      
      wifi_getf = 0;      

#ifdef wifi_DEBUG
        SCI_send(wifi_data);
#endif
      
      return error;
}

/******************************************************************
    ��������wifi_command(char *command , char* data,char mode)
    ���ܣ� ����д�룬ģʽ0��ȡ״̬��ģʽ1д������
    ������ command ���� ��������
           data ����ģʽ0 �����ص�״̬
                    ģʽ1 ��д�������
           mode ����ģʽѡ��
    ���أ���������  0  û�д���
                    1  ���ز���
                    2  û�з���
******************************************************************/
char  wifi_command(char *command , char* data,char mode){
      char error = 2;
      int length = 0;
      
      UART2_send("AT+");
      UART2_send(command);
      if(mode == 1){   //�����ݣ���д��ָ�����鿴״̬
          UART2_send("=");
          UART2_send(data);
      }
      UART2_send("\n");
      
      wifi_getf = 0;
      UART2_delay(100);
      while(uart2_flag);
      
      if(1 == wifi_getf){
            error = 1;           //�յ��źŵ�����OK ˵������ָ�����
            //SCI_send("\n--------");
            //SCI_send(wifi_data);
            //SCI_send("---------\n");
            
            if(mode == 1) length = str_include(wifi_data,"OK");
            else length = str_include(wifi_data,data);
            if( -1 != length){
                 error = 0;      //���յ�OK ˵��״̬���ز���ȷ
            }
      }           


#ifdef wifi_DEBUG
        SCI_send(wifi_data);
#endif
      
      return error;
}


/******************************************************************
    ��������wifi_IP(unsigned char *ip,unsigned int port)
    ���ܣ�д��IP�Ͷ˿ں�
    ������ip ����ip��
          port �����˿ں�
    ���أ���������  0  û�д���
                    1  ���ز���
                    2  û�з���
******************************************************************/
char wifi_IP(unsigned char *ip,unsigned int port){
     char error = 2;
     
     UART2_send("AT+SOCKA=");
     UART2_send("TCPC,");
     
     UART2_send_num(ip[0]);
     UART2_send(".");
     UART2_send_num(ip[1]);
     UART2_send(".");
     UART2_send_num(ip[2]);
     UART2_send(".");
     UART2_send_num(ip[3]);
     UART2_send(",");
     UART2_send_num(port);
     UART2_send("\n");

     wifi_getf = 0;
     UART2_delay(10);  //��ȴ�10ms
     while(uart2_flag);      //�ȴ����ݽ�����ɣ����޷�����10ms���˳�   
     
     if(1 == wifi_getf){
        error = 1;
        if(-1 != str_include(wifi_data,"OK"))error = 0;   //�޸�ip�ɹ�
     }
     wifi_getf = 0;
     
     

#ifdef wifi_DEBUG
        SCI_send(wifi_data);
#endif
           
     
     return error; 
}

/******************************************************************
    ��������wifi_AP(char *name,char *password)
    ���ܣ�д��·������Ϣ
    ������name ����wifi����
          password ����wifi����
    ���أ���������  0  ipд��
                    1  ���ز���
                    2  û�з���
                    3  û������·����
******************************************************************/
char wifi_AP(char *name,char *password){
     char error = 2;
     
     UART2_send("AT+WSTA=");   //д�����ֺ�����
     UART2_send(name);
     UART2_send(",");
     UART2_send(password);
     UART2_send("\n");
     wifi_getf = 0;
     UART2_delay(10);         //��ȴ�10ms
     while(uart2_flag);       //�ȴ����ݽ�����ɣ����޷�����10ms���˳�
     
     if(1 == wifi_getf){
        error = 1;
        if(-1 != str_include(wifi_data,"OK"))error = 0;   //�޸�ip�ɹ�
     }
     wifi_getf = 0;
     
#ifdef wifi_DEBUG
        SCI_send(wifi_data);
#endif
                
     return error; 
}

/******************************************************************
    ��������SearchAP(char* name)
    ���ܣ�д��·������Ϣ
    ������name ����wifi����
    ���أ���������  0  ipд��
                    1  û������·����
                    2  û�з���
******************************************************************/
char SearchAP(char* name){
     char error = 2;
     
     UART2_send("AT+WSCAN\n"); //������������·����
     wifi_getf = 0;
     UART2_delay(10);          //��ȴ�10ms
     while(uart2_flag);        //�ȴ����ݽ�����ɣ����޷�����10ms���˳�
     UART2_delay(100);         //��ȴ�100ms�����ص�ʱ��Ƚϳ�
     while(uart2_flag);      
     if(1 == wifi_getf){
        error = 1;
        if(-1 != str_include(wifi_data,name))error = 0;   //�޸�ip�ɹ�
     }
     wifi_getf = 0;
     
#ifdef wifi_DEBUG
        SCI_send(wifi_data);
#endif
    return error;
}


/******************************************************************
    ��������wifi_TCPtest()
    ���ܣ��鿴����״̬
    ��������
    ���أ���������  0  TCPδ����
                    1  TCP������
                    2  û�з���
******************************************************************/
char wifi_TCPtest(void){
  char error;
  wifi_start();                   //��������ģʽ
  error = wifi_command("SOCKLKA" ,"DISCONNECTED",0); //ֻ�ܼ���޷����ӵ����
                                  //DISCONNECTED ���� CONNECTED 
  wifi_end(0);                    //����͸��ģʽ
  return error;                   //0 δ���ӣ�1 ������
  
}



//-------------------�жϽ���-------------------------//
//---------------����SIM���ص�����--------------------//
#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void)
{
  switch(__even_in_range(UCA1IV,4))
  {
  case 0:break                                                     ; // Vector 0 - no interrupt
  case 2:                                                            // Vector 2 - RXIFG
        if(0 == wifi_getf)uart2_num = 0;
        wifi_data[uart2_num++] = UCA1RXBUF;
        wifi_data[uart2_num] = '\0';
        wifi_getf = 1;
        if(uart2_num < REMAX - 1){
             uart2_error = 0;
             UART2_delay(10);                                         //10ms��û���յ���������Ϊͨ�Ž���
        }else {
            uart2_num = REMAX - 2;
            uart2_error = 1;
       }
   
      break                                                        ;
  case 4:break                                                     ;  // Vector 4 - TXIFG
  default: break                                                   ;  
  }  
}