/*
 * mutex.c
 * 
 * Copyright The SLCam Contributors.
 * 
 * This file is part of SLCam.
 * 
 * SLCam is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * SLCam is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with SLCam. If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief Mutex utility functions.
 * 
 * \author Carlos Augusto Porto Freitas <carlos.portof@hotmail.com>
 * 
 * \version 0.0.0
 * 
 * \date 2024/12/29
 * 
 * \defgroup mutex Mutex
 * \ingroup utils
 * \{
 */
#include <stdint.h>
#include <config/errno.h>
#include <config/config.h>

#ifdef CONFIG_OS_IS_FREERTOS
#include <FreeRTOS.h>
#include <semphr.h>
#include <portmacro.h>
#endif

#include "mutex.h"

int mutex_init(struct mutex *lock)
{
	if (!lock)
		return -ERRNO_MISC_INVALID_ARG;

#ifdef CONFIG_OS_IS_FREERTOS
	lock->lock = xSemaphoreCreateRecursiveMutex();

	if (!lock->lock)
		return -ERRNO_MISC_FAILED_ALLOC;
#else
	return -ERRNO_MISC_UNSUPPORTED_OP;
#endif

	return 0;
}

int mutex_lock(struct mutex *lock)
{
	if (!lock || !lock->lock)
		return -ERRNO_MISC_INVALID_ARG;

#ifdef CONFIG_OS_IS_FREERTOS
	if (xSemaphoreTakeRecursive(lock->lock, portMAX_DELAY) != pdPASS)
		return -ERRNO_MISC_UNKNOWN;
#else
	return -ERRNO_MISC_UNSUPPORTED_OP;
#endif

	return 0;
}

int mutex_lock_with_timeout(struct mutex *lock, uint32_t timeout)
{
	if (!lock || !lock->lock)
		return -ERRNO_MISC_INVALID_ARG;

#ifdef CONFIG_OS_IS_FREERTOS
	if (xSemaphoreTakeRecursive(lock->lock, pdMS_TO_TICKS(timeout)) != pdPASS)
		return -ERRNO_MISC_TIMEOUT;
#else
	return -ERRNO_MISC_UNSUPPORTED_OP;
#endif

	return 0;
}

int mutex_try_lock(struct mutex *lock)
{
	if (!lock || !lock->lock)
		return -ERRNO_MISC_INVALID_ARG;

#ifdef CONFIG_OS_IS_FREERTOS
	xSemaphoreTakeRecursive(lock->lock, 0U);
#else
	return -ERRNO_MISC_UNSUPPORTED_OP;
#endif

	return 0;
}

int mutex_unlock(struct mutex *lock)
{
	if (!lock || !lock->lock)
		return -ERRNO_MISC_INVALID_ARG;

#ifdef CONFIG_OS_IS_FREERTOS
	xSemaphoreGiveRecursive(lock->lock);
#else
	return -ERRNO_MISC_UNSUPPORTED_OP;
#endif

	return 0;
}

/** \} End of mutex group */
