

#ifndef _wifi_h_
#define _wifi_h_

#define wifi_DEBUG               //开启调戏模式
#define REMAX        512          //接收区容量

#define wifi_send        UART2_send        //发送字符串
#define wifi_send_num    UART2_send_num    //发送无符号整型数
#define wifi_send_float  UART2_send_float  //发送精确到小数点后两位的浮点数


//进入命令模式
char  wifi_start(void);
//退出命令模式，写入1重启wifi模块
char  wifi_end(char mode);
//写入命令
char  wifi_command(char *command , char* data,char mode);  
//写入ip地址和端口号
char  wifi_IP(unsigned char *ip,unsigned int port);  
//写入路由器的名字和密码
char  wifi_AP(char *name,char *password);   
//检测是否连接上PTC，1 连接，0断开
char  wifi_TCPtest(void);      
//搜索附近的是否存在想要的AP
char  SearchAP(char* name);     

extern int  uart2_num;        //串口接收数量
extern char uart2_error;      //数据溢出最大错误

extern char wifi_getf;       //SIM模块接收到数据标志
extern char wifi_data[REMAX];//SIM模块数据接收数组


#endif