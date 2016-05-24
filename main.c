
#include "msp430x54x.h"
#include "main.h"


int times = 0;
unsigned char Cstring[5] = "1234";
unsigned char Sflag = 0;

int main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  Init_CLK();
  Init_Timer0_A5();
  UART_init_H();
  _EINT(); 
  P4DIR = 0XFF;
  P4OUT = 0Xff;
  //Flash_write(SAVE_ADD, Cstring,5);
  Flash_read(SAVE_ADD, Cstring,5);
  while(1){
    if(times > 1000){
        times = 0;
        P4OUT ^= 0xff; 
        UART_send("\nhallo\n");
        UART_send_num((char*)Cstring,5);
    }
    if(event_SCI != 0){
       event_SCI = 0;
       Cstring[Sflag++] = RXBuffer_SCI;
       if(Sflag > 4){
         Sflag = 0;
         Flash_write(SAVE_ADD, Cstring,5);
         UART_send("\nsave\n");
       }
    }
  }
  
}



#pragma vector=TIMER0_A0_VECTOR                             
__interrupt void Timer0_A0 (void)
{
  
  if(times <4000)times++;

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