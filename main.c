
#include "msp430x54x.h"
#include "main.h"


int times = 0;

float Tdata;
unsigned int data;

int main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  Init_CLK();
  Init_Timer0_A5();
  SCI_init();
  
  mlx90614_INIT();
  _EINT(); 
  
  P4DIR = 0XFF;
  P4OUT = 0Xff;
  
  while(1){
    //if(times > 1)
    {
        times = 0;
        P4OUT ^= 0xff; 
        
        /*SCI_send("\nTA:");
        MLX_ReadT(TA ,&Tdata);
        SCI_send_float(Tdata);
        
        SCI_send("\nTout1:");
        MLX_ReadT(TOBJ1,&Tdata);
        SCI_send_float(Tdata);
        
        SCI_send("\nTout2:");
        MLX_ReadT(TOBJ2,&Tdata);
        SCI_send_float(Tdata);
        
        SCI_send("\nMAX:");
        MLX_ReadT(TOMAX,&Tdata);
        SCI_send_float(Tdata);  
        
        SCI_send("\nMIN:");
        MLX_Read(TOMIN,&data);
        SCI_send_num(data); 
        
        SCI_send("\nRANG:");
        MLX_ReadT(TOMIN,&Tdata);
        SCI_send_float(Tdata); 
        
        __bis_SR_register(LPM3_bits);             // Enter LPM3
        //__no_operation();
        */
        //MLX_sleep();   //起不到休眠作用？
       // SCI_send("ok\n");
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


#pragma vector=USCI_A3_VECTOR
__interrupt void USCI_A3_ISR(void)
{
  switch(__even_in_range(UCA3IV,4))
  {
  case 0:break                                                     ; // Vector 0 - no interrupt
  case 2:                                                            // Vector 2 - RXIFG
      RXBuffer_SCI      = UCA3RXBUF                                 ;
      event_SCI           |= 0x01                                      ;
      break                                                        ;
  case 4:break                                                     ;  // Vector 4 - TXIFG
  default: break                                                   ;  
  }  
}