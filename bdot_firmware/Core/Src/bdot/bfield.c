/*
 * bfield.c
 *
 * Copyright (C) 2021, SpaceLab.
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
 * along with OBDH 2.0. If not, see <http://www.gnu.org/licenses/>.
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
#include <time.h>

MagnetometerData calculate_derivative(
    MagnetometerData current,
    MagnetometerData previous
) {
    MagnetometerData derivative;
    double time_diff = difftime(current.timestamp, previous.timestamp);

    // Evita divisão por zero
    if (time_diff <= 0) time_diff = 1;

    derivative.x = (current.x - previous.x) / time_diff;
    derivative.y = (current.y - previous.y) / time_diff;
    derivative.z = (current.z - previous.z) / time_diff;
    derivative.timestamp = current.timestamp;

    return derivative;
}
