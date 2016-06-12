
#ifndef _SIM800_H_
#define _SIM800_H_

extern char SIM800_test(void);
extern char SIM800_init(void);
extern char SIM800_Getip(char *ip,unsigned port );
extern char STM800_SEND(char *data);

char* SIM800_START(char*commend);
#endif