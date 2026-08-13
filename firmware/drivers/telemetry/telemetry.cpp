#include <stdint.h>
#include <string.h>
#include "../../pus/pus_crc.h"
#include "../../pus/pus_packet.h"
#include "adcs_telemetry.h"

uint16_t pus_build_tm(uint8_t *tx, const pus_packet_t *pkt) {

    // 1. Header PUS
    tx[0] = (pkt->apid >> 8) & 0xFF;
    tx[1] = pkt->apid & 0xFF;

    tx[2] = pkt->length;
    tx[3] = pkt->service;
    tx[4] = pkt->subtype;

    // 2. Payload
    memcpy(&tx[5], pkt->data, pkt->length);

    // 3. CRC (calculado em cima do pacote inteiro sem CRC)
    uint16_t size_no_crc = 5 + pkt->length;

    uint16_t crc = crc16_ccitt(tx, size_no_crc);

    // 4. adiciona CRC no final
    tx[size_no_crc]     = (crc >> 8) & 0xFF;
    tx[size_no_crc + 1] = crc & 0xFF;

    // 5. retorna tamanho total do frame
    return size_no_crc + 2;
}