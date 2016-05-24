/**********************************************************************
      �ļ�����SPI.h
      ˵��  ��
              ���� ��SPI_MISO -> P3.1 -> Pin34
                     SPI_MOSI -> P3.2 -> Pin35
                     SPI_CLK  -> P3.3 -> Pin36 
      ʱ��  ��2016.05.24
      ����  ��������
**********************************************************************/
#ifndef _spi_h_
#define _spi_h_

void Init_SPI(void);                        //SPI��ʼ��
inline void SPI_write(unsigned char data);  //д��һ���ֽ�
inline unsigned char SPI_read(void);        //����һ���ֽ�
                         
                        
#endif