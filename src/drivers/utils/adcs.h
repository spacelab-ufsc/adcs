#ifndef ADCS_H
#define ADCS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Declaração da função para alterar o modo de operação do ADCS
// Recebe um uint8_t extraído de pkt->data[0]
void adcs_set_mode(uint8_t mode);

// Declaração da função para alterar o ganho do B-Dot
// Recebe um float extraído dos bytes de pkt->data
void adcs_set_bdot_gain(float gain);

#ifdef __cplusplus
}
#endif

#endif // ADCS_H