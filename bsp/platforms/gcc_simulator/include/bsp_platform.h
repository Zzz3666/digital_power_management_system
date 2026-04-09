#ifndef BSP_PLATFORM_H
#define BSP_PLATFORM_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 平台特定初始化
 * @return 0 表示成功, 非0 表示失败
 */
int bsp_platform_init(void);

/**
 * @brief 平台特定反初始化
 */
void bsp_platform_deinit(void);

#ifdef __cplusplus
}
#endif

#endif /* BSP_PLATFORM_H */
