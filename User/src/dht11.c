/*
  ******************************************************************************
  * Copyright (c) 2024 Yspring.
  * All rights reserved..
  * @file    dht11.C
  * @author  Yspring Firmware Team  
  * @brief   dht11 Source Code.
  ******************************************************************************      
*/

#include "dht11.h"   
#include "ys32t031.h"
#include "delay.h"
#include "bsp.h"


uint8_t Data_Read_Finish_f;
uint16_t data_read_interval;
uint16_t humidity;
uint16_t temperature;


void SET_DHT11_DATA_IN_MODE(void);
void SET_DHT11_DATA_OUT_MODE(void);
static uint8_t DHT11_Read_Byte(void);
#if 0
static void Delay_US(uint32_t us) 
{
    uint32_t count = us * 8;//16//32 // ?? 48MHz ????,???????
    while(count--) {
        __NOP();
		__NOP();
	    __NOP();
		__NOP();
		 __NOP();
		__NOP();
	    __NOP();
		__NOP();
	
    }
}
#endif 


//DHT11Êı¾İ¿ÚÅäÎªÊäÈëÄ£Ê½
void SET_DHT11_DATA_IN_MODE(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_StructInit(&GPIO_InitStructure);

    RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_GPIOA, ENABLE);	
	
    GPIO_InitStructure.GPIO_Pin = DHT11_DATA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(DHT11_DATA_GPIO_PORT, &GPIO_InitStructure);
}



//DHT11Êı¾İ¿ÚÅäÎªÊä³öÄ£Ê½
void SET_DHT11_DATA_OUT_MODE(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_StructInit(&GPIO_InitStructure);	
	
	  RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_GPIOA, ENABLE);	
	
    GPIO_InitStructure.GPIO_Pin = DHT11_DATA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Low;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(DHT11_DATA_GPIO_PORT, &GPIO_InitStructure);
}



/***********************************************************************
º¯ÊıÃû£ºstatic u8 DHT11_Read_Byte(void)
²ÎÊıËµÃ÷£ºÎŞ
·µ»ØÖµ£º·µ»Ø´ÓDHT11ÉÏ¶ÁÈ¡µÄÒ»¸öbyteÊı¾İ
º¯Êı×÷ÓÃ£º´ÓDHT11ÉÏ¶ÁÈ¡Ò»¸öbyteÊı¾İ
************************************************************************/
static uint8_t DHT11_Read_Byte(void)    
{        
    uint8_t i,dat=0;
	
    for(i=0;i<8;i++) 
	  {
		    while(GPIO_ReadInputDataBit(DHT11_DATA_GPIO_PORT,DHT11_DATA_PIN)==0)
        {
				
				}					
			  
				DelayUS(40);
			
			  if(GPIO_ReadInputDataBit(DHT11_DATA_GPIO_PORT,DHT11_DATA_PIN)==1)
				{
				    while(GPIO_ReadInputDataBit(DHT11_DATA_GPIO_PORT,DHT11_DATA_PIN)==1)
				    {
				
				    }	
					
				    dat|=(uint8_t)(1 << (7 - i)); // Êı¾İ1
				}
				else
				{
				    dat&=(uint8_t)~(1 << (7 - i)); // Êı¾İ0
				}
    }	
	
    return dat;
}


