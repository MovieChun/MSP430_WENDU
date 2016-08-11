
#include "msp430x54x.h"
#include "main.h"
#include <string.h>

#define MIN    4 
#define SAVEF  6   //�洢��־��

void READ(void);
void SAVE(void);

char wifi_init(void);
char SIM_init(void);
char SIM_status(void);
char data_send(char mode,float tem);

int time_min = 0;    //�ж�10sһ�Σ�6��Ϊ1min
char TCP_COM = 0;    //����״̬��1 ��ʾ������ 0��ʾδ����

unsigned char SIM_IP[4]={211,81,253,244};
unsigned int SIM_PORT = 7999;
char APname[20] = "DC411";
char APkey[20] = "c411123456";
unsigned char WIFI_IP[4]={192,168,31,102};
unsigned int  WIFI_PORT = 8000;
unsigned int tem_num = 0;     //����ģ��ı��
int   cnt = 0;
char debug = 0;

int main( void )
{  
 
  float tem[6] = {0};
  char mode = 2;  //ģʽѡ�� 0��ֻ��WIFI  1:ֻ��SIM  2:ͬʱʹ�ã�SIM��Ϊ����

  unsigned char save_flag = 0;

  
  //Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;

  //---------��Ƭ��ģ���ʼ��-------------------------------------  
  Init_CLK();
  Init_Timer0_A5();
  Init_Timer1_A3();  //1ms��ȷ��ʱ
  TCP_COM = 1;
  SCI_init();        //����ͨ��
  UART_init();       //SIMģ��ͨ��
  UART2_init();      //WIFIģ��ͨ��
  mlx90614_INIT();   //����ģ���ʼ��
  _EINT();           //�����ж�
  
  P4DIR |= BIT7;    //���ݷ���ָʾ
  P4OUT |= BIT7;    //��ʼ�����쳣ָʾ
  
  P5DIR |= BIT5;
  P5OUT |= BIT5;
  
  P9DIR |= BIT3;
  P9OUT |= BIT3;

  Flash_read(SAVE_ADD, &save_flag ,1);   //��ȡ��־λ���鿴�洢���Ƿ�������
  if(SAVEF == save_flag){                //�����������
     READ();
  }else{
     SAVE();                             //���������������
  }
  
   SCI_send("����ģ������\n");
   __bis_SR_register(LPM4_bits);             // �ȴ�10s������ģ���ϵ��ʼ��
//--------wifiģ������-------------------------------------   
   if(wifi_init())SCI_send("wifi ��ʼ�����\n");
   else SCI_send("wifi ��ʼ��ʧ��\n");
//----------SIM������-----------------------------------  
   
   
   if(SIM_init())SCI_send("SIM ��ʼ���ɹ�\n");
   else SCI_send("SIM ��ʼ��ʧ��\n");
   SCI_send("��ʼ������\n");
   
  P5OUT &= ~BIT5;
  P4OUT &= ~BIT7;
  Delay_ms(20);
 //-------------------������---------------------------------------//

  
  while(1){
 //-------------------ָ���޸�------------------------------------
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
             
             wifi_TCPtest();             //����WIFI
             wifi_send("NUM:");
             wifi_send_num(tem_num);
             wifi_send(",WIFI ���ݷ���\n");
             //wifi_sleep();
             SCI_send("WIFI ����\n");
             
             GPRS_Start();                //GPRS���뷢��ģʽ
             GPRS_Send("NUM:");  
             GPRS_SendNum(tem_num);
             GPRS_Send(",SIM ���ݷ���\n");          
             GPRS_SendEnd();                 //��������
             SCI_send("SIM ����\n");
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
          else if(getUI(SCI_data,"NUM:",&tem_num)){  //�ı����ģ����
            SCI_send("NEW_NUM:");
            SCI_send_num(tem_num);
            SAVE();     
          }
          else if(-1 != str_include(SCI_data,"MODE0")){    //����SIMģ�����ģʽ����������ֱ�ӷ��͵�SIM
            mode = 0;
            SCI_send("��ʹ��WIFI\n");
            wifi_TCPtest();             //����WIFI
            wifi_send("NUM:");
            wifi_send_num(tem_num);
            wifi_send(",WIFI ���ݷ���\n");
            SCI_send("WIFI ����\n");
          }
           else if(-1 != str_include(SCI_data,"MODE1")){    //����SIMģ�����ģʽ����������ֱ�ӷ��͵�SIM
            mode = 1;
            SCI_send("��ʹ��SIM\n");
            GPRS_Start();                //GPRS���뷢��ģʽ
            GPRS_Send("NUM:");  
            GPRS_SendNum(tem_num);
            GPRS_Send(",SIM ���ݷ���\n");          
            GPRS_SendEnd();                 //��������
            SCI_send("SIM ����\n");
          }
          else if(-1 != str_include(SCI_data,"MODE2")){    //����SIMģ�����ģʽ����������ֱ�ӷ��͵�SIM
            mode = 2;
            SCI_send("ʹ��WIFI��SIM\n");
            wifi_TCPtest();             //����WIFI
            wifi_send("NUM:");
            wifi_send_num(tem_num);
            wifi_send(",WIFI ���ݷ���\n");
            SCI_send("WIFI ����\n");
            GPRS_Start();                //GPRS���뷢��ģʽ
            GPRS_Send("NUM:");  
            GPRS_SendNum(tem_num);
            GPRS_Send(",SIM ���ݷ���\n");          
            GPRS_SendEnd();                 //��������
            SCI_send("SIM ����\n");
          }
          
          else if(-1 != str_include(SCI_data,"SIM DEBUG")){    //����SIMģ�����ģʽ����������ֱ�ӷ��͵�SIM
           debug = 1;
           P5OUT |= BIT5;
           SCI_send("����sim\n");
          }else if(-1 != str_include(SCI_data,"WIFI DEBUG")){  //����WIFIģ�����ģʽ����������ֱ�ӷ��͵�WIFI
            debug = 2;
            SCI_send("����wifi\n");
            P5OUT |= BIT5;
         }
         else SCI_send("command error\n");   //����ָ�������
            SCI_getf = 0;
       }
      
      
//-------------------���ݷ���-------------------------------------------------------------    
     if(time_min == 0)                                        //����1���ӷ�������
     {    cnt++;
          P4OUT |= BIT7;
          P5OUT |= BIT5;
          /*tem[0] = MLX_RT();             //�ɼ�5���˲���ͬʱ��ʱ20ms
          Delay_ms(5);
          tem[1] = MLX_RT();
          Delay_ms(5);
          tem[2] = MLX_RT();
          Delay_ms(5);
          tem[3] = MLX_RT();        
          Delay_ms(5);
          tem[4] = MLX_RT();
          MLX_sleep();
            */
          for(int i = 0; i < 5 ;i++){    
            if(tem[0] < tem[i]){        //�ҳ����ֵ�ŵ�tem[0]
               tem[5] = tem[0];
               tem[0] = tem[i];
               tem[i] = tem[5];
            }
            if(tem[1] > tem[i]){        //�ҳ���Сֵ�ŵ�tem[1]
               tem[5] = tem[1];
               tem[1] = tem[i];
               tem[i] = tem[5];
            }
          }
          
          tem[0] = (tem[2] + tem[3] + tem[4])/3;   //ȥ�������Сֵ��ƽ��
          time_min = MIN;
          P5OUT &= ~BIT5;
                       //���ڼ�ⶪ�����
          TCP_COM = data_send(mode,tem[0]);
         if(TCP_COM == 0){                          //�������״̬
            if(mode != 1){
                if(wifi_TCPtest() == 0)wifi_init();     //��������
                TCP_COM = data_send(0,tem[0]);
             }
            if(mode != 0 && TCP_COM == 0){
              if(SIM_status() == 0)SIM_init();
                 TCP_COM = data_send(1,tem[0]);
            }
         }
        __bis_SR_register(LPM4_bits);             // Enter LPM3
    
      }
    }
