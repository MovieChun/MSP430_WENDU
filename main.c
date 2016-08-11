
#include "msp430x54x.h"
#include "main.h"
#include <string.h>

#define MIN    4 
#define SAVEF  6   //存储标志号

void READ(void);
void SAVE(void);

char wifi_init(void);
char SIM_init(void);
char SIM_status(void);
char data_send(char mode,float tem);

int time_min = 0;    //中断10s一次，6次为1min
char TCP_COM = 0;    //连接状态，1 表示连接上 0表示未连接

unsigned char SIM_IP[4]={211,81,253,244};
unsigned int SIM_PORT = 7999;
char APname[20] = "DC411";
char APkey[20] = "c411123456";
unsigned char WIFI_IP[4]={192,168,31,102};
unsigned int  WIFI_PORT = 8000;
unsigned int tem_num = 0;     //测温模块的编号
int   cnt = 0;
char debug = 0;

int main( void )
{  
 
  float tem[6] = {0};
  char mode = 2;  //模式选择 0：只用WIFI  1:只用SIM  2:同时使用，SIM作为备份

  unsigned char save_flag = 0;

  
  //Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;

  //---------单片机模块初始化-------------------------------------  
  Init_CLK();
  Init_Timer0_A5();
  Init_Timer1_A3();  //1ms精确延时
  TCP_COM = 1;
  SCI_init();        //电脑通信
  UART_init();       //SIM模块通信
  UART2_init();      //WIFI模块通信
  mlx90614_INIT();   //测温模块初始化
  _EINT();           //开启中断
  
  P4DIR |= BIT7;    //数据发送指示
  P4OUT |= BIT7;    //初始化及异常指示
  
  P5DIR |= BIT5;
  P5OUT |= BIT5;
  
  P9DIR |= BIT3;
  P9OUT |= BIT3;

  Flash_read(SAVE_ADD, &save_flag ,1);   //读取标志位，查看存储区是否有数据
  if(SAVEF == save_flag){                //有则读出数据
     READ();
  }else{
     SAVE();                             //无数据则存入数据
  }
  
   SCI_send("测温模块启动\n");
   __bis_SR_register(LPM4_bits);             // 等待10s让其他模块上电初始化
//--------wifi模块配置-------------------------------------   
   if(wifi_init())SCI_send("wifi 初始化完成\n");
   else SCI_send("wifi 初始化失败\n");
//----------SIM卡配置-----------------------------------  
   
   
   if(SIM_init())SCI_send("SIM 初始化成功\n");
   else SCI_send("SIM 初始化失败\n");
   SCI_send("初始化结束\n");
   
  P5OUT &= ~BIT5;
  P4OUT &= ~BIT7;
  Delay_ms(20);
 //-------------------主程序---------------------------------------//

  
  while(1){
 //-------------------指令修改------------------------------------
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
             
             wifi_TCPtest();             //激活WIFI
             wifi_send("NUM:");
             wifi_send_num(tem_num);
             wifi_send(",WIFI 数据发送\n");
             //wifi_sleep();
             SCI_send("WIFI 发送\n");
             
             GPRS_Start();                //GPRS进入发送模式
             GPRS_Send("NUM:");  
             GPRS_SendNum(tem_num);
             GPRS_Send(",SIM 数据发送\n");          
             GPRS_SendEnd();                 //发送数据
             SCI_send("SIM 发送\n");
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
          else if(getUI(SCI_data,"NUM:",&tem_num)){  //改变测温模块编号
            SCI_send("NEW_NUM:");
            SCI_send_num(tem_num);
            SAVE();     
          }
          else if(-1 != str_include(SCI_data,"MODE0")){    //进入SIM模块调试模式，串口数据直接发送到SIM
            mode = 0;
            SCI_send("仅使用WIFI\n");
            wifi_TCPtest();             //激活WIFI
            wifi_send("NUM:");
            wifi_send_num(tem_num);
            wifi_send(",WIFI 数据发送\n");
            SCI_send("WIFI 发送\n");
          }
           else if(-1 != str_include(SCI_data,"MODE1")){    //进入SIM模块调试模式，串口数据直接发送到SIM
            mode = 1;
            SCI_send("仅使用SIM\n");
            GPRS_Start();                //GPRS进入发送模式
            GPRS_Send("NUM:");  
            GPRS_SendNum(tem_num);
            GPRS_Send(",SIM 数据发送\n");          
            GPRS_SendEnd();                 //发送数据
            SCI_send("SIM 发送\n");
          }
          else if(-1 != str_include(SCI_data,"MODE2")){    //进入SIM模块调试模式，串口数据直接发送到SIM
            mode = 2;
            SCI_send("使用WIFI和SIM\n");
            wifi_TCPtest();             //激活WIFI
            wifi_send("NUM:");
            wifi_send_num(tem_num);
            wifi_send(",WIFI 数据发送\n");
            SCI_send("WIFI 发送\n");
            GPRS_Start();                //GPRS进入发送模式
            GPRS_Send("NUM:");  
            GPRS_SendNum(tem_num);
            GPRS_Send(",SIM 数据发送\n");          
            GPRS_SendEnd();                 //发送数据
            SCI_send("SIM 发送\n");
          }
          
          else if(-1 != str_include(SCI_data,"SIM DEBUG")){    //进入SIM模块调试模式，串口数据直接发送到SIM
           debug = 1;
           P5OUT |= BIT5;
           SCI_send("调试sim\n");
          }else if(-1 != str_include(SCI_data,"WIFI DEBUG")){  //进入WIFI模块调试模式，串口数据直接发送到WIFI
            debug = 2;
            SCI_send("调试wifi\n");
            P5OUT |= BIT5;
         }
         else SCI_send("command error\n");   //其他指令不做处理
            SCI_getf = 0;
       }
      
      
//-------------------数据发送-------------------------------------------------------------    
     if(time_min == 0)                                        //到达1分钟发送数据
     {    cnt++;
          P4OUT |= BIT7;
          P5OUT |= BIT5;
          /*tem[0] = MLX_RT();             //采集5次滤波，同时延时20ms
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
            if(tem[0] < tem[i]){        //找出最大值放到tem[0]
               tem[5] = tem[0];
               tem[0] = tem[i];
               tem[i] = tem[5];
            }
            if(tem[1] > tem[i]){        //找出最小值放到tem[1]
               tem[5] = tem[1];
               tem[1] = tem[i];
               tem[i] = tem[5];
            }
          }
          
          tem[0] = (tem[2] + tem[3] + tem[4])/3;   //去除最大最小值求平均
          time_min = MIN;
          P5OUT &= ~BIT5;
                       //用于检测丢包情况
          TCP_COM = data_send(mode,tem[0]);
         if(TCP_COM == 0){                          //检测连接状态
            if(mode != 1){
                if(wifi_TCPtest() == 0)wifi_init();     //丢包补发
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
//---------------------调节SIM模块-------------------------------    
    else if(debug == 1){         
       if(SCI_getf == 1){   
          while(SCI_flag);
          SCI_send(SCI_data);
          if(-1 == str_include(SCI_data,"ESC")){   //退出调试模式
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
//---------------调节wifi模块-----------------------
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

/********************************************************
    wifi模块初始化
*********************************************************/
char wifi_init(void){
   char cnt = 0;
   char ok = 0;
   while(wifi_start() && cnt++ < 5);
   
   if(cnt == 6)ok = 0;
   else{
      wifi_command("WMODE", "STA",1);  //配置成端口模式
      wifi_command("WANN", "DHCP",1);  //使用动态地址
      wifi_command("E", "ON",1);       //关闭回显
      wifi_command("SOCKDISA", "OFF",1);       //关闭自动重连接
      wifi_command("SLPTYPE", "3,3",1);       //设置省电模式
      wifi_AP(APname,APkey);           //输入wifi名和密码
      wifi_IP(WIFI_IP,WIFI_PORT);      //AP用的是局内网
      Delay_ms(20);
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
  while(SIM800_test()&&cnt++ < 2){    //检测3次SIM卡模块
         Delay_ms(10);
   }
   if( SIM800_test())ok = 0;             
   else{                                 //SIM模块正常
     UART_send("ATE1");                  //关闭回显示
     SIM_command("CIPCLOSE","CLOSED");   //成功返回 CLOSED，失败返回 ERROR  先关闭连接
     SIM800_Getip(SIM_IP,SIM_PORT);
     Delay_ms(50);                      //等待50ms连接时间
     
     cnt = 0;
     while(0 == SIM_status() && cnt++ < 3)Delay_ms(10);;  //连接3次看是否能连接上
     if(cnt == 4)ok = 0;     //连接不上TCP标志     
   }
     return ok;
  }

/*******************************************************************************
      SIM模块状态检测和处理
********************************************************************************/
char SIM_status(void){
    char ok = 0;
    
    if(SIM_command( "CIPSTATUS","CONNECTING"))Delay_ms(30);
    else if(SIM_command( "CIPSTATUS","CONNECT"))ok = 1;
    else if(-1 != str_include(SIM_data,"IP INITIAL")||  //未连接状态
            -1 != str_include(SIM_data,"CLOSE")){
            SIM800_Getip(SIM_IP,SIM_PORT);                   //重新连接
            Delay_ms(500);
    }
    else if(-1 != str_include(SIM_data,"PDP DEACT")){   //场景被注销
      SIM_command( "CIPSHUT","OK"); 
      Delay_ms(5);
      SIM_command( "CSTT","OK");                        //激活场景
      Delay_ms(500);
      SIM_command( "CIICR","OK");
      Delay_ms(5);
      SIM_command( "CIFSR","OK");
    }
    else if(-1 != str_include(SIM_data,"IP GPRSACT")){   //场景被注销
      SIM_command( "CIPSHUT","OK"); 
      Delay_ms(30);
     
    }
    else  Delay_ms(500);                              //其余，等待初始化完成
    
    return ok;
}



char data_send(char mode,float tem){
  char  com = 0; 
  if(mode != 1)com = wifi_TCPtest();
  if(com)                              //wifi连接，优先用wifi发送
         {                                               //wifi模块连接中
             
             wifi_send("W_NUM:");                          //发送温度和序号
             wifi_send_num(tem_num); 
             wifi_send(",温度");                          //发送温度和序号
             wifi_send_num(cnt); 
             wifi_send(": ");
             wifi_send_float(tem);
             wifi_send("\n");
             //wifi_sleep();   //手动休眠
             P4OUT &= ~BIT7; 
             
         }
        else
         {   //wifi未连接，改为gprs发送
          
           if(mode != 0)com = SIM_status();
           if(com){
             GPRS_Start();                //GPRS进入发送模式
         
             GPRS_Send("S_NUM:");           //发送温度
             GPRS_SendNum(tem_num);
             GPRS_Send(",温度");            //发送温度
             GPRS_SendNum(cnt);
             GPRS_Send("： ");
             GPRS_Send_float(tem);
             GPRS_Send("\n");
             GPRS_SendEnd();                 //发送数据
             P4OUT &= ~BIT7;                 //关闭灯 
            }
     
           if(mode == 2)wifi_init();
        }    
  
      return com;
}