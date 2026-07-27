#include <stdint.h>
#include "pus_packet.h"
#include "pus_crc.h"
#include "pus_dispatch.h"
#include "../drivers/utils/adcs.h"

// interface do seu ADCS (core)
//#include "adcs.h"


void pus_dispatch(pus_packet_t *pkt) {

    // 1. VERIFICAÇÃO DE TC (ACK de recepção)
    send_ack(0); // acceptance OK

    switch (pkt->service) {

        // -----------------------------
        // TEST SERVICE
        // -----------------------------
        case PUS_SERVICE_TEST:
            send_test();
            send_ack(1); // completion OK
            break;

        // -----------------------------
        // HOUSEKEEPING
        // -----------------------------
        case PUS_SERVICE_HK:
            send_hk();
            send_ack(1);
            break;

        // -----------------------------
        // COMMAND SERVICE (principal)
        // -----------------------------
        case PUS_SERVICE_COMMAND:

            switch (pkt->subtype) {

                case SUBTYPE_SET_MODE:
                    adcs_set_mode(pkt->data[0]);
                    send_ack(1);
                    break;

                case SUBTYPE_SET_BDOT_GAIN:
                    adcs_set_bdot_gain(*(float*)&pkt->data[0]);
                    send_ack(1);
                    break;

                default:
                    send_ack(2); // unknown command
                    break;
            }

            break;

        // -----------------------------
        // INVALID SERVICE
        // -----------------------------
        default:
            send_ack(2); // service not supported
            break;
    }
}