//---------------------����SIMģ��-------------------------------    
    else if(debug == 1){         
       if(SCI_getf == 1){   
          while(SCI_flag);
          SCI_send(SCI_data);
          if(-1 == str_include(SCI_data,"ESC")){   //�˳�����ģʽ
             UART_send(SCI_data);
             SIM_getf = 0;
             UART1_delay(50);
             while(uart1_flag);             
          }else{
            debug = 0;
            P5OUT &= ~BIT5;
            SCI_send("Esc ok\n");
          }
           if(SIM_getf){
               SCI_send(SIM_data);
               SIM_getf = 0;
          }
       SCI_getf = 0;
       }
    }
//---------------����wifiģ��-----------------------
    else if(debug == 2){              
       if(SCI_getf == 1){   
          while(SCI_flag);
          SCI_send(SCI_data);
          if(-1 == str_include(SCI_data,"ESC")){
             UART2_send(SCI_data);
             wifi_getf = 0;
             UART2_delay(50);
             while(uart2_flag);            
          }else{
            debug = 0;
            P5OUT &= ~BIT5;
            SCI_send("Esc ok\n");
          }
          if(wifi_getf){
               SCI_send(wifi_data);
               wifi_getf = 0;
          }
          SCI_getf = 0;
       }      
     }
     else  debug = 0;
       
    }

}


