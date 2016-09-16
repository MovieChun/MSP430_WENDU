

#include"WIFI_RUN.h"

/********************************************************
    wifiģ���ʼ��
*********************************************************/
void wifi_init(void){        //wifi��ʼ��
    WIFI_REST_ON;            //���͸�λ
    WIFI_LED_INIT;
    UART2_init();            //WIFIģ��ͨ��
    WIFI_LED_ON;
    Delay_sleepms(10);
    WIFI_REST_OFF;
    Delay_sleepms(2000);
    
   if(wifi_start() == 0){
      wifi_command("WMODE", "STA",1);    //���óɶ˿�ģʽ
      wifi_command("E", "ON",1);         //�رջ���
      wifi_command("SLPTYPE", "3,3",1);  //����100ms
      Delay_sleepms(10);
      wifi_AP(APname,APkey);           //����wifi��������
      Delay_sleepms(10);
      wifi_IP(WIFI_IP,WIFI_PORT);      //AP�õ��Ǿ�����
      Delay_sleepms(10);
      wifi_command("SOCKA", "ok",0);
      Delay_sleepms(5);
      wifi_end(1);                     //����wifi
      Delay_sleepms(20);
      WIFI_LED_OFF;
   } 
  
}

/****************************************************

******************************************************/
void wifi_sleep(int mode){  //����ģʽѡ��
    /*��������ģʽ*/
    wifi_start();
    if(mode)wifi_command("SLPTYPE", "3,3",1);  
    else{
      wifi_command("SLPTYPE", "4,100",1);    //���˯�ߣ�����10s  ģ������
      wifi_command("MSLP" ,"OK",0); //ֱ�ӽ�������
    }
    wifi_end(1); 
    /*�˳�����ģʽ*/
}

/****************************************************

*****************************************************/
void wifi_wake(int mode){   //����
      WIFI_TX_DOWN;         //���Ϳ����ͻ���
      Delay_sleepms(50);
      WIFI_TX_ON;           //�������ڹ���
      Delay_sleepms(50);
}
/****************************************************

****************************************************/
int wifi_send_data(int num,float* data,int n,int flag){ //�������ݰ������������������쳣��־
    
  int ok = 0;
  
  WIFI_LED_ON;
  if(wifi_TCPtest())ok = 1;
  UART2_send("W");
  UART2_send_num(num); 
  for(int i = 0;i < n;i++){
        UART2_send_num(i); 
        UART2_send(":");
        UART2_send_float(data[i]);
        UART2_send("\n");
    }
  if(ok)WIFI_LED_OFF;
  return ok;
}
/*****************************************************

****************************************************/
void wifi_close(void){      //�ر�wifiģ��
    wifi_sleep(0);
    WIFI_UART_OFF;
    WIFI_LED_OFF;

}
/*************************************************

**************************************************/
char* wifi_SR(char*s){    //���ڵ���ģ��
     UART2_send(s);
     wifi_getf = 0;
     UART2_delay(10);  //��ȴ�50ms
     while(uart2_flag);      //�ȴ����ݽ�����ɣ����޷�����50ms���˳�   
     if(0 == wifi_getf)wifi_data[0] = '\0';
     wifi_getf = 0;
     return wifi_data;
}

void wifi_gets(void){
    if(1 == wifi_getf)SCI_send(wifi_data);
    wifi_getf = 0;
}