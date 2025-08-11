/*
 * uart.h
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
 * \brief UART driver definition.
 * 
 * \author Miguel Boing <miguelboing13@gmail.com>
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.2.1
 * 
 * \date 2023/02/17
 * 
 * \defgroup uart UART
 * \ingroup drivers
 * \{
 */

#ifndef UART_H_
#define UART_H_

#include <stdint.h>

/**
 * \brief UART ports.
 */
typedef enum
{
    UART_PORT_1=0,          /**< UART port 1. */
    UART_PORT_2,            /**< UART port 2. */
    UART_PORT_3,            /**< UART port 3. */
    UART_PORT_4,            /**< UART port 4. */
    UART_PORT_5             /**< UART port 5. */
} uart_port_t;

/**
 * \brief UART modes.
 */
typedef enum 
{
    UART_MODE_TX=0,         /**< TX mode. */
    UART_MODE_RX,           /**< RX mode. */
    UART_MODE_TX_RX         /**< TX/RX mode. */
} uart_mode_t;

/**
 * \brief Parity configuration.
 */
typedef enum
{
    UART_NO_PARITY=0,       /**< No parity. */
    UART_ODD_PARITY,        /**< Odd parity. */
    UART_EVEN_PARITY        /**< Even parity. */
} uart_parity_t;

/**
 * \brief Stop bits configuration.
 */
typedef enum 
{
    UART_SB_0_5=0,          /**< 0.5 stop bit. */
    UART_SB_1,              /**< 1 stop bit. */
    UART_SB_1_5,            /**< 1.5 stop bit. */
    UART_SB_2               /**< 2 stop bits. */
} uart_sb_t;

/**
 *  \brief Word length configuration.
 */
typedef enum
{
    UART_WL_8=0,            /**< 8-bits. */
    UART_WL_9               /**< 9-bits. */
} uart_wl_t;


/**
 *  \brief Hardware Flow Control.
 */
typedef enum 
{
    UART_FC_NONE=0,         /**< No flow control. */
    UART_FC_RTS,            /**< RTS. */
    UART_FC_CTS,            /**< CTS. */
    UART_FC_RTS_CTS         /**< RTS and CTS. */
} uart_fc_t;

/**
 *  \brief Baud rate configuration.
 */
typedef uint32_t uart_br_t;

/**
 * \brief Configuration structure.
 */
typedef struct
{
    uart_port_t port;       /**< Port. */
    uart_mode_t mode;       /**< Mode. */
    uart_parity_t parity;   /**< Parity. */
    uart_sb_t stop_bits;    /**< Stop bits. */
    uart_wl_t word_length;  /**< Word lenght. */
    uart_fc_t flow_control; /**< Flow control. */
    uart_br_t baudrate;     /**< Baudrate in bits per second. */
} uart_config_t;

/**
 * \brief Initializes a given UART port.
 *
 * \param[in] config are the configuration parameters of the given UART port.
 *
 * \return The status/error code.
 */
int uart_init(uart_config_t config);

/**
 * \brief Writes data to a given UART port.
 *
 * \param[in] config are the configuration parameters of the given UART port.
 *
 * \param[in] data is the array of bytes to be written.
 *
 * \param[in] len is the number of bytes to be written.
 *
 * \return The status/error code.
 */
int uart_write(uart_config_t config, uint16_t *data, uint16_t len);

/**
 * \brief Reads data from a given UART port.
 *
 * \param[in] config are the configuration parameters of the given UART port.
 *
 * \param[in,out] data is a pointer to store the read data.
 *
 * \param[in] len is the number of bytes to read.
 *
 * \return The status/error code.
 */
int uart_read(uart_config_t config, uint16_t *data, uint16_t len);

/**
 * \brief Enables the RX of a given UART port.
 *
 * \param[in] config are the configuration parameters of the given UART port.
 *
 * \return The status/error code.
 */
int uart_rx_enable(uart_config_t config);

/**
 * \brief Disables the RX of a given UART port.
 *
 * \param[in] config are the configuration parameters of the given UART port.
 *
 * \return The status/error code.
 */
int uart_rx_disable(uart_config_t config);

/**
 * \brief Checks if there is available data to read from a given UART port.
 *
 * \param[in] config are the configuration parameters of the given UART port.
 *
 * \return The status/error code.
 */
uint16_t uart_read_available(uart_config_t config);

/**
 * \brief Flushes the RX buffer of a given UART port.
 *
 * \param[in] config are the configuration parameters of the given UART port.
 *
 * \return The status/error code.
 */
int uart_flush(uart_config_t config);

#endif /* UART_H_ */

/** \} End of uart group */
