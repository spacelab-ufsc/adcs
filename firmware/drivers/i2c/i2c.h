#ifndef I2C_DRIVER_H
#define I2C_DRIVER_H

#include <libopencm3/stm32/i2c.h>
#include <stdbool.h>
#include <stdint.h>

typedef enum {
    I2C_MODE_STANDARD = i2c_speed_sm_100k,
    I2C_MODE_FAST = i2c_speed_fm_400k,
    I2C_MODE_FAST_PLUS = i2c_speed_fmp_1m
} i2c_mode_t;

typedef struct {
    uint32_t i2c_peripheral;
    uint32_t clock_frequency;
    uint8_t own_address;
    i2c_mode_t mode;
} i2c_config_t;

void i2c_init(const i2c_config_t *config);
bool i2c_write(uint32_t i2c, uint8_t slave_addr, const uint8_t *data, size_t len);
bool i2c_read(uint32_t i2c, uint8_t slave_addr, uint8_t *buffer, size_t len);
bool i2c_write_read(uint32_t i2c, uint8_t slave_addr, 
                   const uint8_t *write_data, size_t write_len,
                   uint8_t *read_data, size_t read_len);
bool i2c_transfer_complete(uint32_t i2c);
void i2c_stop(uint32_t i2c);

#endif // I2C_DRIVER_H