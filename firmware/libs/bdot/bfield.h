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

// bfield.h

#ifndef BFIELD_H
#define BFIELD_H

#include <time.h>
#include <stdint.h> // Para uint8_t

// Estrutura para armazenar os dados do magnetômetro
typedef struct {
    double x;
    double y;
    double z;
    time_t timestamp;
} MagnetometerData;

// Protótipo da nova função para ler os dados do sensor
uint8_t bfield_read_magnetometer(MagnetometerData *data);

// Protótipo da função que você já tinha
MagnetometerData calculate_derivative(
    MagnetometerData current,
    MagnetometerData previous
);

#endif // BFIELD_H