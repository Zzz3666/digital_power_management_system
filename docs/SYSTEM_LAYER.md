# System Layer Architecture

## 概述

System 层（系统层）位于 Component 层之上，负责封装第三方 RTOS（实时操作系统）的系统接口，为上层应用提供统一的操作系统抽象层（OSAL）。

## 架构层次

```
┌─────────────────────────────────┐
│      Application Layer          │  应用层
└──────────────┬──────────────────┘
               │ Uses system API
┌──────────────▼──────────────────┐
│       System Layer              │  系统层 (新增)
│  - OS Abstraction Layer (OSAL)  │
│  - RTOS wrapper (FreeRTOS, etc.)│
│  - Task/Mutex/Semaphore/Queue   │
└──────────────┬──────────────────┘
               │ Depends on Component only
┌──────────────▼──────────────────┐
│     Component Layer             │  组件层
│  - Middleware encapsulation     │
└──────────────┬──────────────────┘
               │ Depends on Driver
┌──────────────▼──────────────────┐
│       Driver Layer              │  驱动层
└──────────────┬──────────────────┘
               │ Depends on HAL
┌──────────────▼──────────────────┐
│        HAL Layer                │  硬件抽象层
└──────────────┬──────────────────┘
               │ Depends on BSP
┌──────────────▼──────────────────┐
│        BSP Layer                │  板级支持包
└──────────────┬──────────────────┘
               │
┌──────────────▼──────────────────┐
│       Hardware                  │  硬件层
└─────────────────────────────────┘
```

## 依赖关系

**完整的六层架构：**
```
Application → System → Component → Driver → HAL → BSP → Hardware
```

- **System** 只依赖 **Component**（不直接访问 Driver/HAL/BSP）
- **Component** 依赖 **Driver**
- **Driver** 依赖 **HAL**
- **HAL** 依赖 **BSP**
- **BSP** 是平台特定的实现

## 核心特性

### 1. RTOS 抽象层 (OSAL)

提供统一的操作系统接口，屏蔽不同 RTOS 的差异：

**支持的 RTOS：**
- FreeRTOS
- ThreadX
- RT-Thread
- Bare Metal (无 RTOS)

**切换 RTOS：**
只需修改 CMake 配置，无需修改应用代码：
```cmake
option(SYSTEM_USE_FREERTOS "Use FreeRTOS" ON)
option(SYSTEM_USE_THREADX "Use ThreadX" OFF)
```

### 2. 任务管理

提供统一的任务创建、删除、调度接口：

```c
// 创建任务
task_handle_t task = system_task_create(
    "MyTask",           // 任务名称
    my_task_function,   // 任务函数
    argument,           // 参数
    2048,               // 栈大小
    TASK_PRIORITY_HIGH  // 优先级
);

// 删除任务
system_task_delete(task);

// 挂起/恢复
system_task_suspend(task);
system_task_resume(task);

// 延时
system_delay_ms(100);
```

**任务优先级：**
```c
typedef enum {
    TASK_PRIORITY_IDLE = 0,      // 空闲
    TASK_PRIORITY_LOW,           // 低
    TASK_PRIORITY_MEDIUM,        // 中
    TASK_PRIORITY_HIGH,          // 高
    TASK_PRIORITY_REALTIME,      // 实时
} task_priority_t;
```

### 3. 互斥锁 (Mutex)

提供线程安全的资源保护机制：

```c
// 创建互斥锁
mutex_handle_t mutex = system_mutex_create("SharedResource");

// 加锁
system_mutex_lock(mutex, 1000);  // 超时 1000ms

// 临界区
// ... 访问共享资源 ...

// 解锁
system_mutex_unlock(mutex);

// 删除
system_mutex_delete(mutex);
```

### 4. 信号量 (Semaphore)

支持二值信号量和计数信号量：

```c
// 二值信号量
semaphore_handle_t bin_sem = system_semaphore_create_binary("BinarySem");
system_semaphore_give(bin_sem);
system_semaphore_take(bin_sem, 1000);

// 计数信号量
semaphore_handle_t count_sem = system_semaphore_create_counting(
    "CountingSem",  // 名称
    10,             // 最大计数
    5               // 初始计数
);
```

### 5. 消息队列 (Queue)

提供任务间通信机制：

```c
// 创建队列
queue_handle_t queue = system_queue_create(
    "DataQueue",      // 队列名称
    sizeof(int),      // 每项大小
    10                // 队列长度
);

// 发送数据
int data = 42;
system_queue_send(queue, &data, 1000);

// 接收数据
int received;
system_queue_receive(queue, &received, 1000);

// 删除队列
system_queue_delete(queue);
```

### 6. 系统信息

获取系统运行状态：

