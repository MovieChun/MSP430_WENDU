
#ifndef _flash_h_
#define _flash_h_

#define SAVE_ADD  0XE200
void FERASE(unsigned int address);
extern void Flash_read(unsigned int address, unsigned char* data,unsigned int length);
extern void Flash_write(unsigned int address, unsigned char* data,unsigned int length);


#endif