/*
 * sys_log_uart.c
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
 * \brief System log UART implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.2.5
 * 
 * \date 2024/01/14
 * 
 * \defgroup sys_log_uart UART
 * \ingroup sys_log
 * \{
 */

#include <drivers/uart/uart.h>

#include "sys_log.h"

uart_config_t sys_log_conf = {0};

bool sys_log_uart_init(void)
{
    sys_log_conf.port           = UART_PORT_1;
    sys_log_conf.mode           = UART_MODE_TX;
    sys_log_conf.parity         = UART_NO_PARITY;
    sys_log_conf.stop_bits      = UART_SB_1;
    sys_log_conf.word_length    = UART_WL_8;
    sys_log_conf.flow_control   = UART_FC_NONE;
    sys_log_conf.baudrate       = 115200;

    return uart_init(sys_log_conf) == 0 ? true : false;
}

void sys_log_uart_write_byte(uint8_t byte)
{
    uart_write(sys_log_conf, (uint16_t*)&byte, 1);
}

/** \} End of sys_log_uart group */