```c
// 获取系统 tick
uint32_t tick = system_get_tick();

// 获取运行时间（毫秒）
uint32_t uptime = system_get_uptime_ms();
```

## 目录结构

```
system/
├── CMakeLists.txt              # System 库配置
├── include/
│   ├── system.h                # System 框架核心接口
│   ├── freertos_wrapper.h      # FreeRTOS 包装器接口
│   └── threadx_wrapper.h       # ThreadX 包装器接口
├── src/
│   └── system.c                # System 框架实现
├── os/
│   ├── freertos_wrapper.c      # FreeRTOS 包装器实现
│   └── threadx_wrapper.c       # ThreadX 包装器实现
└── examples/
    └── system_demo.c           # 演示程序
```

## 编译配置

在 `system/CMakeLists.txt` 中选择 RTOS：

```cmake
option(SYSTEM_USE_FREERTOS "Use FreeRTOS as underlying RTOS" ON)
option(SYSTEM_USE_THREADX "Use ThreadX as underlying RTOS" OFF)
option(SYSTEM_USE_RTT "Use RT-Thread as underlying RTOS" OFF)
option(SYSTEM_USE_NONE "Use no RTOS (bare metal)" OFF)
```

## 设计原则

### 1. RTOS 无关性

- 应用层代码不依赖特定 RTOS
- 可以轻松切换不同的 RTOS
- 降低移植成本

### 2. 统一接口

- 所有 RTOS 使用相同的 API
- 一致的命名规范
- 降低学习成本

### 3. 灵活性

- 支持有 RTOS 和无 RTOS 模式
- 可根据项目需求选择
- 裸机模式也能使用相同接口

### 4. 依赖隔离

- System 只依赖 Component
- 通过 Component 间接访问底层
- 保持清晰的依赖链

## 实际应用场景

### 1. 多任务系统

```c
// 创建多个任务处理不同功能
task_handle_t sensor_task = system_task_create(
    "SensorTask", read_sensors, NULL, 1024, TASK_PRIORITY_HIGH);

task_handle_t comm_task = system_task_create(
    "CommTask", handle_communication, NULL, 2048, TASK_PRIORITY_MEDIUM);

task_handle_t ui_task = system_task_create(
    "UITask", update_display, NULL, 1024, TASK_PRIORITY_LOW);
```

### 2. 线程安全的数据共享

```c
// 使用互斥锁保护共享数据
mutex_handle_t data_mutex = system_mutex_create("SensorData");

void sensor_reader(void *arg)
{
    while (1) {
        // 读取传感器
        float temperature = read_temperature();
        
        // 加锁更新共享数据
        system_mutex_lock(data_mutex, 1000);
        shared_data.temperature = temperature;
        system_mutex_unlock(data_mutex);
        
        system_delay_ms(100);
    }
}

void data_processor(void *arg)
{
    while (1) {
        // 加锁读取共享数据
        system_mutex_lock(data_mutex, 1000);
        float temp = shared_data.temperature;
        system_mutex_unlock(data_mutex);
        
        // 处理数据
        process_temperature(temp);
        
        system_delay_ms(50);
    }
}
```

### 3. 任务间通信

```c
// 使用队列传递传感器数据
queue_handle_t sensor_queue = system_queue_create(
    "SensorData", sizeof(sensor_data_t), 10);

void producer_task(void *arg)
{
    while (1) {
        sensor_data_t data;
        data.temperature = read_temperature();
        data.humidity = read_humidity();
        
        // 发送到队列
        system_queue_send(sensor_queue, &data, 1000);
        
        system_delay_ms(100);
    }
}

void consumer_task(void *arg)
{
    while (1) {
        sensor_data_t data;
        
        // 从队列接收
        if (system_queue_receive(sensor_queue, &data, 1000) == 0) {
            // 处理数据
            send_to_cloud(&data);
        }
    }
}
```

### 4. 事件同步

```c
// 使用信号量同步任务
semaphore_handle_t event_sem = system_semaphore_create_binary("EventTrigger");

void event_generator(void *arg)
{
    while (1) {
        // 等待外部事件
        if (button_pressed()) {
            // 触发事件
            system_semaphore_give(event_sem);
        }
        system_delay_ms(10);
    }
}

void event_handler(void *arg)
{
    while (1) {
        // 等待事件
        if (system_semaphore_take(event_sem, UINT32_MAX) == 0) {
            // 处理事件
            handle_button_event();
        }
    }
}
```

### 5. 与 Component 层集成

