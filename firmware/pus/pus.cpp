#include <stdint.h>
#include "pus_packet.h"
#include "pus_crc.h"
#include "pus_dispatch.h"
#include "pus_parser.h"

/**
 * @brief função principal de entrada (ex: chamada na UART)
 */
void pus_handle_rx(uint8_t *rx, uint16_t size) {

    pus_packet_t pkt;

    int ret = pus_parse(rx, size, &pkt);

    if (ret != 0) {
        // erro → pode logar ou mandar NACK
        pus_send_ack(2); // erro genérico
        return;
    }

    // pacote válido → despacha
    pus_dispatch(&pkt);
}