/*
  ******************************************************************************
  * Copyright (c) 2024 Yspring.
  * All rights reserved..
  * @file    user.C
  * @author  Yspring Firmware Team  
  * @brief   user Source Code.
  ******************************************************************************      
*/


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
#include "bsp.h"



#define KEY_TICKS_SHORT    4    // 40ms 消抖
#define KEY_TICKS_LONG_P   250  // 2.5s 电源键长按
#define KEY_TICKS_LONG_M   200  // 2s 模式/下键长按

static void power_on_fan_normal_handler(void);
static void power_on_fan_error_handler(void);



volatile uint8_t Times5msCnt;
uint8_t Times10msCnt;
uint8_t Times100msCnt;
uint8_t Times1minute;
uint16_t Times1minCnt;
uint8_t Cacl_time_sec;

volatile uint8_t time_5ms_f;
volatile uint8_t time_10ms_f;
uint8_t time_100ms_f;


uint8_t time_1s_f;
uint8_t time_1minute_f;

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

volatile uint8_t beep_times;				//����
volatile uint8_t beep_lenght;			  //��ĳ��� *100ms
volatile uint8_t non_beep_length;		//���ʱ��
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
    0,                                    //����
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



//������ʼ��
void Clear_Ram(void)
{
    time_5ms_f = 0;
	  time_10ms_f = 0;
	  time_100ms_f = 0;
	  time_1s_f = 0;
	  time_1minute_f=0;
	
	 // Times5msCnt = 0;
	  Times10msCnt = 0;
	  Times100msCnt = 0;
	  Times1minute = 0;
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
		
		//wifi_connected_f = 0;
		
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




//ADCͨ������
void Adc_Channel_Sample(void)
{
    volatile uint16_t ad_temp;
	
    ad_temp = ADC_GetValue(_FCUR_CH,VREFBUF_ADC_VCC);
	
    ad_value[_AD_FCUR] = ad_temp;
}



//AD����һ�׻����˲�
void AD_Filter(void)
{
    //FAN_CURRENT
	  fan_current=(ad_value[_AD_FCUR]*2+fan_current*18)/20;
}


void Process_Short_Key(uint16_t key) ;
void Process_Short_Key(uint16_t key);
void Process_Long_Key(uint16_t key);
void System_Status_PowerOff(void) ;
void System_Status_PowerOn(void) ;
void Handle_Value_Adjustment(uint8_t is_up);



void Key_Scan(void)
{
    uint16_t key_i = 0;
    
    // 物理层扫描
    if      (KEY10_PIN) key_i = _POWER_KEY_DOWN;
    else if (KEY9_PIN)  key_i = _MODE_KEY_DOWN;
    else if (KEY8_PIN)  key_i = _UP_KEY_DOWN;
    else if (KEY7_PIN)  key_i = _DOWN_KEY_DOWN;

    // 状态机处理
    if (key_i == 0) { // 松手或无按键
        if (!key_long_f && key_time >= KEY_TICKS_SHORT) {
            // --- 执行短按逻辑 ---
            Process_Short_Key(key_data);
        }
        key_time = 0;
        key_data = 0;
        key_long_f = 0;
    } 
    else if (key_i != key_data) { // 切换按键
        key_data = key_i;
        key_time = 0;
        key_long_f = 0;
    } 
    else { // 持续按下
        if (key_time < 0xFFF) key_time++;
        
        // --- 执行长按逻辑 ---
        Process_Long_Key(key_data);
    }
}

// 短按处理子函数
void Process_Short_Key(uint16_t key) 
{
    if (key == _POWER_KEY_DOWN) {
        if (discharge_f) System_Status_PowerOff();
        else System_Status_PowerOn();
        return;
    }

    // 仅在开机且无负载故障时允许操作
    if (!discharge_f || no_fan_load_f) return;

    switch (key) {
        case _MODE_KEY_DOWN:
            Is_timing_hour_disp_f = 1;
            Is_time_setting_f = 0;
            Is_temp_setting_f = 0;
            key_flash_time = 300;
            if (setting_timing_hour == 0) AI_timing_open_f = 0;
            Beep(BEEP_ONCE);
            break;

        case _UP_KEY_DOWN:
            Handle_Value_Adjustment(1);
            Beep(BEEP_ONCE);
            break;

        case _DOWN_KEY_DOWN:
            Handle_Value_Adjustment(0);
            Beep(BEEP_ONCE);
            break;
    }
}

// 长按处理子函数
void Process_Long_Key(uint16_t key) {
    if (key_long_f) return; // 已处理过长按则退出

    switch (key) {
        case _POWER_KEY_DOWN:
            if (key_time >= KEY_TICKS_LONG_P) {
                key_long_f = 1;
                if (discharge_f && !key_net_config_f) {
                    key_net_config_f = 1;
                    key_net_config_time = 0;
                    Beep(BEEP_ONCE);
                }
            }
            break;

        case _MODE_KEY_DOWN:
            if (key_time >= KEY_TICKS_LONG_M) {
                key_long_f = 1;
                if (discharge_f && !no_fan_load_f) {
                    Is_time_setting_f = 1;
                    Is_temp_setting_f = 0;
                    Is_timing_hour_disp_f = 0;
                    key_flash_time = 300;
                    Beep(BEEP_ONCE);
                }
            }
            break;

        case _DOWN_KEY_DOWN:
            if (key_time >= KEY_TICKS_LONG_M) {
                key_long_f = 1;
                if (discharge_f && !no_fan_load_f) {
                    led_strip_open_f = !led_strip_open_f; // 翻转灯带状态
                    Beep(BEEP_ONCE);
                }
            }
            break;
    }
}

/**
  * @brief  数值调节处理函数
  * @param  is_up: 1 为增加(UP), 0 为减少(DOWN)
  */
void Handle_Value_Adjustment(uint8_t is_up) 
{
    // 情况 A: 正在设置时间 (定时小时)
    if (Is_time_setting_f) 
    {
        if (is_up) {
            if (setting_timing_hour < 24) setting_timing_hour++;
        } else {
            if (setting_timing_hour > 0)  setting_timing_hour--;
        }
        
        // 逻辑关联：如果定时不为0，开启定时标志
        AI_timing_open_f = (setting_timing_hour > 0) ? 1 : 0;
        
        // 只要动了时间，秒和分计数值都要清零重新开始
        timing_min_cnt = 0;
        timing_hour_cnt = 0;
        Cacl_time_sec = 0;
    } 
    // 情况 B: 正在设置温度
    else if (Is_temp_setting_f) 
    {
        if (is_up) {
            if (setting_temperature < 40) setting_temperature++;
        } else {
            if (setting_temperature > 20) setting_temperature--;
        }
    } 
    // 情况 C: 当前没在任何设置界面，按下加减键默认进入“温度设置”状态
    else 
    {
        Is_temp_setting_f = 1;
        Is_time_setting_f = 0;
        Is_timing_hour_disp_f = 0;
    }

    // 统一处理：设置闪烁倒计时（比如数码管闪烁 3 秒）
    key_flash_time = 300; 
}


/**
  * @brief  系统状态初始化（开机）
  * @note   设定系统启动时的默认工作参数
  */
void System_Status_PowerOn(void) 
{
    // 1. 开启核心工作标志位
    discharge_f = 1;            // 总输出使能
    PTC_heat_open_f = 1;        // 默认开启加热
    Ultra_Sound_open_f = 1;     // 默认开启超声波
    plasma_open_f = 1;          // 默认开启等离子
    fan_open_f = 1;             // 默认开启风扇
    led_strip_open_f = 1;       // 默认开启灯带
    
    // 2. 设定启动默认参数
    setting_temperature = 40;   // 默认设定温度 40°C
    setting_timing_hour = 0;    // 默认不设置定时（常开模式）
    
    // 3. 状态显示切换
    Is_time_setting_f = 0;
    Is_temp_setting_f = 0;
    Is_timing_hour_disp_f = 0;
    
    // 4. 重置计数器（确保从 0 开始计时）
    timing_min_cnt = 0;
    timing_hour_cnt = 0;
    Cacl_time_sec = 0;
    work_time = 0;              // 重置工作时间累计
    device_rest_f = 0;          // 退出休息模式
    device_rest_time = 0;
    
    // 5. 清除异常标志
    no_fan_load_f = 0;          // 清除负载异常
    fan_delay_time_off = 0;     // 清除风扇延时关闭倒计时
    
    // 6. 执行开机提示音
    Beep(BEEP_ONCE);
}


/**
  * @brief  系统状态复位（关机/重置）
  * @note   将所有业务逻辑标志位恢复至初始关闭状态
  */
void System_Status_PowerOff(void) 
{
    // 1. 关闭所有输出负载标志
    
    discharge_f = 0;
    PTC_heat_open_f = 0;
    Ultra_Sound_open_f = 0;
    led_strip_open_f = 0;
    plasma_open_f = 0;
    fan_open_f = 0;
    
    // 2. 重置所有功能模式标志
    AI_timing_open_f = 0;
    Is_time_setting_f = 0;
    Is_temp_setting_f = 0;
    Is_timing_hour_disp_f = 0;
    
    // 3. 重置所有时间/计数器
    timing_min_cnt = 0;
    timing_hour_cnt = 0;
    Cacl_time_sec = 0;
    work_time = 0;
    device_rest_time = 0;
    device_rest_f = 0;
    
    // 4. 特殊逻辑处理
    no_fan_load_f = 0;
    fan_delay_time_off = 600; // 开启风扇延时关闭倒计时
    
    // 5. 提示音
    Beep(BEEP_ONCE);
}



#endif 


uint8_t counter;
uint8_t power_Led_switch;	

 volatile uint16_t i;
 volatile uint16_t bw_i=0;
volatile uint16_t sw_i=0;
volatile uint16_t gw_i=0;
 volatile uint16_t disp_timing_time_temp;
 volatile uint16_t timing_diff_value_hour;
 volatile uint16_t timing_diff_value_min;

//����LED��ʾ
void Update_LED_Display(void)
{

  for(i=0;i<8;i++)
	{
		com_data_temp[i]=0x00;
	}

	LED_AI_OFF();LED_PTC_OFF();LED_PLASMA_OFF();LED_MOUSE_OFF();
	LED_WIFI_OFF();LED_TEMP_OFF();LED_HUMI_OFF();//LED_WIFI_OFF();

	switch(discharge_f)//if(discharge_f)//power on 
	{

	case 1:

	switch(no_fan_load_f){

		case 1:

	      power_on_fan_error_handler();
		break;

		case 0:


		  power_on_fan_normal_handler();

	    break;

	}
	break;

	case 0://power off

		all_led_off();

		if(++counter > 120){//
		counter =0;
		LED_POWER_TOGGLE();
		}
    break;
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




/**
  * @brief  fan run is error
  * @note  
  * @param: no_fan_load_f =1
  *
**/
static void power_on_fan_error_handler(void)
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

/**
  * @brief  fan run is ok
  * @note  
  *
  *
**/
static void power_on_fan_normal_handler(void)
{

	if(Is_timing_hour_disp_f){
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






























