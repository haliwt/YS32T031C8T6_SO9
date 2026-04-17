#ifndef __BSP_TASK_H
#define __BSP_TASK_H
#include "main.h"

extern volatile uint32_t g_system_tick;


// 任务结构体：这次我们直接检查标志位
typedef struct {
    volatile uint8_t *task_f; // 指向定时器置位的标志位
    void (*task_func)(void);  // 任务函数
} Task_Config_t;


void Task_Scheduler(void) ;

#endif 

