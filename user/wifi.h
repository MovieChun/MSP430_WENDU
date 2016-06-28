#ifndef _wifi_h_
#define _wifi_h_

char  wifi_start(void);
char  wifi_end(char mode);
char  wifi_command(char *command , char* data,char mode);
char  wifi_IP(unsigned char *ip,unsigned int port);
char  wifi_AP(char *name,char *password);
char SearchAP(char* name);




#endif