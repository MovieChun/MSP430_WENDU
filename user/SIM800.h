
#ifndef _SIM800_H_
#define _SIM800_H_

extern char SIM800_test(void);
extern char SIM800_init(void);
extern char SIM800_Getip(char *ip,unsigned port );
extern char STM800_SEND(char *data);

char* SIM800_START(char*commend);
char SIM800_close(void);
char SIM800_status(void);
void send_data(void);
#endif