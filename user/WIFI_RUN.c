

#include"WIFI_RUN.h"

/********************************************************
    wifi模块初始化
*********************************************************/
void wifi_init(void){        //wifi初始化
    WIFI_REST_ON;            //拉低复位
    WIFI_LED_INIT;
    UART2_init();            //WIFI模块通信
    WIFI_LED_ON;
    Delay_sleepms(10);
    WIFI_REST_OFF;
    Delay_sleepms(2000);
    
   if(wifi_start() == 0){
      wifi_command("WMODE", "STA",1);    //配置成端口模式
      wifi_command("E", "ON",1);         //关闭回显
      wifi_command("SLPTYPE", "3,3",1);  //唤醒100ms
      Delay_sleepms(10);
      wifi_AP(APname,APkey);           //输入wifi名和密码
      Delay_sleepms(10);
      wifi_IP(WIFI_IP,WIFI_PORT);      //AP用的是局内网
      Delay_sleepms(10);
      wifi_command("SOCKA", "ok",0);
      Delay_sleepms(5);
      wifi_end(1);                     //重启wifi
      Delay_sleepms(20);
      WIFI_LED_OFF;
   } 
  
}

/****************************************************

******************************************************/
void wifi_sleep(int mode){  //休眠模式选择
    /*进入命令模式*/
    wifi_start();
    if(mode)wifi_command("SLPTYPE", "3,3",1);  
    else{
      wifi_command("SLPTYPE", "4,100",1);    //深度睡眠，唤醒10s  模块重启
      wifi_command("MSLP" ,"OK",0); //直接进入休眠
    }
    wifi_end(1); 
    /*退出命令模式*/
}

/****************************************************

*****************************************************/
void wifi_wake(int mode){   //唤醒
      WIFI_TX_DOWN;         //发送口拉低唤醒
      Delay_sleepms(50);
      WIFI_TX_ON;           //开启串口功能
      Delay_sleepms(50);
}
/****************************************************

****************************************************/
int wifi_send_data(int num,float* data,int n,int flag){ //发送数据包，数量，数组名，异常标志
    
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
void wifi_close(void){      //关闭wifi模块
    wifi_sleep(0);
    WIFI_UART_OFF;
    WIFI_LED_OFF;

}
/*************************************************

**************************************************/
char* wifi_SR(char*s){    //用于调试模块
     UART2_send(s);
     wifi_getf = 0;
     UART2_delay(10);  //最长等待50ms
     while(uart2_flag);      //等待数据接收完成，若无返回则50ms后退出   
     if(0 == wifi_getf)wifi_data[0] = '\0';
     wifi_getf = 0;
     return wifi_data;
}

void wifi_gets(void){
    if(1 == wifi_getf)SCI_send(wifi_data);
    wifi_getf = 0;
}