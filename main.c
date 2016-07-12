
#include "msp430x54x.h"
#include "main.h"
#include <string.h>

char flag = 0;
char time = 0;

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
  int   cnt = 0;
  float tem = 0;
  char SIM_ERR = 0;    //SIM�����ӱ�־
  char wifi_ERR = 0;   //wifi���ӱ�־
  
  //Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;

  //---------��Ƭ��ģ���ʼ��-------------------------------------  
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
  Delay_ms(300);  //�ϵ�ȴ�����ģ��

//--------wifiģ������-------------------------------------   
   cnt = 0;
   while(wifi_start() && cnt++ < 5){
         Delay_ms(5);
         wifi_end(1);      //�����Ǵ�������ģʽ����wifi����һ��
         Delay_ms(5);
        
   }
   //SCI_send_num(cnt);
   if(cnt == 6)wifi_ERR = 1;
   else{
      wifi_command("WMODE", "STA",1);  //���óɶ˿�ģʽ
      wifi_command("WANN", "DHCP",1);  //ʹ�ö�̬��ַ
      wifi_command("E", "OFF",1);    //�رջ���
      wifi_AP(APname,APkey);           //����wifi��������
      wifi_IP(APip,APport);            //AP�õ��Ǿ�����
      wifi_end(1);                     //����wifi
   }
  
   
  
//----------SIM������-----------------------------------  
   cnt = 0;

   while(SIM800_test()&&cnt++ < 5){    //���5��SIM��ģ��
         Delay_ms(100);
   }
   SCI_send_num(cnt);
   if( SIM800_test())SIM_ERR = 1;             
   else{                     //SIMģ������
     //SIM800_START("ATE0");              //�رջ���ʾ
     //SIM_command( "CGREG=1","\0");
     SIM_command( "CIPSTATUS","OK");
     SIM_command("CIPCLOSE","CLOSED"); //�ɹ����� CLOSED��ʧ�ܷ��� ERROR  �ȹر�����
     SIM800_Getip(IP,PORT);
    // SIM_command( "AT+CMGF=1","OK");  //���ı���ʽ�༭����
    // SIM_command( "AT+CNMI=2,2","OK");  //��������ֱ�Ӷ�ȡ

     cnt = 0; 
     Delay_ms(500);
     
     while(SIM_command( "CIPSTATUS","CONNECT") && cnt++ < 5){  //����5�ο��Ƿ���������
              Delay_ms(10);
     }
     if(cnt == 6)SIM_ERR = 2;     //���Ӳ���TCP��־
   }
  
//------------���󷵻�--------------------------------------  
       if(SIM_ERR == 2)SCI_send("\n-------SIMģ�����Ӳ���TCP------\n");
       else if(SIM_ERR == 1)SCI_send("\n-------SIM�����ź�------\n");
       else SCI_send("\n-------SIM�����ӳɹ�------\n");
       
       if(wifi_ERR == 2)SCI_send("\n-------���Ӳ���wifiģ��------\n");
       else if(wifi_ERR == 1)SCI_send("\n-------wifi�����ź�------\n");
       else SCI_send("\n-------wifi�����ӳɹ�------\n");
   
 //-------------------������---------------------------------------//

  cnt = 0;
  
  while(1){
    if(SCI_getf == 1){   //wifiģ����յ���Ϣ
         while(SCI_flag);
         SCI_send(SCI_data);
         UART_send(SCI_data);
         UART1_delay(10);
         while(uart1_flag);
         SCI_send(SIM_data);
         SCI_getf = 0;
         
       }
    
    if(SIM_getf == 1){   //wifiģ����յ���Ϣ
         SCI_send(SIM_data);
         SIM_getf = 0;
         
       }
    
    if(time == 1)
    {   time = 0;
       P4OUT |= BIT7;        //����
       /*
       //tem = MLX_RT();       //�¶ȶ�ȡ��IICδ���ӻῨ��
       if(SCI_getf == 1){   //wifiģ����յ���Ϣ
         while(SCI_flag);
         SCI_send(SCI_data);
         SCI_getf = 0;
       }
    
       if(wifi_getf == 1){   //wifiģ����յ���Ϣ
         while(uart2_flag);
         SCI_send(wifi_data);
         wifi_getf = 0;
       }
       
       if(SIM_getf == 1){    //SIM�����յ���Ϣ
         
 
          SIM_getf = 0;  
       }
       
       
      
       SIM_ERR = SIM_command( "CIPSTATUS","CONNECT");  //���SIM����״��
       
      // if( wifi_TCPtest())    //wifi���ӣ�������wifi����
       {      //wifiģ��������
             
             wifi_send("W�¶�");       //�����¶Ⱥ����
             wifi_send_num(cnt++); 
             wifi_send(": ");
             wifi_send_float(tem);
             wifi_send("\n");
             
             if(0 == SIM_ERR) wifi_send("SIM ERROR\n");  //���SIM�����������⣬���ʹ��󱨸�
       }
      // else
        if(0 == SIM_ERR){
          if(SIM_DelayRecom("PDP DEACT"))SIM_command("CIICR","OK");
          SIM_command("CIPCLOSE","CLOSED");
          Delay_ms(10);     
          SIM800_Getip(IP,PORT);                   //���û�����ϣ�д��IP��ַ��������
                               //�ȴ�����
          
       }
       else
       {                        //wifiδ���ӣ���Ϊgprs����
             GPRS_Start();              //GPRS���뷢��ģʽ
         
             GPRS_Send("S�¶�");         //�����¶�
             GPRS_SendNum(cnt++);
             GPRS_Send("�� ");
             GPRS_Send_float(tem);
             GPRS_Send("\n");
             GPRS_Send("\n----wifi�Ͽ�-----\n");   //���ʹ��󱨸�
             GPRS_SendEnd();            //��������
         
       }
             
      */
      
       P4OUT &= ~BIT7;   //�رյ� 
       //__bis_SR_register(LPM4_bits);             // Enter LPM3
    
    }
   
    
  }
  
}



#pragma vector=TIMER0_A0_VECTOR                             
__interrupt void Timer0_A0 (void)
{
  time = 1;
  __bic_SR_register_on_exit(LPM0_bits);   // Exit active CPU
}





