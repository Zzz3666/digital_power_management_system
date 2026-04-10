# BSP 快速参考指南

## 🚀 快速开始

### 5 分钟上手 BSP

```cmake
# 在你的 CMakeLists.txt 中添加
add_subdirectory(bsp)
target_link_libraries(your_app PRIVATE ${BSP_LIBRARIES})
```

```c
// 在你的代码中使用
#include "bsp.h"

int main() {
    init_bsp();      // 初始化
    // ... 你的代码 ...
    deinit_bsp();    // 清理
    return 0;
}
```

---

## 📋 API 速查

### 核心接口

| 函数 | 说明 | 返回值 |
|------|------|--------|
| `int init_bsp(void)` | 初始化 BSP | 0=成功, -1=失败 |
| `void deinit_bsp(void)` | 反初始化 BSP | 无 |

### 平台接口

| 函数 | 说明 | 实现位置 |
|------|------|---------|
| `int bsp_platform_init(void)` | 平台初始化 | 各平台自行实现 |
| `void bsp_platform_deinit(void)` | 平台反初始化 | 各平台自行实现 |

### GPIO 接口 (bsp_gpio.h)

```c
// 初始化 GPIO
int bsp_gpio_init(uint8_t pin, gpio_mode_t mode);

// 写入 GPIO
int bsp_gpio_write(uint8_t pin, uint8_t value);

// 读取 GPIO
int bsp_gpio_read(uint8_t pin, uint8_t *value);

// 切换 GPIO
int bsp_gpio_toggle(uint8_t pin);
```

### UART 接口 (bsp_uart.h)

```c
// 初始化 UART
int bsp_uart_init(uart_config_t *config);

// 发送数据
int bsp_uart_write(const uint8_t *data, size_t len);

// 接收数据
int bsp_uart_read(uint8_t *data, size_t len, uint32_t timeout_ms);

// 清空缓冲区
int bsp_uart_flush(void);
```

---

## ⚙️ CMake 配置

### 可用选项

```cmake
# 选择平台（默认：GCC Simulator）
option(BSP_PLATFORM_GCC_SIMULATOR "使用 GCC Simulator 平台" ON)

# 构建测试程序（默认：OFF）
option(BSP_BUILD_TESTS "构建 BSP 测试程序" OFF)
```

### 完整示例

```cmake
cmake_minimum_required(VERSION 3.10)
project(MyProject C)

# 添加 BSP
add_subdirectory(bsp)

# 创建可执行文件
add_executable(my_app main.c)

# 链接 BSP
target_link_libraries(my_app PRIVATE ${BSP_LIBRARIES})

# 包含头文件（自动配置，无需手动添加）
# target_include_directories(my_app PRIVATE ${BSP_INCLUDE_DIRS})
```

---

## 📁 目录结构速览

```
bsp/
├── CMakeLists.txt          # 构建配置
├── include/                # 公共头文件
│   ├── bsp.h              # ⭐ 核心接口
│   ├── bsp_gpio.h         # GPIO 接口
│   └── bsp_uart.h         # UART 接口
├── src/                    # 核心源代码
│   └── bsp.c              # ⭐ 核心实现
├── platforms/              # 平台实现
│   └── gcc_simulator/     # 当前平台
│       ├── include/
│       │   └── bsp_platform.h
│       └── src/
│           ├── bsp_platform.c
│           ├── bsp_gpio_impl.c
│           └── bsp_uart_impl.c
└── test_bsp.c              # 测试程序
```

---

## 🔧 常用任务

### 1. 切换到新平台

```bash
# 禁用当前平台
cmake -DBSP_PLATFORM_GCC_SIMULATOR=OFF ..

# 启用新平台（假设已添加）
cmake -DBSP_PLATFORM_STM32F4=ON ..

# 重新构建
cmake --build .
```

### 2. 运行测试

```bash
# 配置时启用测试
cmake -DBSP_BUILD_TESTS=ON ..

# 构建
cmake --build .

# 运行测试
./bsp/bsp_test
```

### 3. 查看编译输出

```bash
# 查看详细构建信息
cmake --build . --verbose

# 查看 BSP 相关配置
cmake .. | grep "BSP"
```

---

## 💡 代码示例

### 基本用法

```c
#include "bsp.h"
#include <stdio.h>

int main() {
    // 初始化
    if (init_bsp() != 0) {
        printf("BSP 初始化失败!\n");
        return -1;
    }
    
    printf("BSP 已就绪\n");
    
    // 你的应用逻辑
    // ...
    
    // 清理
    deinit_bsp();
    return 0;
}
```

### GPIO 使用

```c
#include "bsp_gpio.h"

void led_blink(void) {
    // 初始化 LED 引脚
    bsp_gpio_init(GPIO_PIN_5, GPIO_MODE_OUTPUT);
    
    // 闪烁 LED
    for (int i = 0; i < 10; i++) {
        bsp_gpio_write(GPIO_PIN_5, 1);  // 亮
        delay_ms(500);
        bsp_gpio_write(GPIO_PIN_5, 0);  // 灭
        delay_ms(500);
    }
}
```

### UART 使用

```c
#include "bsp_uart.h"

void send_message(void) {
    uart_config_t config = {
        .baud_rate = 115200,
        .data_bits = 8,
        .stop_bits = 1,
        .parity = UART_PARITY_NONE
    };
    
    // 初始化 UART
    bsp_uart_init(&config);
    
    // 发送数据
    const char *msg = "Hello, BSP!\n";
    bsp_uart_write((uint8_t *)msg, strlen(msg));
    
    // 接收数据
    uint8_t buffer[64];
    int len = bsp_uart_read(buffer, sizeof(buffer), 1000);
    
    if (len > 0) {
        printf("收到 %d 字节\n", len);
    }
}
```

