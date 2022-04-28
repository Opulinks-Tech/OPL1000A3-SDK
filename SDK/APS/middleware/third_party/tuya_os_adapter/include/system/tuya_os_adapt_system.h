/**
 * @file tuya_os_adapt_system.h
 * @brief 系统接口封装
 * 
 * @copyright Copyright(C),2018-2020, 涂鸦科技 www.tuya.com
 * 
 */

#ifndef _TUYA_OS_ADAPT_SYSTEM_H
#define _TUYA_OS_ADAPT_SYSTEM_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


#ifndef SYS_TICK_T
typedef uint32_t SYS_TICK_T; 
#endif /* SYS_TICK_T */
    
#ifndef  TY_RST_REASON_E
typedef enum {
	TY_RST_POWER_OFF = 0,
	TY_RST_HARDWARE_WATCHDOG,
	TY_RST_FATAL_EXCEPTION,
	TY_RST_SOFTWARE_WATCHDOG,
	TY_RST_SOFTWARE,
	TY_RST_DEEPSLEEP,
	TY_RST_HARDWARE,
	TY_RST_OTHER = 0xAA,
	TY_RST_UNSUPPORT = 0xFF,
} TY_RST_REASON_E;
#endif /* TY_RST_REASON_E */

/**
 * @brief 获取系统当前的tick值
 * @return 系统当前的tick值
 */
SYS_TICK_T tuya_os_adapt_get_systemtickcount(void);
#define tuya_hal_get_systemtickcount tuya_os_adapt_get_systemtickcount

/**
 * @brief 获取系统1个ticket是多少个ms
 * 
 * @return 系统1个ticket是多少个ms
 */
unsigned int tuya_os_adapt_get_tickratems(void);
#define tuya_hal_get_tickratems tuya_os_adapt_get_tickratems

/**
 * @brief 线程睡眠
 * 
 * @param [in] msTime 系统睡眠时间（ms）
 */
void tuya_os_adapt_system_sleep(const unsigned long msTime);
#define tuya_hal_system_sleep tuya_os_adapt_system_sleep

/**
 * @brief Reset system
 */
void tuya_os_adapt_system_reset(void);
#define tuya_hal_system_reset tuya_os_adapt_system_reset

/**
 * @brief 检测系统是否处于上下文中断
 * 
 * @retval true : 处于上下文中断
 * @retval false : 没处于上下文中断
 */
bool tuya_os_adapt_system_isrstatus(void);
#define tuya_hal_system_isrstatus tuya_os_adapt_system_isrstatus

/**
 * @brief 初始化并运行看门狗
 * 
 * @param [in] timeval 看门狗复位检测时间间隔
 * @note 如果timeval大于看门狗的最大可设置时间，则使用平台可设置时间的最大值，并且返回该最大值
 * @return 芯片实际设置的看门狗时间 (sec)
 */
unsigned int tuya_os_adapt_watchdog_init_start(const unsigned int timeval);

/**
 * @brief 用于刷新watch dog（喂狗）
 * 
 */
void tuya_os_adapt_watchdog_refresh(void);

/**
 * @brief 停止watch dog
 * 
 */
void tuya_os_adapt_watchdog_stop(void);

/**
 * @brief 获取系统剩余内存大小
 * 
 * @return 剩余内存
 */
int tuya_os_adapt_system_getheapsize(void);
#define tuya_hal_system_getheapsize tuya_os_adapt_system_getheapsize

/**
 * @brief 获取系统最小剩余内存大小
 * 
 * @return 最小剩余内存
 */
int tuya_os_adapt_system_getMiniheapsize(void);
#define tuya_hal_system_getMiniheapsize tuya_os_adapt_system_getMiniheapsize

/**
 * @brief 获取系统重启原因
 * 
 * @return 重启原因 ::TY_RST_REASON_E
 * @retval TY_RST_POWER_OFF         电源重启
 * @retval TY_RST_HARDWARE_WATCHDOG 硬件看门狗复位
 * @retval TY_RST_FATAL_EXCEPTION   异代码常
 * @retval TY_RST_SOFTWARE_WATCHDOG 软件看门狗重启
 * @retval TY_RST_SOFTWARE          软件复位
 * @retval TY_RST_DEEPSLEEP         深度睡眠
 * @retval TY_RST_HARDWARE          硬件复位
 * @retval TY_RST_OTHER             其它原因
 * @retval TY_RST_UNSUPPORT         不支持
 */
TY_RST_REASON_E tuya_os_adapt_system_get_rst_info(void);
#define tuya_hal_system_get_rst_info tuya_os_adapt_system_get_rst_info

/**
 * @brief init random
 *
 * @param  void
 * @retval void
 */
void tuya_os_adapt_srandom(void);
#define tuya_hal_srandom tuya_os_adapt_srandom

/**
 * @brief tuya_os_adapt_get_random_data用于获取指定条件下的随机数
 * 
 * @param[in] range 
 * @return 随机值
 */
int tuya_os_adapt_get_random_data(const unsigned int range);
#define tuya_hal_get_random_data tuya_os_adapt_get_random_data

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

