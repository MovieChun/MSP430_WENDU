
#ifndef _SIM800_H_
#define _SIM800_H_



#define RECMAX  512   //����������

#define  com_start()      UART_send("AT+")  //���ʼ
#define  com_send         UART_send         //�����ַ�
#define  com_send_num     UART_send_num     //�����޷�������
#define  com_quote()      UART_send("\"")   //��������


extern char SIM800_test(void);                       //�����Ƿ�������SIM
extern char SIM800_init(void);                       //SIMʹ��
extern char SIM800_Getip(char *ip,unsigned port );   //д��IP�Ͷ˿ں�
extern char SIM_DelayRecom(unsigned int ms ,const char *recomm); //��ʱ���������Ϣ


#endif