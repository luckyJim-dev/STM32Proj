#include "stm32f10x.h"
#include "led.h"
#include "delay.h"

/************************************************
 ALIENTEK ս��STM32F103������ʵ��0
 ����ģ��
 ע�⣬�����ֲ��е��½������½�ʹ�õ�main�ļ� 
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

 
 int main(void)
 {	
 
	 delay_init();
	 
	 LED_Init();
	 
	 while(1){
	 
	  PBout(5)=1;
		PEout(5)=1;

	  delay_ms(500);
		 
	  PBout(5)=0;
		PEout(5)=0;
		 
		 delay_ms(500);
	 
	 
	 }
	 
 }
