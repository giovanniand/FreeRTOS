核心参数
#define configUSE_TIME_SLICING                          1
配置为 1（默认状态：开启时间片）
控制“同优先级任务时间片轮转
当多个同等优先级的任务都处于就绪态时，FreeRTOS 会在每一次 SysTick 中断触发时，强制剥夺当前任务的 CPU 使用权，切换给下一个同级任务。

适用场景： 绝大多数标准工程的默认选择。它能保证同级任务之间的绝对公平，大家都能均匀地推进进度，防止某个计算密集型任务饿死其他同级伙伴。

#define configUSE_PREEMPTION                            1
抢占式调度总开关

#define configTICK_RATE_HZ          ( ( TickType_t ) 20 ) 
系统心跳频率
物理意义与计算： 1s/20次=50ms，这意味着系统的一个 Tick（时间片）长达 50 毫秒。
在绝大多数 Cortex-M 内核的工业级工程中，这个值通常被牢牢锁定为 1000（即 1 毫秒精度）

打印
Task2 is running
Task2 is running
Task2 is running
Task1 is running
Task1 is running
Task1 is running
Task1 is running
Task2 is running
Task2 is running
Task2 is running
Task2 is running
Task1 is running
Task1 is running
Task1 is running
Task1 is running
Task1 is running
Task2 is running
Task2 is running
Task2 is running
Task2 is running
Task2 is running
Task1 is running
Task1 is running
Task1 is running
Task1 is running
Task2 is running
Task2 is running
Task2 is running
Task2 is running
Task1 is running
Task1 is running
Task1 is running