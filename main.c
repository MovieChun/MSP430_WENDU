
#include "msp430x54x.h"
#include "main.h"
#include "Clock.h"

int times = 0;

int main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  Init_CLK();
  Init_Timer0_A5();
  _EINT(); 
  P4DIR = 0XFF;
  P4OUT = 0Xff;
  while(1){
    if(times > 1000){
        times = 0;
        P4OUT ^= 0xff; 
    }
  }
  
}



#pragma vector=TIMER0_A0_VECTOR                             
__interrupt void Timer0_A0 (void)
{
  
  if(times <4000)times++;

}
