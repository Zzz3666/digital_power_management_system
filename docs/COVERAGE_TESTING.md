# 代码覆盖率测试指南

## 概述

本项目使用 **gcov** 和 **gcovr** 工具进行代码覆盖率测试，可以生成 HTML 和文本格式的覆盖率报告。

## 前置要求

### 1. 安装 gcov（已包含在 GCC 中）

检查是否已安装：
```bash
gcc --version
gcov --version
```

### 2. 安装 gcovr

```bash
# 使用 pip 安装
pip install gcovr

# 或使用 pip3
pip3 install gcovr

# 验证安装
gcovr --version
```

## 使用方法

### 快速开始（推荐）

项目提供了便捷的测试脚本，位于 `docs/scripts/` 目录：

**Windows:**
```bash
docs\scripts\coverage_test.bat
```

**Linux/macOS:**
```bash
chmod +x docs/scripts/coverage_test.sh
./docs/scripts/coverage_test.sh
```

脚本会交互式地让你选择生成 HTML 报告、文本报告或两者都生成。

### 方法一：启用覆盖率构建

```bash
# 清理之前的构建
rm -rf build

# 配置项目并启用覆盖率
cmake -DENABLE_COVERAGE=ON -B build -S .

# 编译项目
cmake --build build
```

### 方法二：生成 HTML 覆盖率报告

```bash
# 运行所有测试并生成 HTML 报告
cmake --build build --target coverage

# 在浏览器中打开报告
# Windows
start build/coverage/index.html

# Linux
xdg-open build/coverage/index.html

# macOS
open build/coverage/index.html
```

### 方法三：生成文本格式报告

```bash
# 生成文本格式的覆盖率报告
cmake --build build --target coverage-text
```

## 覆盖率报告说明

### HTML 报告内容

生成的 HTML 报告包含：
- **总体覆盖率统计**：行覆盖率、分支覆盖率
- **文件列表**：每个源文件的覆盖率
- **详细视图**：点击文件可查看每行的执行情况
  - ✅ 绿色：已执行的代码
  - ❌ 红色：未执行的代码
  - ⚠️ 黄色：部分执行的分支

### 文本报告内容

文本报告提供简洁的覆盖率摘要，适合 CI/CD 集成。

## 示例输出

```
------------------------------------------------------------------------------
                           GCC Code Coverage Report
Directory: .
------------------------------------------------------------------------------
File                                       Lines    Exec  Cover   Missing
------------------------------------------------------------------------------
application/src/application.c                280     245    87%   45,67,89
bsp/src/bsp.c                               120     115    95%   23,45
driver/devices/gpio_driver.c                 95      80    84%   12,34,56
hal/src/hal_gpio.c                          60      55    91%   15,20
------------------------------------------------------------------------------
TOTAL                                      1200    1050    87%
------------------------------------------------------------------------------
```

## 排除文件和目录

默认配置会排除：
- `build/*` - 构建目录
- `examples/*` - 示例代码

如需调整，编辑 `CMakeLists.txt` 中的 `--exclude-directories` 参数。

## 注意事项

1. **性能影响**：启用覆盖率会降低程序运行速度，仅用于测试
2. **文件大小**：覆盖率数据会增加可执行文件大小
3. **生产构建**：不要在生产环境中启用覆盖率
4. **清理数据**：每次重新测试前建议清理构建目录

## 常见问题

### Q: 找不到 gcovr 命令
A: 确保已安装 gcovr 并且在 PATH 中
```bash
pip install gcovr
```

### Q: 覆盖率显示为 0%
A: 确保：
1. 使用了 `-DENABLE_COVERAGE=ON` 配置
2. 运行了至少一个可执行文件
3. 可执行文件正常退出（非崩溃）

### Q: 如何只测试特定模块？
A: 修改 `coverage` 目标中的 COMMAND，只运行需要的测试程序

## CI/CD 集成示例

### GitHub Actions

```yaml
name: Code Coverage
on: [push, pull_request]

jobs:
  coverage:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v2
      
      - name: Install dependencies
        run: |
          sudo apt-get update
          sudo apt-get install -y gcovr
      
      - name: Configure
        run: cmake -DENABLE_COVERAGE=ON -B build -S .
      
      - name: Build
        run: cmake --build build
      
      - name: Generate coverage
        run: cmake --build build --target coverage
      
      - name: Upload coverage report
        uses: actions/upload-artifact@v2
        with:
          name: coverage-report
          path: build/coverage/
```

## 相关资源

- [gcov 官方文档](https://gcc.gnu.org/onlinedocs/gcc/Gcov.html)
- [gcovr 官方文档](https://gcovr.com/)
- [CMake 自定义命令](https://cmake.org/cmake/help/latest/command/add_custom_target.html)
