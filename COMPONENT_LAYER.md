# Component Layer Architecture

## 概述

Component 层（组件层）位于 Driver 层之上，负责第三方软件/中间件的封装和抽象，为上层应用提供统一的组件接口。

## 架构层次

```
┌─────────────────────────────────┐
│      Application Layer          │  应用层
└──────────────┬──────────────────┘
               │ Uses component API
┌──────────────▼──────────────────┐
│     Component Layer             │  组件层 (新增)
│  - Middleware encapsulation     │
│  - Third-party software wrapper │
│  - Component management         │
└──────────────┬──────────────────┘
               │ Depends on Driver only
┌──────────────▼──────────────────┐
│       Driver Layer              │  驱动层
│  - POSIX-like interface         │
│  - Device management            │
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

**完整的五层架构：**
```
Application → Component → Driver → HAL → BSP → Hardware
```

- **Component** 只依赖 **Driver**（不直接访问 HAL/BSP）
- **Driver** 依赖 **HAL**
- **HAL** 依赖 **BSP**
- **BSP** 是平台特定的实现

## 核心特性

### 1. 组件管理框架

提供组件的注册、查找和管理功能：

```c
// Initialize component layer
component_init();

// Register a component
component_info_t info;
info.name = "json_codec";
info.type = COMPONENT_TYPE_CODEC;
info.version = "1.0.0";
component_register(&info);

// Find component
const component_info_t *comp = component_find("json_codec");

// Get component status
component_status_t status = component_get_status("json_codec");
```

### 2. 中间件封装

Component 层封装常见的第三方库：

#### 已实现的组件

##### JSON Codec
- **头文件**: `include/json_codec.h`
- **实现**: `middleware/json_codec.c`
- **功能**: JSON 编解码（模拟 cJSON）
- **用途**: 数据序列化/反序列化

**使用示例**:
```c
// Initialize
json_codec_config_t config;
config.max_depth = 10;
config.max_size = 4096;
json_codec_init(&config);

// Create JSON object
json_object_t obj = json_object_create();
json_object_add_string(obj, "device", "power_meter");
json_object_add_int(obj, "voltage", 220);
json_object_add_bool(obj, "enabled", 1);

// Encode to string
char buffer[512];
json_object_encode(obj, buffer, sizeof(buffer));

// Decode from string
json_object_t decoded = json_object_decode(buffer);
int voltage;
json_object_get_int(decoded, "voltage", &voltage);

// Cleanup
json_object_delete(obj);
json_codec_deinit();
```

##### Modbus Protocol
- **头文件**: `include/modbus_protocol.h`
- **实现**: `middleware/modbus_protocol.c`
- **功能**: Modbus RTU/TCP 协议栈（模拟 libmodbus）
- **用途**: 工业通信协议

**使用示例**:
```c
// Initialize
modbus_config_t config;
config.mode = MODBUS_MODE_RTU;
config.role = MODBUS_ROLE_MASTER;
config.slave_id = 1;
config.port_name = "uart0";
config.baudrate = 9600;
modbus_protocol_init(&config);

// Read registers
uint16_t regs[10];
modbus_read_holding_registers(1, 0, 10, regs);

// Write register
modbus_write_single_register(1, 100, 1234);

// Write coil
modbus_write_single_coil(1, 0, 1);

// Cleanup
modbus_protocol_deinit();
```

### 3. 组件类型系统

支持多种组件类型：

```c
typedef enum {
    COMPONENT_TYPE_PROTOCOL = 0,    // 协议栈 (Modbus, MQTT, etc.)
    COMPONENT_TYPE_FILESYSTEM,      // 文件系统 (FATFS, LittleFS, etc.)
    COMPONENT_TYPE_CRYPTO,          // 加密 (AES, RSA, etc.)
    COMPONENT_TYPE_CODEC,           // 编解码 (JSON, XML, etc.)
    COMPONENT_TYPE_ALGORITHM,       // 算法 (PID, Filter, etc.)
    COMPONENT_TYPE_CUSTOM,          // 自定义组件
} component_type_t;
```

### 4. 组件状态管理

跟踪组件的运行状态：

```c
typedef enum {
    COMPONENT_STATUS_UNINIT = 0,    // 未初始化
    COMPONENT_STATUS_READY,         // 就绪
    COMPONENT_STATUS_RUNNING,       // 运行中
    COMPONENT_STATUS_ERROR,         // 错误
} component_status_t;
```

## 目录结构

```
component/
├── CMakeLists.txt              # Component 库配置
├── include/
│   ├── component.h             # Component 框架核心接口
│   ├── json_codec.h            # JSON 编解码组件接口
│   └── modbus_protocol.h       # Modbus 协议组件接口
├── src/
│   └── component.c             # Component 框架实现
├── middleware/
│   ├── json_codec.c            # JSON codec 实现
│   └── modbus_protocol.c       # Modbus protocol 实现
└── examples/
    └── component_demo.c        # 演示程序
