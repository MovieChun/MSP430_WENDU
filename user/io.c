#include "msp430x54x.h"

void io_init(void){
  // Port Configuration
  P1OUT = 0x00;
  P2OUT = 0x00;
  P3OUT = 0x00;
  P4OUT = 0x00;
  P5OUT = 0x00;
  P6OUT = 0x00;
  P7OUT = 0x00;
  P8OUT = 0x00;
  P9OUT = 0x00;
  P10OUT = 0x00;
  P11OUT = 0x00;
  PJOUT = 0x00;

  P1DIR = 0xFF;
  P2DIR = 0xFF;
  P3DIR = 0xFF;
  P4DIR = 0xFF;
  P5DIR = 0xFF;
  P6DIR = 0xFF;
  P7DIR = 0xFF;
  P8DIR = 0xFF;
  P9DIR = 0xFF;
  P10DIR = 0xFF;
  P11DIR = 0xFF;
  PJDIR = 0xFF;
   
  // Disable SVS
  PMMCTL0_H = PMMPW_H;                    // PMM Password
  SVSMHCTL &= ~(SVMHE | SVSHE);           // Disable High side SVS
  SVSMLCTL &= ~(SVMLE | SVSLE);           // Disable Low side SVS
}