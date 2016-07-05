
#include "msp430x54x.h"
#include "main.h"
#include <string.h>

char flag = 0;


unsigned char IP[4]={211,81,253,244};
unsigned int PORT = 8000;
char data[]="hallo world\n";

char APname[20] = "DC411";
char APkey[20] = "c411123456";
unsigned char APip[4]={192,168,31,102};
unsigned int  APport = 8000;

char phone[11];

int main( void )
{  
  char  cnt = 0;
  float tem = 0;
  char SIM_ERR = 0;    //SIM卡连接标志
  char wifi_ERR = 0;   //wifi连接标志
  
  //Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;

  //---------单片机模块初始化-------------------------------------  
  Init_CLK();
  Init_Timer0_A5();
  Init_Timer1_A3();
  SCI_init();
  UART_init();
  UART2_init(); 
  mlx90614_INIT();
  _EINT(); 
  
  P4DIR = BIT7;
  P4OUT = BIT7;
 
//--------------------------------------------
  SCI_send("go");
  SCI_send("\n");
  Delay_ms(3000);  //上电等待其他模块

//--------wifi模块配置-------------------------------------   
   cnt = 0;
   while(wifi_start() && cnt++ < 5){
         Delay_ms(5);
         wifi_end(1);      //可能是处在命令模式，让wifi重启一下
         Delay_ms(5);
        
   }
   SCI_send_num(cnt);
   if(cnt == 6)wifi_ERR = 1;
   else{
      wifi_command("WMODE", "STA",1);  //配置成端口模式
      wifi_command("WANN", "DHCP",1);  //使用动态地址
      //wifi_command("E", "OFF",1);    //关闭回显
      wifi_AP(APname,APkey);           //输入wifi名和密码
      wifi_IP(APip,APport);            //AP用的是局内网
      wifi_end(1);                     //重启wifi
   }
  
  
//----------SIM卡配置-----------------------------------  
     while(SIM800_test() && cnt++ < 5){    //检测5次SIM卡模块
          Delay_ms(5);
   }
   if(cnt == 6)SIM_ERR = 1;             
   else{                     //SIM模块正常
      //SIM800_START("ATE0");              //关闭回显示
     SIM_command( "CGREG=1","\0");
     SIM_command( "CIPSTATUS","OK");
     SIM_command("CIPCLOSE=1","CLOSED"); //成功返回 CLOSED，失败返回 ERROR  先关闭连接
     SIM800_Getip(IP,PORT);
    // SIM_command( "AT+CMGF=1","OK");  //以文本形式编辑短信
    // SIM_command( "AT+CNMI=2,2","OK");  //短信来了直接读取
     cnt = 0;
     while(SIM_command( "CIPSTATUS","CONNECT") && cnt++ < 5){  //连接5次看是否能连接上
            SIM_command("CIPCLOSE=1","CLOSED");
            SIM800_Getip(IP,PORT);
            Delay_ms(10);
     }
     if(cnt == 6)SIM_ERR = 2;     //连接不上TCP标志
   }
  
//------------错误返回--------------------------------------  
       if(SIM_ERR == 2)SCI_send("\n-------SIM模块连接不上TCP------\n");
       else if(SIM_ERR == 1)SCI_send("\n-------SIM卡无信号------\n");
       else SCI_send("\n-------SIM卡连接成功------\n");
       
       if(wifi_ERR == 2)SCI_send("\n-------连接不上wifi模块------\n");
       else if(wifi_ERR == 1)SCI_send("\n-------wifi卡无信号------\n");
       else SCI_send("\n-------wifi卡连接成功------\n");
   
 //-------------------主程序---------------------------------------//
  Delay_ms(500);
  cnt = 0;
  
  while(1){
    
    {   
       P4OUT |= BIT7;        //亮灯
       tem = MLX_RT();       //温度读取，IIC未连接会卡死
       
       if(wifi_getf == 1){   //wifi模块接收到信息
        
         wifi_getf = 0;
       }
       
       /*if(SIM_getf == 1){    //SIM卡接收到信息
         if(-1 != str_include(SIM_data,"+CMT")){   //有新的信息
            getphone(SIM_data,phone,"+86");
            if(getIP(SIM_data,"AP",APip,&APport)){
                wifi_start();
                wifi_IP(APip,APport);            //AP用的是局内网
                wifi_end(1);                     //重启wifi
                note_send(phone,"APip change ok");
                Delay_ms(10);   //等待wifi重启
            }else note_send(phone,"APip change error");
         }  
          SIM_getf = 0;  
       }
       */
       
       if(SIM_command( "CIPSTATUS","CONNECT")){  //检测SIM连接状况
          SIM_command( "CGREG=1","\0"); 
          SIM_command("CIPCLOSE=1","CLOSED");
          SIM800_Getip(IP,PORT);                   //如果没连接上，写入IP地址重新连接
          Delay_ms(100);                           //等待连接
          SIM_ERR = SIM_command( "CIPSTATUS","CONNECT");  //再次查看链接情况
       }else SIM_ERR = 0;
       
      
       if( wifi_TCPtest())    //wifi连接，优先用wifi发送
       {      //wifi模块连接中
             
             wifi_send("W温度");       //发送温度和序号
             wifi_send_num(cnt++); 
             wifi_send(": ");
             wifi_send_float(tem);
             wifi_send("\n");
             
             if(SIM_ERR) wifi_send("SIM ERROR\n");  //如果SIM卡连接有问题，发送错误报告
       }
       else
       {                        //wifi未连接，改为gprs发送
             GPRS_Start();              //GPRS进入发送模式
         
             GPRS_Send("S温度");         //发送温度
             GPRS_SendNum(cnt++);
             GPRS_Send("： ");
             GPRS_Send_float(tem);
             GPRS_Send("\n");
             GPRS_Send("\n----wifi断开-----\n");   //发送错误报告
             GPRS_SendEnd();            //发送数据
         
       }
      
       
      
       P4OUT &= ~BIT7;   //关闭灯 
       __bis_SR_register(LPM4_bits);             // Enter LPM3
    
    }
   
    
  }
  
}



#pragma vector=TIMER0_A0_VECTOR                             
__interrupt void Timer0_A0 (void)
{
  
  __bic_SR_register_on_exit(LPM0_bits);   // Exit active CPU
}