```

## 编译配置

在 `component/CMakeLists.txt` 中可以选择性启用/禁用组件：

```cmake
option(COMPONENT_ENABLE_JSON_CODEC "Enable JSON codec component" ON)
option(COMPONENT_ENABLE_MODBUS "Enable Modbus protocol component" ON)
option(COMPONENT_ENABLE_FILESYSTEM "Enable Filesystem component" OFF)
option(COMPONENT_ENABLE_CRYPTO "Enable Cryptography component" OFF)
```

## 设计原则

### 1. 解耦第三方依赖

- 应用程序不直接依赖第三方库
- Component 层统一封装所有外部依赖
- 可以轻松替换底层实现（如 cJSON → jansson）

### 2. 标准化接口

- 统一的组件初始化和清理接口
- 一致的 API 设计风格
- 降低集成难度

### 3. 模块化设计

- 每个组件独立可选
- 按需启用/禁用
- 减少内存占用

### 4. 依赖隔离

- Component 只依赖 Driver
- 不直接访问 HAL/BSP
- 通过 Driver 的标准接口访问硬件

## 典型应用场景

### 1. 数据序列化

```c
// 使用 JSON codec 打包传感器数据
json_object_t data = json_object_create();
json_object_add_int(data, "temperature", 25);
json_object_add_int(data, "humidity", 60);
char buffer[256];
json_object_encode(data, buffer, sizeof(buffer));

// 通过 UART 发送
driver_handle_t uart = driver_open("uart0", DEVICE_FLAG_WRONLY);
driver_write(uart, buffer, strlen(buffer));
```

### 2. 工业通信

```c
// 使用 Modbus 读取 PLC 数据
modbus_protocol_init(&config);
uint16_t regs[10];
modbus_read_holding_registers(1, 0, 10, regs);

// 解析数据并打包为 JSON
json_object_t response = json_object_create();
for (int i = 0; i < 10; i++) {
    char key[16];
    snprintf(key, sizeof(key), "reg%d", i);
    json_object_add_int(response, key, regs[i]);
}
```

### 3. 配置文件处理

```c
// 从文件读取 JSON 配置
FILE *f = fopen("config.json", "r");
fread(buffer, 1, sizeof(buffer), f);
fclose(f);

// 解析配置
json_object_t config = json_object_decode(buffer);
int baudrate;
json_object_get_int(config, "baudrate", &baudrate);
```

## 扩展新组件

添加新组件的步骤：

1. **创建头文件** (`include/new_component.h`)
```c
#ifndef NEW_COMPONENT_H
#define NEW_COMPONENT_H

#include "component.h"

int new_component_init(void);
void new_component_deinit(void);
// ... 其他 API

#endif
```

2. **创建实现文件** (`middleware/new_component.c`)
```c
#include "new_component.h"
#include "driver.h"

int new_component_init(void)
{
    // 初始化逻辑
    component_info_t info;
    info.name = "new_component";
    info.type = COMPONENT_TYPE_CUSTOM;
    info.version = "1.0.0";
    component_register(&info);
    return 0;
}
```

3. **更新 CMakeLists.txt**
```cmake
option(COMPONENT_ENABLE_NEW "Enable new component" OFF)
if(COMPONENT_ENABLE_NEW)
    list(APPEND COMPONENT_SOURCES middleware/new_component.c)
endif()
```

## 调用链示例

以 Modbus 写寄存器为例：

```
Application: modbus_write_single_register(slave, addr, value)
    ↓
Modbus Component: 构建 Modbus 帧
    ↓
Driver Layer: driver_write(uart_handle, frame, size)
    ↓
UART Driver: uart_write() 被调用
    ↓
HAL Layer: hal_uart_send()
    ↓
BSP Layer: bsp_uart_write()
    ↓
Hardware: 实际串口发送
```

**日志输出**:
```
[MODBUS] Write Single Register: Slave=1, Addr=100, Value=1234
[DRIVER] Opening device 'uart0' with flags=0x0004
[BSP-UART0 TX] [Modbus Frame Data]
[MODBUS] Sent 8 bytes via UART
```

## 优势

1. **第三方库隔离**: 应用层不直接依赖外部库
2. **易于替换**: 可以无缝切换不同的实现
3. **统一管理**: 所有组件通过统一框架管理
4. **状态跟踪**: 实时监控组件运行状态
5. **按需加载**: 只启用需要的组件，节省资源

## 与 Driver 层的区别

| 特性 | Driver 层 | Component 层 |
|------|-----------|--------------|
| 职责 | 设备管理 | 中间件封装 |
| 接口 | POSIX-like (open/close/read/write) | 功能特定 API |
| 依赖 | HAL | Driver |
| 示例 | GPIO, UART, SPI | JSON, Modbus, MQTT |
| 粒度 | 硬件设备 | 软件功能 |

## 总结

Component 层的引入完成了完整的五层架构：

- **BSP**: 平台特定的硬件访问
- **HAL**: 硬件抽象，统一 API
- **Driver**: 设备管理，标准接口
- **Component**: 中间件封装，第三方库抽象
- **Application**: 业务逻辑

这种架构使得：
- ✅ 第三方依赖完全隔离
- ✅ 组件可插拔、可替换
- ✅ 应用层代码简洁清晰
- ✅ 系统高度模块化和可扩展
