#include "delay.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//Èç¹ûÐèÒªÊ¹ÓÃOS,Ôò°üÀ¨ÏÂÃæµÄÍ·ÎÄ¼þ¼´¿É.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos Ê¹ÓÃ	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//±¾³ÌÐòÖ»¹©Ñ§Ï°Ê¹ÓÃ£¬Î´¾­×÷ÕßÐí¿É£¬²»µÃÓÃÓÚÆäËüÈÎºÎÓÃÍ¾
//ALIENTEK STM32¿ª·¢°å
//Ê¹ÓÃSysTickµÄÆÕÍ¨¼ÆÊýÄ£Ê½¶ÔÑÓ³Ù½øÐÐ¹ÜÀí£¨ÊÊºÏSTM32F10xÏµÁÐ£©
//°üÀ¨delay_us,delay_ms
//ÕýµãÔ­×Ó@ALIENTEK
//¼¼ÊõÂÛÌ³:www.openedv.com
//´´½¨ÈÕÆÚ:2010/1/1
//°æ±¾£ºV1.8
//°æÈ¨ËùÓÐ£¬µÁ°æ±Ø¾¿¡£
//Copyright(C) ¹ãÖÝÊÐÐÇÒíµç×Ó¿Æ¼¼ÓÐÏÞ¹«Ë¾ 2009-2019
//All rights reserved
//********************************************************************************
//V1.2ÐÞ¸ÄËµÃ÷
//ÐÞÕýÁËÖÐ¶ÏÖÐµ÷ÓÃ³öÏÖËÀÑ­»·µÄ´íÎó
//·ÀÖ¹ÑÓÊ±²»×¼È·,²ÉÓÃdo while½á¹¹!
//V1.3ÐÞ¸ÄËµÃ÷
//Ôö¼ÓÁË¶ÔUCOSIIÑÓÊ±µÄÖ§³Ö.
//Èç¹ûÊ¹ÓÃucosII,delay_init»á×Ô¶¯ÉèÖÃSYSTICKµÄÖµ,Ê¹Ö®ÓëucosµÄTICKS_PER_SEC¶ÔÓ¦.
//delay_msºÍdelay_usÒ²½øÐÐÁËÕë¶ÔucosµÄ¸ÄÔì.
//delay_us¿ÉÒÔÔÚucosÏÂÊ¹ÓÃ,¶øÇÒ×¼È·¶ÈºÜ¸ß,¸üÖØÒªµÄÊÇÃ»ÓÐÕ¼ÓÃ¶îÍâµÄ¶¨Ê±Æ÷.
//delay_msÔÚucosÏÂ,¿ÉÒÔµ±³ÉOSTimeDlyÀ´ÓÃ,ÔÚÎ´Æô¶¯ucosÊ±,Ëü²ÉÓÃdelay_usÊµÏÖ,´Ó¶ø×¼È·ÑÓÊ±
//¿ÉÒÔÓÃÀ´³õÊ¼»¯ÍâÉè,ÔÚÆô¶¯ÁËucosÖ®ºódelay_ms¸ù¾ÝÑÓÊ±µÄ³¤¶Ì,Ñ¡ÔñOSTimeDlyÊµÏÖ»òÕßdelay_usÊµÏÖ.
//V1.4ÐÞ¸ÄËµÃ÷ 20110929
//ÐÞ¸ÄÁËÊ¹ÓÃucos,µ«ÊÇucosÎ´Æô¶¯µÄÊ±ºò,delay_msÖÐÖÐ¶ÏÎÞ·¨ÏìÓ¦µÄbug.
//V1.5ÐÞ¸ÄËµÃ÷ 20120902
//ÔÚdelay_us¼ÓÈëucosÉÏËø£¬·ÀÖ¹ÓÉÓÚucos´ò¶Ïdelay_usµÄÖ´ÐÐ£¬¿ÉÄÜµ¼ÖÂµÄÑÓÊ±²»×¼¡£
//V1.6ÐÞ¸ÄËµÃ÷ 20150109
//ÔÚdelay_ms¼ÓÈëOSLockNestingÅÐ¶Ï¡£
//V1.7ÐÞ¸ÄËµÃ÷ 20150319
//ÐÞ¸ÄOSÖ§³Ö·½Ê½,ÒÔÖ§³ÖÈÎÒâOS(²»ÏÞÓÚUCOSIIºÍUCOSIII,ÀíÂÛÉÏÈÎÒâOS¶¼¿ÉÒÔÖ§³Ö)
//Ìí¼Ó:delay_osrunning/delay_ostickspersec/delay_osintnestingÈý¸öºê¶¨Òå
//Ìí¼Ó:delay_osschedlock/delay_osschedunlock/delay_ostimedlyÈý¸öº¯Êý
//V1.8ÐÞ¸ÄËµÃ÷ 20150519
//ÐÞÕýUCOSIIIÖ§³ÖÊ±µÄ2¸öbug£º
//delay_tickspersec¸ÄÎª£ºdelay_ostickspersec
//delay_intnesting¸ÄÎª£ºdelay_osintnesting
//////////////////////////////////////////////////////////////////////////////////  

