/*
 * Copyright (c) 2016 Samsung Electronics Co., Ltd.
 *	      http://www.samsung.com/
 *
 * EXYNOS - EL3 Monitor support
 * Author: Junho Choi <junhosj.choi@samsung.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __EXYNOS_EL3_MON_H
#define __EXYNOS_EL3_MON_H

/* Error code */
#define EXYNOS_ERROR_TZASC_WRONG_REGION		(-1)

#define EXYNOS_ERROR_ALREADY_INITIALIZED	(1)
#define EXYNOS_ERROR_NOT_VALID_ADDRESS		(0x1000)

int exynos_tz_peri_save(unsigned int addr);
int exynos_tz_peri_restore(unsigned int addr);

#endif	/* __EXYNOS_EL3_MON_H */
