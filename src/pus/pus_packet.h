#ifndef PUS_PACKET_H
#define PUS_PACKET_H

#include <stdint.h>

#define MAX_DATA_SIZE 256

typedef struct {
    uint16_t apid;
    uint16_t length;
    uint8_t service;
    uint8_t subtype;
    uint8_t data[256];
    uint16_t crc;
} pus_packet_t;

#endif