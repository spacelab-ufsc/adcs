#pragma once

#include <stdint.h>
#include <stddef.h>

int i2c_dma_init(void);

/* Escrita I2C não-bloqueante para CPU (bloqueia a task) */
int i2c_dma_write(uint8_t addr,
                  const uint8_t *data,
                  size_t len,
                  uint32_t timeout_ms);
