/*********************************************************
    SIMģ���ʼ��
*********************************************************/
/*char SIM_init(void){
  char cnt = 0;
  char ok = 1;
  while(SIM800_test()&&cnt++ < 2){    //���3��SIM��ģ��
         Delay_sleepms(10);
   }
   if( SIM800_test())ok = 0;             
   else{                                 //SIMģ������
     UART_send("ATE0\n");                  //�رջ���ʾ
     Delay_sleepms(10);
     SIM_command("CIPCLOSE","CLOSED");   //�ɹ����� CLOSED��ʧ�ܷ��� ERROR  �ȹر�����
     Delay_sleepms(10);
     SIM800_Getip(SIM_IP,SIM_PORT);
     Delay_sleepms(50);                      //�ȴ�50ms����ʱ��
     
     cnt = 0;
     while(0 == SIM_status() && cnt++ < 3)Delay_ms(10);;  //����3�ο��Ƿ���������
     if(cnt == 4)ok = 0;     //���Ӳ���TCP��־     
   }
     return ok;
  }
*/
/*******************************************************************************
      SIMģ��״̬���ʹ���
********************************************************************************/
/*char SIM_status(void){
    char ok = 0;
    
    if(SIM_command( "CIPSTATUS","CONNECTING"))Delay_ms(30);
    else if(SIM_command( "CIPSTATUS","CONNECT"))ok = 1;
    else if(-1 != str_include(SIM_data,"IP INITIAL")||  //δ����״̬
            -1 != str_include(SIM_data,"CLOSE")){
            SIM800_Getip(SIM_IP,SIM_PORT);                   //��������
            Delay_sleepms(500);
    }
    else if(-1 != str_include(SIM_data,"PDP DEACT")){   //������ע��
      SIM_command( "CIPSHUT","OK"); 
      Delay_sleepms(5);
      SIM_command( "CSTT","OK");                        //�����
      Delay_sleepms(500);
      SIM_command( "CIICR","OK");
      Delay_sleepms(5);
      SIM_command( "CIFSR","OK");
    }
    else if(-1 != str_include(SIM_data,"IP GPRSACT")){   //������ע��
      SIM_command( "CIPSHUT","OK"); 
      Delay_sleepms(30);
     
    }
    else  Delay_sleepms(500);                              //���࣬�ȴ���ʼ�����
    
    return ok;
}
   */