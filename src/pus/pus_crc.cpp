/*
 * pus_crc.cpp
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
 * \brief CRC of PUS implementation.
 * 
 * \author Felipe Costa Juliano <felipecostajuliano@live.com>
 * 
 * \version 0.1
 * 
 * \date 09/04/2026
 */

#include <stdint.h>
#include "pus_crc.h"

uint16_t crc16_ccitt(const uint8_t *data, uint16_t length) {
    uint16_t crc = 0xFFFF;

    for (uint16_t i = 0; i < length; i++) {
        crc ^= (uint16_t)data[i] << 8;

        for (uint8_t j = 0; j < 8; j++) {
            if (crc & 0x8000) {
                crc = (crc << 1) ^ 0x1021;
            }

            else {
                
                crc <<= 1;
            
            }
                
        }
    }

    return crc;
}