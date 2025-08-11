/*
 * uart.c
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
 * \brief UART driver implementation.
 * 
 * \author Miguel Boing <miguelboing13@gmail.com>
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.2.1
 * 
 * \date 2023/02/17
 * 
 * \addtogroup uart
 * \{
 */

#include <hal/include/libopencm3/stm32/rcc.h>
#include <hal/include/libopencm3/stm32/gpio.h>
#include <hal/include/libopencm3/stm32/usart.h>

#include <libs/containers/queue.h>
#include <config/errno.h>

#include "uart.h"

static queue_t uart_port_1_rx_buffer;
static queue_t uart_port_2_rx_buffer;
static queue_t uart_port_3_rx_buffer;
static queue_t uart_port_4_rx_buffer;
static queue_t uart_port_5_rx_buffer;

static inline int uart_send_byte(uart_config_t config, uint16_t c);

static inline int uart_read_byte(uart_config_t config, uint16_t *c);

static inline uint16_t uart_read_mtu(queue_t *uart_rx_buffer);

static inline int uart_select_port_buffer(uart_config_t config, queue_t *uart_rx_buffer);

static inline int uart_select_port_address(uart_config_t config, uint32_t *uart_address);

int uart_init(uart_config_t config)
{
    uint32_t usart;
    uint32_t baud;
    uint32_t stopbits;
    uint32_t parity;
    uint32_t mode;
    uint32_t bits;
    uint32_t flowcontrol;

    switch(config.port)
    {
        case UART_PORT_1:
            usart = USART1_BASE;
            rcc_periph_clock_enable(RCC_GPIOA);
            rcc_periph_clock_enable(RCC_USART1);
            gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_USART1_TX);
            break;
        case UART_PORT_2:
            usart = USART2_BASE;
            /* TODO */
            break;
        case UART_PORT_3:
            usart = USART3_BASE;
            /* TODO */
            break;
        case UART_PORT_4:
            usart = UART4_BASE;
            /* TODO */
            break;
        case UART_PORT_5:
            usart = UART5_BASE;
            /* TODO */
            break;
        default:
            /* TODO Add error log system */
            break;
    }

    baud = (uint32_t)config.baudrate;

    switch(config.stop_bits)
    {
        case UART_SB_0_5:   stopbits = USART_STOPBITS_0_5;  break;
        case UART_SB_1:     stopbits = USART_STOPBITS_1;    break;
        case UART_SB_1_5:   stopbits = USART_STOPBITS_1_5;  break;
        case UART_SB_2:     stopbits = USART_STOPBITS_2;    break;
        default:                                            break;
    }

    switch(config.parity)
    {
        case UART_NO_PARITY:    parity = USART_PARITY_NONE; break;
        case UART_ODD_PARITY:   parity = USART_PARITY_ODD;  break;
        case UART_EVEN_PARITY:  parity = USART_PARITY_EVEN; break;
        default:
            /* TODO Add error log system. */
            break;
    }

    switch(config.mode)
    {
        case UART_MODE_TX:      mode = USART_MODE_TX;       break;
        case UART_MODE_RX:      mode = USART_MODE_RX;       break;
        case UART_MODE_TX_RX:   mode = USART_MODE_TX_RX;    break;
        default:
            /* TODO Add error log system. */
            break;
    }

    switch(config.word_length)
    {
        case UART_WL_8: bits = 8;   break;
        case UART_WL_9: bits = 9;   break;
        default:
            /* TODO Add error log system. */
            break;
    }

    switch(config.flow_control)
    {
        case UART_FC_NONE:      flowcontrol = USART_FLOWCONTROL_NONE;       break;
        case UART_FC_RTS:       flowcontrol = USART_FLOWCONTROL_RTS;        break;
        case UART_FC_CTS:       flowcontrol = USART_FLOWCONTROL_CTS;        break;
        case UART_FC_RTS_CTS:   flowcontrol = USART_FLOWCONTROL_RTS_CTS;    break;
        default:
            /* TODO Add error log system. */
            break;
    }

    /* Setup UART parameters. */
    usart_set_baudrate(usart, baud);
    usart_set_databits(usart, bits);
    usart_set_stopbits(usart, stopbits);
    usart_set_mode(usart, mode);
    usart_set_parity(usart, parity);
    usart_set_flow_control(usart, flowcontrol);

    /* Enable UART. */
    usart_enable(usart);

    return 0;
}

