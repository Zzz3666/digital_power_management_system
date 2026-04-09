#ifndef BSP_H
#define BSP_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 初始化板级支持包
 * @return 0 表示成功, 非0 表示失败
 */
int init_bsp(void);

/**
 * @brief 反初始化板级支持包
 */
void deinit_bsp(void);

#ifdef __cplusplus
}
#endif

#endif /* BSP_H */
