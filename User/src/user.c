/*
  ******************************************************************************
  * Copyright (c) 2024 Yspring.
  * All rights reserved..
  * @file    user.C
  * @author  Yspring Firmware Team  
  * @brief   user Source Code.
  ******************************************************************************      
*/

#include "user.h"
#include "delay.h"
#include "ys32t031.h"
#include "ys32t031_it.h"
#include <stdint.h>
#include "dht11.h"
#include "adc.h"
#include "key.h"
#include "display.h"
#include "system_init.h"
#include "tim.h"
#include "tm1639.h"
#include "ys32t031_tsc_lib.h"


volatile uint8_t Times5msCnt;
uint8_t Times10msCnt;
uint8_t Times100msCnt;
uint8_t Times1sCnt;
uint16_t Times1minCnt;
uint8_t Cacl_time_sec;

volatile uint8_t time_5ms_f;
volatile uint8_t time_10ms_f;
uint8_t time_100ms_f;
uint8_t time_1s_f;

uint16_t ad_value[1];
uint16_t fan_current;

uint8_t key_worked_f;
uint8_t key_long_f;
uint16_t key_data;
uint16_t key_time;

uint8_t discharge_f;
uint16_t work_time;

uint16_t current_temperature;
uint16_t setting_temperature;
uint16_t disp_temperature;
uint16_t disp_timing_time;
uint16_t disp_humidity;

uint8_t AI_timing_open_f;
uint8_t PTC_heat_open_f;
uint8_t Ultra_Sound_open_f;
uint8_t plasma_open_f;

uint16_t timing_is_reach_disptime;

uint16_t setting_timing_hour;

uint8_t Is_time_setting_f;
uint8_t Is_temp_setting_f;
uint8_t Is_timing_hour_disp_f;

uint16_t timing_hour_disp_time;
uint16_t led_scan_time;

uint16_t key_flash_time;
uint8_t key_net_config_f;
uint16_t key_net_config_time;
uint8_t led_strip_open_f;

uint8_t flash_f;

uint8_t device_rest_f;
uint16_t device_rest_time;

uint8_t timing_min_cnt;
uint8_t timing_hour_cnt;

uint8_t fan_open_f;
uint8_t fan_speed_level;
uint16_t fan_delay_time_off;

volatile uint8_t beep_times;				//次数
volatile uint8_t beep_lenght;			  //响的长度 *100ms
volatile uint8_t non_beep_length;		//间隔时间
uint16_t beep_interval_time;

uint16_t over_40C_det_time;
uint16_t less_38C_det_time;
uint8_t over_40C_f;

uint16_t reach_SetTemperature_time;
uint16_t lower_SetTemperature_time;

uint16_t temperature_det_more_time;
uint16_t temperature_det_less_time;

uint16_t fan_current_det_time;
uint8_t no_fan_load_f;

uint16_t disp_switch_time;

uint8_t wifi_connected_f;

uint8_t com_data_temp[8];
uint8_t com_data_buf[16];


const uint8_t LED_TAB[11]={ 
    _SMA|_SMB|_SMC|_SMD|_SME|_SMF,        //0
    _SMB|_SMC,                            //1
    _SMA|_SMB|_SMD|_SME|_SMG,             //2
    _SMA|_SMB|_SMC|_SMD|_SMG,             //3
    _SMB|_SMC|_SMF|_SMG,                  //4
    _SMA|_SMC|_SMD|_SMF|_SMG,             //5
    _SMA|_SMC|_SMD|_SME|_SMF|_SMG,        //6
    _SMA|_SMB|_SMC,                       //7
    _SMA|_SMB|_SMC|_SMD|_SME|_SMF|_SMG,   //8
    _SMA|_SMB|_SMC|_SMD|_SMF|_SMG,        //9
    0,                                    //清屏
};





void Clear_Ram(void);
void Real_Time(void);
void Adc_Channel_Sample(void);
void AD_Filter(void);
void Key_Scan(void);
void LED_Strip_Ctrl(void);
void Plasma_Ctrl(void);
void Fan_Ctrl_Process(void);
void Beep(Beep_TypeDef music);   
void Task_beep_called_100ms(void);
void Ultra_Sound_Ctrl(void);
void Relay_Ctrl(void);
void Heat_Process(void);
void Update_LED_Display(void);