static u8  fac_us=0;							//usÑÓÊ±±¶³ËÊý			   
static u16 fac_ms=0;							//msÑÓÊ±±¶³ËÊý,ÔÚucosÏÂ,´ú±íÃ¿¸ö½ÚÅÄµÄmsÊý
	
	
#if SYSTEM_SUPPORT_OS							//Èç¹ûSYSTEM_SUPPORT_OS¶¨ÒåÁË,ËµÃ÷ÒªÖ§³ÖOSÁË(²»ÏÞÓÚUCOS).
//µ±delay_us/delay_msÐèÒªÖ§³ÖOSµÄÊ±ºòÐèÒªÈý¸öÓëOSÏà¹ØµÄºê¶¨ÒåºÍº¯ÊýÀ´Ö§³Ö
//Ê×ÏÈÊÇ3¸öºê¶¨Òå:
//    delay_osrunning:ÓÃÓÚ±íÊ¾OSµ±Ç°ÊÇ·ñÕýÔÚÔËÐÐ,ÒÔ¾ö¶¨ÊÇ·ñ¿ÉÒÔÊ¹ÓÃÏà¹Øº¯Êý
//delay_ostickspersec:ÓÃÓÚ±íÊ¾OSÉè¶¨µÄÊ±ÖÓ½ÚÅÄ,delay_init½«¸ù¾ÝÕâ¸ö²ÎÊýÀ´³õÊ¼¹þsystick
// delay_osintnesting:ÓÃÓÚ±íÊ¾OSÖÐ¶ÏÇ¶Ì×¼¶±ð,ÒòÎªÖÐ¶ÏÀïÃæ²»¿ÉÒÔµ÷¶È,delay_msÊ¹ÓÃ¸Ã²ÎÊýÀ´¾ö¶¨ÈçºÎÔËÐÐ
//È»ºóÊÇ3¸öº¯Êý:
//  delay_osschedlock:ÓÃÓÚËø¶¨OSÈÎÎñµ÷¶È,½ûÖ¹µ÷¶È
//delay_osschedunlock:ÓÃÓÚ½âËøOSÈÎÎñµ÷¶È,ÖØÐÂ¿ªÆôµ÷¶È
//    delay_ostimedly:ÓÃÓÚOSÑÓÊ±,¿ÉÒÔÒýÆðÈÎÎñµ÷¶È.

//±¾Àý³Ì½ö×÷UCOSIIºÍUCOSIIIµÄÖ§³Ö,ÆäËûOS,Çë×ÔÐÐ²Î¿¼×ÅÒÆÖ²
//Ö§³ÖUCOSII
#ifdef 	OS_CRITICAL_METHOD						//OS_CRITICAL_METHOD¶¨ÒåÁË,ËµÃ÷ÒªÖ§³ÖUCOSII				
#define delay_osrunning		OSRunning			//OSÊÇ·ñÔËÐÐ±ê¼Ç,0,²»ÔËÐÐ;1,ÔÚÔËÐÐ
#define delay_ostickspersec	OS_TICKS_PER_SEC	//OSÊ±ÖÓ½ÚÅÄ,¼´Ã¿Ãëµ÷¶È´ÎÊý
#define delay_osintnesting 	OSIntNesting		//ÖÐ¶ÏÇ¶Ì×¼¶±ð,¼´ÖÐ¶ÏÇ¶Ì×´ÎÊý
#endif

//Ö§³ÖUCOSIII
#ifdef 	CPU_CFG_CRITICAL_METHOD					//CPU_CFG_CRITICAL_METHOD¶¨ÒåÁË,ËµÃ÷ÒªÖ§³ÖUCOSIII	
#define delay_osrunning		OSRunning			//OSÊÇ·ñÔËÐÐ±ê¼Ç,0,²»ÔËÐÐ;1,ÔÚÔËÐÐ
#define delay_ostickspersec	OSCfg_TickRate_Hz	//OSÊ±ÖÓ½ÚÅÄ,¼´Ã¿Ãëµ÷¶È´ÎÊý
#define delay_osintnesting 	OSIntNestingCtr		//ÖÐ¶ÏÇ¶Ì×¼¶±ð,¼´ÖÐ¶ÏÇ¶Ì×´ÎÊý
#endif


