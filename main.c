
#include "msp430x54x.h"
#include "main.h"
#include <string.h>

/********配置参数*****************/
unsigned char SIM_IP[4]={211,81,253,244};
unsigned int SIM_PORT = 7999;
char APname[20] = "DC411";
char APkey[20] = "c411123456";
unsigned char WIFI_IP[4]={192,168,31,102};
unsigned int  WIFI_PORT = 8000;
unsigned int tem_num = 0;     //测温模块的编号
unsigned char  send_mode = 0;

float tem_max = 25.1;
/***********************************/
/**********标志参数*****************/
int time_min = 0;    //中断10s一次，6次为1min
int time_10s = 0;    //十秒计时
#define SAVEF  0     //存储标志号
unsigned char debug = 0;       //0 运行模式 ，1调节WIFI ,2调节 GU900


/***********************************/


int main( void )
{  
/*********临时变量区******************/  
  unsigned char save_flag = SAVEF;
  float tem[60];                   //温度数据包
  int tem_cnt = 0;                 //数据包数量计数
  unsigned char AR_flag = 1;          //异常标志位
/**************************************/  
  //Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  
//---------单片机模块初始化-------------------------------------  
  Init_CLK();
  Init_Timer0_A5();
  Init_Timer1_A3();  //1ms延时
  SCI_init();        //电脑通信
 
  //mlx90614_INIT();   //测温模块初始化
  _EINT();           //开启中断
  
  WIFI_LED_INIT;
  WIFI_LED_ON;
  //Flash_read(SAVE_ADD, &save_flag ,1);   //读取标志位，查看存储区是否有数据
  //if(SAVEF == save_flag){                //有则读出数据
  //   READ();
  //}else{
  //   SAVE();                             //无数据则存入数据
  //}
  
  while(1){
   SCI_send("A\n");
  }
   __bis_SR_register(LPM4_bits);             // 等待10s让其他模块上电初始化
   time_10s = 2;
   if(send_mode == 0)wifi_init();
   
   while(1){
     
//-----------10s测温处理-----------
     if(time_10s == 0){
        time_10s = 1;   //6
        time_min++;
       //测量温度
        //tem[tem_cnt] = TEM_AVE();
       //检测是否异常
        if(tem[tem_cnt] > tem_max)AR_flag = 1;
        else AR_flag = 0;
       //发送数据    要先检测模块的状态
        tem_cnt++;
        if(send_mode == 0){      //wifi模式
           if(AR_flag == 1 || tem_cnt >= 10){   //60
               wifi_send_data(tem_num,tem,tem_cnt,AR_flag);
               tem_cnt = 0;
           } 
        }        
        
     }
//-----------10s测温处理结束------------------------    
//-----------串口指令处理--------------------------
     PC_control();
     
     
//-----------串口指令处理结束----------------------
    __bis_SR_register(LPM4_bits);             // Enter LPM4 单片机休眠
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
    读取配置参数
************************************************************/
void READ(void){           //读取配置参数
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
    存储配置参数
***********************************************************/
void SAVE(void){              //存储配置参数
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


