# 完整系统架构 - 六层设计

## 架构总览

```
┌─────────────────────────────────────────┐
│         Application Layer               │  应用层 - 业务逻辑
│  • Power Management Control            │
│  • Monitoring & Diagnostics             │
│  • Task Management                      │
└────────────────┬────────────────────────┘
                 │ 依赖 System
┌────────────────▼────────────────────────┐
│         System Layer                    │  系统层 - RTOS 抽象
│  • Task/Mutex/Semaphore/Queue          │
│  • OS Abstraction (FreeRTOS, etc.)     │
└────────────────┬────────────────────────┘
                 │ 依赖 Component
┌────────────────▼────────────────────────┐
│        Component Layer                  │  组件层 - 中间件封装
│  • JSON Codec, Modbus, MQTT, etc.      │
│  • Third-party software wrapper        │
└────────────────┬────────────────────────┘
                 │ 依赖 Driver
┌────────────────▼────────────────────────┐
│         Driver Layer                    │  驱动层 - 设备管理
│  • POSIX-like interface                │
│  • open/close/read/write/control       │
└────────────────┬────────────────────────┘
                 │ 依赖 HAL
┌────────────────▼────────────────────────┐
│          HAL Layer                      │  硬件抽象层
│  • Unified hardware API                │
│  • Platform independent                │
└────────────────┬────────────────────────┘
                 │ 依赖 BSP
┌────────────────▼────────────────────────┐
│          BSP Layer                      │  板级支持包
│  • Platform-specific code              │
│  • Hardware initialization             │
└────────────────┬────────────────────────┘
                 │
┌────────────────▼────────────────────────┐
│         Hardware                        │  硬件层
└─────────────────────────────────────────┘
```

## 依赖关系链

```
Application → System → Component → Driver → HAL → BSP → Hardware
```

**关键原则：**
- 上层可以调用下层
- 下层不知道上层的存在
- 每层只依赖直接的下层

## HAL 与 BSP 的设计原则

### 正确的依赖关系

```
✅ 正确: HAL 依赖 BSP (HAL 封装 BSP 接口)
❌ 错误: BSP 依赖 HAL (BSP 调用 HAL 接口)
```

### 职责划分

#### BSP (Board Support Package) - 底层实现

**职责：**
- 提供平台特定的原始硬件访问接口
- 直接与硬件寄存器或驱动交互
- 实现具体的硬件操作

**特点：**
- 平台相关（不同平台有不同的实现）
- 接口简单直接
- 不包含复杂的逻辑
- **不依赖 HAL**

**示例接口：**
```c
// BSP GPIO 接口 (底层、原始)
int bsp_gpio_init(uint8_t port, uint8_t pin, uint8_t mode);
void bsp_gpio_write(uint8_t port, uint8_t pin, uint8_t state);
uint8_t bsp_gpio_read(uint8_t port, uint8_t pin);
```

#### HAL (Hardware Abstraction Layer) - 上层封装

**职责：**
- 封装 BSP 提供的底层接口
- 提供统一的、跨平台的 API
- 添加额外的功能（超时控制、错误处理、数据转换等）

**特点：**
- 平台无关（同一套 API 用于所有平台）
- 接口友好、易用
- 包含业务逻辑和抽象
- **依赖 BSP**

**示例接口：**
```c
// HAL GPIO 接口 (高级、统一)
int hal_gpio_init(uint8_t port, const hal_gpio_config_t *config);
void hal_gpio_write(uint8_t port, hal_gpio_pin_t pin, uint8_t state);
uint8_t hal_gpio_read(uint8_t port, hal_gpio_pin_t pin);
```

### 调用流程示例

```c
// 应用层代码
hal_gpio_write(0, GPIO_PIN_5, 1);  // 调用 HAL
    ↓
// HAL 层实现 (hal/src/hal_gpio.c)
void hal_gpio_write(uint8_t port, hal_gpio_pin_t pin, uint8_t state) {
    bsp_gpio_write(port, (uint8_t)pin, state);  // HAL 调用 BSP
        ↓
    // BSP 层实现 (bsp/platforms/xxx/src/bsp_gpio_impl.c)
    void bsp_gpio_write(uint8_t port, uint8_t pin, uint8_t state) {
        // 实际的硬件寄存器操作
    }
}
```

### CMake 依赖配置

