#ifndef PUS_PARSER_H
#define PUS_PARSER_H

#include <stdint.h>
#include "pus_packet.h"

enum pus_parse_status {
    PUS_PARSE_OK                  = 0,
    PUS_PARSE_ERR_TOO_SHORT       = -1,
    PUS_PARSE_ERR_BAD_CRC         = -2,
    PUS_PARSE_ERR_SIZE_MISMATCH   = -3,
    PUS_PARSE_ERR_INVALID_SERVICE = -4,
    PUS_PARSE_ERR_TOO_LARGE       = -5,
    PUS_PARSE_ERR_NULL_PTR        = -6,
};

//int pus_parse(uint8_t *rx, pus_packet_t *pkt);
int pus_parse(const uint8_t *rx, uint16_t size, pus_packet_t *pkt);

#endif /* PUS_PARSER_H */