#ifndef ADCS_H
#define ADCS_H

#include <stdint.h>

/**
 * @brief Declaração da função para alterar o modo de operação do ADCS
 * @param mode Recebe um uint8_t extraído de pkt->data[0]
 */
void adcs_set_mode(uint8_t mode);

/**
 * @brief Declaração da função para alterar o ganho do B-Dot
 * @param gain Recebe um float extraído dos bytes de pkt->data
 */
void adcs_set_bdot_gain(float gain);

#endif // ADCS_H