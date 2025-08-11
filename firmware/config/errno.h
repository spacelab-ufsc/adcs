/*
 * errno.h
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
 * \brief Errors definition.
 * 
 * \author Miguel Boing <miguelboing13@gmail.com>
 * 
 * \version 0.1.3
 * 
 * \date 2023/02/16
 * 
 * \defgroup errno Error Codes
 * \{
 */

#ifndef ERRNO_H
#define ERRNO_H

#include <utils/macros/macros.h>

/* No error ocurred */
#define ERRNO_SUCCESS 0x00U /** No errors found */

/* Driver Level errors */
#define ERRNO_DRIVER_NO_PORT 0x11U /** No port found */
#define ERRNO_DRIVER_NO_PARAMETER 0x12U /** Invalid parameter value */
#define ERRNO_DRIVER_FAILED 0x13U /** Failed to execute HAL function */
#define ERRNO_DRIVER_UNINITIALIZED 0x14U /** Driver was not yet initialized */
#define ERRNO_DRIVER_NO_HW_IMPL \
	0x15U /** Driver could not get the specific hardware implementation */

/* Device Level errors */
#define ERRNO_DEVICE_FAILED_CONFIG 0x21U /** Failed to configure driver */
#define ERRNO_DEVICE_FAILED_COM 0x22U /** Failed to communicate */

/* Miscellaneous errors */
#define ERRNO_MISC_INVALID_ARG 0x81U /** Invalid argument passed to function */
#define ERRNO_MISC_FAILED_ALLOC 0x82U /** Failed to allocate memory */
#define ERRNO_MISC_UNSUPPORTED_OP 0x83U /** Unsupported operation */
#define ERRNO_MISC_UNKNOWN 0x84U /** Unknown error occured */
#define ERRNO_MISC_TIMEOUT 0x85U /** Operation timeout */

static inline const char *errno_to_string(long errno)
{
	switch (-errno) {
	case ERRNO_SUCCESS:
		return STRINGZ(ERRNO_SUCCESS);
	case ERRNO_DRIVER_NO_PORT:
		return STRINGZ(ERRNO_DRIVER_NO_PORT);
	case ERRNO_DRIVER_NO_PARAMETER:
		return STRINGZ(ERRNO_DRIVER_NO_PARAMETER);
	case ERRNO_DRIVER_FAILED:
		return STRINGZ(ERRNO_DRIVER_FAILED);
	case ERRNO_DRIVER_UNINITIALIZED:
		return STRINGZ(ERRNO_DRIVER_UNINITIALIZED);
	case ERRNO_DEVICE_FAILED_CONFIG:
		return STRINGZ(ERRNO_DEVICE_FAILED_CONFIG);
	case ERRNO_DEVICE_FAILED_COM:
		return STRINGZ(ERRNO_DEVICE_FAILED_COM);
	case ERRNO_MISC_INVALID_ARG:
		return STRINGZ(ERRNO_MISC_INVALID_ARG);
	case ERRNO_MISC_FAILED_ALLOC:
		return STRINGZ(ERRNO_MISC_FAILED_ALLOC);
	case ERRNO_MISC_UNSUPPORTED_OP:
		return STRINGZ(ERRNO_MISC_UNSUPPORTED_OP);
	case ERRNO_MISC_UNKNOWN:
		return STRINGZ(ERRNO_MISC_UNKNOWN);
	case ERRNO_MISC_TIMEOUT:
		return STRINGZ(ERRNO_MISC_TIMEOUT);
	default:
		return "Unmapped error!";
	}
	return "";
}

#endif /*ERRNO_H */

/** \} End of errno group */
