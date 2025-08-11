/*
 * sys_log_mutes.c
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
 * \brief System log mutex implementation.
 *
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 *
 * \version 0.2.5
 *
 * \date 2024/01/14
 *
 * \defgroup sys_log_mutex Mutex
 * \ingroup sys_log
 * \{
 */

#include "sys_log.h"
#include "sys_log_config.h"

SemaphoreHandle_t xSysLogSemaphore = NULL;

bool sys_log_mutex_create(void)
{
    /* Create a mutex type semaphore */
    xSysLogSemaphore = xSemaphoreCreateMutex();

    if (xSysLogSemaphore == NULL)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, SYS_LOG_DEVICE_NAME, "Error creating a mutex!");
        sys_log_new_line();

        return false;
    }

    return true;
}

bool sys_log_mutex_take(void)
{
    if (xSysLogSemaphore != NULL)
    {
        /* See if we can obtain the semaphore. If the semaphore is not */
        /* available wait SYS_LOG_MUTEX_WAIT_TIME_MS ms to see if it becomes free */
        if (xSemaphoreTake(xSysLogSemaphore, pdMS_TO_TICKS(SYS_LOG_MUTEX_WAIT_TIME_MS)) == pdTRUE)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

bool sys_log_mutex_give(void)
{
    if (xSysLogSemaphore != NULL)
    {
        xSemaphoreGive(xSysLogSemaphore);

        return true;
    }
    else
    {
        return false;
    }
}

/** \} End of sys_log_mutex group */
