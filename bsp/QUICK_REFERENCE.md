# BSP 库快速参考

## 📁 文件结构

```
bsp/
├── CMakeLists.txt          # ⚙️ 构建配置
├── README.md               # 📖 使用文档
├── IMPLEMENTATION_SUMMARY.md  # 📋 实现总结
├── QUICK_REFERENCE.md      # 🚀 本文件
├── test_bsp.c              # 🧪 测试程序
├── include/
│   └── bsp.h              # 🔌 公共 API
├── src/
│   └── bsp.c              # 💻 核心实现
└── platforms/
    └── gcc_simulator/     # 🎯 平台实现
        ├── include/
        │   └── bsp_platform.h
        └── src/
            └── bsp_platform.c
```

## 🚀 快速开始

### 1. 基本构建
```bash
mkdir build && cd build
cmake -G Ninja ..
cmake --build .
```

### 2. 带测试构建
```bash
cmake -G Ninja .. -DBSP_BUILD_TESTS=ON
cmake --build .
./bsp/bsp_test.exe
```

### 3. 运行主程序
```bash
./DigitalPowerManagementSystem.exe
```

## 🔧 CMake 选项

| 选项 | 默认值 | 说明 |
|------|--------|------|
| `BSP_PLATFORM_GCC_SIMULATOR` | ON | 启用 GCC Simulator 平台 |
| `BSP_BUILD_TESTS` | OFF | 构建测试程序 |

## 📝 API 参考

### BSP 核心接口

```c
#include "bsp.h"

// 初始化 BSP
int init_bsp(void);  // 返回: 0=成功, 非0=失败

// 反初始化 BSP
void deinit_bsp(void);
```

### 平台接口（平台实现者）

```c
#include "bsp_platform.h"

// 平台初始化
int bsp_platform_init(void);  // 返回: 0=成功, 非0=失败

// 平台反初始化
void bsp_platform_deinit(void);
```

## ➕ 添加新平台

### 步骤 1: 创建平台目录
```bash
mkdir bsp/platforms/your_platform/{include,src}
```

### 步骤 2: 实现平台接口

**bsp/platforms/your_platform/include/bsp_platform.h**
```c
#ifndef BSP_PLATFORM_H
#define BSP_PLATFORM_H

int bsp_platform_init(void);
void bsp_platform_deinit(void);

#endif
```

**bsp/platforms/your_platform/src/bsp_platform.c**
```c
#include "bsp_platform.h"

int bsp_platform_init(void) {
    // 你的平台初始化代码
    return 0;
}

void bsp_platform_deinit(void) {
    // 你的平台清理代码
}
```

### 步骤 3: 更新 CMakeLists.txt

在 `bsp/CMakeLists.txt` 中添加：

```cmake
option(BSP_PLATFORM_YOUR "使用 Your Platform" OFF)
if(BSP_PLATFORM_YOUR)
    message(STATUS "BSP: Using Your Platform")
    
    list(APPEND BSP_SOURCES
        platforms/your_platform/src/bsp_platform.c
    )
    
    list(APPEND BSP_INCLUDE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}/platforms/your_platform/include
    )
    
    add_definitions(-DBSP_PLATFORM_YOUR)
endif()
```

### 步骤 4: 构建
```bash
cmake -G Ninja .. -DBSP_PLATFORM_YOUR=ON
cmake --build .
```

## 🎯 常用命令

### 清理构建
```bash
# Windows PowerShell
Remove-Item -Recurse -Force build

# Linux/Mac
rm -rf build
```

### 重新构建
```bash
mkdir build && cd build
cmake -G Ninja ..
cmake --build .
```

### 查看生成的文件
```bash
# 查看库文件
ls build/bsp/*.a

# 查看可执行文件
ls build/*.exe
```

## 🐛 故障排除

### 问题: 找不到头文件
**解决**: 检查 `target_include_directories` 是否正确配置

### 问题: 链接错误
**解决**: 确保 `target_link_libraries` 包含 `${BSP_LIBRARIES}`

### 问题: 平台未找到
**解决**: 检查 CMake 选项是否正确设置

### 问题: 编译错误
**解决**: 验证平台接口实现是否完整

## 📊 构建产物

| 文件 | 位置 | 大小 | 说明 |
|------|------|------|------|
| libbsp.a | build/bsp/ | ~3KB | BSP 静态库 |
| DigitalPowerManagementSystem.exe | build/ | ~78KB | 主程序 |
| bsp_test.exe | build/bsp/ | ~ | 测试程序 (可选) |

## 🔗 相关链接

- [BSP 详细文档](README.md)
- [实现总结](IMPLEMENTATION_SUMMARY.md)
- [主项目文档](../README.md)

## 💡 提示

1. **开发时**: 启用 `BSP_BUILD_TESTS` 进行快速测试
2. **发布时**: 关闭测试以减小编译时间
3. **多平台**: 一次只能选择一个平台
4. **调试**: 查看 CMake 输出消息了解配置详情

---

**版本**: 1.0  
**最后更新**: 2026-04-09