//us¼¶ÑÓÊ±Ê±,¹Ø±ÕÈÎÎñµ÷¶È(·ÀÖ¹´ò¶Ïus¼¶ÑÓ³Ù)
void delay_osschedlock(void)
{
#ifdef CPU_CFG_CRITICAL_METHOD   				//Ê¹ÓÃUCOSIII
	OS_ERR err; 
	OSSchedLock(&err);							//UCOSIIIµÄ·½Ê½,½ûÖ¹µ÷¶È£¬·ÀÖ¹´ò¶ÏusÑÓÊ±
#else											//·ñÔòUCOSII
	OSSchedLock();								//UCOSIIµÄ·½Ê½,½ûÖ¹µ÷¶È£¬·ÀÖ¹´ò¶ÏusÑÓÊ±
#endif
}

//us¼¶ÑÓÊ±Ê±,»Ö¸´ÈÎÎñµ÷¶È
void delay_osschedunlock(void)
{	
#ifdef CPU_CFG_CRITICAL_METHOD   				//Ê¹ÓÃUCOSIII
	OS_ERR err; 
	OSSchedUnlock(&err);						//UCOSIIIµÄ·½Ê½,»Ö¸´µ÷¶È
#else											//·ñÔòUCOSII
	OSSchedUnlock();							//UCOSIIµÄ·½Ê½,»Ö¸´µ÷¶È
#endif
}

//µ÷ÓÃOS×Ô´øµÄÑÓÊ±º¯ÊýÑÓÊ±
//ticks:ÑÓÊ±µÄ½ÚÅÄÊý
void delay_ostimedly(u32 ticks)
{
#ifdef CPU_CFG_CRITICAL_METHOD
	OS_ERR err; 
	OSTimeDly(ticks,OS_OPT_TIME_PERIODIC,&err);	//UCOSIIIÑÓÊ±²ÉÓÃÖÜÆÚÄ£Ê½
#else
	OSTimeDly(ticks);							//UCOSIIÑÓÊ±
#endif 
}
 
//systickÖÐ¶Ï·þÎñº¯Êý,Ê¹ÓÃucosÊ±ÓÃµ½
void SysTick_Handler(void)
{	
	if(delay_osrunning==1)						//OS¿ªÊ¼ÅÜÁË,²ÅÖ´ÐÐÕý³£µÄµ÷¶È´¦Àí
	{
		OSIntEnter();							//½øÈëÖÐ¶Ï
		OSTimeTick();       					//µ÷ÓÃucosµÄÊ±ÖÓ·þÎñ³ÌÐò               
		OSIntExit();       	 					//´¥·¢ÈÎÎñÇÐ»»ÈíÖÐ¶Ï
	}
}
#endif

			   
//³õÊ¼»¯ÑÓ³Ùº¯Êý
//µ±Ê¹ÓÃOSµÄÊ±ºò,´Ëº¯Êý»á³õÊ¼»¯OSµÄÊ±ÖÓ½ÚÅÄ
//SYSTICKµÄÊ±ÖÓ¹Ì¶¨ÎªHCLKÊ±ÖÓµÄ1/8
//SYSCLK:ÏµÍ³Ê±ÖÓ
void delay_init()
{
#if SYSTEM_SUPPORT_OS  							//Èç¹ûÐèÒªÖ§³ÖOS.
	u32 reload;
#endif
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//Ñ¡ÔñÍâ²¿Ê±ÖÓ  HCLK/8
	fac_us=SystemCoreClock/8000000;				//ÎªÏµÍ³Ê±ÖÓµÄ1/8  
#if SYSTEM_SUPPORT_OS  							//Èç¹ûÐèÒªÖ§³ÖOS.
	reload=SystemCoreClock/8000000;				//Ã¿ÃëÖÓµÄ¼ÆÊý´ÎÊý µ¥Î»ÎªK	   
	reload*=1000000/delay_ostickspersec;		//¸ù¾Ýdelay_ostickspersecÉè¶¨Òç³öÊ±¼ä
												//reloadÎª24Î»¼Ä´æÆ÷,×î´óÖµ:16777216,ÔÚ72MÏÂ,Ô¼ºÏ1.86s×óÓÒ	
	fac_ms=1000/delay_ostickspersec;			//´ú±íOS¿ÉÒÔÑÓÊ±µÄ×îÉÙµ¥Î»	   

	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;   	//¿ªÆôSYSTICKÖÐ¶Ï
	SysTick->LOAD=reload; 						//Ã¿1/delay_ostickspersecÃëÖÐ¶ÏÒ»´Î	
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;   	//¿ªÆôSYSTICK    

#else
	fac_ms=(u16)fac_us*1000;					//·ÇOSÏÂ,´ú±íÃ¿¸ömsÐèÒªµÄsystickÊ±ÖÓÊý   
#endif
}								    

