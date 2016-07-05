
#include "msp430x54x.h"
#include "Clock.h"
#include "uart2.h"
#include "SCI.h"
#include "Fstring.h"
#include "wifi.h"

//#define wifi_DEBUG       //开启调戏模式
#define REMAX     512    //接收区容量

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
******************************************************************/
char  wifi_start(void){
      char error = 2;
      int length = 0;
     
      UART2_send("+++");    //发送"+++"和"a"进入命令模式 
      wifi_getf = 0;
      UART2_delay(10);
      while(uart2_flag);
     
      UART2_send("a");       
      UART2_delay(10);
      while(uart2_flag);
      
      if(1 == wifi_getf){
            error = 1;           //收到信号但不是OK 说明命令指令错误
            //SCI_send("\n--------");
            //SCI_send(wifi_data);
            //SCI_send("\n");
            length = str_include(wifi_data,"OK");
            if( -1 != length){              
                error = 0;      //能收到OK 说明状态返回不正确
            }
      }      
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
      UART2_delay(10);
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
            //SCI_send("\n--------");
            //SCI_send(wifi_data);
            //SCI_send("---------\n");
            
            if(mode == 1) length = str_include(wifi_data,"OK");
            else length = str_include(wifi_data,data);
            if( -1 != length){
                 error = 0;      //能收到OK 说明状态返回不正确
            }
      }           


#ifdef wifi_DEBUG
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
     UART2_delay(10);  //最长等待10ms
     while(uart2_flag);      //等待数据接收完成，若无返回则10ms后退出   
     
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
     
     UART2_send("AT+WSTA=");   //写入名字和密码
     UART2_send(name);
     UART2_send(",");
     UART2_send(password);
     UART2_send("\n");
     wifi_getf = 0;
     UART2_delay(10);         //最长等待10ms
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
    函数名：SearchAP(char* name)
    功能：写入路由器信息
    参数：name ——wifi名字
    返回：错误类型  0  ip写入
                    1  没搜索到路由器
                    2  没有返回
******************************************************************/
char SearchAP(char* name){
     char error = 2;
     
     UART2_send("AT+WSCAN\n"); //先搜索附近的路由器
     wifi_getf = 0;
     UART2_delay(10);          //最长等待10ms
     while(uart2_flag);        //等待数据接收完成，若无返回则10ms后退出
     UART2_delay(100);         //最长等待100ms，搜素的时间比较长
     while(uart2_flag);      
     if(1 == wifi_getf){
        error = 1;
        if(-1 != str_include(wifi_data,name))error = 0;   //修改ip成功
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
    返回：错误类型  0  TCP未连接
                    1  TCP连接中
                    2  没有返回
******************************************************************/
char wifi_TCPtest(void){
  char error;
  wifi_start();                   //进入命令模式
  error = wifi_command("SOCKLKA" ,"DISCONNECTED",0); //只能检测无法连接的情况
                                  //DISCONNECTED 包含 CONNECTED 
  wifi_end(0);                    //返回透传模式
  return error;                   //0 未连接，1 已连接
  
}



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
             UART2_delay(10);                                         //10ms内没接收到新数据视为通信结束
        }else {
            uart2_num = REMAX - 2;
            uart2_error = 1;
       }
   
      break                                                        ;
  case 4:break                                                     ;  // Vector 4 - TXIFG
  default: break                                                   ;  
  }  
}