
#include "msp430x54x.h"
#include "main.h"
#include <string.h>

int times = 0;

float Tdata;


char IP[]="211.81.253.244";
unsigned int PORT = 8000;
char data[]="hallo world\n";

int main( void )
{  
  //char flag;
  //Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  Init_CLK();
  Init_Timer0_A5();
  Init_Timer1_A3();
  SCI_init();
  UART_init();
    
  mlx90614_INIT();
  _EINT(); 
  
  P4DIR = 0XFF;
  P4OUT = 0Xff;
  
  SCI_send_num(250);
  SCI_send("\n");
  
  while(SIM800_test());
  //SIM800_START("ATE0");  //�رջ���ʾ
  //while(!SIM800_init());
  //SIM800_START("ATE1");  //�򿪻���ʾ����ʱ��
  //if(SIM800_status() != 2)SIM800_close();
  //SIM800_Getip(IP,PORT);
  
  times = 0;
  while(1){
    if(times > 0)
    {   //flag++;
        times = 0;
        P4OUT ^= 0xff; 
        SIM800_init();
        //SIM800_test();
        // send_data();
        //STM800_SEND(data);
        
       // __bis_SR_register(LPM4_bits);             // Enter LPM3
    
    }
   
    
  }
  
}



#pragma vector=TIMER0_A0_VECTOR                             
__interrupt void Timer0_A0 (void)
{
  
  if(times <5000)times++;
  //__bic_SR_register_on_exit(LPM0_bits);   // Exit active CPU
}





