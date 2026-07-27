// adcs_status.h
#ifndef HK_ADCS_STATUS_H
#define HK_ADCS_STATUS_H

#include <stdint.h>

#define ROTATION_THRESHOLD_DPS  5   // ajuste conforme necessidade

/**
 * @brief Obtém o status do ADCS com base na leitura do giroscópio.
 * 
 * @return uint8_t
 *         - 0x01 : parado (detumbling concluído)
 *         - 0x02 : em rotação (detumbling ativo)
 *         - 0xFF : erro na leitura do giroscópio (opcional, se implementado)
 */
uint8_t get_status(void);

#endif // HK_ADCS_STATUS_H