```cmake
# BSP CMakeLists.txt - BSP 不依赖 HAL
add_library(bsp STATIC ${BSP_SOURCES})

# HAL CMakeLists.txt - HAL 依赖 BSP
add_library(hal STATIC ${HAL_SOURCES})
target_link_libraries(hal PRIVATE bsp)  # HAL 链接 BSP
```

### 为什么这样设计？

1. **分层抽象**：应用开发者只需关心 "我要控制 GPIO"，不需要知道底层如何实现
2. **可移植性**：切换平台时只需更换 BSP，HAL 和应用代码无需修改
3. **职责清晰**：BSP 负责平台实现，HAL 负责接口抽象
4. **易于测试**：可以单独测试每一层，HAL 测试时可以 Mock BSP

### 常见误区

| 错误做法 | 正确做法 |
|---------|----------|
| BSP 调用 HAL 函数 | HAL 调用 BSP 函数 |
| 应用直接使用 BSP | 应用使用 HAL 接口 |
| BSP 包含 HAL 头文件 | HAL 包含 BSP 头文件 |

**类比说明：**
```
HAL = 汽车的方向盘、油门、刹车 (统一的操作接口)
BSP = 不同车型的传动系统 (平台特定的实现)
Hardware = 发动机、轮子 (物理硬件)

驾驶员 (应用) → 操作方向盘 (HAL) → 传动系统 (BSP) → 车轮转动 (Hardware)

- 驾驶员不需要知道是前驱还是后驱 (BSP 的细节)
- 方向盘的操作方式对所有车都一样 (HAL 的统一接口)
- 但不同车的传动系统可能不同 (BSP 的平台差异)
```

## 各层职责

### 1. BSP (Board Support Package) - 板级支持包

**职责：**
- 提供平台特定的硬件访问
- 硬件初始化和配置
- 最底层的硬件操作

**特点：**
- 与具体硬件平台强相关
- 不同平台有不同的实现
- 不包含任何业务逻辑

**示例文件：**
- `bsp/platforms/gcc_simulator/src/bsp_gpio_impl.c`
- `bsp/platforms/gcc_simulator/src/bsp_uart_impl.c`

**API 示例：**
```c
int bsp_gpio_init(uint8_t port, uint8_t pin, uint8_t mode);
void bsp_gpio_write(uint8_t port, uint8_t pin, uint8_t state);
uint8_t bsp_gpio_read(uint8_t port, uint8_t pin);
```

---

### 2. HAL (Hardware Abstraction Layer) - 硬件抽象层

**职责：**
- 封装 BSP，提供统一的硬件接口
- 屏蔽平台差异
- 为上层提供平台无关的 API

**特点：**
- 依赖 BSP
- 同一套 API 可用于不同平台
- 不包含设备管理逻辑

**示例模块：**
- GPIO, UART, SPI, I2C, ADC, Timer, PWM

**API 示例：**
```c
int hal_gpio_init(uint8_t port, const hal_gpio_config_t *config);
void hal_gpio_write(uint8_t port, uint8_t pin, uint8_t state);
uint8_t hal_gpio_read(uint8_t port, uint8_t pin);
```

---

### 3. Driver Layer - 驱动层

**职责：**
- 提供标准的 POSIX-like 设备接口
- 设备注册和管理
- 句柄管理和权限控制

**特点：**
- **只依赖 HAL**（不直接访问 BSP）
- 统一的 open/close/read/write/control 接口
- 支持多设备实例

**核心接口：**
```c
driver_handle_t driver_open(const char *path, uint32_t flags);
int driver_close(driver_handle_t handle);
ssize_t driver_read(driver_handle_t handle, void *buffer, size_t size);
ssize_t driver_write(driver_handle_t handle, const void *buffer, size_t size);
int driver_control(driver_handle_t handle, int cmd, void *arg);
```

**已实现驱动：**
- GPIO Driver (`gpio_driver.c`)
- UART Driver (`uart_driver.c`)

**使用示例：**
```c
// 打开设备
driver_handle_t led = driver_open("led1", DEVICE_FLAG_WRONLY);

// 写入数据
uint8_t value = 1;
driver_write(led, &value, 1);

// 关闭设备
driver_close(led);
```

---

### 4. Component Layer - 组件层

**职责：**
- 封装第三方软件和中间件
- 提供高级功能组件
- 隔离外部依赖

**特点：**
- **只依赖 Driver**（不直接访问 HAL/BSP）
- 组件可插拔、可替换
- 统一的组件管理框架