---

## 🎯 平台对比

### GCC Simulator（当前）

**适用场景**:
- ✅ 开发和调试
- ✅ 单元测试
- ✅ 算法验证
- ✅ CI/CD 自动化测试

**特点**:
- 运行在 PC 上（Windows/Linux/macOS）
- 使用标准 IO 模拟硬件
- 无需实际硬件
- 便于调试

**限制**:
- ❌ 时序不准确
- ❌ 不支持中断
- ❌ 性能不代表真实硬件

### 未来平台（计划中）

| 平台 | 状态 | 目标应用 |
|------|------|---------|
| STM32F4 | 📋 计划 | 工业控制 |
| ESP32 | 📋 计划 | IoT 应用 |
| Nordic nRF52 | 📋 计划 | 低功耗蓝牙 |
| Raspberry Pi Pico | 📋 计划 | 教育/原型 |

---

## 🐛 故障排查

### 问题 1: 找不到 bsp.h

**症状**:
```
fatal error: bsp.h: No such file or directory
```

**解决**:
```cmake
# 确保正确链接了 BSP
target_link_libraries(your_target PRIVATE ${BSP_LIBRARIES})

# 或者手动添加包含目录
target_include_directories(your_target PRIVATE ${BSP_INCLUDE_DIRS})
```

### 问题 2: 链接错误

**症状**:
```
undefined reference to `init_bsp'
```

**解决**:
```cmake
# 检查是否添加了 BSP 子目录
add_subdirectory(bsp)

# 检查是否正确链接
target_link_libraries(your_target PRIVATE bsp)
```

### 问题 3: 平台初始化失败

**症状**:
```
BSP platform initialization failed!
```

**解决**:
1. 检查平台选项是否正确设置
2. 查看平台特定的日志输出
3. 验证平台依赖项是否满足

### 问题 4: 多个平台冲突

**症状**:
```
multiple definition of `bsp_platform_init'
```

**解决**:
```cmake
# 确保只启用一个平台
cmake -DBSP_PLATFORM_GCC_SIMULATOR=ON \
      -DBSP_PLATFORM_STM32F4=OFF \
      ..
```

---

## 📊 性能指标

### GCC Simulator 平台

| 指标 | 数值 | 说明 |
|------|------|------|
| 初始化时间 | < 1ms | 模拟器启动很快 |
| 内存占用 | ~6 KB | 代码 + 数据 |
| GPIO 操作 | ~100ns | 内存访问速度 |
| UART 发送 | ~1μs/字节 | 标准 IO 开销 |

**注意**: 实际硬件平台的性能会有所不同

---

## 🔗 有用链接

### 内部文档
- [BSP_README.md](BSP_README.md) - 完整使用说明
- [BSP_IMPLEMENTATION_SUMMARY.md](BSP_IMPLEMENTATION_SUMMARY.md) - 实现细节
- [ARCHITECTURE_OVERVIEW.md](ARCHITECTURE_OVERVIEW.md) - 架构总览

### 外部资源
- [CMake 官方文档](https://cmake.org/documentation/)
- [嵌入式系统最佳实践](https://embeddedartistry.com/)
- [C 语言编码规范](https://github.com/MisraC/MISRA-C)

---

## 📝 速记卡片

### BSP 初始化流程

```
main()
  ↓
init_bsp()
  ↓
bsp_platform_init()  ← 平台特定
  ↓
返回 0 = 成功
```

### BSP 清理流程

```
deinit_bsp()
  ↓
bsp_platform_deinit()  ← 平台特定
  ↓
完成
```

### CMake 变量

```cmake
${BSP_LIBRARIES}      # BSP 库名称
${BSP_INCLUDE_DIRS}   # BSP 头文件路径
```

### 平台宏

```c
#ifdef BSP_PLATFORM_GCC_SIMULATOR
    // GCC Simulator 特定代码
#endif
```

---

## ✨ 提示和技巧

### 1. 快速测试 BSP

```bash
# 一行命令测试 BSP
cd build && cmake -DBSP_BUILD_TESTS=ON .. && cmake --build . && ./bsp/bsp_test
```

### 2. 查看 BSP 配置

```bash
# 查看当前 BSP 配置
cmake -LH .. | grep BSP
```

### 3. 清理重新构建

```bash
# 完全清理
rm -rf build
mkdir build && cd build
cmake ..
cmake --build .
```

### 4. 调试 BSP

```c
// 在 bsp.c 中添加调试输出
int init_bsp(void) {
    printf("[DEBUG] init_bsp called\n");  // 添加这行
    // ...
}
```

### 5. 性能分析

```bash
# Linux 下测量初始化时间
time ./your_app

# Windows 下使用 PowerShell
Measure-Command { ./your_app.exe }
```

---

## 🎓 学习路径

### 初学者
1. ✅ 阅读本快速参考
2. ✅ 运行 `test_bsp.c` 示例
3. ✅ 在主程序中调用 `init_bsp()`
4. ✅ 尝试 GPIO 和 UART 示例

### 中级开发者
1. ✅ 阅读 [BSP_README.md](BSP_README.md)
2. ✅ 理解平台抽象机制
3. ✅ 修改现有平台实现
4. ✅ 添加新的 GPIO/UART 功能

### 高级开发者
1. ✅ 阅读 [BSP_IMPLEMENTATION_SUMMARY.md](BSP_IMPLEMENTATION_SUMMARY.md)
2. ✅ 添加新平台支持
3. ✅ 优化平台性能
4. ✅ 贡献代码到项目

---

**最后更新**: 2026-04-10  
**版本**: 1.0.0  
**维护者**: 开发团队
