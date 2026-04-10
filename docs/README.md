# 文档索引

本目录包含数字电源管理系统的所有文档。

## 📚 架构文档

### 核心架构
- [ARCHITECTURE_OVERVIEW.md](ARCHITECTURE_OVERVIEW.md) - **完整六层架构总览**（推荐首先阅读）
  - 包含 HAL 与 BSP 的设计原则和依赖关系说明
- [ARCHITECTURE.md](ARCHITECTURE.md) - 架构设计详细说明

### 项目总结
- [PROJECT_SUMMARY.md](PROJECT_SUMMARY.md) - **项目完成情况和统计**

### 测试与质量
- [COVERAGE_TESTING.md](COVERAGE_TESTING.md) - **代码覆盖率测试指南**

## 📖 各层详细文档

### 应用层 (Application Layer)
- [APPLICATION_LAYER.md](APPLICATION_LAYER.md) - 应用层 API 和使用指南
- [COMPLETION_REPORT.md](COMPLETION_REPORT.md) - 应用层开发完成报告

### 系统层 (System Layer)
- [SYSTEM_LAYER.md](SYSTEM_LAYER.md) - 系统层 RTOS 抽象说明

### 组件层 (Component Layer)
- [COMPONENT_LAYER.md](COMPONENT_LAYER.md) - 组件层中间件封装

### 驱动层 (Driver Layer)
- [DRIVER_LAYER.md](DRIVER_LAYER.md) - 驱动层设备管理

### 硬件抽象层 (HAL)
- [HAL_README.md](HAL_README.md) - HAL 层详细说明
- [HAL_IMPLEMENTATION_SUMMARY.md](HAL_IMPLEMENTATION_SUMMARY.md) - HAL 实现总结

### 板级支持包 (BSP)
- [BSP_README.md](BSP_README.md) - BSP 层详细说明
- [BSP_IMPLEMENTATION_SUMMARY.md](BSP_IMPLEMENTATION_SUMMARY.md) - BSP 实现总结
- [BSP_QUICK_REFERENCE.md](BSP_QUICK_REFERENCE.md) - BSP 快速参考

## 🚀 快速开始

建议的阅读顺序：

1. **新手入门**：
   - 先阅读根目录的 [README.md](../README.md)
   - 然后阅读 [ARCHITECTURE_OVERVIEW.md](ARCHITECTURE_OVERVIEW.md) 了解整体架构

2. **深入了解**：
   - 按照从下到上的顺序阅读各层文档：
     - BSP → HAL → Driver → Component → System → Application

3. **开发参考**：
   - 根据需要查阅具体层的详细文档
   - 参考各层的 examples 目录中的示例代码

## 📊 文档统计

| 类别 | 文档数量 |
|------|----------|
| 架构文档 | 2 |
| 项目文档 | 1 |
| 应用层文档 | 2 |
| 系统层文档 | 1 |
| 组件层文档 | 1 |
| 驱动层文档 | 1 |
| HAL 文档 | 2 |
| BSP 文档 | 1 |
| 测试与质量 | 1 |
| 补充文档 | 5 |
| **总计** | **17** |

## 🔗 相关链接

- [项目主页](../README.md)
- [GitHub Repository](https://github.com/your-repo/digital_power_management_system)
- [问题反馈](https://github.com/your-repo/digital_power_management_system/issues)

## 📝 文档维护

- 最后更新：2026-04-10
- 维护者：开发团队
- 如有文档问题，请提交 Issue 或 Pull Request

## 🛠️ 工具脚本

在 `scripts/` 目录中提供了一些便捷的工具脚本：

- [coverage_test.bat](scripts/coverage_test.bat) - Windows 覆盖率测试脚本
- [coverage_test.sh](scripts/coverage_test.sh) - Linux/macOS 覆盖率测试脚本
