/*
 * Copyright (c) 2015 Samsung Electronics Co., Ltd.
 *		http://www.samsung.com/
 *
 * EXYNOS - PMU(Power Management Unit) support
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#ifndef __EXYNOS_POWERMODE_H
#define __EXYNOS_POWERMODE_H __FILE__
#include <soc/samsung/cal-if.h>

extern int exynos_prepare_sys_powerdown(enum sys_powerdown mode);
extern void exynos_wakeup_sys_powerdown(enum sys_powerdown mode, bool early_wakeup);
extern void exynos_prepare_cp_call(void);
extern void exynos_wakeup_cp_call(bool early_wakeup);
extern int exynos_rtc_wakeup(void);

static char *sys_powerdown_str[NUM_SYS_POWERDOWN] = {
	"SICD",
	"SICD_CPD",
	"AFTR",
	"STOP",
	"LPD",
	"LPA",
	"ALPA",
	"DSTOP",
	"SLEEP",
	"SLEEP_VTS_ON",
	"SLEEP_AUD_ON",
	"FAPO",
#ifdef CONFIG_SOC_EXYNOS7885
	"SLEEP_FSYS_ON",
#endif
};

static inline char* get_sys_powerdown_str(int mode)
{
	return sys_powerdown_str[mode];
}

/**
 * Functions for cpuidle driver
 */
extern int exynos_cpu_pm_enter(unsigned int cpu, int index);
extern void exynos_cpu_pm_exit(unsigned int cpu, int enter_failed);

/**
  IDLE_IP control
 */
#define IDLE_IP_REG_SIZE		32
#define IDLE_IP_MAX_INDEX		127
#define IDLE_IP_FIX_INDEX_COUNT		2
#define IDLE_IP_MAX_CONFIGURABLE_INDEX	(IDLE_IP_MAX_INDEX - IDLE_IP_FIX_INDEX_COUNT)


#ifdef CONFIG_ARM64_EXYNOS_CPUIDLE
void exynos_update_ip_idle_status(int index, int idle);
int exynos_get_idle_ip_index(const char *name);
void exynos_get_idle_ip_list(char *(*idle_ip_list)[IDLE_IP_REG_SIZE]);
#else
static inline void exynos_update_ip_idle_status(int index, int idle)
{
	return;
}

static inline int exynos_get_idle_ip_index(const char *name)
{
	return 0;
}

static inline void exynos_get_idle_ip_list(char *(*idle_ip_list)[IDLE_IP_REG_SIZE])
{
	return;
}
#endif

enum exynos_idle_ip {
	IDLE_IP0,
	IDLE_IP1,
	IDLE_IP2,
	IDLE_IP3,
	NUM_IDLE_IP,
};

#ifndef CONFIG_SOC_EXYNOS7885
#define MAX_CLUSTER		2
#endif

/**
  IDLE_IP control
 */
#define for_each_idle_ip(num)					\
        for ((num) = 0; (num) < NUM_IDLE_IP; (num)++)

#define for_each_syspwr_mode(mode)				\
	for ((mode) = 0; (mode) < NUM_SYS_POWERDOWN; (mode)++)

#define for_each_cluster(id)					\
	for ((id) = 0; (id) < CONFIG_NR_CLUSTERS; (id)++)

/**
 * external driver APIs
 */
#ifdef CONFIG_SERIAL_SAMSUNG
extern void s3c24xx_serial_fifo_wait(void);
#else
static inline void s3c24xx_serial_fifo_wait(void) { }
#endif

#ifdef CONFIG_PINCTRL_EXYNOS
extern u64 exynos_get_eint_wake_mask(void);
#else
static inline u64 exynos_get_eint_wake_mask(void) { return 0xffffffffL; }
#endif
#endif /* __EXYNOS_POWERMODE_H */