//变量初始化
void Clear_Ram(void)
{
    time_5ms_f = 0;
	  time_10ms_f = 0;
	  time_100ms_f = 0;
	  time_1s_f = 0;
	
	  Times5msCnt = 0;
	  Times10msCnt = 0;
	  Times100msCnt = 0;
	  Times1sCnt = 0;
	  Times1minCnt = 0;
	  Cacl_time_sec = 0;
	
	  Data_Read_Finish_f = 1;
	  data_read_interval = 0;
	
	  key_worked_f = 0;
	  key_long_f = 0;
	  key_data = 0;
	  key_time = 0;
	
	  discharge_f = 0;
		work_time = 0;
		
		device_rest_f = 0;
		device_rest_time = 0;
		
		fan_speed_level = 3;
		fan_open_f = 0;
		fan_delay_time_off = 0;
		
		AI_timing_open_f = 0;
		PTC_heat_open_f = 0;
		Ultra_Sound_open_f = 0;
		plasma_open_f = 0;
		led_strip_open_f = 0;
		
		timing_is_reach_disptime = 0;
		
		Is_time_setting_f = 0;
		Is_temp_setting_f = 0;
		Is_timing_hour_disp_f = 0;
		
		key_flash_time = 0;
		flash_f = 0;
		led_scan_time = 0;
		
		timing_min_cnt = 0;
		timing_hour_cnt = 0;
		
		over_40C_f = 0;
		over_40C_det_time = 0;
		less_38C_det_time = 0;
		
		reach_SetTemperature_time = 0;
		lower_SetTemperature_time = 0;
		
		temperature_det_more_time = 0;
		temperature_det_less_time = 0;
		
		no_fan_load_f = 0;
		fan_current_det_time = 0;
		
		disp_switch_time = 0;
		beep_interval_time = 0;
		
		wifi_connected_f = 0;
		
		com_data_temp[0]=0;
	  com_data_temp[1]=0;
	  com_data_temp[2]=0;
	  com_data_temp[3]=0;
		com_data_temp[4]=0;
	  com_data_temp[5]=0;
	  com_data_temp[6]=0;
	  com_data_temp[7]=0;
		
		com_data_buf[0]=0;
	  com_data_buf[1]=0;
	  com_data_buf[2]=0;
	  com_data_buf[3]=0;
		com_data_buf[4]=0;
	  com_data_buf[5]=0;
	  com_data_buf[6]=0;
	  com_data_buf[7]=0;
	  com_data_buf[8]=0;
	  com_data_buf[9]=0;
	  com_data_buf[10]=0;
	  com_data_buf[11]=0;
		com_data_buf[12]=0;
	  com_data_buf[13]=0;
	  com_data_buf[14]=0;
	  com_data_buf[15]=0;
	  //TM1639_Write_Display_Data(com_data_buf,16);
		
}



