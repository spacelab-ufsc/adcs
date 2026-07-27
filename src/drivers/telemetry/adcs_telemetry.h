#ifndef ADCS_TELEMETRY_H
#define ADCS_TELEMETRY_H

// =========================
// APID (ADCS apenas)
// =========================
#define APID_ADCS 0x01

// =========================
// SERVICES (somente o que o ADCS usa)
// =========================
#define PUS_SERVICE_HK  3
#define PUS_SERVICE_TM  8

// =========================
// HOUSEKEEPING (saúde do sistema)
// =========================
#define HK_ADCS_STATUS      0x01
#define HK_ADCS_POWER       0x02
#define HK_ADCS_TEMPERATURE 0x03
#define HK_ADCS_FAULTS      0x04

// =========================
// ADCS TELEMETRY (dados de controle)
// =========================
#define ADCS_TM_QUATERNION   0x10
#define ADCS_TM_GYRO         0x11
#define ADCS_TM_BFIELD       0x12
#define ADCS_TM_CONTROL_MODE 0x13

#endif