```c
// 在任务中使用 JSON 组件
void telemetry_task(void *arg)
{
    while (1) {
        // 收集数据
        sensor_data_t sensors = read_all_sensors();
        
        // 使用 JSON 组件打包
        json_object_t data = json_object_create();
        json_object_add_int(data, "temp", sensors.temperature);
        json_object_add_int(data, "humid", sensors.humidity);
        
        char json_str[256];
        json_object_encode(data, json_str, sizeof(json_str));
        
        // 通过 Modbus 组件发送
        modbus_write_multiple_registers(1, 0, 10, (uint16_t*)json_str);
        
        json_object_delete(data);
        system_delay_ms(1000);
    }
}
```

## RTOS 移植指南

### 移植 FreeRTOS

1. **包含 FreeRTOS 头文件**
```c
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
```

2. **实现任务创建**
```c
task_handle_t system_task_create(...)
{
    TaskHandle_t handle;
    xTaskCreate(func, name, stack_size/sizeof(StackType_t),
                argument, priority, &handle);
    return (task_handle_t)handle;
}
```

3. **实现延时**
```c
void system_delay_ms(uint32_t delay_ms)
{
    vTaskDelay(pdMS_TO_TICKS(delay_ms));
}
```

### 移植 ThreadX

1. **包含 ThreadX 头文件**
```c
#include "tx_api.h"
```

2. **实现任务创建**
```c
task_handle_t system_task_create(...)
{
    TX_THREAD *thread = malloc(sizeof(TX_THREAD));
    tx_thread_create(thread, name, func, argument,
                     stack_ptr, stack_size, priority, priority,
                     TX_NO_TIME_SLICE, TX_AUTO_START);
    return (task_handle_t)thread;
}
```

3. **实现延时**
```c
void system_delay_ms(uint32_t delay_ms)
{
    tx_thread_sleep((delay_ms * TX_TIMER_TICKS_PER_SECOND) / 1000);
}
```

## 调用链示例

以任务延时为例：

```
Application: system_delay_ms(100)
    ↓
System Layer: 调用 RTOS 包装器
    ↓
FreeRTOS Wrapper: vTaskDelay(pdMS_TO_TICKS(100))
    ↓
FreeRTOS Kernel: 任务调度
    ↓
Hardware: 定时器中断
```

**日志输出：**
```
[SYSTEM-TASK] Delaying for 100 ms
[FREERTOS] vTaskDelay called (ticks=100)
```

## 优势

1. **RTOS 无关**: 应用代码不绑定特定 RTOS
2. **易于移植**: 更换 RTOS 只需修改包装器
3. **统一接口**: 所有 RTOS 使用相同 API
4. **灵活选择**: 支持多种 RTOS 和裸机模式
5. **完整功能**: 任务、互斥锁、信号量、队列
6. **与下层集成**: 无缝使用 Component 层功能

## 性能考虑

### 优点
- 抽象层开销小（只是函数调用转发）
- 可以选择最适合的 RTOS
- 裸机模式无额外开销

### 注意事项
- 每次调用都有少量函数调用开销
- 对于硬实时场景，需要仔细评估
- 可以考虑关键路径直接调用 RTOS API

## 最佳实践

### 1. 任务优先级分配
```
TASK_PRIORITY_REALTIME   - 紧急中断处理
TASK_PRIORITY_HIGH       - 关键控制任务
TASK_PRIORITY_MEDIUM     - 常规业务逻辑
TASK_PRIORITY_LOW        - 后台任务
TASK_PRIORITY_IDLE       - 空闲任务
```

### 2. 栈大小估算
```c
// 简单任务：512-1024 bytes
// 中等任务：1024-2048 bytes
// 复杂任务：2048-4096 bytes
// 注意：根据实际需求调整
```

### 3. 超时设置
```c
// 非阻塞：timeout = 0
// 短等待：timeout = 10-100 ms
// 正常等待：timeout = 100-1000 ms
// 永久等待：timeout = UINT32_MAX
```

### 4. 资源清理
```c
// 确保在退出前清理所有资源
system_task_delete(task);
system_mutex_delete(mutex);
system_semaphore_delete(sem);
system_queue_delete(queue);
```

## 总结

System 层的引入完成了完整的六层架构：

- **BSP**: 平台特定的硬件访问
- **HAL**: 硬件抽象，统一 API
- **Driver**: 设备管理，标准接口
- **Component**: 中间件封装，第三方库抽象
- **System**: RTOS 抽象，操作系统接口
- **Application**: 业务逻辑

这种架构使得：
- ✅ 完全 RTOS 无关
- ✅ 易于在不同 RTOS 间移植
- ✅ 支持裸机和 RTOS 模式
- ✅ 提供完整的并发原语
- ✅ 与下层组件无缝集成

这是一个工业级的嵌入式系统架构，适用于从小型裸机系统到复杂的多任务 RTOS 系统。
