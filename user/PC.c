#include "msp430x54x.h"
#include "PC.h"

void PC_control(void){
  
  if(SCI_getf == 1){         //���ڽ��յ��ź�         
    while(SCI_flag);          
    SCI_send(SCI_data);          
    SCI_send("\n");
    SCI_getf = 0;     
    if(-1 != str_include(SCI_data,"TEST=")){
                
    }       
//-------------�������-----------------------------//
   if(-1 != str_include(SCI_data,"CHACK")){  //CHACK �鿴���Ӳ���
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
//--------------������ý���--------------------------
//--------------WIFI����----------------------------- 
    if(-1 != str_include(SCI_data,"WIFI DEBUG")){  //����WIFIģ�����ģʽ����������ֱ�ӷ��͵�WIFI
        char a = 1;
        SCI_send("����wifi\n");
        if(wifi_start() == 0)
        {
           wifi_command("SLPTYPE", "0",1);       //����ʡ��ģʽ           
           wifi_end(1); 
         } 
        while(a){
           if(SCI_getf == 1){         //���ڽ��յ��ź�             
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
                wifi_SR(SCI_data);  //���ڵ���ģ��  
              }              
            } 
                      
           wifi_gets();
        }
     }
//------------------WIFI���Խ���-----------------------------------------------
        // else SCI_send("command error\n");   //����ָ�������
  }
  
}