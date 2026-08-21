/*
 * i34250d.cpp
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
 * along with ACS B-DOT. If not, see <http:/\/www.gnu.org/licenses/>.
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


#ifndef GYRO_I3G4250D_H
#define GYRO_I3G4250D_H

#include <stdint.h>

/* Mapa de Registradores do Giroscópio I3G4250D */
#define I3G4250D_WHO_AM_I      0x0F
#define I3G4250D_CTRL_REG1     0x20
#define I3G4250D_CTRL_REG4     0x23
#define I3G4250D_OUT_X_L       0x28

/* Sensibilidade para a escala padrão de 245 dps */
#define SENSITIVITY_245DPS     0.00875f

/* Protótipos */
int gyro_i3g4250d_init(void);
bool gyro_i3g4250d_read_dps(float *gx, float *gy, float *gz);

#endif /* GYRO_I3G4250D_H */