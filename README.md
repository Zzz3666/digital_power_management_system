# Digital Power Management System

数字电源管理系统 - 一个完整的六层嵌入式软件框架

## 项目结构

```
digital_power_management_system/
├── application/            # 应用层 (Application Layer)
│   ├── CMakeLists.txt     # 应用层构建配置
│   ├── include/           # 应用层公共头文件
│   ├── src/               # 应用层源代码
│   ├── examples/          # 应用层示例程序
│   └── APPLICATION_LAYER.md  # 应用层详细文档
├── system/                 # 系统层 (System Layer)
│   ├── CMakeLists.txt     # 系统层构建配置
│   ├── include/           # 系统层公共头文件
│   ├── src/               # 系统层源代码
│   ├── os/                # RTOS 包装器
│   ├── examples/          # 系统层示例程序
│   └── SYSTEM_LAYER.md    # 系统层详细文档
├── component/              # 组件层 (Component Layer)
│   ├── CMakeLists.txt     # 组件层构建配置
│   ├── include/           # 组件层公共头文件
│   ├── src/               # 组件层源代码
│   └── COMPONENT_LAYER.md # 组件层详细文档
├── driver/                 # 驱动层 (Driver Layer)
│   ├── CMakeLists.txt     # 驱动层构建配置
│   ├── include/           # 驱动层公共头文件
│   ├── src/               # 驱动层源代码
│   └── DRIVER_LAYER.md    # 驱动层详细文档
├── hal/                    # 硬件抽象层 (HAL)
│   ├── CMakeLists.txt     # HAL 库构建配置
│   ├── include/           # HAL 公共头文件
│   ├── src/               # HAL 核心源代码
│   └── README.md          # HAL 详细文档
├── bsp/                    # 板级支持包 (BSP)
│   ├── CMakeLists.txt     # BSP 库构建配置
│   ├── include/           # BSP 公共头文件
│   ├── src/               # BSP 核心源代码
│   ├── platforms/         # 平台特定实现
│   └── README.md          # BSP 详细文档
├── main.c                  # 主应用程序
├── CMakeLists.txt         # 主项目构建配置
├── ARCHITECTURE_OVERVIEW.md  # 完整架构总览
├── PROJECT_SUMMARY.md     # 项目总结
└── build/                 # 构建输出目录
```

## 快速开始

### 构建项目

```bash
# 创建构建目录
mkdir build
cd build

# 配置项目 (使用 Ninja 生成器，启用示例程序)
cmake -G Ninja .. -DBUILD_EXAMPLES=ON

# 编译
cmake --build .
```

### 运行演示程序

```bash
# 应用层演示（完整功能展示）
./application/app_demo.exe

# 系统层演示
./system/system_demo.exe

# 组件层演示
./component/component_demo.exe

# 驱动层演示
./driver/driver_demo.exe

# HAL 层演示
./hal/hal_demo.exe

# BSP 层演示
./bsp/bsp_test.exe
```

### 运行主程序

```bash
./DigitalPowerManagementSystem.exe
```

## BSP 库

BSP (Board Support Package) 库提供了平台无关的硬件抽象层：

- **模块化设计**: 核心功能与平台实现分离
- **多平台支持**: 轻松添加新的目标平台
- **易于集成**: 通过 CMake 子目录方式集成

详细文档请查看 [bsp/README.md](bsp/README.md)

## HAL 库

HAL (Hardware Abstraction Layer) 库提供了统一的硬件访问接口：

- **统一接口**: 标准化的 GPIO、UART、SPI、I2C、ADC、Timer、PWM 接口
- **跨平台**: 同一套 API 可用于不同硬件平台
- **模块化**: 可选择性启用需要的模块
- **完整示例**: 包含详细的使用示例和文档

详细文档请查看 [hal/README.md](hal/README.md)

## 架构设计

系统采用严格的六层分层架构设计：

```
Application → System → Component → Driver → HAL → BSP → Hardware
```

### 各层职责

| 层级 | 职责 | 关键功能 |
|------|------|----------|
| **Application** | 业务逻辑 | 电源管理、监控诊断、任务调度 |
| **System** | OS 抽象 | RTOS 封装、任务/同步/通信 |
| **Component** | 中间件 | JSON codec、协议栈封装 |
| **Driver** | 设备驱动 | POSIX-like 设备接口 |
| **HAL** | 硬件抽象 | 统一硬件 API |
| **BSP** | 板级支持 | 平台特定代码 |
| **Hardware** | 物理硬件 | MCU、外设 |

### 依赖关系

- ✅ 严格的单向依赖：每层只依赖直接的下层
- ✅ 无循环依赖：确保架构清晰可维护
- ✅ 模块化设计：每层独立编译和测试

详细架构说明请查看 [ARCHITECTURE_OVERVIEW.md](ARCHITECTURE_OVERVIEW.md)

## 文档

- [完整架构总览](ARCHITECTURE_OVERVIEW.md) - 六层架构详细说明
- [项目总结](PROJECT_SUMMARY.md) - 项目完成情况和统计
- [应用层文档](application/APPLICATION_LAYER.md) - 应用层 API 和使用指南
- [系统层文档](system/SYSTEM_LAYER.md) - 系统层 RTOS 抽象说明
- [组件层文档](component/COMPONENT_LAYER.md) - 组件层中间件封装
- [驱动层文档](driver/DRIVER_LAYER.md) - 驱动层设备管理
- [HAL 文档](hal/README.md) - 硬件抽象层说明
- [BSP 文档](bsp/README.md) - 板级支持包说明

## 技术栈

- **语言**: C11
- **构建系统**: CMake 3.10+
- **编译器**: GCC 13.1.0
- **生成器**: Ninja

## 许可证

详见 [LICENSE](LICENSE) 文件
