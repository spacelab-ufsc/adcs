/*
 * magnetorquer.h
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
#ifndef CONTROLPID_H
#define CONTROLPID_H

#include "magnetometer.h"

// Estrutura de controle PID
typedef struct {
    double kp;  // Ganho proporcional
    double ki;  // Ganho integral
    double kd;  // Ganho derivativo
    double integral_x, integral_y, integral_z;  // Erros integrais
    double prev_error_x, prev_error_y, prev_error_z;  // Erros anteriores
} PIDController;

// Funções públicas
void init_controller(PIDController* pid, double kp, double ki, double kd);
void actuate_magnetorquer(MagnetometerData derivative, PIDController* pid);

#endif
