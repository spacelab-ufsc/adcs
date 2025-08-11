/**
 * \brief CAN driver definition.
 *
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * \author Carlos Augusto Porto Freitas <carlos.portof@hotmail.com>
 *
 * \version 0.2.2
 *
 * \date 2023/12/18
 *
 * \defgroup can CAN
 * \ingroup drivers
 * \{
 */

#ifndef CAN_H_
#define CAN_H_

#include <config/config.h>
#include <hal/include/libopencm3/stm32/can.h>
#include <hal/include/libopencm3/stm32/gpio.h>
#include <utils/macros/macros.h>
#include <stdbool.h>
#include <stdint.h>

#define CAN_MODULE_NAME "CAN"

#define CAN_SHDN_PIN GPIO4
#define CAN_S_PIN GPIO5

/**
 * \brief Tries to receive a message on the CAN queue.
 */
#define can_fifo_pop(__msg, wait_time) \
	xQueueReceive(xCommand_ISR, &(__msg), pdMS_TO_TICKS((wait_time)))

/**
 * \brief Tries to push a message on the CAN queue.
 *
 *  It should only be used on the CAN FIFO ISR and xHigherPriorityTaskWoken
 *  has to be defined, as a BaseType_t, beforehand.
 */
#define can_fifo_push(__msg) \
	xQueueSendFromISR(xCommand_ISR, &(__msg), &xHigherPriorityTaskWoken);

/**
 * @brief CAN ports.
 */
typedef enum {
	CAN_PORT_1 = CAN1, /**< CAN port 1. */
	CAN_PORT_2 = CAN2, /**< CAN port 2. */
} can_port_e;

/**
 * @brief CAN speed type.
 */
typedef enum {
	CAN_SPEED_1Mbps = 0,
	CAN_SPEED_500Kbps,
	CAN_SPEED_250Kbps,
} can_speed_e;

/**
 * @brief CAN ID type.
 */
typedef uint32_t can_id_t;

/**
 * @brief CAN FIFO type.
 */
typedef enum {
	CAN_FIFO_0 = 0U,
	CAN_FIFO_1,
} can_fifo_e;

/**
 * @brief Configuration structure.
 */
typedef struct {
	can_port_e port; /**< Port. */
	can_speed_e speed; /**< Speed in bits per second. */
	bool loopback; /**< Loopback mode (TX=On, RX=Off). */
	bool silent; /**< Silent mode (TX=Off, RX=On). */
	bool both_fifos; /**< Using both fifo's flag */
	bool rx_irq; /**< Enables RX interrupt */
} can_config_t;

/**
 * @brief CAN filter bank type.
 */
typedef uint32_t can_filter_bank_t;

/**
 * @brief CAN filter id/mask type.
 */
typedef uint32_t can_filter_id_t;

/**
 * @brief Filter configuration structure.
 */
typedef struct {
	can_fifo_e fifo; /**< FIFO associated with the filter */
	can_filter_bank_t bank; /**< Filter bank number*/
	can_filter_id_t
		id_mask_1; /**< Either two 16 bit ID AND Mask or one 32 bit ID OR MASK */
	can_filter_id_t
		id_mask_2; /**< Either two 16 bit ID AND Mask or one 32 bit ID OR MASK */
	bool filter_id_mode; /**< Filter mode (if true -> ID LIST MODE else -> ID MASK MODE) */
	bool filter_scale_32; /**< Filter scale (if true -> 32 bit else -> 16 bit) */
	bool enable; /**< Enables the filter */
} can_filter_config_t;

/**
 * @brief CAN data buffer type
 */
typedef uint8_t can_data_t;

/**
 * @brief Trasmission header structure
 */
typedef struct {
	can_port_e port; /**< Can port */
	can_id_t id; /**< Can message id */
	can_data_t data[8]; /**< Can message buffer */
	uint8_t len; /**< Lenght of message in bytes */
	bool extended_id; /**< Extended message ID's? */
	bool retransmit; /**< Request Transmit? */
} can_tx_t;

/**
 * @brief CAN message receiver structure
 */
typedef struct {
	can_port_e port; /**< Can port */
	can_fifo_e fifo; /**< Can FIFO */
	can_id_t id; /**< Can message id */
	can_data_t data[8]; /**< Can message buffer */
	uint8_t len; /**< Lenght of message in bytes */
	uint8_t fmi; /**< ID of the matched filter */
	uint16_t timestamp; /**< Timestamp of the message, only available with time can, otherwise = NULL */
	bool extended_id; /**< Extended message ID's? */
	bool retransmit; /**< Request Transmit? */
	bool release; /**< Release the FIFO automatically after rx */
} can_rx_t;

/**
 * @brief Initializes a given CAN filters
 *
 * @param[in] fconfig are the configuration parameters of the given CAN Filter.
 */
void can_config_filters(can_filter_config_t fconfig);

/**
 * @brief Initializes a given CAN port.
 *
 * @param[in] config are the configuration parameters of the given CAN port.
 *
 * @return The status/error code.
 */
int can_init_drv(can_config_t config);

/**
 * @brief Writes data to a given CAN port.
 *
 * @param[in] msg is the transmission header.
 *
 * @return The status/error code.
 */
int can_write(can_tx_t *msg_header);

/**
 * @brief Reads data from a given CAN port.
 *
 * @param[in,out] rx is a structure that should contain all informations about message
 *  should have the port and fifo struct fields mapped to the ones to read as inputs.
 *
 * @return The status/error code.
 */
int can_read(can_rx_t *rx);

/**
 * @brief Verifies if there is a free transmission mailbox in the CAN port.
 *
 * @param[in] config is the configuration of the desired CAN port.
 *
 * @return True if there is mailboxes available, returns false otherwise.
 */
bool can_available(can_config_t config);

/**
 * @brief Enables rx interrupt for a CAN fifo.
 *
 * @param[in] config is the configuration of the desired CAN port.
 *
 * @param[in] fifo is the CAN fifo which reception triggers a interrupt.
 */
void can_enable_rx_irq(can_config_t config, can_fifo_e fifo);

/**
 * @brief Enables nvic rx interrupt for a CAN fifo.
 *
 * @param[in] config is the configuration of the desired CAN port.
 *
 * @param[in] fifo is the CAN fifo which reception triggers a interrupt.
 */
void can_enable_nvic_irq(can_config_t config, can_fifo_e fifo);

#endif /* CAN_H_ */

/** \} End of can group */
