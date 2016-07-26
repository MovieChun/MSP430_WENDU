
#include "msp430x54x.h"
#include "main.h"
#include <string.h>

#define SAVEF  5   //存储标志号
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
  char mode = 2;  //模式选择 0：只用WIFI  1:只用SIM  2:同时使用，SIM作为备份
  
  //Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;

  //---------单片机模块初始化-------------------------------------  
  Init_CLK();
  Init_Timer0_A5();
  Init_Timer1_A3();  //1ms精确延时
  SCI_init();        //电脑通信
  UART_init();       //SIM模块通信
  UART2_init();      //WIFI模块通信
  mlx90614_INIT();   //测温模块初始化
  _EINT();           //开启中断
  
  P4DIR |= BIT7;
  P4OUT |= BIT7;
 
  Flash_read(SAVE_ADD, &save_flag ,1);   //读取标志位，查看存储区是否有数据
  if(SAVEF == save_flag){                //有则读出数据
     READ();
    // SCI_send("\nREAD\n");
  }else{
     SAVE();                             //无数据则存入数据
    // READ();                           //再次读出查看数据的正确性
    // SCI_send("\nSAVE\n");
  }
  
   SCI_send("测温模块启动\n");
   Delay_ms(3000);
//--------wifi模块配置-------------------------------------   
   if(wifi_init())SCI_send("wifi 初始化完成\n");
   else SCI_send("wifi 初始化失败\n");
