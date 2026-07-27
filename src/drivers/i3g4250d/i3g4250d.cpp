/*
 * i3g4250d.cpp
 * 
 * Copyright (C) 2026, SpaceLab.
 * 
 * This file is part of ACS B-DOT.
 * 
 * ACS B-DOT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * ACS B-DOT is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with ACS B-DOT. If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief Gyroscope driver implementation.
 * 
 * \author Felipe Costa Juliano <felipecostajuliano@live.com>
 * 
 * \version 0.1
 * 
 * \date 09/04/2026
 */

#include "i3g4250d.h"
#include "../spi_commons.h" /* Conecta o sensor às funções de barramento */
#include <stdio.h>
#include <stdbool.h>

int gyro_i3g4250d_init(void) {
    /* Checa se o barramento SPI já foi levantado pelo sistema */
    if (spi_fd < 0) {
        printf("SPI is off.\n");
        return -1;
    }

    uint8_t id = spi_read_register(I3G4250D_WHO_AM_I);

    if (id != 0xD3) { 
        printf("Gyroscope not found, read value: 0x%X\n", id);
        return -1;
    }

    /* ODR 100Hz, Cut-off 12.5Hz, Eixos X,Y,Z habilitados */
    spi_write_register(I3G4250D_CTRL_REG1, 0x0F); 
    
    /* Block Data Update contínuo, 245 dps */
    spi_write_register(I3G4250D_CTRL_REG4, 0x80); 

    printf("Gyroscope is ready to use\n");
    return 0;
}

bool gyro_i3g4250d_read_dps(float *gx, float *gy, float *gz) {
    uint8_t raw_data[6];
    
    /* 0x80 (Leitura) | 0x40 (Auto-incremento de endereço) */
    if (spi_read_multiple_registers(I3G4250D_OUT_X_L | 0x80 | 0x40, raw_data, 6) != 0) {
        return false;
    }

    int16_t raw_x = (int16_t)(raw_data[1] << 8 | raw_data[0]);
    int16_t raw_y = (int16_t)(raw_data[3] << 8 | raw_data[2]);
    int16_t raw_z = (int16_t)(raw_data[5] << 8 | raw_data[4]);

    *gx = (float)raw_x * SENSITIVITY_245DPS;
    *gy = (float)raw_y * SENSITIVITY_245DPS;
    *gz = (float)raw_z * SENSITIVITY_245DPS;
    
    return true;
}