**已实现组件：**
- **JSON Codec**: JSON 编解码（模拟 cJSON）
- **Modbus Protocol**: Modbus RTU/TCP 协议栈（模拟 libmodbus）

**组件类型：**
- PROTOCOL: 协议栈 (Modbus, MQTT, etc.)
- FILESYSTEM: 文件系统 (FATFS, LittleFS, etc.)
- CRYPTO: 加密算法 (AES, RSA, etc.)
- CODEC: 编解码器 (JSON, XML, etc.)
- ALGORITHM: 算法 (PID, Filter, etc.)

**使用示例：**
```c
// JSON 编解码
json_object_t obj = json_object_create();
json_object_add_string(obj, "device", "meter");
json_object_add_int(obj, "voltage", 220);
char buffer[256];
json_object_encode(obj, buffer, sizeof(buffer));

// Modbus 通信
modbus_protocol_init(&config);
uint16_t regs[10];
modbus_read_holding_registers(1, 0, 10, regs);
```

---

### 5. Application Layer - 应用层

**职责：**
- 实现业务逻辑
- 调用组件和驱动接口
- 处理用户交互

**特点：**
- 不关心底层实现细节
- 通过标准接口访问硬件和功能
- 高度可移植

**典型应用：**
- 数字电源管理系统
- 工业控制系统
- IoT 设备应用

---

## 目录结构

```
digital_power_management_system/
├── bsp/                        # BSP 层
│   ├── include/
│   │   ├── bsp.h
│   │   ├── bsp_gpio.h
│   │   └── bsp_uart.h
│   ├── src/
│   │   └── bsp.c
│   └── platforms/
│       └── gcc_simulator/
│           └── src/
│               ├── bsp_platform.c
│               ├── bsp_gpio_impl.c
│               └── bsp_uart_impl.c
│
├── hal/                        # HAL 层
│   ├── include/
│   │   ├── hal.h
│   │   ├── hal_gpio.h
│   │   ├── hal_uart.h
│   │   └── ...
│   └── src/
│       ├── hal.c
│       ├── hal_gpio.c
│       └── hal_uart.c
│
├── driver/                     # Driver 层
│   ├── include/
│   │   ├── driver.h
│   │   ├── gpio_driver.h
│   │   └── uart_driver.h
│   ├── src/
│   │   └── driver.c
│   ├── devices/
│   │   ├── gpio_driver.c
│   │   └── uart_driver.c
│   └── examples/
│       └── driver_demo.c
│
├── component/                  # Component 层
│   ├── include/
│   │   ├── component.h
│   │   ├── json_codec.h
│   │   └── modbus_protocol.h
│   ├── src/
│   │   └── component.c
│   ├── middleware/
│   │   ├── json_codec.c
│   │   └── modbus_protocol.c
│   └── examples/
│       └── component_demo.c
│
├── system/                     # System 层
│   ├── include/
│   │   ├── system.h
│   │   ├── freertos_wrapper.h
│   │   └── threadx_wrapper.h
│   ├── src/
│   │   └── system.c
│   ├── os/
│   │   ├── freertos_wrapper.c
│   │   └── threadx_wrapper.c
│   └── examples/
│       └── system_demo.c
│
├── CMakeLists.txt              # 主构建配置
└── main.c                      # 主程序入口
```

---

## 完整调用链示例

### 场景：通过 Modbus 读取数据并打包为 JSON

```c
// ===== Application Layer =====
void read_and_send_data(void)
{
    // 1. 使用 Modbus 组件读取 PLC 数据
    uint16_t registers[5];
    modbus_read_holding_registers(1, 0, 5, registers);
    
    // 2. 使用 JSON 组件打包数据
    json_object_t data = json_object_create();
    for (int i = 0; i < 5; i++) {
        char key[16];
        snprintf(key, sizeof(key), "reg%d", i);
        json_object_add_int(data, key, registers[i]);
    }
    
    char json_str[512];
    json_object_encode(data, json_str, sizeof(json_str));
    
    // 3. 通过 UART 发送
    driver_handle_t uart = driver_open("uart0", DEVICE_FLAG_WRONLY);
    driver_write(uart, json_str, strlen(json_str));
    driver_close(uart);
    
    json_object_delete(data);
}
```

**调用链追踪：**

```
Application: modbus_read_holding_registers()
    ↓
Component: 构建 Modbus 帧
    ↓
Driver: driver_write(uart_handle, frame, size)
    ↓
Driver: uart_write() [UART Driver 实现]
    ↓
HAL: hal_uart_send()
    ↓
BSP: bsp_uart_write()
    ↓
Hardware: 实际串口发送
```

