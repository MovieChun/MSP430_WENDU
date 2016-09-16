
#ifndef _GU900_INIT_H_
#define _GU900_INIT_H_


#define RECMAX  128   //����������
#define SIM_DEBUG   //������Ϸģʽ

extern int  uart_num;      //���ڽ�������
extern char uart_error;    //�������������

extern char SIM_getf;      //SIMģ����յ����ݱ�־
extern char SIM_data[RECMAX];  //SIMģ�����ݽ�������



#define  com_start()      UART_send("AT+")  //���ʼ
#define  com_send         UART_send         //�����ַ�
#define  com_send_num     UART_send_num     //�����޷�������



extern char SIM800_init(void);                       //SIMʹ��
extern char SIM800_Getip(unsigned char *ip,unsigned port );   //д��IP�Ͷ˿ں�
extern char SIM_DelayRecom(const char *recomm); //���������Ϣ
extern char SIM_command(const char*command ,const char* recomm);

//------------GPRS���ͺ���------------
extern char GPRS_Start(void);
#define  GPRS_Send        UART_send            //�����ַ�
#define  GPRS_SendNum     UART_send_num        //�����޷�������
#define  GPRS_Send_float  UART_send_float
char GPRS_SendEnd(void);
char note_send(char *phone , char*str);

#endif