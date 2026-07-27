#include <stdint.h>
#include <string.h>
#include "pus_crc.h"
#include "pus_packet.h"

int pus_parse(const uint8_t *rx, uint16_t size, pus_packet_t *pkt) {

    if (!rx || !pkt){
         return -6;
    }

    if (size < 8){
        return -1;
    }  

   uint16_t rx_crc = (rx[size-2] << 8) | rx[size-1];
   uint16_t calc_crc = crc16_ccitt(rx, size - 2);

    if (rx_crc != calc_crc){
        return -2;
    }

    pkt->apid    = (rx[0] << 8) | rx[1];
    pkt->length  = rx[2];
    pkt->service = rx[3];
    pkt->subtype = rx[4];

    if ((uint16_t)(5 + pkt->length + 2) != size){
       return -3;
    }

    if (pkt->length > MAX_DATA_SIZE){
        
        return -5;
    } 

    memcpy(pkt->data, &rx[5], pkt->length);

  
    pkt->crc = rx_crc;

    if (pkt->service == 0){
        return -4;
    }

    return 0;

}