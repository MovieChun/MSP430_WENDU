
#include "msp430x54x.h"
#include "main.h"
#include <string.h>

#define SAVEF  5   //�洢��־��
void READ(void);
void SAVE(void);

char wifi_init(void);
char SIM_init(void);
char SIM_status(void);

char time = 0;

unsigned char SIM_IP[4]={211,81,253,244};
unsigned int SIM_PORT = 8000;
char APname[20] = "DC411";
char APkey[20] = "c411123456";
unsigned char WIFI_IP[4]={192,168,31,102};
unsigned int  WIFI_PORT = 8000;


int main( void )
{  
  int   cnt = 0;
  float tem = 0;
  char debug = 0;
  unsigned char save_flag = 0;
  char mode = 2;  //ģʽѡ�� 0��ֻ��WIFI  1:ֻ��SIM  2:ͬʱʹ�ã�SIM��Ϊ����
  
  //Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;

  //---------��Ƭ��ģ���ʼ��-------------------------------------  
  Init_CLK();
  Init_Timer0_A5();
  Init_Timer1_A3();  //1ms��ȷ��ʱ
  SCI_init();        //����ͨ��
  UART_init();       //SIMģ��ͨ��
  UART2_init();      //WIFIģ��ͨ��
  mlx90614_INIT();   //����ģ���ʼ��
  _EINT();           //�����ж�
  
  P4DIR |= BIT7;
  P4OUT |= BIT7;
 
  Flash_read(SAVE_ADD, &save_flag ,1);   //��ȡ��־λ���鿴�洢���Ƿ�������
  if(SAVEF == save_flag){                //�����������
     READ();
    // SCI_send("\nREAD\n");
  }else{
     SAVE();                             //���������������
    // READ();                           //�ٴζ����鿴���ݵ���ȷ��
    // SCI_send("\nSAVE\n");
  }
  
   SCI_send("����ģ������\n");
   Delay_ms(3000);
//--------wifiģ������-------------------------------------   
   if(wifi_init())SCI_send("wifi ��ʼ�����\n");
   else SCI_send("wifi ��ʼ��ʧ��\n");
//----------SIM������-----------------------------------  
   if(SIM_init())SCI_send("SIM ��ʼ���ɹ�\n");
   else SCI_send("SIM ��ʼ��ʧ��\n");
  
   SCI_send("��ʼ������\n");
   
 //-------------------������---------------------------------------//

  
  while(1){
    //P4OUT |= BIT7; 
    if(debug == 0){
      if(SCI_getf == 1){         //���ڽ��յ��ź�
          while(SCI_flag);
          SCI_send(SCI_data);
          SCI_send("\n");
          
          if(-1 != str_include(SCI_data,"CHACK")){  //CHACK �鿴���Ӳ���
              SCI_send("SIM_IP:");
              SCI_send_IP(SIM_IP,SIM_PORT);  
              SCI_send("WIFI_IP:");
              SCI_send_IP(WIFI_IP,WIFI_PORT); 
              SCI_send("AP name:");
              SCI_send(APname);
              SCI_send("\nAP key:");
              SCI_send(APkey);
              SCI_send("\n");
          }
          else if(getAP(SCI_data, "AP:",APname, APkey)){  //AP: ����·���������ֺ�����
            SCI_send("AP name:");
            SCI_send(APname);
            SCI_send("\nAP key:");
            SCI_send(APkey);
            SCI_send("\n");
            SAVE();
            wifi_init();     //���³�ʼ��wifiģ��
          }
          else if(getIP(SCI_data,"SIM IP:",SIM_IP, &SIM_PORT)){ //SIM IP: ����SIMģ���ip��ַ
            SCI_send("SIM_NEW_IP:");
            SCI_send_IP(SIM_IP,SIM_PORT);
            SAVE();  
            SIM_command("CIPCLOSE","CLOSED");   //�ɹ����� CLOSED��ʧ�ܷ��� ERROR  �ȹر�����
            SIM800_Getip(SIM_IP,SIM_PORT);
          }
          else if(getIP(SCI_data,"WIFI IP:",WIFI_IP, &WIFI_PORT)){  //WIFI IP: ����WIFIģ���ip��ַ
            SCI_send("WIFI_NEW_IP:");
            SCI_send_IP(WIFI_IP,WIFI_PORT);
            SAVE();
            wifi_init();     //���³�ʼ��wifiģ��
          }
          else if(-1 != str_include(SCI_data,"SIM DEBUG")){    //����SIMģ�����ģʽ����������ֱ�ӷ��͵�SIM
           debug = 1;
           SCI_send("����sim\n");
          }else if(-1 != str_include(SCI_data,"WIFI DEBUG")){  //����WIFIģ�����ģʽ����������ֱ�ӷ��͵�WIFI
            debug = 2;
            SCI_send("����wifi\n");
         }
         else SCI_send("command error\n");   //����ָ�������
            SCI_getf = 0;
       }
      
     
     if(time == 1)                                        //����1���ӷ�������
     {    time = 0;
          P4OUT |= BIT7; 
      
       
       if( wifi_TCPtest())                              //wifi���ӣ�������wifi����
          {                                               //wifiģ��������
             
             wifi_send("W�¶�");                          //�����¶Ⱥ����
             wifi_send_num(cnt++); 
             wifi_send(": ");
             wifi_send_float(tem);
             wifi_send("\n");
             
         }
        // else
         {   //wifiδ���ӣ���Ϊgprs����
           if(SIM_status()){
             GPRS_Start();                //GPRS���뷢��ģʽ
         
             GPRS_Send("S�¶�");           //�����¶�
             GPRS_SendNum(cnt++);
             GPRS_Send("�� ");
             GPRS_Send_float(tem);
             GPRS_Send("\n");
             //GPRS_Send("\n----wifi�Ͽ�-----\n");   //���ʹ��󱨸�
             GPRS_SendEnd();                 //��������
            // wifi_init();     //���³�ʼ��wifiģ��
           }
        }   
        P4OUT &= ~BIT7;   //�رյ� 
      }
    
    }else if(debug == 1){         //����SIMģ��
       if(SCI_getf == 1){   
          while(SCI_flag);
          SCI_send(SCI_data);
          if(-1 == str_include(SCI_data,"ESC")){   //�˳�����ģʽ
             UART_send(SCI_data);
             SIM_getf = 0;
             UART1_delay(50);
             while(uart1_flag);
             SCI_send(SIM_data);
             
          }else{
            debug = 0;
            SCI_send("Esc ok\n");
          }
       SCI_getf = 0;
       }
    }else if(debug == 2){    //����wifiģ��          
       if(SCI_getf == 1){   
          while(SCI_flag);
          SCI_send(SCI_data);
          if(-1 == str_include(SCI_data,"ESC")){
             UART2_send(SCI_data);
             wifi_getf = 0;
             UART2_delay(50);
             while(uart2_flag);
             if(wifi_getf)
               SCI_send(wifi_data);
             
          }else{
            debug = 0;
            SCI_send("Esc ok\n");
          }
          SCI_getf = 0;
       }      
   }
   else  debug = 0;
       

    //__bis_SR_register(LPM4_bits);             // Enter LPM3
  }
  
}



