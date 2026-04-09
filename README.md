# Digital Power Management System

数字电源管理系统 - 一个模块化的嵌入式软件框架

## 项目结构

```
digital_power_management_system/
├── bsp/                    # 板级支持包 (BSP)
│   ├── CMakeLists.txt     # BSP 库构建配置
│   ├── include/           # BSP 公共头文件
│   ├── src/               # BSP 核心源代码
│   ├── platforms/         # 平台特定实现
│   │   └── gcc_simulator/ # GCC 模拟器平台
│   ├── test_bsp.c         # BSP 测试程序
│   └── README.md          # BSP 详细文档
├── main.c                  # 主应用程序
├── CMakeLists.txt         # 主项目构建配置
└── build/                 # 构建输出目录
```

## 快速开始

### 构建项目

```bash
# 创建构建目录
mkdir build
cd build

# 配置项目 (使用 Ninja 生成器)
cmake -G Ninja ..

# 编译
cmake --build .
```

### 启用 BSP 测试

```bash
# 配置时启用 BSP 测试
cmake -G Ninja .. -DBSP_BUILD_TESTS=ON

# 编译并运行测试
cmake --build .
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

## 技术栈

- **语言**: C11
- **构建系统**: CMake 3.10+
- **编译器**: GCC 13.1.0
- **生成器**: Ninja

## 许可证

详见 [LICENSE](LICENSE) 文件
