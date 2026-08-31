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

/* 任务2配置 */
#define TASK_STACK_SIZE2 128
#define TASK2_PRIO 1
TaskHandle_t task2_handle;

/* 任务3配置 */
#define TASK_STACK_SIZE3 128
#define TASK3_PRIO 1
TaskHandle_t task3_handle;

void start_tasks(void);

void startup_task(void *pvParameters)
{
    /* Startup task code here */
    start_tasks();

    /* 启动任务只需要执行一次，用完就删，删除启动任务 */
    vTaskDelete(NULL);
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
        LED1_TOGGLE;
				printf("task1\r\n");
        vTaskDelay(pdMS_TO_TICKS(500)); // Delay for 500ms
    }
}

/**
 * @description: 任务2：LED2每500ms闪烁一次
 * @param {void} *pvParameters
 * @return {*}
 */
void task2(void *pvParameters)
{
    vTaskDelay(pdMS_TO_TICKS(500)); // 与task1错开半个周期,实现红绿交替
    while(1)
    {
        LED2_TOGGLE;
				printf("task2\r\n");
        vTaskDelay(pdMS_TO_TICKS(500)); // Delay for 500ms
    }
}

/**
 * @description: 任务3：KEY1按下，挂起task1，再按下KEY1在任务中恢复task1，KEY2按下，挂起调度器，再按下KEY2，恢复调度器，并打印任务的状态
 * @param {void} *pvParameters
 * @return {*}
 */
void task3(void *pvParameters)
{
    uint8_t task1_state = 0;
    uint8_t Scheduler_state = 0;
    while(1)
    {
        switch (task1_state)
        {
            case 0:
                if (Key_Scan_RTOS(KEY1_GPIO_PORT, KEY1_PIN) == KEY_ON)
                {
                    task1_state = 1;
										printf("task1 suapend\r\n");
                    vTaskSuspend(task1_handle); // 挂起task1
                }
            break;
        
            case 1 : 
                if (Key_Scan_RTOS(KEY1_GPIO_PORT, KEY1_PIN) == KEY_ON)
                {
                    task1_state = 0;
										printf("task1 resume\r\n");
                    vTaskResume(task1_handle); // 恢复task1
                }
            break;

            default:
                task1_state = 0;
            break;
        }
        
        switch (Scheduler_state)
        {
            case 0:
                if (Key_Scan_RTOS(KEY2_GPIO_PORT, KEY2_PIN) == KEY_ON)
                {
                    Scheduler_state = 1;
										printf("Scheduler suapend\r\n");
                    vTaskSuspendAll(); // 挂起调度器
                }
            break;
        
            case 1 : 
                if (HAL_GPIO_ReadPin(KEY2_GPIO_PORT, KEY2_PIN) == KEY_ON)
                {
                    /* 调度器被挂起，回到裸机死等消抖， */
                    HAL_Delay(20);
                    if (HAL_GPIO_ReadPin(KEY2_GPIO_PORT, KEY2_PIN) == KEY_ON)
                    {
                        while(HAL_GPIO_ReadPin(KEY2_GPIO_PORT, KEY2_PIN) == KEY_ON); // 等待按键释放
                        
                        Scheduler_state = 0;
                        xTaskResumeAll(); // 恢复调度器
                        printf("state: %d, %d, %d\r\n", eTaskGetState(task1_handle), eTaskGetState(task2_handle), eTaskGetState(task3_handle));
                    }
                }
            break;

            default:
                Scheduler_state = 0;
            break;
        }

        if (Scheduler_state == 0)
        {
            vTaskDelay(pdMS_TO_TICKS(100)); // 只有调度器正常时才允许系统延时
        }
        else
        {
            HAL_Delay(100); // 调度器瘫痪时，只能用裸机延时硬扛
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

    xTaskCreate((TaskFunction_t) task2,                             
							 (char *) "Task2",                     
							  (configSTACK_DEPTH_TYPE) TASK_STACK_SIZE2,   
							  (void *) NULL,                                
							  (UBaseType_t) TASK2_PRIO,              
							  (TaskHandle_t *) &task2_handle); 

    xTaskCreate((TaskFunction_t) task3,                             
							 (char *) "Task3",                     
							  (configSTACK_DEPTH_TYPE) TASK_STACK_SIZE3,   
							  (void *) NULL,                                
							  (UBaseType_t) TASK3_PRIO,              
							  (TaskHandle_t *) &task3_handle); 

    taskEXIT_CRITICAL();                          

    /* 启动任务只需要执行一次，用完就删，删除启动任务 */
    vTaskDelete(NULL);
}


