/**
 * \brief CAN driver implementation.
 *
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 *
 * \version 0.2.2
 *
 * \date 2023/07/28
 *
 * \addtogroup can
 * \{
 */

#include <hal/include/libopencm3/cm3/nvic.h>
#include <hal/include/libopencm3/stm32/can.h>
#include <hal/include/libopencm3/stm32/gpio.h>
#include <hal/include/libopencm3/stm32/rcc.h>

#include <config/errno.h>
#include <stdint.h>

#include "can.h"

int can_init_drv(can_config_t config)
{
	uint32_t ts1;
	uint32_t ts2;
	uint32_t sjw;
	uint32_t brp;
	int err = ERRNO_SUCCESS;

	switch (config.speed) {
	case CAN_SPEED_1Mbps:
		sjw = CAN_BTR_SJW_2TQ;
		ts1 = CAN_BTR_TS1_15TQ;
		ts2 = CAN_BTR_TS2_2TQ;
		brp = 2U;
		break;
	case CAN_SPEED_500Kbps:
		sjw = CAN_BTR_SJW_2TQ;
		ts1 = CAN_BTR_TS1_15TQ;
		ts2 = CAN_BTR_TS2_2TQ;
		brp = 4U;
		break;
	case CAN_SPEED_250Kbps:
		sjw = CAN_BTR_SJW_2TQ;
		ts1 = CAN_BTR_TS1_15TQ;
		ts2 = CAN_BTR_TS2_2TQ;
		brp = 8U;
		break;
	default: /* OpenCM3 default config */
		sjw = CAN_BTR_SJW_1TQ;
		ts1 = CAN_BTR_TS1_3TQ;
		ts2 = CAN_BTR_TS2_4TQ;
		brp = 12U;
		break;
	}

	if ((config.port != CAN_PORT_1) && (config.port != CAN_PORT_2)) {
		err = ERRNO_DRIVER_NO_PORT;
	}

	if (err == ERRNO_SUCCESS) {
		rcc_periph_clock_enable(RCC_AFIO);
		rcc_periph_clock_enable(RCC_GPIOA);
		rcc_periph_clock_enable(RCC_GPIOB);
		rcc_periph_clock_enable(RCC_CAN);

		/* Configure CAN pin: RX (input pull-up). */
		gpio_set_mode(GPIO_BANK_CAN1_RX, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN,
			      GPIO_CAN1_RX);
		gpio_set(GPIOA, GPIO_CAN1_RX);

		/* Configure CAN pin: TX. */
		gpio_set_mode(GPIO_BANK_CAN1_TX, GPIO_MODE_OUTPUT_50_MHZ,
			      GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_CAN1_TX);

		/* Configure CAN transceiver pins: S and SHDN. */
		gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL,
			      CAN_S_PIN);
		gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL,
			      CAN_SHDN_PIN);
		gpio_clear(GPIOB, CAN_S_PIN);
		gpio_clear(GPIOB, CAN_SHDN_PIN);

		/* Configure PB4 as GPIO. */
		AFIO_MAPR |= AFIO_MAPR_SWJ_CFG_FULL_SWJ_NO_JNTRST;

		if (config.port == CAN_PORT_2) {
			rcc_periph_clock_enable(RCC_CAN2);
		}

		can_reset(config.port);

		if (can_init(config.port, false, true, false, false, false, false, sjw, ts1, ts2,
			     brp, config.loopback, config.silent) < 0) {
			err = ERRNO_DRIVER_FAILED;
		} else {
			if (config.rx_irq) {
				if (config.both_fifos) {
					can_enable_rx_irq(config, CAN_FIFO_0);
					can_enable_rx_irq(config, CAN_FIFO_1);
				} else {
					can_enable_rx_irq(config, CAN_FIFO_0);
				}
			}
		}
	}

	return err;
}

int can_write(can_tx_t *msg)
{
	int err = ERRNO_SUCCESS;

	if (can_transmit(msg->port, msg->id, msg->extended_id, msg->retransmit, msg->len,
			 msg->data) < 0) {
		err = ERRNO_DRIVER_FAILED;
	}

	return err;
}

int can_read(can_rx_t *rx)
{
	can_receive(rx->port, rx->fifo, rx->release, &rx->id, &rx->extended_id, &rx->retransmit,
		    &rx->fmi, &rx->len, rx->data, &rx->timestamp);

	return ERRNO_SUCCESS;
}

bool can_available(can_config_t config)
{
	return can_available_mailbox(config.port);
}

void can_config_filters(can_filter_config_t fconfig)
{
	can_filter_init(fconfig.bank, fconfig.filter_scale_32, fconfig.filter_id_mode,
			fconfig.id_mask_1, fconfig.id_mask_2, fconfig.fifo, fconfig.enable);
}

void can_enable_rx_irq(can_config_t config, can_fifo_e fifo)
{
	if (fifo == CAN_FIFO_0) {
		can_enable_irq(config.port, CAN_IER_FMPIE0);
	} else {
		can_enable_irq(config.port, CAN_IER_FMPIE1);
	}

	can_enable_nvic_irq(config, CAN_FIFO_0);
}

/*
 * The NVIC interrupt priority setting is quite complicated and has
 * to be lower than configMAX_SYSCALL_INTERRUPT_PRIORITY from FreeRTOSConfg.h
 */
void can_enable_nvic_irq(can_config_t config, can_fifo_e fifo)
{
	if (config.port == CAN_PORT_1) {
		if (fifo == CAN_FIFO_0) {
			nvic_enable_irq(NVIC_USB_LP_CAN_RX0_IRQ);
			nvic_set_priority(NVIC_USB_LP_CAN_RX0_IRQ, 200U);
		} else {
			nvic_enable_irq(NVIC_CAN_RX1_IRQ);
			nvic_set_priority(NVIC_CAN_RX1_IRQ, 200U);
		}
	} else {
		if (fifo == CAN_FIFO_0) {
			nvic_enable_irq(NVIC_CAN2_RX0_IRQ);
			nvic_set_priority(NVIC_CAN2_RX0_IRQ, 200U);
		} else {
			nvic_enable_irq(NVIC_CAN2_RX1_IRQ);
			nvic_set_priority(NVIC_CAN2_RX1_IRQ, 200U);
		}
	}
}

/** \} End of can group */
