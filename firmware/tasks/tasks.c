/*
 * tasks.c
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
 * \brief Tasks implementation.
 *
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 *
 * \version 0.2.7
 *
 * \date 2024/01/13
 *
 * \addtogroup tasks
 * \{
 */

#include <FreeRTOS.h>
#include <task.h>

#include <config/config.h>

#include "tasks.h"
#include "startup.h"
#include "heartbeat.h"

void create_tasks(void)
{
	/* Startup task */
#if CONFIG_TASK_STARTUP_ENABLED == 1
	xTaskCreate(vTaskStartup, TASK_STARTUP_NAME, TASK_STARTUP_STACK_SIZE, NULL,
		    TASK_STARTUP_PRIORITY, &xTaskStartupHandle);

	if (xTaskStartupHandle == NULL) {
		/* Error creating the startup task */
	}
#endif /* CONFIG_TASK_STARTUP_ENABLED */

	/* Heartbeat task */
#if CONFIG_TASK_HEARTBEAT_ENABLED == 1
	xTaskCreate(vTaskHeartbeat, TASK_HEARTBEAT_NAME, TASK_HEARTBEAT_STACK_SIZE, NULL,
		    TASK_HEARTBEAT_PRIORITY, &xTaskHeartbeatHandle);

	if (xTaskHeartbeatHandle == NULL) {
		/* Error creating the heartbeat task */
	}
#endif /* CONFIG_TASK_HEARTBEAT_ENABLED */
}

/** \} End of tasks group */