int uart_write(uart_config_t config, uint16_t *data, uint16_t len)
{
    int err = ERRNO_SUCCESS;

    uint16_t i = 0U;
    for(i = 0U; i < len; i++)
    {
        err = uart_send_byte(config, data[i]);
    }

    return err;
}

int uart_read(uart_config_t config, uint16_t *data, uint16_t len)
{
    int err = ERRNO_SUCCESS;
    uint16_t num_bytes = len;
    uint16_t i = 0U;
    queue_t *uart_rx_buffer;

    err = uart_select_port_buffer(config, uart_rx_buffer);

    if (err == ERRNO_SUCCESS)
    {
        /* Check if read size isn't bigger than mtu */
        if (num_bytes > uart_read_mtu(uart_rx_buffer))
        {
            num_bytes = uart_read_mtu(uart_rx_buffer);
        }

        for(i = 0U; i < num_bytes; i++)
        {
            data[i] = queue_pop_front(uart_rx_buffer);
        }
    }

    return err;
}

int uart_rx_enable(uart_config_t config)
{
    int err = -1;
    uint32_t usart;

    err  = uart_select_port_address(config, &usart);
    usart_enable_rx_interrupt(usart);

    return err;
}

int uart_rx_disable(uart_config_t config)
{
    int err = -1;
    uint32_t usart = UINT32_MAX;

    err  = uart_select_port_address(config, &usart);
    usart_disable_rx_interrupt(usart);

    return err;
}

uint16_t uart_read_available(uart_config_t config)
{
    uint16_t available_bytes = 0U;
    queue_t *uart_rx_buffer;

    if (uart_select_port_buffer(config, uart_rx_buffer) == ERRNO_SUCCESS)
    {
        available_bytes = queue_size(uart_rx_buffer);
    }

    return available_bytes;
}

int uart_flush(uart_config_t config)
{
    int err = ERRNO_SUCCESS;

    queue_t *uart_rx_buffer;

    if (uart_select_port_buffer(config, uart_rx_buffer) == ERRNO_SUCCESS)
    {
        queue_clear(uart_rx_buffer);
    }
    else
    {
        err = -1;
    }

    return err;
}

static inline int uart_send_byte(uart_config_t config, uint16_t c)
{
    int err = ERRNO_SUCCESS;

    uint32_t usart;

    if (uart_select_port_address(config, &usart) == ERRNO_SUCCESS)
    {
        usart_send_blocking(usart, c);
    }
    else
    {
        err = -1;
    }

    return err;
}

static inline int uart_read_byte(uart_config_t config, uint16_t *c)
{
    int err;
  
    uint32_t usart;

    err = uart_select_port_address(config, &usart);

    if (err = ERRNO_SUCCESS)
    {
        *c = usart_recv_blocking(usart);
    }

    return err;
}

static inline uint16_t uart_read_mtu(queue_t *uart_rx_buffer)
{
    return queue_length(uart_rx_buffer);
}

static inline int uart_select_port_buffer(uart_config_t config, queue_t *uart_rx_buffer)
{
    int err = ERRNO_SUCCESS;
  
    switch(config.port)
    {
        case UART_PORT_1:   uart_rx_buffer = &uart_port_1_rx_buffer;    break;
        case UART_PORT_2:   uart_rx_buffer = &uart_port_2_rx_buffer;    break;
        case UART_PORT_3:   uart_rx_buffer = &uart_port_3_rx_buffer;    break;
        case UART_PORT_4:   uart_rx_buffer = &uart_port_4_rx_buffer;    break;
        case UART_PORT_5:   uart_rx_buffer = &uart_port_5_rx_buffer;    break;
        default:
            err = ERRNO_DRIVER_NO_PORT;
            /* Add error log system */
            break;
    }

    return err;
}

static inline int uart_select_port_address(uart_config_t config, uint32_t *usart)
{
    int err = ERRNO_SUCCESS;

    switch(config.port)
    {
        case UART_PORT_1:   *usart = USART1_BASE;   break;
        case UART_PORT_2:   *usart = USART2_BASE;   break;
        case UART_PORT_3:   *usart = USART3_BASE;   break;
        case UART_PORT_4:   *usart = UART4_BASE;    break;
        case UART_PORT_5:   *usart = UART5_BASE;    break;
        default:
            err = ERRNO_DRIVER_NO_PORT;
            /* Add error log system */
            break;
    }
}

/** \} End of uart group */
