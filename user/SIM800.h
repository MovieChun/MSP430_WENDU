
#ifndef _SIM800_H_
#define _SIM800_H_


#define RECMAX  512   //接收区容量
#define SIM_DEBUG   //开启调戏模式

extern int  uart_num;      //串口接收数量
extern char uart_error;    //数据溢出最大错误

extern char SIM_getf;      //SIM模块接收到数据标志
extern char SIM_data[RECMAX];  //SIM模块数据接收数组



#define  com_start()      UART_send("AT+")  //命令开始
#define  com_send         UART_send         //发送字符
#define  com_send_num     UART_send_num     //发送无符号整数



extern char SIM800_test(void);                       //测试是否连接上SIM
extern char SIM800_init(void);                       //SIM使能
extern char SIM800_Getip(unsigned char *ip,unsigned port );   //写入IP和端口号
extern char SIM_DelayRecom(unsigned int ms ,const char *recomm); //延时处理接收信息
extern char SIM_command(const char*command ,const char* recomm);

//------------GPRS发送函数------------
extern char GPRS_Start(void);
#define  GPRS_Send        UART_send            //发送字符
#define  GPRS_SendNum     UART_send_num        //发送无符号整数
#define  GPRS_Send_float  UART_send_float
char GPRS_SendEnd(void);
char note_send(char *phone , char*str);

#endif