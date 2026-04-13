#include "bsp.h"


//�ƴ�����
void LED_Strip_Ctrl(void)
{
	  if(discharge_f)
		{
        if((led_strip_open_f)&&(!device_rest_f))
        {
		        LED_TAPE_ON();
        }
        else
        {
		        LED_TAPE_OFF();   
        }
	  }
		else
		{
		    LED_TAPE_OFF();
		}
}



//�����ӿ���
void Plasma_Ctrl(void)
{
	  if(discharge_f)
		{
        if((plasma_open_f)&&(!device_rest_f))
		    {
		        PLASMA_ON();
		    }
		    else
		    {
		        PLASMA_OFF();
		    }
    }
		else
		{
		    PLASMA_OFF();
		}
}



//���ȿ���
/**
*@brief : in 100ms processing .

**/
void Fan_Ctrl_Process(void)
{
    if(discharge_f){
		if((fan_open_f)&&(!device_rest_f)){
			if(fan_speed_level==1 || fan_speed_level < 34)
			{
			fan_on(287);
			}
			else if(fan_speed_level==2 || (fan_speed_level > 33 && fan_speed_level < 67))
			{
			fan_on(303);
			}
			else if(fan_speed_level==3)
			{
			fan_on(319);
			}

			__NOP();__NOP();__NOP();__NOP();__NOP();

			FAN_RUN_ON();
		}
        else
			{
			if(fan_delay_time_off!=0){
				fan_delay_time_off--;

				if(fan_speed_level==1)
				{
				fan_on(287);
				}   
				else if(fan_speed_level==2)
				{
				fan_on(303);
				}
				else if(fan_speed_level==3)
				{
				fan_on(319);
				}

				__NOP();__NOP();__NOP();__NOP();__NOP();

				FAN_RUN_ON();
			}
			else
			{
			FAN_RUN_OFF();

			__NOP();__NOP();__NOP();__NOP();__NOP();

			fan_off();
			}
			}					
		}
		else
		{
        	if(fan_delay_time_off!=0)
				{
				    fan_delay_time_off--;
							
						if(fan_speed_level==1)
						{
						    fan_on(287);
						}   
						else if(fan_speed_level==2)
						{
						    fan_on(303);
						}
						else if(fan_speed_level==3)
						{
						    fan_on(319);
						}
						
						__NOP();__NOP();__NOP();__NOP();__NOP();
						
						FAN_RUN_ON();
				}
				else
				{
				    FAN_RUN_OFF();
			
			      __NOP();__NOP();__NOP();__NOP();__NOP();
			
		        fan_off();
				}
		}
}



//�������������
void Beep(Beep_TypeDef music)
{
    switch (music)
	  {
		    case BEEP_ONCE:
			     beep_times=1;
			     beep_lenght=2; 
			     non_beep_length=0;
			     break;
		
		    case BEEP_TWO:
			     beep_times=4;
			     beep_lenght=BEEP_LENGTH_DEFAULT; 
			     non_beep_length=NON_BEEP_LENGTH_DEFAULT;
			     break;			
		
				case BEEP_THREE:
			     beep_times=6;
			     beep_lenght=2; 
			     non_beep_length=NON_BEEP_LENGTH_DEFAULT;
			     break;
				
		    case BEEP_1SECONDS:
			     beep_times=1;
			     beep_lenght=100; 			//10ms*100=1seconds
			     non_beep_length=0;
			     break;	
		
		    case BEEP_TIME_OVER:
			     beep_times=15;
			     beep_lenght=200; 			//10ms*200=2seconds
			     non_beep_length=0;
			     break;			
		
		    default:
			     beep_times=0;
			     beep_lenght=0; 
			     non_beep_length=0;	
           break;		
    }
} 





//����������
void Ultra_Sound_Ctrl(void)
{
    if(discharge_f)
    {
		    if((Ultra_Sound_open_f)&&(!device_rest_f))
				{
				    ultra_sound_on(159);     
				}
				else
				{
				    ultra_sound_off();
				}
		}
    else
    {
		    ultra_sound_off();
		}			
}



/**
*
*@brief 
*@notice
*@param
*
**/
void Relay_Ctrl(void)
{
    if(discharge_f)
		{
		    if((PTC_heat_open_f)&&(!device_rest_f)&& ptc_prohibit_off_f == 0)
				{
                    LED_PTC_ON();
					RELAY_ON();
					
				}
				else
				{
					LED_PTC_OFF();
					RELAY_OFF();
					
				}
		}
		else
		{
		    RELAY_OFF();
			LED_PTC_OFF();
		}
}	



