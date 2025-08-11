/**
 * \brief Interrupt Service Routines Handlers
 *
 * \author Carlos Augusto Porto Freitas <carlos.portof@hotmail.com>
 *
 * \version 0.0.1
 *
 * \date 2023/11/11
 *
 * \addtogroup isr
 * \{
 */

#include <drivers/can/can.h>
#include <libopencm3/stm32/can.h>
#include <os/include/FreeRTOS.h>
#include <os/include/queue.h>
#include <utils/macros/macros.h>
#include <tasks/tasks.h>
#include "portmacro.h"
#include "projdefs.h"

/**
 * \brief Can1 FIFO0 message received isr
 */
void usb_lp_can_rx0_isr(void) // cppcheck-suppress misra-c2012-8.4
{
	can_rx_t rx;
	BaseType_t xHigherPriorityTaskWoken;

	rx.port = CAN_PORT_1;
	rx.fifo = CAN_FIFO_0;
	rx.release = true;

	xHigherPriorityTaskWoken = pdFALSE;

	can_read(&rx);

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

/**
 * \brief Can1 FIFO1 message received isr
 */
void can_rx1_isr(void) // cppcheck-suppress misra-c2012-8.4
{
	BaseType_t xHigherPriorityTaskWoken;
	can_rx_t rx;

	rx.port = CAN_PORT_1;
	rx.fifo = CAN_FIFO_1;
	rx.release = true;

	xHigherPriorityTaskWoken = pdFALSE;

	can_read(&rx);

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

/** \} End of isr group */