#if SYSTEM_SUPPORT_OS  							//Èç¹ûÐèÒªÖ§³ÖOS.
//ÑÓÊ±nus
//nusÎªÒªÑÓÊ±µÄusÊ?;
)
{
}		    								   
void delay_us(u32 nus)
{		
	u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=SysTick->LOAD;					//LOADµÄÖµ	    	 
	ticks=nus*fac_us; 							//ÐèÒªµÄ½ÚÅÄÊý	  		 
	tcnt=0;
	delay_osschedlock();						//×èÖ¹OSµ÷¶È£¬·ÀÖ¹´ò¶ÏusÑÓÊ±
	told=SysTick->VAL;        					//¸Õ½øÈëÊ±µÄ¼ÆÊýÆ÷Öµ
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;		//ÕâÀï×¢ÒâÒ»ÏÂSYSTICKÊÇÒ»¸öµÝ¼õµÄ¼ÆÊýÆ÷¾Í¿ÉÒÔÁË.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;				//Ê±¼ä³¬¹ý/µÈÓÚÒªÑÓ³ÙµÄÊ±¼ä,ÔòÍË³ö.
		}  
	};
	delay_osschedunlock();						//»Ö¸´OSµ÷¶È									    
}
//ÑÓÊ±nms
//nms:ÒªÑÓÊ±µÄmsÊý
void delay_ms(u16 nms)
{	
	if(delay_osrunning&&delay_osintnesting==0)	//Èç¹ûOSÒÑ¾­ÔÚÅÜÁË,²¢ÇÒ²»ÊÇÔÚÖÐ¶ÏÀïÃæ(ÖÐ¶ÏÀïÃæ²»ÄÜÈÎÎñµ÷¶È)	    
	{		 
		if(nms>=fac_ms)							//ÑÓÊ±µÄÊ±¼ä´óÓÚOSµÄ×îÉÙÊ±¼äÖÜÆÚ 
		{ 
   			delay_ostimedly(nms/fac_ms);		//OSÑÓÊ±
		}
		nms%=fac_ms;							//OSÒÑ¾­ÎÞ·¨Ìá¹©ÕâÃ´Ð¡µÄÑÓÊ±ÁË,²ÉÓÃÆÕÍ¨·½Ê½ÑÓÊ±    
	}
	delay_us((u32)(nms*1000));					//ÆÕÍ¨·½Ê½ÑÓÊ±  
}
#else //²»ÓÃOSÊ±
//ÑÓÊ±nus
//nusÎªÒªÑÓÊ±µÄusÊý.		    								   
void delay_us(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us; 					//Ê±¼ä¼ÓÔØ	  		 
	SysTick->VAL=0x00;        					//Çå¿Õ¼ÆÊýÆ÷
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//¿ªÊ¼µ¹Êý	  
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));		//µÈ´ýÊ±¼äµ½´ï   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//¹Ø±Õ¼ÆÊýÆ÷
	SysTick->VAL =0X00;      					 //Çå¿Õ¼ÆÊýÆ÷	 
}
//ÑÓÊ±nms
//×¢ÒânmsµÄ·¶Î§
//SysTick->LOADÎª24Î»¼Ä´æÆ÷,ËùÒÔ,×î´óÑÓÊ±Îª:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLKµ¥Î»ÎªHz,nmsµ¥Î»Îªms
//¶Ô72MÌõ¼þÏÂ,nms<=1864 
void delay_ms(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(u32)nms*fac_ms;				//Ê±¼ä¼ÓÔØ(SysTick->LOADÎª24bit)
	SysTick->VAL =0x00;							//Çå¿Õ¼ÆÊýÆ÷
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//¿ªÊ¼µ¹Êý  
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));		//µÈ´ýÊ±¼äµ½´ï   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//¹Ø±Õ¼ÆÊýÆ÷
	SysTick->VAL =0X00;       					//Çå¿Õ¼ÆÊýÆ÷	  	    
} 
#endif 








































