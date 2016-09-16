

#include "WIFI_INIT.h"


#define wifi_send        UART2_send        //发送字符串
#define wifi_send_num    UART2_send_num    //发送无符号整型数
#define wifi_send_float  UART2_send_float  //发送精确到小数点后两位的浮点数

int  uart2_num = 0;      //串口接收数量
char uart2_error = 0;    //数据溢出最大错误

char wifi_getf = 0;      //SIM模块接收到数据标志
char wifi_data[REMAX];   //SIM模块数据接收数组

/*==============================================================
wifi模块备忘录：
    连续发送"+++",返回"a"后3s内发送a,即可进入命令模式
    wifi模块使用的地址和端口号是局域网的ip地址和端口号
    连接路由器：at+wsta=DC411,c411123456   //添加AP名和密码，AP名只能是字母和数字
    设置IP地址：at+socka=TCPC,192.168.31.102,8000  //TCP主机
    退出命令模式：at+entm
    重新启动： at+z  //改变IP地址等使用
    关闭回显： at+e=off
    搜索附近AP: at+wscan
    恢复出厂设置：at+reld
    立即休眠：at+slp
    设置工作模式：at+wmode=sta
    使用动态IP:at+wann=dhcp
================================================================*/

/******************************************************************
    函数名：wifi_start
    功能：进入命令模式,同时也是测试功能
    参数：无
    返回：错误类型  0  没有错误
                    1  返回不符
                    2  没有返回

这个初始化有问题
******************************************************************/
char  wifi_start(void){
      char error = 2;
      int length = 0;
      wifi_getf = 0;
      
      //Delay_sleepms(5); 
      //UART2_send_char('\r');    //唤醒模块，但又不发送数据
      WIFI_TX_DOWN;
      Delay_sleepms(50);
      WIFI_TX_ON;
      Delay_sleepms(100);          //等待唤醒
      
      UART2_send("+++");     //发送"+++"和"a"进入命令模式 
      UART2_delay(50);
      while(uart2_flag);
      
      if(1 == wifi_getf){
           length = str_include(wifi_data,"a");   //有返回但不是a说明已经进入命令模式
           if( -1 != length){                 
               UART2_send("a"); 
            }
           else  UART2_send_char('\n');     //清空这个命令
         
      } else {
              UART2_send_char('\n');           //清空这个命令
            }     
      
           
      UART2_delay(50);
      while(uart2_flag);
      
      if(1 == wifi_getf){        //有回应说明就是命令模式
            error = 0;          
      }
      else error = 1;       
      wifi_getf = 0;      


#ifdef wifi_DEBUG
        SCI_send(wifi_data);
#endif
      
      return error;
}

/******************************************************************
    函数名：wifi_end
    功能：退出命令模式
    参数：无
    返回：错误类型  0  没有错误
                    1  返回不符
                    2  没有返回
******************************************************************/
char  wifi_end(char mode){
      char error = 2;
      int length = 0;
      
      UART2_delay(1);
      if(mode == 0)UART2_send("AT+ENTM\n");    //退出命令模式
      else UART2_send("AT+Z\n");    //退出命令模式
      wifi_getf = 0;
      UART2_delay(50);
      while(uart2_flag);
      
      if(1 == wifi_getf){
            error = 1;           //收到信号但不是OK 说明命令指令错误            
            length = str_include(wifi_data,"OK");
            if( -1 != length){
                 error = 0;      //能收到OK 说明状态返回不正确
            }
      }      
      wifi_getf = 0;      

#ifdef wifi_DEBUG
        if(mode == 0)SCI_send("AT+ENTM\n");    //退出命令模式
        else SCI_send("AT+Z\n");    //退出命令模式
        SCI_send(wifi_data);
#endif
      
      return error;
}