#pragma vector=TIMER0_A0_VECTOR                             
__interrupt void Timer0_A0 (void)
{
  time = 1;
  __bic_SR_register_on_exit(LPM0_bits);   // Exit active CPU
}

/*********************************************************
    ��ȡ���ò���
************************************************************/
void READ(void){           //��ȡ���ò���
    unsigned char data[2];
    Flash_read(SAVE_ADD+1, (unsigned char*)APname ,20);
    Flash_read(SAVE_ADD+21, (unsigned char*)APkey ,20);
    Flash_read(SAVE_ADD+41, (unsigned char*)SIM_IP ,4);
    Flash_read(SAVE_ADD+45, (unsigned char*)WIFI_IP ,4);
    Flash_read(SAVE_ADD+49, (unsigned char*)data ,2);
    SIM_PORT = (unsigned int)data[1]<<8 | data[0];
    Flash_read(SAVE_ADD+51, (unsigned char*)data ,2);
    WIFI_PORT = (unsigned int)data[1]<<8 | data[0];   
}
/**********************************************************
    �洢���ò���
***********************************************************/
void SAVE(void){              //�洢���ò���
    unsigned char data[2];
    data[0] = SAVEF;
    FERASE(SAVE_ADD);
    Flash_write(SAVE_ADD,data,1);
    Flash_write(SAVE_ADD+1,  (unsigned char*)APname ,20);
    Flash_write(SAVE_ADD+21, (unsigned char*)APkey ,20);
    Flash_write(SAVE_ADD+41, (unsigned char*)SIM_IP ,4);
    Flash_write(SAVE_ADD+45, (unsigned char*)WIFI_IP ,4);
    data[0] = (unsigned char)(SIM_PORT & 0x00ff);
    data[1] = (unsigned char)(SIM_PORT>>8 & 0x00ff);
    Flash_write(SAVE_ADD+49, (unsigned char*)data ,2);
    data[0] = (unsigned char)(WIFI_PORT & 0x00ff);
    data[1] = (unsigned char)(WIFI_PORT>>8 & 0x00ff);
    Flash_write(SAVE_ADD+51, (unsigned char*)data ,2);
}

