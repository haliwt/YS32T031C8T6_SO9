/* USER CODE BEGIN header */
/**
  ******************************************************************************
  * @file    main.c  
  * @author  YSPRING Application Team
  * @version 1.0.0
  * @date    2023.3.20
  * @brief   Main program
  ******************************************************************************
  * @attention
  ******************************************************************************
  */
/* USER CODE END header */
#include "ys32t031.h"
#include "main.h"
#include "delay.h"
#include "system_init.h"
#include "uart.h"  
#include "tim.h"
#include "iwdg.h"
#include "key.h"
#include "adc.h"
#include "user.h"
#include "dht11.h"
#include "ys32t031_tsc.h"
#include "ys32t031_tsc_lib.h"
#include "ys32t031_tsc_config.h"


/******************************************************
函数名：main
功能：主函数入口
参数：无
返回值：int
*******************************************************/
int main(void)
{
    RCC_Configuration();           //系统时钟配置   
	
    GPIO_Configuration();          //IO口配置
	
    Clear_Ram();                   //变量初始化
	
    UART1_Configuration(19520);    //串口1 用于和外接显示板通信
	
	  UART2_Configuration(9600);     //串口2 用于和WIFI模组通信
	
    TIM1_Configuration();          //TIM1-PWM输出配置
	
    TIM3_Configuration();          //TIM3-PWM输出配置
	
    TIM6_Configuration();          //TIM6基本定时配置
	
    TIM14_Configuration();         //TIM14-PWM输出配置
	
    IWDG_Configuration();          //独立看门狗配置
	
	  TSC_Lib_Init();                //触摸初始化
	
    ADC_Configuration();           //ADC配置
		
		NVIC_Configuration();          //中断嵌套向量配置
		
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
		TSC_StartCmd(ENABLE);          //开始扫描
		
		Beep(BEEP_ONCE);	
		
		
    while(1)
    {
        IWDG->KR = 0xAAAA;
			
			  if(TSC_GetFlagStatus(TSC_Flag_TimeSlot) == SET)
				{
				    TSC_ClearFlagStatus(TSC_Flag_TimeSlot);
					  TSC_StartCmd(DISABLE); //停止扫描
					  TSC_Handle();          //触摸处理
					  TSC_StartCmd(ENABLE);  //开始扫描;  tk enable
				}
			
			  if(time_5ms_f)
				{
				    time_5ms_f = 0;
					
					  Read_DHT11_Data();  
					  
					  Adc_Channel_Sample();
				}
		
		    if(time_10ms_f)
				{
				    time_10ms_f = 0;
					
				    Real_Time();
					
					  AD_Filter();
					
					  Key_Scan();
					  
					  LED_Strip_Ctrl();      
					
					  Plasma_Ctrl();
					
					  Fan_Ctrl_Process();
					
					  Ultra_Sound_Ctrl();  

            Relay_Ctrl();		

            Heat_Process();	
					
					  Fan_Current_Det();  
						
						Update_LED_Display();
						
						Task_beep_called_100ms();
				}
				
				if(time_100ms_f)
				{
				    time_100ms_f = 0;
				}
				
				if(time_1s_f)
				{
				    time_1s_f = 0;
				}
    }
}


