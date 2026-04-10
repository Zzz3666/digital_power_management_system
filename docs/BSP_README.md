# BSP (Board Support Package) 库

## 概述

BSP 库提供了数字电源管理系统的板级支持包，包含平台无关的核心功能和平台特定的实现。

## 目录结构

```
bsp/
├── CMakeLists.txt          # BSP 库的 CMake 配置文件
├── include/                # 公共头文件
│   └── bsp.h              # BSP 核心接口
├── src/                    # 核心源代码
│   └── bsp.c              # BSP 核心实现
└── platforms/              # 平台特定实现
    └── gcc_simulator/     # GCC 模拟器平台
        ├── include/
        │   └── bsp_platform.h
        └── src/
            └── bsp_platform.c
```

## 构建配置

### CMake 选项

- `BSP_PLATFORM_GCC_SIMULATOR`: 启用 GCC Simulator 平台支持（默认：ON）

### 使用方法

在主项目的 CMakeLists.txt 中：

```cmake
# 添加 BSP 子目录
add_subdirectory(bsp)

# 链接 BSP 库
target_link_libraries(your_target PRIVATE ${BSP_LIBRARIES})
```

### 添加新平台

1. 在 `platforms/` 目录下创建新的平台文件夹
2. 添加平台的 `include/` 和 `src/` 目录
3. 实现 `bsp_platform.h` 中定义的接口
4. 在 `CMakeLists.txt` 中添加新平台的配置选项

## API 接口

### 核心接口 (bsp.h)

```c
// 初始化 BSP
int init_bsp(void);

// 反初始化 BSP
void deinit_bsp(void);
```

### 平台接口 (bsp_platform.h)

每个平台需要实现以下接口：

```c
// 平台特定初始化
int bsp_platform_init(void);

// 平台特定反初始化
void bsp_platform_deinit(void);
```

## 编译输出

- 静态库: `libbsp.a` (Linux/Mac) 或 `bsp.lib` (Windows)
- 头文件: 自动包含到使用项目中
