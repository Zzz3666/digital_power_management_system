# HAL (Hardware Abstraction Layer) 库

## 概述

HAL 库提供了统一的硬件抽象接口，将上层应用与底层硬件平台解耦。BSP 负责对接不同平台，HAL 提供标准化的硬件访问接口。

## 架构设计

```
┌─────────────────────────────────────┐
│      Application Layer              │  应用层
├─────────────────────────────────────┤
│         BSP Layer                   │  板级支持包 (平台特定)
├─────────────────────────────────────┤
│         HAL Layer                   │  硬件抽象层 (统一接口)
├─────────────────────────────────────┤
│      Hardware Platform              │  硬件平台
└─────────────────────────────────────┘
```

## 目录结构

```
hal/
├── CMakeLists.txt          # HAL 库构建配置
├── include/                # 公共头文件
│   ├── hal.h              # HAL 主接口
│   ├── hal_gpio.h         # GPIO 接口
│   ├── hal_uart.h         # UART 接口
│   ├── hal_spi.h          # SPI 接口
│   ├── hal_i2c.h          # I2C 接口
│   ├── hal_adc.h          # ADC 接口
│   ├── hal_timer.h        # Timer 接口
│   └── hal_pwm.h          # PWM 接口
├── src/                    # 核心源代码
│   └── hal.c              # HAL 核心实现
└── drivers/                # 驱动实现 (按模块分类)
    ├── gpio/              # GPIO 驱动
    ├── uart/              # UART 驱动
    ├── spi/               # SPI 驱动
    ├── i2c/               # I2C 驱动
    ├── adc/               # ADC 驱动
    ├── timer/             # Timer 驱动
    └── pwm/               # PWM 驱动
```

## 支持的模块

### 1. GPIO (通用输入输出)
- 引脚初始化/反初始化
- 读写操作
- 上下拉配置
- 输出速度控制

### 2. UART (通用异步收发器)
- 多种波特率支持
- 数据位/停止位/校验位配置
- 发送/接收数据
- 超时控制

### 3. SPI (串行外设接口)
- 4 种 SPI 模式
- 可配置时钟速度
- 全双工/半双工通信
- 超时控制

### 4. I2C (集成电路总线)
- 标准/快速/快速+ 模式
- 7位地址支持
- 内存读写操作
- 超时控制

### 5. ADC (模数转换器)
- 多分辨率支持 (8/10/12/16位)
- 多通道读取
- 电压值转换

### 6. Timer (定时器)
- 单次/周期模式
- 回调函数支持
- 毫秒/微秒延时

### 7. PWM (脉冲宽度调制)
- 频率可调
- 占空比控制 (0-100%)
- 启动/停止控制

## 构建配置

### CMake 选项

| 选项 | 默认值 | 说明 |
|------|--------|------|
| `HAL_ENABLE_GPIO` | ON | 启用 GPIO 模块 |
| `HAL_ENABLE_UART` | ON | 启用 UART 模块 |
| `HAL_ENABLE_SPI` | ON | 启用 SPI 模块 |
| `HAL_ENABLE_I2C` | ON | 启用 I2C 模块 |
| `HAL_ENABLE_ADC` | ON | 启用 ADC 模块 |
| `HAL_ENABLE_TIMER` | ON | 启用 Timer 模块 |
| `HAL_ENABLE_PWM` | ON | 启用 PWM 模块 |

### 使用方法

在主项目的 CMakeLists.txt 中：

```cmake
# 添加 HAL 子目录
add_subdirectory(hal)

# 链接 HAL 库
target_link_libraries(your_target PRIVATE ${HAL_LIBRARIES})
```

### 选择性启用模块

```cmake
# 只启用需要的模块以减小程序体积
set(HAL_ENABLE_GPIO ON CACHE BOOL "" FORCE)
set(HAL_ENABLE_UART ON CACHE BOOL "" FORCE)
set(HAL_ENABLE_SPI OFF CACHE BOOL "" FORCE)
set(HAL_ENABLE_I2C OFF CACHE BOOL "" FORCE)
set(HAL_ENABLE_ADC OFF CACHE BOOL "" FORCE)
set(HAL_ENABLE_TIMER ON CACHE BOOL "" FORCE)
set(HAL_ENABLE_PWM OFF CACHE BOOL "" FORCE)

add_subdirectory(hal)
```

## API 使用示例

### GPIO 示例

```c
#include "hal.h"

// 配置 GPIO
hal_gpio_config_t config = {
    .pin = GPIO_PIN_5,
    .mode = GPIO_MODE_OUTPUT,
    .pull = GPIO_NOPULL,
    .speed = GPIO_SPEED_MEDIUM
};

// 初始化 GPIO
hal_gpio_init(0, &config);

// 控制 GPIO
hal_gpio_write(0, GPIO_PIN_5, 1);  // 置高
hal_gpio_write(0, GPIO_PIN_5, 0);  // 置低
hal_gpio_toggle(0, GPIO_PIN_5);    // 翻转

// 读取 GPIO
uint8_t state = hal_gpio_read(0, GPIO_PIN_5);
```

### UART 示例

