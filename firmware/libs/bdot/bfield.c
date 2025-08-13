/*
 * bfield.c
 *
 * Copyright (C) 2025, SpaceLab.
 *
 * This file is part of BDOT ADCS.
 *
 * BDOT ADCS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * BDOT is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with BDOT ADCS. If not, see <http://www.gnu.org/licenses/>.
 *
 *
 *
 * \author Felipe Costa Juliano
 *
 * \version 0.1
 *
 * \date 2025/08/05
 */
#include "bfield.h"
#include "BM1422AGMV.h" // <-- 1. Incluir o header do driver
#include <stdio.h>      // Para mensagens de erro

// Funções do driver que precisam ser implementadas em outro lugar
extern uint8_t i2c_write(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint8_t size);
extern uint8_t i2c_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint8_t size);
extern void    delay_ms(uint32_t ms);


/**
 * @brief Lê os dados do magnetômetro BM1422AGMV.
 *
 * Esta função inicializa o sensor, lê os eixos X, Y e Z,
 * e preenche a estrutura MagnetometerData com os valores e o timestamp.
 *
 * @param data Ponteiro para a estrutura MagnetometerData que será preenchida.
 * @return 0 em caso de sucesso, 1 em caso de falha.
 */
uint8_t bfield_read_magnetometer(MagnetometerData *data) {
    BM1422AGMV_t mag_dev;
    uint8_t rc; // Return code

    // Define o endereço I2C do sensor.
    // Verifique no datasheet qual endereço usar (0x0E ou 0x0F).
    mag_dev.device_address = BM1422AGMV_DEVICE_ADDRESS_0E;

    // Inicializa o sensor
    rc = BM1422AGMV_init(&mag_dev);
    if (rc != 0) {
        printf("Erro ao inicializar o magnetometro BM1422AGMV.\n");
        return 1;
    }

    // Array para receber os valores em float [X, Y, Z]
    float mag_values[3];

    // Lê os valores já convertidos para microtesla (uT)
    rc = BM1422AGMV_get_val(&mag_dev, mag_values);
    if (rc != 0) {
        printf("Erro ao ler os valores do magnetometro.\n");
        return 1;
    }

    // Preenche a estrutura com os dados lidos
    data->x = (double)mag_values[0];
    data->y = (double)mag_values[1];
    data->z = (double)mag_values[2];
    data->timestamp = time(NULL); // Pega o timestamp atual

    return 0; // Sucesso
}


/**
 * @brief Calcula a derivada temporal do campo magnético.
 */
MagnetometerData calculate_derivative(
    MagnetometerData current,
    MagnetometerData previous
) {
    MagnetometerData derivative;
    double time_diff = difftime(current.timestamp, previous.timestamp);

    // Evita divisão por zero
    if (time_diff <= 0) {
        time_diff = 1.0;
    }

    derivative.x = (current.x - previous.x) / time_diff;
    derivative.y = (current.y - previous.y) / time_diff;
    derivative.z = (current.z - previous.z) / time_diff;
    derivative.timestamp = current.timestamp;

    return derivative;
}
