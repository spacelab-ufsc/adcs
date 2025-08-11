/*
 * mutex.h
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

#ifndef MUTEX_H_
#define MUTEX_H_

#include <stdint.h>

#include <config/config.h>

#ifdef CONFIG_OS_IS_FREERTOS
#include <FreeRTOS.h>
#include <semphr.h>
#include <portmacro.h>
#endif

/**
 * \brief Mutex structure. The underlying type depends entirely on the OS used.
 */
struct mutex {
#ifdef CONFIG_OS_IS_FREERTOS
	SemaphoreHandle_t lock;
#endif
};

/**
 * \brief Initializes a mutex.
 *
 * \param[out] lock is the mutex to be initialized.
 *
 * \return The status/error code.
 */
int mutex_init(struct mutex *lock);

/**
 * \brief Locks a mutex. This function will block indefinitely, if it is used 
 * in critical real time context use `mutex_lock_with_timeout` for better 
 * versatility.
 *
 * \param[in] lock is the mutex to be locked.
 *
 * \return The status/error code.
 */
int mutex_lock(struct mutex *lock);

/**
 * \brief Tries to lock a mutex. Basically a NON-BLOCK version of the 
 * `mutex_lock` function.
 *
 * \param[in] lock is the mutex to be locked.
 *
 * \return 1 if the mutex was locked and 0 if the mutex was already locked.
 * For errors negative numbers are returned.
 */
int mutex_try_lock(struct mutex *lock);

/**
 * \brief Locks onto a mutex.
 *
 * \param[in] lock is the mutex to be locked.
 *
 * \param[in] timeout is the max time in milliseconds this function will block 
 * if the mutex is already locked.
 *
 * \return The status/error code.
 */
int mutex_lock_with_timeout(struct mutex *lock, uint32_t timeout);

/**
 * \brief Unlocks a mutex. 
 *
 * \param[in] lock is the mutex to be unlocked.
 *
 * \return The status/error code.
 */
int mutex_unlock(struct mutex *lock);

/**
 * \brief Destroy a mutex.
 *
 * \param[in] lock is the mutex to be destroyed.
 *
 * \return The status/error code.
 */
int mutex_destroy(struct mutex *lock);

#endif

/** \} End of mutex group */
