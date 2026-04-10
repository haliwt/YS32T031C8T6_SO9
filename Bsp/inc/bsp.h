#ifndef __BSP_H
#define __BSP_H
#include "main.h"

#include "ys32t031.h"

#include "tim.h"


#include "dht11.h" 
#include "tm1639.h"
#include "key.h"
#include "system_init.h"

//
#include "bsp_power.h"
#include "bsp_led.h"
#include "bsp_peripheral.h"
#include "bsp_key.h"

//wifi
#include "bsp_usart_wifi.h"
#include "bsp_mqtt_iot.h"




void task_scheduler(void);

void Task_beep_called_100ms(void);

void delay_ms(uint16_t ms);




#endif 