/********************************************************
    wifiģ���ʼ��
*********************************************************/
char wifi_init(void){
   char cnt = 0;
   char ok = 0;
   while(wifi_start() && cnt++ < 5){
         UART2_send("\n");   //�����һ������
         Delay_ms(5);
         wifi_end(1);        //�����Ǵ�������ģʽ����wifi����һ��
         Delay_ms(5);       
   }
   if(cnt == 6)ok = 0;
   else{
      wifi_command("WMODE", "STA",1);  //���óɶ˿�ģʽ
      wifi_command("WANN", "DHCP",1);  //ʹ�ö�̬��ַ
      wifi_command("E", "ON",1);       //�رջ���
      wifi_AP(APname,APkey);           //����wifi��������
      wifi_IP(WIFI_IP,WIFI_PORT);      //AP�õ��Ǿ�����
      wifi_end(1);                     //����wifi
      ok = 1;
   } 
   return ok;
}

/*********************************************************
    SIMģ���ʼ��
*********************************************************/
char SIM_init(void){
  char cnt = 0;
  char ok = 1;
  while(SIM800_test()&&cnt++ < 5){    //���5��SIM��ģ��
         Delay_ms(100);
   }
   if( SIM800_test())ok = 0;             
   else{                                 //SIMģ������
     //UART_send("ATE0");               //�رջ���ʾ
     SIM_command("CIPCLOSE","CLOSED");   //�ɹ����� CLOSED��ʧ�ܷ��� ERROR  �ȹر�����
     SIM800_Getip(SIM_IP,SIM_PORT);
     Delay_ms(500);                      //�ȴ�500ms����ʱ��
     
     cnt = 0;
     while(0 == SIM_status() && cnt++ < 5);  //����5�ο��Ƿ���������
     if(cnt == 6)ok = 0;     //���Ӳ���TCP��־     
   }
     return ok;
  }

/*******************************************************************************
      SIMģ��״̬���ʹ���
********************************************************************************/
char SIM_status(void){
    char ok = 0;
    if(SIM_command( "CIPSTATUS","CONNECT"))ok = 1;
    else if(-1 != str_include(SIM_data,"IP INITIAL")||  //δ����״̬
            -1 != str_include(SIM_data,"CLOSE")){
            SIM800_Getip(SIM_IP,SIM_PORT);                   //��������
            Delay_ms(1000);
    }
    else if(-1 != str_include(SIM_data,"PDP DEACT")){   //������ע��
      SIM_command( "CIPSHUT","OK"); 
      Delay_ms(30);
      SIM_command( "CSTT","OK");                        //�����
      Delay_ms(1000);
      SIM_command( "CIICR","OK");
      Delay_ms(30);
      SIM_command( "CIFSR","OK");
    }
    else if(-1 != str_include(SIM_data,"IP GPRSACT")){   //������ע��
      SIM_command( "CIPSHUT","OK"); 
      Delay_ms(30);
     
    }
    else  Delay_ms(3000);                              //���࣬�ȴ���ʼ�����
    
    return ok;
}