int lose = 0;
#pragma vector=TIMER0_A0_VECTOR                             
__interrupt void Timer0_A0 (void)
{   
  if(time_min > 0 && debug == 0){
   // P5OUT |= BIT5;
    time_min--;
    //Delay_ms(20);
    //P5OUT &= ~BIT5;
  }
  else __bic_SR_register_on_exit(LPM0_bits);   // Exit active CPU
  if( TCP_COM == 0)lose++;
  else lose = 0;
  if(lose > 1){
     lose = 0;
     __bic_SR_register_on_exit(LPM0_bits);   // Exit active CPU
  }
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
    Flash_read(SAVE_ADD+53, (unsigned char*)data ,2);
    tem_num = (unsigned int)data[1]<<8 | data[0]; 
    
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
    data[0] = (unsigned char)(tem_num & 0x00ff);
    data[1] = (unsigned char)(tem_num>>8 & 0x00ff);
    Flash_write(SAVE_ADD+53, (unsigned char*)data ,2);
}

/********************************************************
    wifiģ���ʼ��
*********************************************************/
char wifi_init(void){
   char cnt = 0;
   char ok = 0;
   while(wifi_start() && cnt++ < 5);
   
   if(cnt == 6)ok = 0;
   else{
      wifi_command("WMODE", "STA",1);  //���óɶ˿�ģʽ
      wifi_command("WANN", "DHCP",1);  //ʹ�ö�̬��ַ
      wifi_command("E", "ON",1);       //�رջ���
      wifi_command("SOCKDISA", "OFF",1);       //�ر��Զ�������
      wifi_command("SLPTYPE", "3,3",1);       //����ʡ��ģʽ
      wifi_AP(APname,APkey);           //����wifi��������
      wifi_IP(WIFI_IP,WIFI_PORT);      //AP�õ��Ǿ�����
      Delay_ms(20);
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
  while(SIM800_test()&&cnt++ < 2){    //���3��SIM��ģ��
         Delay_ms(10);
   }
   if( SIM800_test())ok = 0;             
   else{                                 //SIMģ������
     UART_send("ATE1");                  //�رջ���ʾ
     SIM_command("CIPCLOSE","CLOSED");   //�ɹ����� CLOSED��ʧ�ܷ��� ERROR  �ȹر�����
     SIM800_Getip(SIM_IP,SIM_PORT);
     Delay_ms(50);                      //�ȴ�50ms����ʱ��
     
     cnt = 0;
     while(0 == SIM_status() && cnt++ < 3)Delay_ms(10);;  //����3�ο��Ƿ���������
     if(cnt == 4)ok = 0;     //���Ӳ���TCP��־     
   }
     return ok;
  }

/*******************************************************************************
      SIMģ��״̬���ʹ���
********************************************************************************/
char SIM_status(void){
    char ok = 0;
    
    if(SIM_command( "CIPSTATUS","CONNECTING"))Delay_ms(30);
    else if(SIM_command( "CIPSTATUS","CONNECT"))ok = 1;
    else if(-1 != str_include(SIM_data,"IP INITIAL")||  //δ����״̬
            -1 != str_include(SIM_data,"CLOSE")){
            SIM800_Getip(SIM_IP,SIM_PORT);                   //��������
            Delay_ms(500);
    }
    else if(-1 != str_include(SIM_data,"PDP DEACT")){   //������ע��
      SIM_command( "CIPSHUT","OK"); 
      Delay_ms(5);
      SIM_command( "CSTT","OK");                        //�����
      Delay_ms(500);
      SIM_command( "CIICR","OK");
      Delay_ms(5);
      SIM_command( "CIFSR","OK");
    }
    else if(-1 != str_include(SIM_data,"IP GPRSACT")){   //������ע��
      SIM_command( "CIPSHUT","OK"); 
      Delay_ms(30);
     
    }
    else  Delay_ms(500);                              //���࣬�ȴ���ʼ�����
    
    return ok;
}



char data_send(char mode,float tem){
  char  com = 0; 
  if(mode != 1)com = wifi_TCPtest();
  if(com)                              //wifi���ӣ�������wifi����
         {                                               //wifiģ��������
             
             wifi_send("W_NUM:");                          //�����¶Ⱥ����
             wifi_send_num(tem_num); 
             wifi_send(",�¶�");                          //�����¶Ⱥ����
             wifi_send_num(cnt); 
             wifi_send(": ");
             wifi_send_float(tem);
             wifi_send("\n");
             //wifi_sleep();   //�ֶ�����
             P4OUT &= ~BIT7; 
             
         }
        else
         {   //wifiδ���ӣ���Ϊgprs����
          
           if(mode != 0)com = SIM_status();
           if(com){
             GPRS_Start();                //GPRS���뷢��ģʽ
         
             GPRS_Send("S_NUM:");           //�����¶�
             GPRS_SendNum(tem_num);
             GPRS_Send(",�¶�");            //�����¶�
             GPRS_SendNum(cnt);
             GPRS_Send("�� ");
             GPRS_Send_float(tem);
             GPRS_Send("\n");
             GPRS_SendEnd();                 //��������
             P4OUT &= ~BIT7;                 //�رյ� 
            }
     
           if(mode == 2)wifi_init();
        }    
  
      return com;
}