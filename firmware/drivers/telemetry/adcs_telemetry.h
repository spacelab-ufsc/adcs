#ifndef ADCS_TELEMETRY_H
#define ADCS_TELEMETRY_H

#include <stdint.h>
#include <stdbool.h>
#include "../../pus/pus_packet.h"

/*
 * =========================
 * APID (ADCS apenas)
 * =========================
 */
#define APID_ADCS 0x01

/*
 * =========================
 * SERVICES (somente o que o ADCS usa)
 * =========================
 */
#define PUS_SERVICE_HK  3
#define PUS_SERVICE_TM  8

/*
 * =========================
 * HOUSEKEEPING (saúde do sistema)
 * =========================
 */
#define HK_ADCS_STATUS      0x01
#define HK_ADCS_POWER       0x02
#define HK_ADCS_TEMPERATURE 0x03
#define HK_ADCS_FAULTS      0x04

/*
 * =========================
 * ADCS TELEMETRY (dados de controle)
 * =========================
 */
#define ADCS_TM_QUATERNION   0x10
#define ADCS_TM_GYRO         0x11
#define ADCS_TM_BFIELD       0x12
#define ADCS_TM_CONTROL_MODE 0x13

enum adcs_telemetry_status {
    ADCS_TELEMETRY_OK                     = 0,
    ADCS_TELEMETRY_ERR_NULL_PTR           = -1,
    ADCS_TELEMETRY_ERR_UNKNOWN_HK_SUBTYPE = -2,
    ADCS_TELEMETRY_ERR_UNKNOWN_TM_SUBTYPE = -3,
};

int check_telemetry(pus_packet_t *pkt, bool hk, uint8_t subtype);

#endif