**日志输出：**
```
[MODBUS] Read Holding Registers: Slave=1, Addr=0, Count=5
[JSON-CODEC] Created new JSON object
[JSON-CODEC] Added int: reg0 = 0
[JSON-CODEC] Added int: reg1 = 1
...
[JSON-CODEC] Encoded JSON: {"reg0": 0, "reg1": 1, ...}
[DRIVER] Opening device 'uart0' with flags=0x0002
[BSP-UART0 TX] {"reg0": 0, "reg1": 1, ...}
```

---

## 编译和链接

### CMake 配置

主 `CMakeLists.txt` 按依赖顺序添加子目录：

```cmake
# 依赖顺序：从底层到高层
add_subdirectory(bsp)        # 最底层
add_subdirectory(hal)        # 依赖 BSP
add_subdirectory(driver)     # 依赖 HAL
add_subdirectory(component)  # 依赖 Driver
add_subdirectory(system)     # 依赖 Component

# 主程序链接所有库
target_link_libraries(${PROJECT_NAME} 
    PRIVATE 
    ${SYSTEM_LIBRARIES}      # 最高层
    ${COMPONENT_LIBRARIES}
    ${DRIVER_LIBRARIES}
    ${HAL_LIBRARIES}
    ${BSP_LIBRARIES}         # 最底层
)
```

### 模块化选项

每层都支持选择性启用/禁用模块：

**BSP:**
```cmake
set(BSP_PLATFORM "gcc_simulator" CACHE STRING "Target platform")
```

**HAL:**
```cmake
option(HAL_ENABLE_GPIO "Enable GPIO module" ON)
option(HAL_ENABLE_UART "Enable UART module" ON)
```

**Driver:**
```cmake
option(DRIVER_ENABLE_GPIO "Enable GPIO driver" ON)
option(DRIVER_ENABLE_UART "Enable UART driver" ON)
```

**Component:**
```cmake
option(COMPONENT_ENABLE_JSON_CODEC "Enable JSON codec" ON)
option(COMPONENT_ENABLE_MODBUS "Enable Modbus protocol" ON)
```

---

## 设计优势

### 1. 清晰的职责划分
- 每层只做自己的事
- 职责明确，易于理解

### 2. 高度的可移植性
- 更换平台只需修改 BSP
- 应用层代码完全不变

### 3. 易于测试
- 可以单独测试每一层
- Mock 下层进行单元测试

### 4. 易于维护
- 修改一层不影响其他层
- 问题定位快速准确

### 5. 可扩展性强
- 添加新硬件：实现新的 BSP
- 添加新驱动：实现 driver_ops
- 添加新组件：创建 middleware

### 6. 依赖隔离
- 第三方库被 Component 层隔离
- 应用层不直接依赖外部库

---

## 性能考虑

### 优点
- 分层清晰，便于优化
- 可以选择性跳过某些层（如直接使用 HAL）

### 注意事项
- 每层都有少量开销（函数调用）
- 对于实时性要求极高的场景，可以考虑层合并

---

## 最佳实践

### 1. 初始化顺序
```c
init_bsp();        // 1. 先初始化 BSP
hal_init();        // 2. 再初始化 HAL
driver_init();     // 3. 然后初始化 Driver
component_init();  // 4. 最后初始化 Component
```

### 2. 清理顺序（反向）
```c
component_deinit();  // 1. 先清理 Component
driver_deinit();     // 2. 再清理 Driver
hal_deinit();        // 3. 然后清理 HAL
deinit_bsp();        // 4. 最后清理 BSP
```

### 3. 错误处理
- 每层都应该检查返回值
- 记录详细的错误日志
- 优雅地处理失败情况

### 4. 资源管理
- 及时释放不再使用的资源
- 避免内存泄漏
- 正确关闭设备句柄

---

## 总结

这个五层架构提供了：

✅ **完整的抽象层次** - 从硬件到应用的完整链路  
✅ **严格的依赖管理** - 单向依赖，无循环依赖  
✅ **高度的模块化** - 每层独立可选  
✅ **优秀的可移植性** - 更换平台成本低  
✅ **清晰的职责划分** - 易于理解和维护  
✅ **良好的可扩展性** - 易于添加新功能  

这是一个工业级的嵌入式软件架构，适用于各种复杂的项目需求。