//----------SIM卡配置-----------------------------------  
   if(SIM_init())SCI_send("SIM 初始化成功\n");
   else SCI_send("SIM 初始化失败\n");
  
   SCI_send("初始化结束\n");
   
 //-------------------主程序---------------------------------------//

  
  while(1){
    //P4OUT |= BIT7; 
    if(debug == 0){
      if(SCI_getf == 1){         //串口接收到信号
          while(SCI_flag);
          SCI_send(SCI_data);
          SCI_send("\n");
          
          if(-1 != str_include(SCI_data,"CHACK")){  //CHACK 查看连接参数
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
          else if(getAP(SCI_data, "AP:",APname, APkey)){  //AP: 更改路由器的名字和密码
            SCI_send("AP name:");
            SCI_send(APname);
            SCI_send("\nAP key:");
            SCI_send(APkey);
            SCI_send("\n");
            SAVE();
            wifi_init();     //重新初始化wifi模块
          }
          else if(getIP(SCI_data,"SIM IP:",SIM_IP, &SIM_PORT)){ //SIM IP: 更改SIM模块的ip地址
            SCI_send("SIM_NEW_IP:");
            SCI_send_IP(SIM_IP,SIM_PORT);
            SAVE();  
            SIM_command("CIPCLOSE","CLOSED");   //成功返回 CLOSED，失败返回 ERROR  先关闭连接
            SIM800_Getip(SIM_IP,SIM_PORT);
          }
          else if(getIP(SCI_data,"WIFI IP:",WIFI_IP, &WIFI_PORT)){  //WIFI IP: 更改WIFI模块的ip地址
            SCI_send("WIFI_NEW_IP:");
            SCI_send_IP(WIFI_IP,WIFI_PORT);
            SAVE();
            wifi_init();     //重新初始化wifi模块
          }
          else if(-1 != str_include(SCI_data,"SIM DEBUG")){    //进入SIM模块调试模式，串口数据直接发送到SIM
           debug = 1;
           SCI_send("调试sim\n");
          }else if(-1 != str_include(SCI_data,"WIFI DEBUG")){  //进入WIFI模块调试模式，串口数据直接发送到WIFI
            debug = 2;
            SCI_send("调试wifi\n");
         }
         else SCI_send("command error\n");   //其他指令不做处理
            SCI_getf = 0;
       }
      
     
     if(time == 1)                                        //到达1分钟发送数据
     {    time = 0;
          P4OUT |= BIT7; 
      
       
       if( wifi_TCPtest())                              //wifi连接，优先用wifi发送
          {                                               //wifi模块连接中
             
             wifi_send("W温度");                          //发送温度和序号
             wifi_send_num(cnt++); 
             wifi_send(": ");
             wifi_send_float(tem);
             wifi_send("\n");
             
         }
        // else
         {   //wifi未连接，改为gprs发送
           if(SIM_status()){
             GPRS_Start();                //GPRS进入发送模式
         
             GPRS_Send("S温度");           //发送温度
             GPRS_SendNum(cnt++);
             GPRS_Send("： ");
             GPRS_Send_float(tem);
             GPRS_Send("\n");
             //GPRS_Send("\n----wifi断开-----\n");   //发送错误报告
             GPRS_SendEnd();                 //发送数据
            // wifi_init();     //重新初始化wifi模块
           }
        }   
        P4OUT &= ~BIT7;   //关闭灯 
      }
    
    }else if(debug == 1){         //调节SIM模块
       if(SCI_getf == 1){   
          while(SCI_flag);
          SCI_send(SCI_data);
          if(-1 == str_include(SCI_data,"ESC")){   //退出调试模式
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
    }else if(debug == 2){    //调节wifi模块          
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
    读取配置参数
************************************************************/
void READ(void){           //读取配置参数
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
}

/********************************************************
    wifi模块初始化
*********************************************************/
char wifi_init(void){
   char cnt = 0;
   char ok = 0;
   while(wifi_start() && cnt++ < 5){
         UART2_send("\n");   //清空上一个命令
         Delay_ms(5);
         wifi_end(1);        //可能是处在命令模式，让wifi重启一下
         Delay_ms(5);       
   }
   if(cnt == 6)ok = 0;
   else{
      wifi_command("WMODE", "STA",1);  //配置成端口模式
      wifi_command("WANN", "DHCP",1);  //使用动态地址
      wifi_command("E", "ON",1);       //关闭回显
      wifi_AP(APname,APkey);           //输入wifi名和密码
      wifi_IP(WIFI_IP,WIFI_PORT);      //AP用的是局内网
      wifi_end(1);                     //重启wifi
      ok = 1;
   } 
   return ok;
}

/*********************************************************
    SIM模块初始化
*********************************************************/
char SIM_init(void){
  char cnt = 0;
  char ok = 1;
  while(SIM800_test()&&cnt++ < 5){    //检测5次SIM卡模块
         Delay_ms(100);
   }
   if( SIM800_test())ok = 0;             
   else{                                 //SIM模块正常
     //UART_send("ATE0");               //关闭回显示
     SIM_command("CIPCLOSE","CLOSED");   //成功返回 CLOSED，失败返回 ERROR  先关闭连接
     SIM800_Getip(SIM_IP,SIM_PORT);
     Delay_ms(500);                      //等待500ms连接时间
     
     cnt = 0;
     while(0 == SIM_status() && cnt++ < 5);  //连接5次看是否能连接上
     if(cnt == 6)ok = 0;     //连接不上TCP标志     
   }
     return ok;
  }

/*******************************************************************************
      SIM模块状态检测和处理
********************************************************************************/
char SIM_status(void){
    char ok = 0;
    if(SIM_command( "CIPSTATUS","CONNECT"))ok = 1;
    else if(-1 != str_include(SIM_data,"IP INITIAL")||  //未连接状态
            -1 != str_include(SIM_data,"CLOSE")){
            SIM800_Getip(SIM_IP,SIM_PORT);                   //重新连接
            Delay_ms(1000);
    }
    else if(-1 != str_include(SIM_data,"PDP DEACT")){   //场景被注销
      SIM_command( "CIPSHUT","OK"); 
      Delay_ms(30);
      SIM_command( "CSTT","OK");                        //激活场景
      Delay_ms(1000);
      SIM_command( "CIICR","OK");
      Delay_ms(30);
      SIM_command( "CIFSR","OK");
    }
    else if(-1 != str_include(SIM_data,"IP GPRSACT")){   //场景被注销
      SIM_command( "CIPSHUT","OK"); 
      Delay_ms(30);
     
    }
    else  Delay_ms(3000);                              //其余，等待初始化完成
    
    return ok;
}