/**
	*
	*@brief environment temperature value compare set temperater value
	*@notice
	*@param
	*
**/
void Heat_Process(void)
{
      static uint8_t default_init = 0xff;   // 第一次比较标志
     if(discharge_f == 1){
	   if(ptc_prohibit_off_f == 1 || set_temperature_value_f ==1) return ;

	  uint8_t target_temp;

	  target_temp = setting_temperature;

	  if(temperature > 39){

        PTC_heat_open_f = 0;   // 立即关闭
	    first_temp_compare_f = 1; 
	    if(default_init != PTC_heat_open_f ){
					default_init= PTC_heat_open_f;
				SendWifiData_To_Cmd(0x02,0);
		        delay_ms(100);//HAL_Delay(5);
		        MqttData_Publish_SetPtc(0);

				}
	  
	     return ;

	  }

      // -----------------------------
    // 2. 第一次比较：必须立即决定 PTC 开关
    // -----------------------------
	  if(first_temp_compare_f == 0){

		if(temperature >= target_temp){
            PTC_heat_open_f = 0;   // 立即关闭

		       if(default_init != PTC_heat_open_f ){
					default_init = PTC_heat_open_f;
				SendWifiData_To_Cmd(0x02,0);
		        delay_ms(100);//HAL_Delay(5);
		        MqttData_Publish_SetPtc(0);

				}
		}
        else{
            PTC_heat_open_f = 1;   // 立即打开
            first_temp_compare_f = 1;         // 以后进入滞后控制
            if(default_init!= PTC_heat_open_f ){
					default_init = PTC_heat_open_f;
				SendWifiData_To_Cmd(0x02,0x01);
		        delay_ms(100);//HAL_Delay(5);
		        MqttData_Publish_SetPtc(0x01);

			}
        }
        return;


	  }

		// -----------------------------
		// 3. 第二次及以后：使用 -2°C 滞后控制
		// -----------------------------
		if(first_temp_compare_f == 1)
		{
			// 当前是开启状态 → 高于设定温度则关闭
			if(temperature >= target_temp){
					PTC_heat_open_f = 0;
				if(default_init != PTC_heat_open_f ){
					default_init = PTC_heat_open_f;
				SendWifiData_To_Cmd(0x02,0);
		        delay_ms(100);//HAL_Delay(5);
		        MqttData_Publish_SetPtc(0);

				}
			}
			else
			{
				// 当前是关闭状态 → 低于设定温度 - 2 才重新打开
				if(temperature <  (target_temp - 2))
				PTC_heat_open_f = 1;
				if(default_init!= PTC_heat_open_f ){
					default_init = PTC_heat_open_f;
				SendWifiData_To_Cmd(0x02,0x01);
		        delay_ms(100);//HAL_Delay(5);
		        MqttData_Publish_SetPtc(0x01);

				}
			}
		}

       }

}

void set_temp_compare(void)
{
   if(discharge_f == 1 && set_temperature_value_f ==1 && time_1s_set_temp_f > 2){
	    set_temperature_value_f ++;

    if(temperature >= setting_temperature){
	     ptc_prohibit_off_f = 0;
	     PTC_heat_open_f = 0;   // 立即关闭
	     SendWifiData_To_Cmd(0x02,0);
		 delay_ms(100);//HAL_Delay(5);
		 MqttData_Publish_SetPtc(0);

    }
	else{
	    ptc_prohibit_off_f = 0;
		PTC_heat_open_f = 1;   // 立即open
		SendWifiData_To_Cmd(0x02,0x01);
		delay_ms(100);//HAL_Delay(5);
		MqttData_Publish_SetPtc(1);


	}


	}

}



/**
*
*@brief environment temperature value compare set temperater value
*@notice
*@param
*
**/
void Fan_Current_Det(void)
{
	if((discharge_f)&&(fan_open_f)&&(!device_rest_f))
	{
		if(fan_current<_NO_FAN_LOAD_CURRENT){
			fan_current_det_time++;
			if(fan_current_det_time>=300){
				fan_current_det_time = 0;

				if(!no_fan_load_f)
				{
					Beep(BEEP_THREE);
					beep_interval_time = 0;

					fan_open_f = 0;
				}

				no_fan_load_f = 1;
			}
		}
		else{
		fan_current_det_time = 0;
		}
	}
	else
	{
		fan_current_det_time = 0;
	}		   
}




#if 0
	
	   if(temperature>= target_temp)
		{
		    temperature_det_less_time = 0;
			
		    if(temperature>=40)
		    {
		        less_38C_det_time = 0;
			
		        over_40C_det_time++;
			      if(over_40C_det_time>=400)
				    {
				        over_40C_det_time = 0;
					
					      over_40C_f = 1;
				    }
		    }
		    else
		    {
		        over_40C_det_time = 0;
			
		        if(temperature<=38)
				    {
				        less_38C_det_time++;
					      if(less_38C_det_time>=400)
						    {
						        less_38C_det_time = 0;
							
							      over_40C_f = 0;
						    }
				    }
				    else
				    {
				        less_38C_det_time = 0;
				    }
		    }
		}
		else
		{
			temperature_det_more_time = 0;

			temperature_det_less_time++;
			if(temperature_det_less_time>=30)
			{
			temperature_det_less_time = 0;

			if(over_40C_f)
			{
				over_40C_f = 0;
				over_40C_det_time = 0;
			}
			}					
		}
		
		if((discharge_f)&&(!over_40C_f)&&(!no_fan_load_f))
		{
			if(temperature>=setting_temperature)
			{
			lower_SetTemperature_time = 0;

			reach_SetTemperature_time++;
			if(reach_SetTemperature_time>=50)
			{
			reach_SetTemperature_time = 0;

			PTC_heat_open_f = 0;
			//fan_open_f = 0;
			}
			}
			else
			{
			reach_SetTemperature_time = 0;

			lower_SetTemperature_time++;
			if(lower_SetTemperature_time>=50)
			{
			lower_SetTemperature_time = 0;

			PTC_heat_open_f = 1;
			fan_open_f = 1;
			}
			}					
		}
		else
		{
		    PTC_heat_open_f = 0;
			  //fan_open_f = 0;
		}
}

#endif 



