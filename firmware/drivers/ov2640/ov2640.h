/*
 * ov2640.h
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
 * \brief OV2640 driver definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.1
 * 
 * \date 2022/08/19
 * 
 * \defgroup ov2640 OV2640
 * \ingroup drivers
 * \{
 */

#ifndef OV2640_H_
#define OV2640_H_

#include <stdint.h>

/**
 * \brief OV2640 initialization routine.
 *
 * \return The status/error code.
 */
int ov2640_init(void);

/**
 * \brief Reads the value of a register.
 *
 * \param[in] adr is the register address to read.
 *
 * \param[in,out] val is a pointer to store the read value.
 *
 * \return The status/error code.
 */
int ov2640_read_reg(uint8_t adr, uint8_t *val);

/**
 * \brief Writes a value to a given register.
 *
 * \param[in] adr is the register address to write.
 *
 * \param[in] val is the register value to write.
 *
 * \return The status/error code.
 */
int ov2640_write_reg(uint8_t adr, uint8_t val);

#endif /* OV2640_H_ */

/** \} End of ov2640 group */
