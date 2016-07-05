

#ifndef _wifi_h_
#define _wifi_h_

#define wifi_DEBUG               //������Ϸģʽ
#define REMAX        512          //����������

#define wifi_send        UART2_send        //�����ַ���
#define wifi_send_num    UART2_send_num    //�����޷���������
#define wifi_send_float  UART2_send_float  //���;�ȷ��С�������λ�ĸ�����


//��������ģʽ
char  wifi_start(void);
//�˳�����ģʽ��д��1����wifiģ��
char  wifi_end(char mode);
//д������
char  wifi_command(char *command , char* data,char mode);  
//д��ip��ַ�Ͷ˿ں�
char  wifi_IP(unsigned char *ip,unsigned int port);  
//д��·���������ֺ�����
char  wifi_AP(char *name,char *password);   
//����Ƿ�������PTC��1 ���ӣ�0�Ͽ�
char  wifi_TCPtest(void);      
//�����������Ƿ������Ҫ��AP
char  SearchAP(char* name);     

extern int  uart2_num;        //���ڽ�������
extern char uart2_error;      //�������������

extern char wifi_getf;       //SIMģ����յ����ݱ�־
extern char wifi_data[REMAX];//SIMģ�����ݽ�������


#endif