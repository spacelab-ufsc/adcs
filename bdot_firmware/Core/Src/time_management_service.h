#ifndef INC_TIME_MANAGEMENT_SERVICE_H_
#define INC_TIME_MANAGEMENT_SERVICE_H_

#include <stdint.h>

// Estrutura para representar data e hora em UTC
typedef struct time_utc {
    uint16_t year;   // ano completo (ex: 2025)
    uint8_t month;   // 1-12
    uint8_t day;     // 1-31
    uint8_t hour;    // 0-23
    uint8_t minute;  // 0-59
    uint8_t second;  // 0-59
} time_utc;

#endif /* INC_TIME_MANAGEMENT_SERVICE_H_ */
