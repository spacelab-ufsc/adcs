#ifndef SPI_COMMS_H
#define SPI_COMMS_H

#include <stdint.h>

/* O file descriptor global para sabermos se o barramento está aberto */
extern int spi_fd;

/* Inicializa o periférico SPI no RTEMS */
void init_sensor_spi(void);

/* Lê um único registrador de 8 bits */
uint8_t spi_read_register(uint8_t reg_address);

/* Escreve um valor de 8 bits em um registrador */
void spi_write_register(uint8_t reg_address, uint8_t value);

/* Lê múltiplos bytes em sequência (Burst Read) */
int spi_read_multiple_registers(uint8_t reg_address, uint8_t *buffer, uint8_t length);

#endif /* SPI_COMMS_H */