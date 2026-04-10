#include "bsp.h"


uint8_t time_200ms_flag;


/**
*@brief:  totall task
*@param:
#@notice

**/
void task_scheduler(void)
{
   uint8_t temp_counter_flag;

	if(time_5ms_f && temp_counter_flag < 200){
		time_5ms_f = 0;
		
	     temp_counter_flag ++;

		 Read_DHT11_Data(); 
		
	}

	if(time_10ms_f){
		

		//Real_Time();

		//AD_Filter();

		Key_Scan();

		LED_Strip_Ctrl();      

	    Relay_Ctrl();		

        Heat_Process();	
	    Update_LED_Display();
		Task_beep_called_100ms();

	
		time_10ms_f = 0;
	}

	if(time_100ms_f)
	{
		//  time_100ms_f = 0;
		Times100msCnt++;
		time_200ms_flag++;
		
		if(time_200ms_flag > 2){
			Read_DHT11_Data(); 
			AD_Filter();
			
		    Adc_Channel_Sample();
			time_200ms_flag =0;
		}

		
		if((Times100msCnt==0)||(Times100msCnt==5)){
		    if(flash_f) {flash_f=0;}
				else {flash_f=1;}
			 
		 }

  
	    Plasma_Ctrl();

		Fan_Ctrl_Process();

		Ultra_Sound_Ctrl();

		Fan_Current_Det();  
		//USART1_Send_DisplayData();  // 外接显示板通信
        //USART2_Send_WiFiData();     // WiFi 模块通信
       
		
		time_100ms_f = 0;
	}
	
	/*timer 1s*/
	if(time_1s_f == 1){
		if(((discharge_f)&&(AI_timing_open_f))){
			if(!device_rest_f){
				if(++Cacl_time_sec>=60){
					Cacl_time_sec = 0;

					timing_min_cnt++;
					if(timing_min_cnt>=60)
					{
					timing_min_cnt = 0;

					timing_hour_cnt++;
					if(timing_hour_cnt>=setting_timing_hour){

					timing_hour_cnt = 0;

					discharge_f = 0;
					AI_timing_open_f = 0;
					PTC_heat_open_f = 0;
					Ultra_Sound_open_f = 0;
					led_strip_open_f = 0;
					plasma_open_f = 0;
					fan_open_f = 0;

					Is_time_setting_f = 0;
					Is_temp_setting_f = 0;
					Is_timing_hour_disp_f = 0;

					timing_min_cnt = 0;
					timing_hour_cnt = 0;	

					no_fan_load_f = 0;									

					fan_delay_time_off = 6000;

					device_rest_f = 0;
					device_rest_time = 0;
					}
					}
				}
			}
		}
		else
		{
			timing_min_cnt = 0;
		}

		if(key_net_config_f)
		{
				key_net_config_time++;
				if(key_net_config_time>=130)
				{
				key_net_config_time = 0;

				key_net_config_f = 0;
				}
		}
		else
		{
			key_net_config_time = 0;
		}
		time_1s_f = 0;
	//}//1s endf 
	}


	 if(time_1minute_f==1){//1s * 60 =60s = 1 minute

	     switch(discharge_f){

		  case 1: //power on
   
			if(device_rest_f)
			{
				device_rest_time++;
				if(device_rest_time>=10){
					device_rest_time = 0;
					device_rest_f = 0;

					work_time = 0;
				}
			}
			else{
				work_time++;
				if(work_time>=120){ // 2 hours
					work_time = 0;

					device_rest_f = 1;
					device_rest_time = 0;

					fan_delay_time_off = 600;
			   }
			}
		

		break;

		case 0: //power off
		



		
		break;
	    }
		time_1minute_f = 0;
	}

	

	
}


// 定义一个全局节拍计数器（在中断中累加）


/**
*@brief:  totall task
*@param:
#@notice

**/