/******************************************************************
    函数名：wifi_command(char *command , char* data,char mode)
    功能： 命令写入，模式0读取状态，模式1写入命令
    参数： command —— 操作命令
           data ——模式0 ：返回的状态
                    模式1 ：写入的命令
           mode ——模式选择
    返回：错误类型  0  没有错误
                    1  返回不符
                    2  没有返回
******************************************************************/
char  wifi_command(char *command , char* data,char mode){
      char error = 2;
      int length = 0;

      Delay_ms(2);
      UART2_send("AT+");
      UART2_send(command);
      if(mode == 1){   //有数据，则写入指令，否则查看状态
          UART2_send("=");
          UART2_send(data);
      }
      UART2_send("\n");
      
      wifi_getf = 0;
      UART2_delay(100);
      while(uart2_flag);
      
      if(1 == wifi_getf){
            error = 1;           //收到信号但不是OK 说明命令指令错误
            if(mode == 1) length = str_include(wifi_data,"OK");
            else length = str_include(wifi_data,data);
            if( -1 != length){
                 error = 0;      //能收到OK 说明状态返回不正确
            }
      }           


#ifdef wifi_DEBUG
        SCI_send("AT+");
         SCI_send(command);
      if(mode == 1){   //有数据，则写入指令，否则查看状态
          SCI_send("=");
          SCI_send(data);
      }
      SCI_send("\n");
        SCI_send(wifi_data);
#endif
      
      return error;
}


/******************************************************************
    函数名：wifi_IP(unsigned char *ip,unsigned int port)
    功能：写入IP和端口号
    参数：ip ——ip号
          port ——端口号
    返回：错误类型  0  没有错误
                    1  返回不符
                    2  没有返回
******************************************************************/
char wifi_IP(unsigned char *ip,unsigned int port){
     char error = 2;

     Delay_sleepms(2);
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
     UART2_delay(100);  //最长等待50ms
     while(uart2_flag);      //等待数据接收完成，若无返回则50ms后退出   
     
     if(1 == wifi_getf){
        error = 1;
        if(-1 != str_include(wifi_data,"OK"))error = 0;   //修改ip成功
     }
     wifi_getf = 0;
     
     

#ifdef wifi_DEBUG
        SCI_send(wifi_data);
#endif
           
     
     return error; 
}

/******************************************************************
    函数名：wifi_AP(char *name,char *password)
    功能：写入路由器信息
    参数：name ——wifi名字
          password ——wifi密码
    返回：错误类型  0  ip写入
                    1  返回不符
                    2  没有返回
                    3  没搜索到路由器
******************************************************************/
char wifi_AP(char *name,char *password){
     char error = 2;

     Delay_ms(2);
     UART2_send("AT+WSTA=");   //写入名字和密码
     UART2_send(name);
     UART2_send(",");
     UART2_send(password);
     UART2_send("\n");
     wifi_getf = 0;
     UART2_delay(50);         //最长等待10ms
     while(uart2_flag);       //等待数据接收完成，若无返回则10ms后退出
     
     if(1 == wifi_getf){
        error = 1;
        if(-1 != str_include(wifi_data,"OK"))error = 0;   //修改ip成功
     }
     wifi_getf = 0;
     
#ifdef wifi_DEBUG
        SCI_send(wifi_data);
#endif
                
     return error; 
}

/******************************************************************
    函数名：wifi_TCPtest()
    功能：查看链接状态
    参数：无
    返回： 0  TCP未连接
           1  TCP连接中
 
******************************************************************/
char wifi_TCPtest(void){
  char error;
  if(wifi_start() == 0){                   //进入命令模式
     error = wifi_command("SOCKLKA" ,"DISCONNECTED",0); //只能检测无法连接的情况
                                  //DISCONNECTED 包含 CONNECTED 
     wifi_end(0);     
  
  }//返回透传模式
  if(error == 1)return 1;
  else return 0;
  
}

/******************************************************************
    函数名：wifi_sleep()
    功能：进入休眠模式
    参数：无
    返回： 
******************************************************************/
/*char wifi_sleep(void){
  char error;
  
  wifi_start();  
  Delay_ms(10);
  error = wifi_command("MSLP" ,"OK",0); //直接进入休眠
                               
  if(error == 1)return 1;
  else return 0;
  
}*/

//-------------------中断接收-------------------------//
//---------------接收SIM返回的数据--------------------//
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
                                                   //10ms内没接收到新数据视为通信结束
        }else {
            uart2_num = REMAX - 2;
            uart2_error = 1;
        }
        UART2_delay(10);   
      break                                                        ;
  case 4:break                                                     ;  // Vector 4 - TXIFG
  default: break                                                   ;  
  }  
}