#if 0
//´ÓDHT11ÉÏ¶ÁÈ¡ÎÂÊª¶ÈÊı¾İ
void Read_DHT11_Data(void)
{
	  uint8_t dht11_data_buf[5];
	  static uint8_t start_delay_time=4;
	
	  dht11_data_buf[0]=0;
	  dht11_data_buf[1]=0;
	  dht11_data_buf[2]=0;
	  dht11_data_buf[3]=0;
	  dht11_data_buf[4]=0;
	
    if(Data_Read_Finish_f)                                     //Í¨ĞÅ¼ä¸ô£º5ms*400=2000ms
    {
			  data_read_interval++;
			  if(data_read_interval>=399)
				{
				    data_read_interval = 0;
					  Data_Read_Finish_f = 0;
				}
		    return;
		}    	
	
	  //Ö÷»ú·¢ËÍ¿ªÊ¼ĞÅºÅµ½DHT11¼´µ¥Æ¬»úÀ­µÍ18msÖ®ºóÔÙÀ­¸ß20-40us
		if(start_delay_time==4)
		{
        SET_DHT11_DATA_OUT_MODE();							               //ÅäÖÃ³ÉÊä³öÄ£Ê½
		}
    GPIO_ResetBits(DHT11_DATA_GPIO_PORT,DHT11_DATA_PIN);       //À­µÍÊı¾İÏß
		
		if(start_delay_time>0)                                     //Êı¾İÏßÀ­µÍ20ms
		{
		    start_delay_time--;
			  return;
		}
		else
		{
		    start_delay_time = 4;
		}
		
    GPIO_SetBits(DHT11_DATA_GPIO_PORT,DHT11_DATA_PIN); 	       //À­¸ßÊı¾İÏß 
	  DelayUS(30);     						                               //Êı¾İÏßÀ­¸ß20~40us
	  GPIO_ResetBits(DHT11_DATA_GPIO_PORT,DHT11_DATA_PIN);
		
		SET_DHT11_DATA_IN_MODE();                                  //ÅäÖÃ³ÉÊäÈëÄ£Ê½
		
		uint8_t reply_cnt=0;
		while((GPIO_ReadInputDataBit(DHT11_DATA_GPIO_PORT,DHT11_DATA_PIN)==0)&&(reply_cnt<120))  
		{
			  DelayUS(1);
		    reply_cnt++;
		}
		if(reply_cnt>=120) 
    { 
		    Data_Read_Finish_f = 1;
		    data_read_interval = 0;
		   
		    return;
		}   
	  
		reply_cnt=0;
		while((GPIO_ReadInputDataBit(DHT11_DATA_GPIO_PORT,DHT11_DATA_PIN)==1)&&(reply_cnt<120))  
		{
			  DelayUS(1);
		    reply_cnt++;
		}
		if(reply_cnt>=120) 
    {
		    Data_Read_Finish_f = 1;
		    data_read_interval = 0;
		   
		    return;
		}
		
		dht11_data_buf[0] = DHT11_Read_Byte();  //Êª¶ÈÕûÊı   
		dht11_data_buf[1] = DHT11_Read_Byte();  //Êª¶ÈĞ¡Êı£¨DHT11²»Ìá¹©Ğ¡Êı²¿·Ö£¬¿ÉÒÔºöÂÔ£©   
		dht11_data_buf[2] = DHT11_Read_Byte();  //ÎÂ¶ÈÕûÊı
		dht11_data_buf[3] = DHT11_Read_Byte();  //ÎÂ¶ÈĞ¡Êı£¨DHT11²»Ìá¹©Ğ¡Êı²¿·Ö£¬¿ÉÒÔºöÂÔ£©
		dht11_data_buf[4] = DHT11_Read_Byte();  //Ğ£ÑéºÍ
		
		if(((uint8_t)(dht11_data_buf[0]+dht11_data_buf[1]+dht11_data_buf[2]+dht11_data_buf[3]))==dht11_data_buf[4])
		{
		    humidity = dht11_data_buf[0];
        temperature = dht11_data_buf[2]; 			
		}
		
		Data_Read_Finish_f = 1;
		data_read_interval = 0;
}

#endif 




uint8_t dht11_data_buf[5] ;

//ï¿½ï¿½DHT11ï¿½Ï¶ï¿½È¡ï¿½ï¿½Êªï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
uint8_t  Read_DHT11_Data(void)
{
	volatile uint32_t timeout = 0;
    uint8_t i, temp_val;
  

    // 1. å‘é€èµ·å§‹ä¿¡å·
    SET_DHT11_DATA_OUT_MODE();
    GPIO_ResetBits(DHT11_DATA_GPIO_PORT, DHT11_DATA_PIN);
     Delay_US(20000);; // 20ms-30ms
    
    GPIO_SetBits(DHT11_DATA_GPIO_PORT, DHT11_DATA_PIN);
    Delay_US(30);
    SET_DHT11_DATA_IN_MODE();

    // 2. æ£€æµ‹å“åº” (å¸¦è¶…æ—¶)
    timeout = 0;
    if(GPIO_ReadInputDataBit(DHT11_DATA_GPIO_PORT, DHT11_DATA_PIN) == 0) {
        
  
    timeout = 0;
    while(GPIO_ReadInputDataBit(DHT11_DATA_GPIO_PORT, DHT11_DATA_PIN) == 0) {
       // if(++timeout > 10000) return 2; // é”™è¯¯2ï¼šå“åº”ä½ç”µå¹³ä¸ç»“æŸ
    }
    timeout = 0;
    while(GPIO_ReadInputDataBit(DHT11_DATA_GPIO_PORT, DHT11_DATA_PIN) == 1) {
        //if(++timeout > 10000) return 3; // é”™è¯¯3ï¼šå“åº”é«˜ç”µå¹³ä¸ç»“æŸ
    }

    // 3. é”å®šä¸­æ–­ï¼Œè¯»å–40ä½æ•°æ®ï¼ˆå…³é”®æ—¶åºåŒºï¼‰
   
   
    for(i = 0; i < 5; i++) {
        dht11_data_buf[i] = DHT11_Read_Byte(); 
    }

	


    // 4. æ ¡éªŒæ•°æ®
    temp_val = dht11_data_buf[0] + dht11_data_buf[1] + dht11_data_buf[2] + 
	dht11_data_buf[3];
    if(dht11_data_buf[4] == temp_val && temp_val != 0) {
        humidity = dht11_data_buf[0];
        temperature = dht11_data_buf[2];
       
        return 0; // æˆåŠŸ
    }
    }
	
    return 4; // æ ¡éªŒå¤±è´¥
}








uint8_t dht11_read_temp_humidity_value(void)
{
    uint8_t dht11_read_flag;
	
	dht11_read_flag = Read_DHT11_Data();//DHT11_Read(humidity,temperature);

	if(dht11_read_flag==0){
      
	 // humidity = dht11_data_buf[0];
	 // temperature = dht11_data_buf[2];
	   LED_PLASMA_ON();
       return 0;
	}
	else{

       LED_PLASMA_OFF();
	 
	 
	  
	   return 1;
		
	}


}




