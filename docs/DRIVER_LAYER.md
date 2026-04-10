# Driver Layer Architecture

## 概述

Driver 层提供了标准的 POSIX-like 设备驱动接口，实现了应用程序与硬件的完全解耦。

## 架构层次

```
┌─────────────────────────────────┐
│      Application Layer          │  应用层
└──────────────┬──────────────────┘
               │ Uses standard interface
┌──────────────▼──────────────────┐
│       Driver Layer              │  驱动层 (新增)
│  - open/close/read/write/ctl    │
│  - Device registration          │
│  - Handle management            │
└──────────────┬──────────────────┘
               │ Depends on HAL only
┌──────────────▼──────────────────┐
│        HAL Layer                │  硬件抽象层
│  - Unified API                  │
│  - Platform independent         │
└──────────────┬──────────────────┘
               │ Depends on BSP
┌──────────────▼──────────────────┐
│        BSP Layer                │  板级支持包
│  - Platform-specific code       │
│  - Hardware initialization      │
└──────────────┬──────────────────┘
               │
┌──────────────▼──────────────────┐
│       Hardware                  │  硬件层
└─────────────────────────────────┘
```

## 依赖关系

**正确的依赖链：**
```
Application → Driver → HAL → BSP → Hardware
```

- **Driver** 只依赖 **HAL**（不直接访问 BSP）
- **HAL** 依赖 **BSP**
- **BSP** 是平台特定的实现

## 核心特性

### 1. POSIX-like 接口

提供标准的文件操作接口：

```c
// Open device
driver_handle_t handle = driver_open("led1", DEVICE_FLAG_WRONLY);

// Write to device
uint8_t value = 1;
driver_write(handle, &value, 1);

// Read from device
uint8_t state;
driver_read(handle, &state, 1);

// Control device
driver_control(handle, GPIO_CMD_SET_MODE, NULL);

// Close device
driver_close(handle);
```

### 2. 设备注册机制

驱动程序通过注册机制将设备添加到系统中：

```c
// Register GPIO device
gpio_drv_config_t config;
config.port = 0;
config.pin = 5;
config.mode = GPIO_DRV_MODE_OUTPUT;
gpio_device_register("led1", &config);

// Register UART device
uart_drv_config_t uart_config;
uart_config.baudrate = 115200;
uart_device_register("console", &uart_config);
```

### 3. 设备操作结构

每个设备类型实现 `driver_ops_t` 结构：

```c
typedef struct {
    driver_handle_t (*open)(const char *path, uint32_t flags);
    int (*close)(driver_handle_t handle);
    ssize_t (*read)(driver_handle_t handle, void *buffer, size_t size);
    ssize_t (*write)(driver_handle_t handle, const void *buffer, size_t size);
    int (*control)(driver_handle_t handle, int cmd, void *arg);
} driver_ops_t;
```

## 目录结构

```
driver/
├── CMakeLists.txt              # Driver 库构建配置
├── include/
│   ├── driver.h                # Driver 框架核心接口
│   ├── gpio_driver.h           # GPIO 驱动接口
│   └── uart_driver.h           # UART 驱动接口
├── src/
│   └── driver.c                # Driver 框架实现
├── devices/
│   ├── gpio_driver.c           # GPIO 驱动实现
│   └── uart_driver.c           # UART 驱动实现
└── examples/
    └── driver_demo.c           # 演示程序
```

## 已实现的驱动

### 1. GPIO Driver

- **头文件**: `gpio_driver.h`
- **实现**: `devices/gpio_driver.c`
- **功能**:
  - 打开/关闭 GPIO 设备
  - 读取/写入 GPIO 状态
  - 控制 GPIO 模式、上下拉等

**使用示例**:
```c
// 配置并注册 GPIO 设备
gpio_drv_config_t config;
config.port = 0;
config.pin = 5;
config.mode = GPIO_DRV_MODE_OUTPUT;
gpio_device_register("led1", &config);

// 使用设备
driver_handle_t led = driver_open("led1", DEVICE_FLAG_WRONLY);
uint8_t value = 1;
driver_write(led, &value, 1);  // 点亮 LED
driver_close(led);
```

### 2. UART Driver

- **头文件**: `uart_driver.h`
- **实现**: `devices/uart_driver.c`
- **功能**:
  - 打开/关闭 UART 设备
  - 发送/接收数据
  - 控制波特率、校验位等

**使用示例**:
```c
// 配置并注册 UART 设备
uart_drv_config_t config;
config.baudrate = 115200;
config.parity = UART_DRV_PARITY_NONE;
config.stop_bits = UART_DRV_STOP_1BIT;
config.data_bits = 8;
uart_device_register("console", &config);

// 使用设备
driver_handle_t uart = driver_open("console", DEVICE_FLAG_RDWR);
const char *msg = "Hello!\n";
driver_write(uart, msg, strlen(msg));
driver_close(uart);
```

## 编译配置

在 `driver/CMakeLists.txt` 中可以选择性启用/禁用驱动：

```cmake
option(DRIVER_ENABLE_GPIO "Enable GPIO driver" ON)
option(DRIVER_ENABLE_UART "Enable UART driver" ON)
option(DRIVER_ENABLE_SPI "Enable SPI driver" OFF)
option(DRIVER_ENABLE_I2C "Enable I2C driver" OFF)
option(DRIVER_ENABLE_ADC "Enable ADC driver" OFF)
option(DRIVER_ENABLE_TIMER "Enable Timer driver" OFF)
option(DRIVER_ENABLE_PWM "Enable PWM driver" OFF)
```

## 设计原则

### 1. 解耦

- 应用程序不关心底层硬件
- 驱动程序只依赖 HAL，不直接访问 BSP
- 可以轻松替换底层实现

### 2. 标准化

- 统一的 open/close/read/write/control 接口
- 类似 POSIX 的使用体验
- 降低学习成本

### 3. 可扩展

- 通过注册机制添加新设备
- 模块化设计，可选择性启用
- 易于添加新的驱动类型

### 4. 可移植

- 上层代码与平台无关
- 只需修改 BSP 层即可移植到新平台
- HAL 层提供统一的抽象

## 调用链示例

以 GPIO 写操作为例：

```
Application: driver_write(led_handle, &value, 1)
    ↓
Driver Framework: 查找 handle 对应的设备
    ↓
GPIO Driver: gpio_write() 被调用
    ↓
HAL Layer: hal_gpio_write(port, pin, value)
    ↓
BSP Layer: bsp_gpio_write(port, pin, value)
    ↓
Hardware: 实际硬件操作
```

**日志输出**:
```
[GPIO-DRV] Write: Port=0, Pin=5, Value=1
[BSP-GPIO] Write: Port=0, Pin=5, State=1
```

## 优势

1. **清晰的职责划分**: 每层只做自己的事
2. **易于测试**: 可以单独测试每一层
3. **易于维护**: 修改一层不影响其他层
4. **易于扩展**: 添加新设备只需实现驱动接口
5. **代码复用**: HAL 和 BSP 可以被多个驱动复用

## 总结

Driver 层的引入完成了完整的四层架构：

- **BSP**: 平台特定的硬件访问
- **HAL**: 硬件抽象，统一 API
- **Driver**: 设备管理，标准接口
- **Application**: 业务逻辑

这种架构使得系统具有极高的可维护性、可移植性和可扩展性。
