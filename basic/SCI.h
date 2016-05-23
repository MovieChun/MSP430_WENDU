
#ifndef _sci_h_
#define _sci_h_

#define RS_PORT_SEL   P10SEL
#define RS_PORT_DIR   P10DIR

#define TXD   BIT4
#define RXD   BIT5

#define UART_IN                P10OUT &=~DE_485;\
                               P10DIR |= DE_485;\
                               __delay_cycles(5)

extern char  event_SCI, RXBuffer_SCI;  //获取标志位，接收的数据
void Init_RSUART(void);
void RS232TX_PROC(char *tx_buf);
void RS232TX_PROC2(char *tx_buf,char length);

#endif