```c
#include "hal.h"

// 配置 UART
hal_uart_config_t config = {
    .baud = UART_BAUD_115200,
    .data_bits = UART_DATA_8BITS,
    .stop_bits = UART_STOP_1BIT,
    .parity = UART_PARITY_NONE
};

// 初始化 UART
hal_uart_init(UART_0, &config);

// 发送数据
const char *msg = "Hello, World!\n";
hal_uart_send(UART_0, (uint8_t*)msg, strlen(msg), 1000);

// 接收数据
uint8_t buffer[64];
int len = hal_uart_receive(UART_0, buffer, sizeof(buffer), 1000);
```

### I2C 示例

```c
#include "hal.h"

// 配置 I2C
hal_i2c_config_t config = {
    .speed = I2C_SPEED_STANDARD
};

// 初始化 I2C
hal_i2c_init(I2C_0, &config);

// 写入寄存器
uint8_t reg_data = 0x55;
hal_i2c_mem_write(I2C_0, 0x68, 0x00, &reg_data, 1, 100);

// 读取寄存器
uint8_t data;
hal_i2c_mem_read(I2C_0, 0x68, 0x00, &data, 1, 100);
```

### Timer 示例

```c
#include "hal.h"

// 定时器回调函数
void timer_callback(void *arg) {
    printf("Timer expired!\n");
}

// 配置定时器
hal_timer_config_t config = {
    .mode = TIMER_MODE_PERIODIC,
    .period_ms = 1000,
    .callback = timer_callback,
    .callback_arg = NULL
};

// 初始化并启动定时器
hal_timer_init(TIMER_0, &config);
hal_timer_start(TIMER_0);

// 延时
hal_delay_ms(500);  // 延时 500ms
hal_delay_us(100);  // 延时 100us
```

### PWM 示例

```c
#include "hal.h"

// 配置 PWM
hal_pwm_config_t config = {
    .frequency = 1000,     // 1kHz
    .duty_cycle = 50       // 50% 占空比
};

// 初始化并启动 PWM
hal_pwm_init(PWM_0, &config);
hal_pwm_start(PWM_0);

// 动态调整占空比
hal_pwm_set_duty_cycle(PWM_0, 75);  // 改为 75%
```

## 移植指南

### 为新的硬件平台添加驱动

1. **在 drivers 目录下创建平台特定文件夹**
   ```
   hal/drivers/gpio/stm32/
   hal/drivers/uart/stm32/
   ...
   ```

2. **实现 HAL 接口**
   
   例如 `hal/drivers/gpio/stm32/hal_gpio_stm32.c`:
   ```c
   #include "hal_gpio.h"
   
   int hal_gpio_init(uint8_t port, const hal_gpio_config_t *config) {
       // STM32 特定的 GPIO 初始化代码
       // ...
       return 0;
   }
   
   void hal_gpio_write(uint8_t port, hal_gpio_pin_t pin, uint8_t state) {
       // STM32 特定的写操作
       // ...
   }
   
   // 实现其他函数...
   ```

3. **更新 CMakeLists.txt**
   
   添加平台选择和编译逻辑：
   ```cmake
   option(HAL_PLATFORM_STM32 "使用 STM32 平台" OFF)
   
   if(HAL_PLATFORM_STM32)
       file(GLOB STM32_GPIO_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/drivers/gpio/stm32/*.c")
       list(APPEND HAL_SOURCES ${STM32_GPIO_SOURCES})
       add_definitions(-DHAL_PLATFORM_STM32)
   endif()
   ```

4. **编译时使用对应平台**
   ```bash
   cmake .. -DHAL_PLATFORM_STM32=ON
   ```

## 与 BSP 的关系

- **HAL**: 提供统一的硬件抽象接口，与平台无关
- **BSP**: 负责特定平台的初始化和配置，调用 HAL 接口

典型初始化流程：
```c
// 应用层
int main() {
    // 1. 初始化 BSP (内部会初始化 HAL)
    init_bsp();
    
    // 2. 使用 HAL 接口访问硬件
    hal_gpio_init(0, &gpio_config);
    hal_uart_init(UART_0, &uart_config);
    
    // 3. 应用程序逻辑
    // ...
    
    // 4. 清理资源
    deinit_bsp();
    return 0;
}
```

## 最佳实践

1. **始终检查返回值**
   ```c
   if (hal_gpio_init(0, &config) != 0) {
       // 处理错误
   }
   ```

2. **合理配置超时时间**
   ```c
   // 根据实际需求设置超时
   hal_uart_send(UART_0, data, size, 100);  // 100ms 超时
   ```

3. **及时释放资源**
   ```c
   // 使用完毕后反初始化
   hal_uart_deinit(UART_0);
   hal_gpio_deinit(0, GPIO_PIN_5);
   ```

4. **模块化设计**
   - 只启用需要的模块
   - 避免不必要的依赖

## 版本历史

- **v1.0.0** (2026-04-10)
  - 初始版本
  - 支持 7 个主要硬件模块
  - 完整的 CMake 构建系统

## 许可证

详见项目根目录的 LICENSE 文件
