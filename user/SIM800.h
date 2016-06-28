
#ifndef _SIM800_H_
#define _SIM800_H_





#define  com_start()      UART_send("AT+")  //���ʼ
#define  com_send         UART_send         //�����ַ�
#define  com_send_num     UART_send_num     //�����޷�������
#define  com_quote()      UART_send("\"")   //��������


extern char SIM800_test(void);                       //�����Ƿ�������SIM
extern char SIM800_init(void);                       //SIMʹ��
extern char SIM800_Getip(unsigned char *ip,unsigned port );   //д��IP�Ͷ˿ں�
extern char SIM_DelayRecom(unsigned int ms ,const char *recomm); //��ʱ���������Ϣ
extern char SIM_command(const char*command ,const char* recomm);

//------------GPRS���ͺ���------------
extern char GPRS_Start(void);
#define  GPRS_Send       UART_send            //�����ַ�
#define  GPRS_SendNum    UART_send_num        //�����޷�������
//#define  GPRS_SendEnd()  UART_send_char(0x1a) //���ͽ�ֹ��
char GPRS_SendEnd(void);

#endif