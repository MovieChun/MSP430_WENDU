#include "msp430x54x.h"
#include "PC.h"

void PC_control(void){
  
  if(SCI_getf == 1){         //串口接收到信号         
    while(SCI_flag);          
    SCI_send(SCI_data);          
    SCI_send("\n");
    SCI_getf = 0;     
    if(-1 != str_include(SCI_data,"TEST=")){
                
    }       
//-------------检查配置-----------------------------//
   if(-1 != str_include(SCI_data,"CHACK")){  //CHACK 查看连接参数
//      SCI_send("SIM_IP:");    
//      SCI_send_IP(SIM_IP,SIM_PORT);      
      SCI_send("WIFI_IP:");    
      SCI_send_IP(WIFI_IP,WIFI_PORT);      
      SCI_send("AP name:");    
      SCI_send(APname);    
      SCI_send("\nAP key:");   
      SCI_send(APkey);    
//      SCI_send("\nMODE=");    
//      SCI_send_num(mode);    
//      SCI_send("\n");                                       
    }
//--------------检查配置结束--------------------------
//--------------WIFI调试----------------------------- 
    if(-1 != str_include(SCI_data,"WIFI DEBUG")){  //进入WIFI模块调试模式，串口数据直接发送到WIFI
        char a = 1;
        SCI_send("调试wifi\n");
        if(wifi_start() == 0)
        {
           wifi_command("SLPTYPE", "0",1);       //设置省电模式           
           wifi_end(1); 
         } 
        while(a){
           if(SCI_getf == 1){         //串口接收到信号             
              while(SCI_flag);          
              SCI_send(SCI_data);          
              SCI_send("\n");
              SCI_getf = 0;     
              if(-1 != str_include(SCI_data,"ESC")){
                a = 0;
                SCI_send("esc ok\n");
              }
              else if(-1 != str_include(SCI_data,"TXD")){
                WIFI_TX_DOWN;
                SCI_send("txd ok\n");
              }else if(-1 != str_include(SCI_data,"TXU")){
                WIFI_TX_UP;
                SCI_send("tsu ok\n");
              }else if(-1 != str_include(SCI_data,"TXON")){
                WIFI_TX_ON;
                SCI_send("txon ok\n");
              }else{
                WIFI_TX_ON;
                Delay_ms(1);
                wifi_SR(SCI_data);  //用于调试模块  
              }              
            } 
                      
           wifi_gets();
        }
     }
//------------------WIFI调试结束-----------------------------------------------
        // else SCI_send("command error\n");   //其他指令不做处理
  }
  
}