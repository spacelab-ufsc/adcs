#ifndef PUS_CRC_H
#define PUS_CRC_H

#include <stdint.h>

uint16_t crc16_ccitt(const uint8_t *data, uint16_t length);

#endif /* PUS_CRC_H */