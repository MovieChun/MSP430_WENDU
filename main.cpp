
#include "msp430x54x.h"

int main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  P4DIR = 0XFF;
  P4OUT = 0XFF;
  while(1);
  

  return 0;
}
