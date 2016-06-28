
#include "msp430x54x.h"
#include "main.h"
#include <string.h>

int times = 0;

float Tdata;


unsigned char IP[4]={211,81,253,244};
unsigned int PORT = 8000;
char data[]="hallo world\n";

int main( void )
{  
  char cnt = 0;
  //Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;

  //---------单片机模块初始化-------------------------------------  
  Init_CLK();
  Init_Timer0_A5();
  Init_Timer1_A3();
  SCI_init();
  UART_init();
  UART2_init(); 
  mlx90614_INIT();
  _EINT(); 
  
  P4DIR = 0XFF;
  P4OUT = 0Xff;
 
//--------------------------------------------
  SCI_send("go");
  SCI_send("\n");
 
  
  
  //while(SIM800_test())Delay_ms(5);
  //SIM800_START("ATE0");  //关闭回显示
  //SIM800_START("ATE1");  //打开回显示调试时用
  //SIM800_init();
  //SIM_command("CIPCLOSE","CLOSED"); //成功返回 CLOSED，失败返回 ERROR
  //SIM800_Getip(IP,PORT);
  
  //while(SIM_DelayRecom(2000,"CONNECT OK")){
  //   SIM800_Getip(IP,PORT);
  //}
  
  
  /*if(GPRS_Start()){
     GPRS_SendEnd();
  }else{
      GPRS_Send("维春最帅");
      GPRS_Send("\n");
      GPRS_SendEnd();
      if(SIM_DelayRecom(500,"SEND OK"))GPRS_SendEnd();
  }
  */
  times = 0;
  while(1){
    
    {   
        wifi_start();
        wifi_command("WSTA","DC411,C411",0);
        wifi_command("WSTA","DC411,C411",1);
        wifi_IP(IP,PORT);
        wifi_end(1);
      
        /*SCI_send("\n");
        P4OUT ^= 0xff; 
        times = 0;
        
        if(GPRS_Start() && SIM_DelayRecom(10,"error")){
          GPRS_SendEnd();
       }else{
        GPRS_Send("同意+");
        GPRS_SendNum(count++);
        GPRS_Send("\n");
        GPRS_SendEnd();
        //if(SIM_DelayRecom(500,"SEND OK"))GPRS_SendEnd();
     }
    */
       __bis_SR_register(LPM4_bits);             // Enter LPM3
    
    }
   
    
  }
  
}



#pragma vector=TIMER0_A0_VECTOR                             
__interrupt void Timer0_A0 (void)
{
  
  if(times <5000)times++;
  __bic_SR_register_on_exit(LPM0_bits);   // Exit active CPU
}





