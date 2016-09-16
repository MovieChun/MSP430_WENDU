
#include "msp430x54x.h"
#include "main.h"
#include <string.h>

/********���ò���*****************/
unsigned char SIM_IP[4]={211,81,253,244};
unsigned int SIM_PORT = 7999;
char APname[20] = "DC411";
char APkey[20] = "c411123456";
unsigned char WIFI_IP[4]={192,168,31,102};
unsigned int  WIFI_PORT = 8000;
unsigned int tem_num = 0;     //����ģ��ı��
unsigned char  send_mode = 0;

float tem_max = 25.1;
/***********************************/
/**********��־����*****************/
int time_min = 0;    //�ж�10sһ�Σ�6��Ϊ1min
int time_10s = 0;    //ʮ���ʱ
#define SAVEF  0     //�洢��־��
unsigned char debug = 0;       //0 ����ģʽ ��1����WIFI ,2���� GU900


/***********************************/


int main( void )
{  
/*********��ʱ������******************/  
  unsigned char save_flag = SAVEF;
  float tem[60];                   //�¶����ݰ�
  int tem_cnt = 0;                 //���ݰ���������
  unsigned char AR_flag = 1;          //�쳣��־λ
/**************************************/  
  //Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  
//---------��Ƭ��ģ���ʼ��-------------------------------------  
  Init_CLK();
  Init_Timer0_A5();
  Init_Timer1_A3();  //1ms��ʱ
  SCI_init();        //����ͨ��
 
  //mlx90614_INIT();   //����ģ���ʼ��
  _EINT();           //�����ж�
  
  WIFI_LED_INIT;
  WIFI_LED_ON;
  //Flash_read(SAVE_ADD, &save_flag ,1);   //��ȡ��־λ���鿴�洢���Ƿ�������
  //if(SAVEF == save_flag){                //�����������
  //   READ();
  //}else{
  //   SAVE();                             //���������������
  //}
  
  while(1){
   SCI_send("A\n");
  }
   __bis_SR_register(LPM4_bits);             // �ȴ�10s������ģ���ϵ��ʼ��
   time_10s = 2;
   if(send_mode == 0)wifi_init();
   
   while(1){
     
//-----------10s���´���-----------
     if(time_10s == 0){
        time_10s = 1;   //6
        time_min++;
       //�����¶�
        //tem[tem_cnt] = TEM_AVE();
       //����Ƿ��쳣
        if(tem[tem_cnt] > tem_max)AR_flag = 1;
        else AR_flag = 0;
       //��������    Ҫ�ȼ��ģ���״̬
        tem_cnt++;
        if(send_mode == 0){      //wifiģʽ
           if(AR_flag == 1 || tem_cnt >= 10){   //60
               wifi_send_data(tem_num,tem,tem_cnt,AR_flag);
               tem_cnt = 0;
           } 
        }        
        
     }
//-----------10s���´������------------------------    
//-----------����ָ���--------------------------
     PC_control();
     
     
//-----------����ָ������----------------------
    __bis_SR_register(LPM4_bits);             // Enter LPM4 ��Ƭ������
   }
}


#pragma vector=TIMER0_A0_VECTOR                             
__interrupt void Timer0_A0 (void)
{   
  if(time_10s > 0 && debug == 0){
   time_10s--;
  }
  if(time_10s == 0) __bic_SR_register_on_exit(LPM0_bits);   // Exit active CPU
}

/*********************************************************
    ��ȡ���ò���
************************************************************/
void READ(void){           //��ȡ���ò���
    unsigned char data[2];
    Flash_read(SAVE_ADD+1,  (unsigned char*)APname ,20);
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


