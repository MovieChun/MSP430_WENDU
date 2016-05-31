/**********************************************************************
      文件名：IIC.h
      说明  ：
              引脚 ：SCL -> P5.4 -> Pin51
                     SDA -> P3.7 -> Pin42
      时间  ：2016.05.30
      
**********************************************************************/
#ifndef _IIC_h_
#define _IIC_h_


void IIC_INIT(unsigned char slave_add);                       //IIC初始化函数
char IIC_readpage(char command, char *pword_buf, char len );  //写入数据  0异常中断 1正常完成
char IIC_writepage(char command, char *pword_buf, char len);  //读出数据  0异常中断 1正常完成
void IIC_SLAVE_ADD(unsigned char add);                        //写入从机地址
char  IIC_Command(char command);                              //写命令

#endif