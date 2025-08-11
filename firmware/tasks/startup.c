/*
 * startup.c
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
 * \brief Startup task implementation.
 *
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 *
 * \version 0.2.7
 *
 * \date 2024/01/17
 *
 * \addtogroup startup
 * \{
 */

#include <stdbool.h>

#include <config/config.h>
#include <system/sys_log/sys_log.h>

#include "startup.h"

xTaskHandle xTaskStartupHandle;

EventGroupHandle_t task_startup_status;

void vTaskStartup(void *pvParameters)
{
	bool error = false;

	/* Logger device initialization */
	sys_log_init();

	/* Print the FreeRTOS version */
	sys_log_print_event_from_module(SYS_LOG_INFO, TASK_STARTUP_NAME, "FreeRTOS ");
	sys_log_print_msg(tskKERNEL_VERSION_NUMBER);
	sys_log_new_line();

	if (error) {
		sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_STARTUP_NAME,
						"Boot completed with ERRORS!");
		sys_log_new_line();
	} else {
		sys_log_print_event_from_module(SYS_LOG_INFO, TASK_STARTUP_NAME,
						"Boot completed with SUCCESS!");
		sys_log_new_line();
	}

	/* Startup task status = Done */
	//xEventGroupSetBits(task_startup_status, TASK_STARTUP_DONE);

	vTaskSuspend(xTaskStartupHandle);
}

/** \} End of startup group */
