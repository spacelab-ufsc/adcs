#include <stdint.h>
#include <string.h>
#include "pus_crc.h"
#include "pus_packet.h"
#include "pus_parser.h"

int pus_parse(const uint8_t *rx, uint16_t size, pus_packet_t *pkt) {

    if (!rx || !pkt) {
        return PUS_PARSE_ERR_NULL_PTR;
    }

    if (size < 8) {
        return PUS_PARSE_ERR_TOO_SHORT;
    }

    uint16_t rx_crc = (rx[size - 2] << 8) | rx[size - 1];
    uint16_t calc_crc = crc16_ccitt(rx, size - 2);

    if (rx_crc != calc_crc) {
        return PUS_PARSE_ERR_BAD_CRC;
    }

    pkt->apid = (rx[0] << 8) | rx[1];
    pkt->length = rx[2];
    pkt->service = rx[3];
    pkt->subtype = rx[4];

    if ((uint16_t)(5 + pkt->length + 2) != size) {
        return PUS_PARSE_ERR_SIZE_MISMATCH;
    }

    if (pkt->length > MAX_DATA_SIZE) {
        return PUS_PARSE_ERR_TOO_LARGE;
    }

    memcpy(pkt->data, &rx[5], pkt->length);

    pkt->crc = rx_crc;

    if (pkt->service == 0) {
        return PUS_PARSE_ERR_INVALID_SERVICE;
    }

    return PUS_PARSE_OK;
}