//时间累加
void Real_Time(void)
{
    Times10msCnt++;
    if(Times10msCnt>=10)
		{
		    Times10msCnt = 0;
			  time_100ms_f = 1;
			
		    Times100msCnt++;
		    if(Times100msCnt>=10)
				{
				    Times100msCnt = 0;
					  time_1s_f = 1;
					
					  Times1sCnt++;
					  if(Times1sCnt>=60)
						{
						    Times1sCnt = 0;
							
						    if(discharge_f)
								{
								    if(device_rest_f)
										{
										    device_rest_time++;
											  if(device_rest_time>=10)
												{
												    device_rest_time = 0;
													  device_rest_f = 0;
													
												    work_time = 0;
												}
										}
										else
										{
										    work_time++;
											  if(work_time>=120)
												{
												    work_time = 0;
													
												    device_rest_f = 1;
												    device_rest_time = 0;
													
													  fan_delay_time_off = 6000;
												}
										}
								}
						}
						
						if(((discharge_f)&&(AI_timing_open_f)))
						{
							  if(!device_rest_f)
								{
						        if(++Cacl_time_sec>=60)
								    {
									      Cacl_time_sec = 0;
									
								        timing_min_cnt++;
									      if(timing_min_cnt>=60)
										    {
										        timing_min_cnt = 0;
											
											      timing_hour_cnt++;
											      if(timing_hour_cnt>=setting_timing_hour)
												    {
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
				}
				
				if((Times100msCnt==0)||(Times100msCnt==5))
				{
				    if(flash_f) {flash_f=0;}
						else {flash_f=1;}
				}
		}
}



//ADC通道采样
void Adc_Channel_Sample(void)
{
    volatile uint16_t ad_temp;
	
    ad_temp = ADC_GetValue(_FCUR_CH,VREFBUF_ADC_VCC);
	
    ad_value[_AD_FCUR] = ad_temp;
}



//AD数据一阶滑动滤波
void AD_Filter(void)
{
    //FAN_CURRENT
	  fan_current=(ad_value[_AD_FCUR]*2+fan_current*18)/20;
}



//按键扫描
void Key_Scan(void)
{
    volatile uint16_t key_i=0;
	
	  if(KEY10_PIN) {key_i=_POWER_KEY_DOWN;}
		else if(KEY9_PIN) {key_i=_MODE_KEY_DOWN;}
		else if(KEY8_PIN) {key_i=_UP_KEY_DOWN;}
	  else if(KEY7_PIN) {key_i=_DOWN_KEY_DOWN;}
		
		if(key_i==0)
		{
		    if((!key_long_f)&&(key_time>=4))
				{
				    if(key_data==_POWER_KEY_DOWN)
            {
						    if(discharge_f)
								{
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

                    Beep(BEEP_ONCE);								
								}
								else
								{
								    discharge_f = 1;
									
									  AI_timing_open_f = 0;
									  PTC_heat_open_f = 1;
									  Ultra_Sound_open_f = 1;
									  plasma_open_f = 1;
									  fan_open_f = 1;
									  led_strip_open_f = 1;
									
									  setting_temperature = 40;
									  setting_timing_hour = 0;
									
									  Is_time_setting_f = 0;
									  Is_temp_setting_f = 0;
									  Is_timing_hour_disp_f = 0;
									
									  timing_min_cnt = 0;
									  timing_hour_cnt = 0;
									
									  no_fan_load_f = 0;
									
								    //Data_Read_Finish_f = 1;
	                  //data_read_interval = 0;
										
										key_net_config_f = 0;
										key_net_config_time = 0;
										
										fan_delay_time_off = 0;
										
										work_time = 0; 
										
										device_rest_f = 0;
										device_rest_time = 0;
										Times1sCnt = 0;
										
										Beep(BEEP_ONCE);
								}
						}
            else if(key_data==_MODE_KEY_DOWN)
            {
						    if((discharge_f)&&(!no_fan_load_f))
                {
								    Is_timing_hour_disp_f = 1;
										Is_time_setting_f=0;
										Is_temp_setting_f=0;
											  
										key_flash_time = 300;
											
										if(setting_timing_hour==0)
										{
										    AI_timing_open_f = 0;
										}
												
										Beep(BEEP_ONCE);									
								}									
						}
            else if(key_data==_UP_KEY_DOWN)
            {
						    if((discharge_f)&&(!no_fan_load_f))
								{
								    if(Is_time_setting_f)
										{
											  if(setting_timing_hour<24)
												{
														setting_timing_hour++;
												}
														
												AI_timing_open_f = 1;
												key_flash_time = 300;
												
												timing_min_cnt = 0;
												timing_hour_cnt = 0;
												Cacl_time_sec = 0;
										}
										else if(Is_temp_setting_f)
										{
                        if(setting_temperature<40)
                        {
												    setting_temperature++;
												}	

                        key_flash_time = 300;											
										}
										else
										{
										    Is_temp_setting_f = 1;
										    Is_time_setting_f=0;
										    Is_timing_hour_disp_f=0;
										    key_flash_time = 300;
										}
										
										Beep(BEEP_ONCE);
								}
						}
            else if(key_data==_DOWN_KEY_DOWN)
            {
						    if((discharge_f)&&(!no_fan_load_f))
								{
								    if(Is_time_setting_f)
										{
										    AI_timing_open_f = 1;	
											
										    if(setting_timing_hour>0)
												{
												    setting_timing_hour--;
													
													  if(setting_timing_hour==0)
														{
														    AI_timing_open_f = 0;
														}
												}
												else
												{
												    AI_timing_open_f = 0;
												}
												
												timing_min_cnt = 0;
												timing_hour_cnt = 0;
												Cacl_time_sec = 0;
												
												key_flash_time = 300;
										}
										else if(Is_temp_setting_f)
										{
										    if(setting_temperature>20)
												{
												    setting_temperature--;
												}
												
												key_flash_time = 300;
										}
										else
										{
										    Is_temp_setting_f = 1;
											  Is_time_setting_f=0;
												Is_timing_hour_disp_f=0;
												key_flash_time = 300;	
										}
										
										Beep(BEEP_ONCE);
								}
						}							
				}
				
				key_time = 0;
				key_data = 0;
				key_worked_f = 0;
				key_long_f = 0;
		}
		else if(key_i!=key_data)
		{
		    key_data = key_i;
			  key_time = 0;
			  key_worked_f = 0;
			  key_long_f = 0;
		}
		else
		{
		    if(key_time<0xfff) {key_time++;}
				if(key_time<4) {return;}
			
			  if(key_data==_POWER_KEY_DOWN)
				{
				    if(key_time>=250)
						{
						    if(!key_long_f)
								{
								    key_long_f = 1;
									
									  if(discharge_f)
										{
								        if(!key_net_config_f)
										    {
										        key_net_config_f = 1;
                            key_net_config_time = 0;

                            Beep(BEEP_ONCE);													
										    }
									  }
								}
						}
				}
				else if(key_data==_MODE_KEY_DOWN)
				{
				    if(key_time>=200)
            {
						    if(!key_long_f)
								{
								    key_long_f = 1;
									 
									  if((discharge_f)&&(!no_fan_load_f))
										{
										    Is_time_setting_f = 1;
											  Is_temp_setting_f = 0;
												Is_timing_hour_disp_f=0;
											
										    key_flash_time = 300;
													
												Beep(BEEP_ONCE);
									  }
								}
								else
								{
								    if(Is_time_setting_f)
										{
										    key_flash_time = 300;
										}
								}
						}							
				}
				else if(key_data==_DOWN_KEY_DOWN)
				{
				    if(key_time>200)
						{
						    if(!key_long_f)
								{
								    key_long_f = 1;
									
								    if((discharge_f)&&(!no_fan_load_f))
										{
										    if(led_strip_open_f)
                        {
												    led_strip_open_f = 0;
												}
                        else
                        {
												    led_strip_open_f = 1;
												}	

                        Beep(BEEP_ONCE);												
										}
								}
						}   
				}
		}
}	



//灯带控制
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



//等离子控制
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



//风扇控制
void Fan_Ctrl_Process(void)
{
    if(discharge_f)
		{
		    if((fan_open_f)&&(!device_rest_f))
        {
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



//蜂鸣器声音类别
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



/*---------------------------------------------------------------------------------
---函 数 名：-Task_beep_called_100ms
---功    能：- 
---参    数：- 
---返 回 值：-
---调    用：- 在大循环里面100ms调用一次
---前提条件：-
---说    明：- 
---------------------------------------------------------------------------------*/
void Task_beep_called_100ms(void)
{
    if(beep_times)
	  {
		    if(beep_times&0x01)
		    {
			      BEEP_ON();
			
			      if(beep_lenght>1)
			      {
				        beep_lenght--;
			      }
			      else 
			      {
				        if(beep_times>0)
				        {
					          beep_times--;
				        }
			      }		
	      }
		    else 
		    {
			      BEEP_OFF();
			
			      if(non_beep_length>1)
			      {
				        non_beep_length--;
			      }
			      else 
			      {
				        if(beep_times>1)//还有一声
				        {
					          beep_times--;
					          non_beep_length=NON_BEEP_LENGTH_DEFAULT;
					          beep_lenght=2;
				        }
				        else 
				        {
					          beep_times=0;//结束
					          non_beep_length=0;
					          beep_lenght=0;
				        }
			      }
		    }	
		}
		else 
	  {
	      non_beep_length=0;
		    beep_lenght=0;
		    BEEP_OFF();
	  }
} 



//超声波控制
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



//继电器控制
void Relay_Ctrl(void)
{
    if(discharge_f)
		{
		    if((PTC_heat_open_f)&&(!device_rest_f))
				{
				    RELAY_ON();
				}
				else
				{
				    RELAY_OFF();
				}
		}
		else
		{
		    RELAY_OFF();
		}
}	



//加热控制流程
void Heat_Process(void)
{
	  if(Is_temp_setting_f)
		{
		    return;
		}
		
		if(temperature>=setting_temperature)
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



//风扇电流检测
void Fan_Current_Det(void)
{
    if((discharge_f)&&(fan_open_f)&&(!device_rest_f))
    {
		    if(fan_current<_NO_FAN_LOAD_CURRENT)
				{
				    fan_current_det_time++;
					  if(fan_current_det_time>=300)
						{
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
				else
				{
				    fan_current_det_time = 0;
				}
		}
    else
    {
		    fan_current_det_time = 0;
		}			
}



//更新LED显示
void Update_LED_Display(void)
{
	  volatile uint16_t i;
    volatile uint16_t bw_i=0;
    volatile uint16_t sw_i=0;
		volatile uint16_t gw_i=0;
	  volatile uint16_t disp_timing_time_temp;
	  volatile uint16_t timing_diff_value_hour;
	  volatile uint16_t timing_diff_value_min;
		
		for(i=0;i<8;i++)
	  {
		    com_data_temp[i]=0x00;
		}
	
		LED_AI_OFF();LED_PTC_OFF();LED_PLASMA_OFF();LED_MOUSE_OFF();
		LED_WIFI_OFF();LED_POWER_OFF();LED_TEMP_OFF();LED_HUMI_OFF();
		
		if(discharge_f)
		{
		    if(no_fan_load_f)
				{
				    com_data_temp[0] |= Lcdch_E;  //E
					  com_data_temp[1] |= Lcdch_r;  //r
					  com_data_temp[1] |= _DP1;     //DP1
					  com_data_temp[2] |= _DP2;     //DP2
					  com_data_temp[2] |= Lcdch_2;
					
					  if(!AI_timing_open_f) {LED_AI_ON();}
						if(PTC_heat_open_f) {LED_PTC_ON();}
						if(plasma_open_f) {LED_PLASMA_ON();}
						if(Ultra_Sound_open_f) {LED_MOUSE_ON();}
						
						LED_POWER_ON();
					
						if(key_net_config_f)
						{
						    led_scan_time++;
						    if(led_scan_time>=33)
								{
								    led_scan_time = 0;
								}
								
								if(led_scan_time<16)
								{
								    LED_WIFI_ON();
								}
						}
						else
						{
						    if(wifi_connected_f)
								{
								    LED_WIFI_ON();
								}
								else
								{
								    led_scan_time++;
                    if(led_scan_time>=150)
                    {
										    led_scan_time = 0;
										}											
									
									  if(led_scan_time<75)
										{
										    LED_WIFI_ON();
										}
								}
						}
						
						com_data_temp[3] |= _A5|_B5|_CC5|_DD5|_E5|_F5|_G5|_H5;
						com_data_temp[4] |= _A1|_B1|_CC1|_DD1|_E1|_F1|_G1|_H1;
						com_data_temp[5] |= _A2|_B2|_CC2|_DD2|_E2|_F2|_G2|_H2;
						com_data_temp[6] |= _A3|_B3|_CC3|_DD3|_E3|_F3|_G3|_H3;
						com_data_temp[7] |= _A4|_B4|_CC4|_DD4|_E4|_F4|_G4|_H4;
						
				    beep_interval_time++;
					  if(beep_interval_time>=1000)
						{
						    beep_interval_time = 0;
							
						    Beep(BEEP_THREE);
						}
				}
				else
				{
				    if(Is_timing_hour_disp_f)
		        {
		            if(setting_timing_hour==0)
                {
				            disp_timing_time_temp = 0;
							      gw_i = Lcdch_H;
				        }
                else
                {
				            if(setting_timing_hour>timing_hour_cnt)
						        {
						            timing_diff_value_hour = setting_timing_hour-timing_hour_cnt;
									
									      if(timing_diff_value_hour>1)
										    {
										        disp_timing_time_temp = timing_diff_value_hour;
											      gw_i = Lcdch_H;
										    }
										    else
										    {
										        timing_diff_value_min = 60-timing_min_cnt;
                        
                            if(timing_diff_value_min>=60)
                            {
												        disp_timing_time_temp=timing_diff_value_hour;
											          gw_i = Lcdch_H;
												    }
                            else
                            {
												        disp_timing_time_temp = timing_diff_value_min;
                                gw_i = 0;													
												    }													
										    }
						        }
						        else
						        {
						            disp_timing_time_temp = 0; 
                        gw_i = Lcdch_H;									
						        }
				        }	

				        disp_timing_time = disp_timing_time_temp;
						
				        bw_i = LED_TAB[disp_timing_time/10];
					      sw_i = LED_TAB[disp_timing_time%10];
				
				        if(key_flash_time!=0)
				        {
				            key_flash_time--;
					
					          if(key_flash_time==0)
						        {
						            Is_timing_hour_disp_f = 0;
						        }
				        }
		        }
		        else if(Is_time_setting_f)
		        {
		            disp_timing_time = setting_timing_hour;
			
				        bw_i = LED_TAB[disp_timing_time/10];
					      sw_i = LED_TAB[disp_timing_time%10];
				        gw_i = Lcdch_H;
				
				        if(key_flash_time!=0)
				        {
				            key_flash_time--;
					
				            if(key_flash_time==0)
						        {
						            Is_time_setting_f = 0;
						        }
				        }
		        }
		        else if(Is_temp_setting_f)
		        {
		            disp_temperature = setting_temperature*10;
			
			          bw_i = LED_TAB[disp_temperature/100];
				        sw_i = LED_TAB[disp_temperature%100/10];
			          sw_i |= _DP1;
			          gw_i = LED_TAB[disp_temperature%10];
			
			          LED_TEMP_ON();
			
		            if(key_flash_time!=0)
				        {
				            key_flash_time--;
					
					          if(key_flash_time==0)
						        {
						            Is_temp_setting_f = 0;
						        }
				        }
		        }
		        else
		        {
		            disp_temperature = temperature*10;
			          disp_humidity = humidity*10;
			
			          disp_switch_time++;
			          if(disp_switch_time>=600)
				        {
				            disp_switch_time = 0;
				        }
				
				        if(disp_switch_time<300)
				        {
				            bw_i = LED_TAB[disp_temperature/100];
				            sw_i = LED_TAB[disp_temperature%100/10];
			              sw_i |= _DP1;
			              gw_i = LED_TAB[disp_temperature%10];
			  
			              LED_TEMP_ON();
				        }
				        else
				        {
				            bw_i = LED_TAB[disp_humidity/100];
				            sw_i = LED_TAB[disp_humidity%100/10];
			              sw_i |= _DP1;
			              gw_i = LED_TAB[disp_humidity%10];
			  
			              LED_HUMI_ON();
				        }
		        }	
					
				    com_data_temp[0] |= bw_i; 
            com_data_temp[1] |= sw_i; 
            com_data_temp[2] |= gw_i;

					  if(!AI_timing_open_f) {LED_AI_ON();}
						if(PTC_heat_open_f) {LED_PTC_ON();}
						if(plasma_open_f) {LED_PLASMA_ON();}
						if(Ultra_Sound_open_f) {LED_MOUSE_ON();}
						
						LED_POWER_ON();
						
						if(key_net_config_f)
						{
						    led_scan_time++;
						    if(led_scan_time>=33)
								{
								    led_scan_time = 0;
								}
								
								if(led_scan_time<16)
								{
								    LED_WIFI_ON();
								}
						}
						else
						{
						    if(wifi_connected_f)
								{
								    LED_WIFI_ON();
								}
								else
								{
								    led_scan_time++;
                    if(led_scan_time>=150)
                    {
										    led_scan_time = 0;
										}											
									
									  if(led_scan_time<75)
										{
										    LED_WIFI_ON();
										}
								}
						}
						
						com_data_temp[3] |= _A5|_B5|_CC5|_DD5|_E5|_F5|_G5|_H5;
						com_data_temp[4] |= _A1|_B1|_CC1|_DD1|_E1|_F1|_G1|_H1;
						com_data_temp[5] |= _A2|_B2|_CC2|_DD2|_E2|_F2|_G2|_H2;
						com_data_temp[6] |= _A3|_B3|_CC3|_DD3|_E3|_F3|_G3|_H3;
						com_data_temp[7] |= _A4|_B4|_CC4|_DD4|_E4|_F4|_G4|_H4;
				}
		}
		else
		{   
				if(flash_f)
        {
				    LED_POWER_ON();
				}					
		}

	
		com_data_buf[0]=(com_data_temp[0]&0x0f);
		com_data_buf[1]=((com_data_temp[0]>>4)&0x0f);
		com_data_buf[2]=(com_data_temp[1]&0x0f);
		com_data_buf[3]=((com_data_temp[1]>>4)&0x0f);
		com_data_buf[4]=(com_data_temp[2]&0x0f);
		com_data_buf[5]=((com_data_temp[2]>>4)&0x0f);
		com_data_buf[6]=(com_data_temp[3]&0x0f);
		com_data_buf[7]=((com_data_temp[3]>>4)&0x0f);
		com_data_buf[8]=(com_data_temp[4]&0x0f);
		com_data_buf[9]=((com_data_temp[4]>>4)&0x0f);
		com_data_buf[10]=(com_data_temp[5]&0x0f);
		com_data_buf[11]=((com_data_temp[5]>>4)&0x0f);
		com_data_buf[12]=(com_data_temp[6]&0x0f);
		com_data_buf[13]=((com_data_temp[6]>>4)&0x0f);
		com_data_buf[14]=(com_data_temp[7]&0x0f);
		com_data_buf[15]=((com_data_temp[7]>>4)&0x0f);
		
		
		TM1639_Write_Display_Data(com_data_buf,16);
}







































