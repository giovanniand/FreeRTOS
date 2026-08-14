#include "freertos_demo.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "gpio.h"
#include "led.h"
#include "key.h"

/* 启动任务配置 */
#define START_STACK_SIZE 128
#define START_TASK_PRIO 0
TaskHandle_t startup_task_handle;

StackType_t startup_task_stack[START_STACK_SIZE];
StaticTask_t startup_task_TCB;

/* 任务1配置 */
#define TASK_STACK_SIZE1 128
#define TASK1_PRIO 1
TaskHandle_t task1_handle;
StackType_t task1_stack[TASK_STACK_SIZE1];
StaticTask_t task1_TCB;

/* 任务2配置 */
#define TASK_STACK_SIZE2 128
#define TASK2_PRIO 1
TaskHandle_t task2_handle;
StackType_t task2_stack[TASK_STACK_SIZE2];
StaticTask_t task2_TCB;

/* 任务3配置 */
#define TASK_STACK_SIZE3 128
#define TASK3_PRIO 1
TaskHandle_t task3_handle;
StackType_t task3_stack[TASK_STACK_SIZE3];
StaticTask_t task3_TCB;

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
        vTaskDelay(pdMS_TO_TICKS(500)); // Delay for 500ms
    }
}

/**
 * @description: 任务3：判断按键是否按下，删除task1
 * @param {void} *pvParameters
 * @return {*}
 */
void task3(void *pvParameters)
{
    while(1)
    {
        if (Key_Scan_RTOS(KEY1_GPIO_PORT, KEY1_PIN) == KEY_ON)
        {
            if(task1_handle != NULL)
            {
                vTaskDelete(task1_handle); // 删除任务1
                task1_handle = NULL;
                LED1_OFF;                 // 删除后关闭LED1,避免停在随机状态
            }
        }
        vTaskDelay(pdMS_TO_TICKS(100)); // Delay for 100ms
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
   startup_task_handle = xTaskCreateStatic((TaskFunction_t) startup_task,                              /* 任务函数地址 */
							(char *) "Startup_Task",                       /* 任务名字 */
							(configSTACK_DEPTH_TYPE) START_STACK_SIZE,    /* 任务栈大小，单位4字节 */
							(void *) NULL,                                /* 任务参数 */
							(UBaseType_t) START_TASK_PRIO,               /* 任务优先级 */	             
                            (StackType_t *) startup_task_stack,     /* 任务栈缓冲区 */
                            (StaticTask_t *) &startup_task_TCB);     /* 任务控制块缓冲区 */;       /* 任务句柄地址 */

    /* 2、启动调度器 :会自动创建空闲任务和软件定时器*/
    vTaskStartScheduler();
}

/**
 * @description: 启动任务：用来创建其他任务
 * @return {*}
 */
void start_tasks(void)
{
    /* 进入临界区，防止在创建任务时被调度器打断 */
    taskENTER_CRITICAL(); // 
    
    /* 使用静态创建三个任务 */
    task1_handle = xTaskCreateStatic((TaskFunction_t) task1,                             
							(char *) "Task1",                     
							(configSTACK_DEPTH_TYPE) TASK_STACK_SIZE1,   
							(void *) NULL,                                
							(UBaseType_t) TASK1_PRIO,              
							(StackType_t *) task1_stack,    
                            (StaticTask_t *) &task1_TCB);      

    task2_handle = xTaskCreateStatic((TaskFunction_t) task2,                             
							(char *) "Task2",                     
							(configSTACK_DEPTH_TYPE) TASK_STACK_SIZE2,   
							(void *) NULL,                                
							(UBaseType_t) TASK2_PRIO,              
							(StackType_t *) task2_stack,    
                            (StaticTask_t *) &task2_TCB);      

    task3_handle = xTaskCreateStatic((TaskFunction_t) task3,                             
							(char *) "Task3",                     
							(configSTACK_DEPTH_TYPE) TASK_STACK_SIZE3,   
							(void *) NULL,                                
							(UBaseType_t) TASK3_PRIO,              
							(StackType_t *) task3_stack,    
                            (StaticTask_t *) &task3_TCB);      
		
		/* 结束临界区 */				
		taskEXIT_CRITICAL();

    /* 结束调度器，防止在创建任务时被调度器打断 */
    vTaskEndScheduler(); 

    /* 启动任务只需要执行一次，用完就删，删除启动任务 */
    vTaskDelete(NULL);
}


