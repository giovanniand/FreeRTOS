/*
 * @Author: Giovanni 1483577470@qq.com
 * @Date: 2026-09-02 22:35:11
 * @LastEditors: Giovanni 1483577470@qq.com
 * @LastEditTime: 2026-09-02 22:53:43
 * @FilePath: \FreeRTOS\4_FreeRTOS_ISR_Manage\Core\Src\freertos_demo.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "freertos_demo.h"
#include "FreeRTOS.h"
#include "task.h"
#include "gpio.h"
#include "led.h"
#include "key.h"
#include <stdio.h>

/* 启动任务配置 */
#define START_STACK_SIZE 128
#define START_TASK_PRIO 0
TaskHandle_t startup_task_handle;

/* 任务1配置 */
#define TASK_STACK_SIZE1 128
#define TASK1_PRIO 1
TaskHandle_t task1_handle;


void start_tasks(void);

void startup_task(void *pvParameters)
{
    /* Startup task code here */
    start_tasks();

    /* 启动任务只需要执行一次，用完就删，删除启动任务 */
    vTaskDelete(NULL);
}

/**
 * @description: 按下KEY1，关中断，按下KEY2，开中断  使用两个定时器，一个优先级为4，一个优先级为6,两个定时器每1s，打印一段字符串

 * @param {void} *pvParameters
 * @return {*}
 */

void task1(void *pvParameters)
{
    vTaskDelay(pdMS_TO_TICKS(500)); // 与task1错开半个周期,实现红绿交替
    while(1)
    {
        if (Key_Scan_RTOS(KEY1_GPIO_PORT, KEY1_PIN) == KEY_ON)
        {
            printf("suspend ISR\r\n");
            portDISABLE_INTERRUPTS(); // 关中断
        }
        else if (Key_Scan_RTOS(KEY2_GPIO_PORT, KEY2_PIN) == KEY_ON)
        {
            printf("resume ISR\r\n");
            portENABLE_INTERRUPTS(); // 开中断
        }
    }
}

/**
 * @description: 启动freertos任务
 * @return {*}
 */
void freertos_start(void)
{
    /* Create tasks and start the scheduler here */
    /* 1、创建一个启动任务 */
    xTaskCreate((TaskFunction_t) startup_task,                              /* 任务函数地址 */
							 (char *) "Startup_Task",                       /* 任务名字 */
							  (configSTACK_DEPTH_TYPE) START_STACK_SIZE,    /* 任务栈大小，单位4字节 */
							  (void *) NULL,                                /* 任务参数 */
							  (UBaseType_t) START_TASK_PRIO,               /* 任务优先级 */
							  (TaskHandle_t *) &startup_task_handle);       /* 任务句柄地址 */

    /* 2、启动调度器 */
    vTaskStartScheduler();
}

/**
 * @description: 启动任务：用来创建其他任务
 * @return {*}
 */
void start_tasks(void)
{
    /* 进入临界区，防止在创建任务时被调度器打断 */
    taskENTER_CRITICAL(); 
    
    /* 创建三个任务 */
    xTaskCreate((TaskFunction_t) task1,                             
							 (char *) "Task1",                     
							  (configSTACK_DEPTH_TYPE) TASK_STACK_SIZE1,   
							  (void *) NULL,                                
							  (UBaseType_t) TASK1_PRIO,              
							  (TaskHandle_t *) &task1_handle);      

    taskEXIT_CRITICAL();                          

    /* 启动任务只需要执行一次，用完就删，删除启动任务 */
    vTaskDelete(NULL);
}


