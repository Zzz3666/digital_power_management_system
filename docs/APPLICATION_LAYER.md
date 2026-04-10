# Application Layer - 应用层

## 概述

应用层（Application Layer）是数字电源管理系统的最高层，直接面向用户和应用场景。它依赖系统层（System Layer），提供完整的业务逻辑和用户接口。

**架构位置：**
```
Application (应用层) ← 当前层
    ↓
System (系统层)
    ↓
Component (组件层)
    ↓
Driver (驱动层)
    ↓
HAL (硬件抽象层)
    ↓
BSP (板级支持包)
    ↓
Hardware (硬件)
```

## 设计目标

1. **业务逻辑封装**：实现具体的电源管理业务逻辑
2. **用户接口提供**：为上层应用提供简洁的 API
3. **系统集成**：整合下层所有功能，提供统一的服务
4. **可扩展性**：支持快速添加新的应用功能

## 目录结构

```
application/
├── include/
│   └── application.h          # 应用层核心接口
├── src/
│   └── application.c          # 应用层实现
├── examples/
│   └── app_demo.c             # 应用层演示程序
└── CMakeLists.txt             # CMake 构建配置
```

## 核心功能

### 1. 应用生命周期管理

#### 初始化与启动

```c
// 配置应用
app_config_t config;
config.app_name = "DigitalPowerManager";
config.version_major = 1;
config.version_minor = 0;
config.version_patch = 0;
config.heap_size = 65536;
config.task_stack_size = 2048;

// 初始化应用
app_init(&config);

// 启动应用
app_start();
```

#### 停止与清理

```c
// 停止应用
app_stop();

// 反初始化
app_deinit();
```

### 2. 电源管理控制

#### 电源模式管理

```c
// 设置电源模式
app_set_power_mode(POWER_MODE_ECO);        // 节能模式
app_set_power_mode(POWER_MODE_PERFORMANCE); // 高性能模式
app_set_power_mode(POWER_MODE_NORMAL);      // 正常模式

// 获取当前模式
power_mode_t mode = app_get_power_mode();
```

支持的电源模式：
- `POWER_MODE_NORMAL` - 正常操作模式
- `POWER_MODE_ECO` - 节能模式
- `POWER_MODE_PERFORMANCE` - 高性能模式
- `POWER_MODE_STANDBY` - 待机模式
- `POWER_MODE_SHUTDOWN` - 关机模式

#### 通道控制

```c
// 启用通道
app_enable_channel(1);
app_enable_channel(2);

// 禁用通道
app_disable_channel(1);

// 获取通道信息
power_channel_info_t info;
app_get_channel_info(1, &info);
printf("Channel 1: V=%.1fV, I=%.1fmA, P=%.1fmW\n",
       info.voltage_mv / 1000.0f,
       info.current_ma,
       info.power_mw);

// 获取所有通道
power_channel_info_t all_channels[4];
int count = app_get_all_channels(all_channels, 4);
```

### 3. 监控与诊断

#### 系统健康状态

```c
system_health_t health;
app_get_system_health(&health);

printf("CPU Load: %.1f%%\n", health.cpu_load_percent);
printf("Memory Usage: %.1f%%\n", health.memory_usage_percent);
printf("Uptime: %u seconds\n", health.total_uptime_seconds);
printf("Errors: %u\n", health.error_count);
```

#### 状态报告生成

```c
char report[2048];
int len = app_generate_status_report(report, sizeof(report));
if (len > 0) {
    printf("Status Report:\n%s\n", report);
}
```

生成的 JSON 格式报告示例：
```json
{
  "application": "DigitalPowerManager",
  "version": "1.0.0",
  "power_mode": "NORMAL",
  "uptime_ms": 12345,
  "channels": [
    {
      "id": 1,
      "status": "ON",
      "voltage_mv": 12000.0,
      "current_ma": 500.0,
      "power_mw": 6000.0
    },
    ...
  ]
}
```

#### 日志记录

```c
app_log(0, "This is a DEBUG message");
app_log(1, "This is an INFO message");
app_log(2, "This is a WARNING message");
app_log(3, "This is an ERROR message");
```

### 4. 任务管理

应用层提供三个预定义的任务：

#### 监控任务（Monitor Task）
- 优先级：MEDIUM
- 功能：监控系统健康状态
- 周期：5秒

```c
task_handle_t monitor_task = app_create_monitor_task();
```

#### 控制任务（Control Task）
- 优先级：HIGH
- 功能：检查和控制电源通道
- 周期：2秒

```c
task_handle_t control_task = app_create_control_task();
```

#### 通信任务（Communication Task）
- 优先级：LOW
- 功能：生成和发送状态报告
- 周期：10秒

```c
task_handle_t comm_task = app_create_communication_task();
```

### 5. 回调机制

#### 电源模式变化回调

```c
void on_power_mode_changed(power_mode_t old_mode, power_mode_t new_mode)
{
    printf("Power mode changed: %d -> %d\n", old_mode, new_mode);
}

app_register_power_mode_callback(on_power_mode_changed);
```

#### 通道状态变化回调

```c
void on_channel_status_changed(uint8_t channel_id,
                                channel_status_t old_status,
                                channel_status_t new_status)
{
    printf("Channel %d status: %d -> %d\n",
           channel_id, old_status, new_status);
}

app_register_channel_status_callback(on_channel_status_changed);
```

