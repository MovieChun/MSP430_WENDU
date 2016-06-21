/**********************************************************************
      �ļ�����Clock.h
      ˵��  ��ʱ�ӳ�ʼ������ʱ����ʼ��
              ���� �����پ��� 32.768k
                     XIN  -> P7.0 -> Pin13
                     XOUT -> P7.1 -> Pin14
                     ���پ��� 16M
                     XT2IN  -> P5.2 -> Pin89
                     XT2OUT -> P5.3 -> Pin90
      ʱ��  ��2016.05.23
      ����  ���Ѳ���
**********************************************************************/
#ifndef _clock_h_
#define _clock_h_

#define  TIME   (500 - 1)         //2s��ʱ  


// ��ʱ������ѡ���ѡ���ڲ�DCO���ⲿ��Ƶ����
#define  EXTERNAL_HF_OSC           //ʹ���ⲿʱ��Դ  

extern void Init_CLK(void);        //ʱ�ӳ�ʼ������
extern void Init_Timer0_A5(void);  //��ʱ����ʼ��
extern void Init_Timer1_A3(void);

extern char* TIM1_delay(unsigned int ms);  //�ж���ʱ,��λms
extern char delay_flag;    //��ʱ������־Ϊ0

#endif