#include "bsp.h"


// 1ms 系统心跳计数器


static void wifi_rx_run_handler(void);


// --- 任务函数声明 ---
void Task_Key_Scan_10ms(void);
void Task_Logic_100ms(void);
void Task_ui_200ms(void);
void Task_Peripheral_300ms(void);
void Task_400ms(void);
void Task_System_1s(void);
void Task_2s(void);
void Task_1minutes(void);
void Task_2minutes(void);

// 2. 任务注册表：将标志位地址与函数关联
static const Task_Config_t Task_Table[] = {
    {&gpro_t.time_10ms_f,  Task_Key_Scan_10ms},
    {&gpro_t.time_100ms_f, Task_Logic_100ms},
    {&gpro_t.time_200ms_f, Task_ui_200ms},
    {&gpro_t.time_300ms_f, Task_Peripheral_300ms},
    {&gpro_t.time_400ms_f, Task_400ms},
    {&gpro_t.time_1s_f,    Task_System_1s},
    {&gpro_t.time_2s_f,    Task_2s},
    {&gpro_t.time_1m_f,    Task_1minutes},
    {&gpro_t.time_2m_f,    Task_2minutes}
};

#define TASK_COUNT (sizeof(Task_Table) / sizeof(Task_Config_t))






// --- 1. 按键任务 (10ms) ---
void Task_Key_Scan_10ms(void) {
    Key_Scan();               // 运行你提供的按键状态机
    Task_beep_called_100ms();     // 蜂鸣器驱动
}

// --- 2. 通信与显示任务 (100ms) ---
void Task_Logic_100ms(void) {
    
    wifi_fast_led_state();
    wifi_rx_run_handler();    // 处理腾讯连连通信
    
}

void Task_ui_200ms(void)
{
    set_temp_compare();
	
    

}

// --- 3. 外设任务 (300ms) ---
void Task_Peripheral_300ms(void) {

    if(discharge_f) dht11_read_temp_humidity_value();
   
}

void Task_400ms(void)
{
	
	wifi_normal_led_state();
	  
	   
	Fan_Ctrl_Process(); 	  // 风扇控制


}

// --- 4. 系统级任务 (1s) ---
void Task_System_1s(void) 
{
    peripheral_fun_handler();
    
 }

void Task_2s(void)
{
	Heat_Process(); 
    #if DEBUG_ENABLE 

	printf("set_temp = %d \n\r",setting_temperature);

	#endif 

}

void Task_1minutes(void)
{
     Fan_Current_Det();		// 电流检测

			  // 加热处理
	 // task_1s_run_handler();	  // 处理关机倒计时等
	  
	  // 每秒执行一次 WiFi 心跳或默认处理
	 wifi_default_handler();

}

void Task_2minutes(void)
{
	// 传感器采样
	   if (discharge_f) {
		 //  dht11_read_temp_humidity_value();
		   AD_Filter();
		   Adc_Channel_Sample();
	   }



}

// --- 核心调度器 (放在 main 的 while(1) 中) ---
// 3. 核心调度器：在 main 的 while(1) 中调用
void Task_Scheduler(void) 
{
    for (uint8_t i = 0; i < TASK_COUNT; i++) {
        // 检查对应的标志位是否被中断置 1
        if (*(Task_Table[i].task_f) == 1) {
            
            // 立即清除标志位（重要！防止重复执行）
            *(Task_Table[i].task_f) = 0; 
            
            // 执行任务函数
            if (Task_Table[i].task_func != NULL) {
                Task_Table[i].task_func();
            }
        }
    }
}


static void wifi_rx_run_handler(void)
{
      
    if(key_net_config_f==0 ){
         wifi_communication_tnecent_handler();//
    
         //getBeijingTime_cofirmLinkNetState_handler();

         wifi_auto_detected_link_state();
		
    }


}


