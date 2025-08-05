/*
 * magnetorquer.c
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

#include "controlpid.h"
#include <stdio.h>
#include <math.h>

// Inicializa o controlador PID
void init_controller(PIDController* pid, double kp, double ki, double kd) {
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;

    // Zera todos os estados
    pid->integral_x = 0;
    pid->integral_y = 0;
    pid->integral_z = 0;

    pid->prev_error_x = 0;
    pid->prev_error_y = 0;
    pid->prev_error_z = 0;
}

// Gera o sinal de controle para o magnetorquer
void actuate_magnetorquer(MagnetometerData derivative, PIDController* pid) {
    // O erro é a própria derivada (queremos zerá-la)
    double error_x = derivative.x;
    double error_y = derivative.y;
    double error_z = derivative.z;

    // Atualiza integrais
    pid->integral_x += error_x;
    pid->integral_y += error_y;
    pid->integral_z += error_z;

    // Calcula derivadas do erro
    double derivative_x = error_x - pid->prev_error_x;
    double derivative_y = error_y - pid->prev_error_y;
    double derivative_z = error_z - pid->prev_error_z;

    // Lei de controle PID
    double control_x = -(pid->kp * error_x + pid->ki * pid->integral_x + pid->kd * derivative_x);
    double control_y = -(pid->kp * error_y + pid->ki * pid->integral_y + pid->kd * derivative_y);
    double control_z = -(pid->kp * error_z + pid->ki * pid->integral_z + pid->kd * derivative_z);

    // Normaliza o vetor de controle (mantém direção, limita magnitude)
    double norm = sqrt(control_x*control_x + control_y*control_y + control_z*control_z);
    if(norm > 1.0) {
        control_x /= norm;
        control_y /= norm;
        control_z /= norm;
    }

    // Atualiza erros anteriores
    pid->prev_error_x = error_x;
    pid->prev_error_y = error_y;
    pid->prev_error_z = error_z;


}
