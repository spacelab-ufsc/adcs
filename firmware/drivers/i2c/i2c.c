/*
 * i2c.c
 * * Copyright The ADCS Contributors.
 * * This file is part of ADCS.
 * * SLCam is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * * SLCam is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * * You should have received a copy of the GNU General Public License
 * along with ADCS. If not, see <http:/\/www.gnu.org/licenses/>.
 * */

/**
 * \brief I2C driver implementation.
 * * \author Felipe Juliano <felipecostajuliano@live.com>
 * * \version 0.0.1
 * * \date 2025/08/14
 * * \addtogroup i2c
 * \{
 */
#include "i2c.h"
#include <hal/include/libopencm3/stm32/f1/i2c.h>
#include <hal/include/libopencm3/cm3/common.h>

#define I2C_TIMEOUT 100000

static bool wait_for_flag(uint32_t i2c, uint32_t flag, bool set) {
    for (uint32_t i = 0; i < I2C_TIMEOUT; i++) {
        bool flag_status = !!(I2C_SR1(i2c) & flag);
        
        // Check for errors
        if (I2C_SR1(i2c) & (I2C_SR1_AF | I2C_SR1_ARLO | I2C_SR1_BERR)) {
            return false;
        }
        
        if (flag_status == set) {
            return true;
        }
    }
    return false; // Timeout
}

void i2c_init(const i2c_config_t *config) {
    // Reset and disable peripheral before configuration
    i2c_reset(config->i2c_peripheral);
    i2c_peripheral_disable(config->i2c_peripheral);
    
    // Calculate APB clock in MHz (required by libopencm3)
    uint32_t apb_clock_mhz = config->clock_frequency / 1000000;
    
    // Configure speed mode
    i2c_set_speed(config->i2c_peripheral, config->mode, apb_clock_mhz);
    
    // Configure own address if needed
    if (config->own_address) {
        i2c_set_own_7bit_slave_address(config->i2c_peripheral, config->own_address);
    }
    
    // Enable peripheral
    i2c_peripheral_enable(config->i2c_peripheral);
}

bool i2c_write(uint32_t i2c, uint8_t slave_addr, const uint8_t *data, size_t len) {
    // Send START condition
    i2c_send_start(i2c);
    if (!wait_for_flag(i2c, I2C_SR1_SB, true)) return false;
    
    // Send slave address (WRITE)
    i2c_send_7bit_address(i2c, slave_addr, I2C_WRITE);
    if (!wait_for_flag(i2c, I2C_SR1_ADDR, true)) return false;
    (void)I2C_SR2(i2c); // Clear ADDR flag
    
    // Send data
    for (size_t i = 0; i < len; i++) {
        i2c_send_data(i2c, data[i]);
        if (!wait_for_flag(i2c, I2C_SR1_TxE, true)) return false;
    }
    
    // Wait for transfer completion
    if (!wait_for_flag(i2c, I2C_SR1_BTF, true)) return false;
    
    return true;
}

bool i2c_read(uint32_t i2c, uint8_t slave_addr, uint8_t *buffer, size_t len) {
    // Send START condition
    i2c_send_start(i2c);
    if (!wait_for_flag(i2c, I2C_SR1_SB, true)) return false;
    
    // Send slave address (READ)
    i2c_send_7bit_address(i2c, slave_addr, I2C_READ);
    if (!wait_for_flag(i2c, I2C_SR1_ADDR, true)) return false;
    
    // Setup ACK/NACK based on data length
    if (len > 1) {
        i2c_enable_ack(i2c);
    } else {
        i2c_disable_ack(i2c);
    }
    (void)I2C_SR2(i2c); // Clear ADDR flag
    
    // Receive data
    for (size_t i = 0; i < len; i++) {
        if (i == len - 1) {
            i2c_disable_ack(i2c);
            i2c_send_stop(i2c);
        }
        
        if (!wait_for_flag(i2c, I2C_SR1_RxNE, true)) return false;
        buffer[i] = i2c_get_data(i2c);
    }
    
    return true;
}

bool i2c_write_read(uint32_t i2c, uint8_t slave_addr, 
                   const uint8_t *write_data, size_t write_len,
                   uint8_t *read_data, size_t read_len) {
    // Write phase
    i2c_send_start(i2c);
    if (!wait_for_flag(i2c, I2C_SR1_SB, true)) return false;
    
    i2c_send_7bit_address(i2c, slave_addr, I2C_WRITE);
    if (!wait_for_flag(i2c, I2C_SR1_ADDR, true)) return false;
    (void)I2C_SR2(i2c);
    
    for (size_t i = 0; i < write_len; i++) {
        i2c_send_data(i2c, write_data[i]);
        if (!wait_for_flag(i2c, I2C_SR1_TxE, true)) return false;
    }
    
    // Repeated START for read
    i2c_send_start(i2c);
    if (!wait_for_flag(i2c, I2C_SR1_SB, true)) return false;
    
    i2c_send_7bit_address(i2c, slave_addr, I2C_READ);
    if (!wait_for_flag(i2c, I2C_SR1_ADDR, true)) return false;
    
    // Read phase
    if (read_len > 1) {
        i2c_enable_ack(i2c);
    } else {
        i2c_disable_ack(i2c);
    }
    (void)I2C_SR2(i2c);
    
    for (size_t i = 0; i < read_len; i++) {
        if (i == read_len - 1) {
            i2c_disable_ack(i2c);
            i2c_send_stop(i2c);
        }
        
        if (!wait_for_flag(i2c, I2C_SR1_RxNE, true)) return false;
        read_data[i] = i2c_get_data(i2c);
    }
    
    return true;
}

bool i2c_transfer_complete(uint32_t i2c) {
    return wait_for_flag(i2c, I2C_SR1_BTF, true);
}

void i2c_stop(uint32_t i2c) {
    i2c_send_stop(i2c);
    while (I2C_CR1(i2c) & I2C_CR1_STOP);
}