## 与下层集成

### 依赖关系

应用层只依赖系统层（System Layer），通过系统层间接使用下层功能：

```
Application
    ↓ (直接依赖)
System (RTOS 抽象、任务管理)
    ↓ (间接使用)
Component (JSON codec、协议栈)
    ↓ (间接使用)
Driver (设备驱动)
    ↓ (间接使用)
HAL (硬件抽象)
    ↓ (间接使用)
BSP (板级支持)
```

### 与 Component 层集成示例

```c
#include "application.h"
#include "json_codec.h"

// 创建综合系统报告
json_object_t report = json_object_create();

// 添加应用信息
json_object_add_string(report, "app_name", "DigitalPowerManager");
json_object_add_string(report, "version", "1.0.0");

// 添加电源模式
power_mode_t mode = app_get_power_mode();
json_object_add_string(report, "power_mode", 
                       (mode == POWER_MODE_NORMAL) ? "NORMAL" : "ECO");

// 添加系统健康
system_health_t health;
app_get_system_health(&health);
json_object_add_int(report, "uptime_seconds", health.total_uptime_seconds);

// 编码为 JSON
char buffer[2048];
json_object_encode(report, buffer, sizeof(buffer));

// 清理
json_object_delete(report);
```

## API 参考

### 应用生命周期

| 函数 | 说明 | 返回值 |
|------|------|--------|
| `app_init()` | 初始化应用 | 0=成功, <0=失败 |
| `app_start()` | 启动应用 | 0=成功, <0=失败 |
| `app_stop()` | 停止应用 | 0=成功, <0=失败 |
| `app_deinit()` | 反初始化应用 | 0=成功, <0=失败 |
| `app_is_initialized()` | 检查是否已初始化 | 1=是, 0=否 |
| `app_is_running()` | 检查是否正在运行 | 1=是, 0=否 |

### 电源管理

| 函数 | 说明 | 返回值 |
|------|------|--------|
| `app_set_power_mode()` | 设置电源模式 | 0=成功, <0=失败 |
| `app_get_power_mode()` | 获取当前电源模式 | 电源模式枚举值 |
| `app_enable_channel()` | 启用通道 | 0=成功, <0=失败 |
| `app_disable_channel()` | 禁用通道 | 0=成功, <0=失败 |
| `app_get_channel_info()` | 获取通道信息 | 0=成功, <0=失败 |
| `app_get_all_channels()` | 获取所有通道信息 | 通道数量, <0=失败 |

### 监控诊断

| 函数 | 说明 | 返回值 |
|------|------|--------|
| `app_get_system_health()` | 获取系统健康状态 | 0=成功, <0=失败 |
| `app_get_uptime_ms()` | 获取运行时间(ms) | 运行时间 |
| `app_generate_status_report()` | 生成状态报告(JSON) | 字符串长度, <0=失败 |
| `app_log()` | 记录日志 | 无 |

### 任务管理

| 函数 | 说明 | 返回值 |
|------|------|--------|
| `app_create_monitor_task()` | 创建监控任务 | 任务句柄, NULL=失败 |
| `app_create_control_task()` | 创建控制任务 | 任务句柄, NULL=失败 |
| `app_create_communication_task()` | 创建通信任务 | 任务句柄, NULL=失败 |

### 回调注册

| 函数 | 说明 | 返回值 |
|------|------|--------|
| `app_register_power_mode_callback()` | 注册电源模式回调 | 0=成功, <0=失败 |
| `app_register_channel_status_callback()` | 注册通道状态回调 | 0=成功, <0=失败 |

## 使用示例

完整的应用层使用示例请参考：
- [application/examples/app_demo.c](examples/app_demo.c)

运行演示：
```bash
cd build
cmake .. -G Ninja -DBUILD_EXAMPLES=ON
cmake --build .
./application/app_demo.exe
```

## 扩展开发

### 添加新的应用功能

1. 在 `application.h` 中添加新的 API 声明
2. 在 `application.c` 中实现功能
3. 更新 CMakeLists.txt（如需要）
4. 编写测试用例

### 自定义任务

```c
static void my_custom_task(void *arg)
{
    while (app_is_running()) {
        // 执行自定义逻辑
        
        system_delay_ms(1000);
    }
}

task_handle_t create_my_task(void)
{
    return system_task_create("MyTask", my_custom_task,
                              NULL, 2048, TASK_PRIORITY_MEDIUM);
}
```

## 注意事项

1. **依赖顺序**：必须先初始化下层（BSP → HAL → Driver → Component → System），再初始化应用层
2. **资源管理**：确保在 `app_deinit()` 中释放所有分配的资源
3. **线程安全**：应用层 API 在设计时考虑了线程安全，但回调函数中应避免阻塞操作
4. **错误处理**：始终检查 API 返回值，适当处理错误情况

## 性能指标

- **初始化时间**：< 10ms
- **任务切换开销**：取决于 RTOS 实现
- **内存占用**：约 8KB（包括任务栈）
- **CPU 负载**：< 5%（空闲状态）

## 版本历史

- **v1.0.0** (2026-04-10)
  - 初始版本
  - 完整的电源管理功能
  - 监控与诊断功能
  - 任务管理
  - 回调机制
