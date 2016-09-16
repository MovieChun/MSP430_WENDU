/*********************************************************
    SIM模块初始化
*********************************************************/
/*char SIM_init(void){
  char cnt = 0;
  char ok = 1;
  while(SIM800_test()&&cnt++ < 2){    //检测3次SIM卡模块
         Delay_sleepms(10);
   }
   if( SIM800_test())ok = 0;             
   else{                                 //SIM模块正常
     UART_send("ATE0\n");                  //关闭回显示
     Delay_sleepms(10);
     SIM_command("CIPCLOSE","CLOSED");   //成功返回 CLOSED，失败返回 ERROR  先关闭连接
     Delay_sleepms(10);
     SIM800_Getip(SIM_IP,SIM_PORT);
     Delay_sleepms(50);                      //等待50ms连接时间
     
     cnt = 0;
     while(0 == SIM_status() && cnt++ < 3)Delay_ms(10);;  //连接3次看是否能连接上
     if(cnt == 4)ok = 0;     //连接不上TCP标志     
   }
     return ok;
  }
*/
/*******************************************************************************
      SIM模块状态检测和处理
********************************************************************************/
/*char SIM_status(void){
    char ok = 0;
    
    if(SIM_command( "CIPSTATUS","CONNECTING"))Delay_ms(30);
    else if(SIM_command( "CIPSTATUS","CONNECT"))ok = 1;
    else if(-1 != str_include(SIM_data,"IP INITIAL")||  //未连接状态
            -1 != str_include(SIM_data,"CLOSE")){
            SIM800_Getip(SIM_IP,SIM_PORT);                   //重新连接
            Delay_sleepms(500);
    }
    else if(-1 != str_include(SIM_data,"PDP DEACT")){   //场景被注销
      SIM_command( "CIPSHUT","OK"); 
      Delay_sleepms(5);
      SIM_command( "CSTT","OK");                        //激活场景
      Delay_sleepms(500);
      SIM_command( "CIICR","OK");
      Delay_sleepms(5);
      SIM_command( "CIFSR","OK");
    }
    else if(-1 != str_include(SIM_data,"IP GPRSACT")){   //场景被注销
      SIM_command( "CIPSHUT","OK"); 
      Delay_sleepms(30);
     
    }
    else  Delay_sleepms(500);                              //其余，等待初始化完成
    
    return ok;
}
   */