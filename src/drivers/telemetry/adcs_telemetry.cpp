#include <stdint.h>
#include <stdbool.h>
#include "adcs_telemetry.h"
#include "../../pus/pus_packet.h"
#include "../utils/HK_ADCS_STATUS.h"

int check_telemetry(pus_packet_t *pkt, bool hk, uint8_t subtype) {

    if (!pkt) return -1;

    pkt->apid = APID_ADCS;

    if (hk) {

        pkt->service = PUS_SERVICE_HK;

        switch (subtype) {

            case HK_ADCS_STATUS:
                pkt->subtype = HK_ADCS_STATUS;
                pkt->length = 2;
                pkt->data[0] = get_status();
                pkt->data[1] = 2;
                break;

            case HK_ADCS_POWER:
                pkt->subtype = HK_ADCS_POWER;
                pkt->length = 2;
                pkt->data[0] = 10;
                pkt->data[1] = 20;
                break;

            case HK_ADCS_TEMPERATURE:
                pkt->subtype = HK_ADCS_TEMPERATURE;
                pkt->length = 2;
                pkt->data[0] = 30;
                pkt->data[1] = 40;
                break;

            case HK_ADCS_FAULTS:
                pkt->subtype = HK_ADCS_FAULTS;
                pkt->length = 1;
                pkt->data[0] = 0;
                break;

            default:
                return -2;
        }

    } else {

        pkt->service = PUS_SERVICE_TM;

        switch (subtype) {

            case ADCS_TM_QUATERNION:
                pkt->subtype = ADCS_TM_QUATERNION;
                pkt->length = 16;
                break;

            case ADCS_TM_GYRO:
                pkt->subtype = ADCS_TM_GYRO;
                pkt->length = 12;
                break;

            case ADCS_TM_BFIELD:
                pkt->subtype = ADCS_TM_BFIELD;
                pkt->length = 12;
                break;

            case ADCS_TM_CONTROL_MODE:
                pkt->subtype = ADCS_TM_CONTROL_MODE;
                pkt->length = 1;
                pkt->data[0] = 0;
                break;

            default:
                return -3;
        }
    }

    return 0;
}