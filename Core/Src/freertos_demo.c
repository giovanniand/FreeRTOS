#include "freertos_demo.h"
#include "FreeRTOS.h"
#include "task.h"
#include "gpio.h"

/* 启动任务配置 */
#define START_STACK_SIZE 128
#define tskIDLE_PRIORITY 1
TaskHandle_t startup_task_handle;

/* 任务1配置 */
#define TASK_STACK_SIZE1 128
#define tskIDLE_PRIORITY1 1
TaskHandle_t task1_handle;

/* 任务2配置 */
#define TASK_STACK_SIZE2 128
#define tskIDLE_PRIORITY2 1
TaskHandle_t task2_handle;

/* 任务3配置 */
#define TASK_STACK_SIZE3 128
#define tskIDLE_PRIORITY3 1
TaskHandle_t task3_handle;

void startup_task(void *pvParameters)
{
    /* Startup task code here */
    for (;;)
    {
        /* Perform startup operations */
        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay for 1 second
    }
}

/**
 * @description: LED1每500ms闪烁一次
 * @param {void} *pvParameters
 * @return {*}
 */
void task1(void *pvParameters)
{
    while(1)
    {

    }
}

/**
 * @description: 任务2：LED2每500ms闪烁一次
 * @param {void} *pvParameters
 * @return {*}
 */
void task2(void *pvParameters)
{
    
}

/**
 * @description: 任务3：判断按键是否按下，删除task1
 * @param {void} *pvParameters
 * @return {*}
 */
void task3(void *pvParameters)
{
    
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
							  (UBaseType_t) tskIDLE_PRIORITY,               /* 任务优先级 */
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
    /* 创建三个任务 */
    xTaskCreate((TaskFunction_t) task1,                             
							 (char *) "Task1",                     
							  (configSTACK_DEPTH_TYPE) TASK_STACK_SIZE1,   
							  (void *) NULL,                                
							  (UBaseType_t) tskIDLE_PRIORITY1,              
							  (TaskHandle_t *) &task1_handle);      

    xTaskCreate((TaskFunction_t) task2,                             
							 (char *) "Task2",                     
							  (configSTACK_DEPTH_TYPE) TASK_STACK_SIZE2,   
							  (void *) NULL,                                
							  (UBaseType_t) tskIDLE_PRIORITY2,              
							  (TaskHandle_t *) &task2_handle); 

    xTaskCreate((TaskFunction_t) task3,                             
							 (char *) "Task3",                     
							  (configSTACK_DEPTH_TYPE) TASK_STACK_SIZE3,   
							  (void *) NULL,                                
							  (UBaseType_t) tskIDLE_PRIORITY3,              
							  (TaskHandle_t *) &task3_handle); 

    /* 启动任务只需要执行一次，用完就删，删除启动任务 */
    vTaskDelete(NULL);
}


