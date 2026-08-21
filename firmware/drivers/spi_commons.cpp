#include "spi_commons.h"
#include <rtems.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <dev/spi/spi.h>
#include <stdio.h>
#include <string.h>

#define SPI_DEVICE_PATH "/dev/spi0"
#define SENSOR_SPI_SPEED 10000000 /* 10 MHz */
#define SENSOR_SPI_MODE  (SPI_CPOL | SPI_CPHA) /* Modo 3 */

int spi_fd = -1;

void init_sensor_spi(void) {
    int mode = SENSOR_SPI_MODE;
    uint32_t speed = SENSOR_SPI_SPEED;

    spi_fd = open(SPI_DEVICE_PATH, O_RDWR);
    if (spi_fd < 0) {
        printf("Erro Crítico: Falha ao abrir %s\n", SPI_DEVICE_PATH);
        return;
    }

    ioctl(spi_fd, SPI_IOC_WR_MODE, &mode);
    ioctl(spi_fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);

    printf("SPI inicializado com sucesso no fd: %d\n", spi_fd);
}

uint8_t spi_read_register(uint8_t reg_address) {
    if (spi_fd < 0) return 0;

    /* Evita o erro de narrowing conversion do C++ */
    uint8_t tx_buf[2];
    tx_buf[0] = (uint8_t)(reg_address | 0x80);
    tx_buf[1] = 0x00;

    uint8_t rx_buf[2] = { 0, 0 };

    /* Inicializa a struct com zeros e atribui os campos para evitar erro de ordem e conversão */
    struct spi_ioc_transfer tr = {};
    tr.tx_buf = tx_buf;
    tr.rx_buf = rx_buf;
    tr.len = 2;
    tr.speed_hz = SENSOR_SPI_SPEED;
    tr.bits_per_word = 8;
    tr.cs_change = 0;

    ioctl(spi_fd, SPI_IOC_MESSAGE(1), &tr);
    return rx_buf[1];
}

void spi_write_register(uint8_t reg_address, uint8_t value) {
    if (spi_fd < 0) return;

    /* Evita o erro de narrowing conversion do C++ */
    uint8_t tx_buf[2];
    tx_buf[0] = (uint8_t)(reg_address & 0x7F);
    tx_buf[1] = value;

    uint8_t rx_buf[2] = { 0, 0 };

    /* Inicializa a struct com zeros e atribui os campos para evitar erro de ordem e conversão */
    struct spi_ioc_transfer tr = {};
    tr.tx_buf = tx_buf;
    tr.rx_buf = rx_buf;
    tr.len = 2;
    tr.speed_hz = SENSOR_SPI_SPEED;
    tr.bits_per_word = 8;
    tr.cs_change = 0;

    ioctl(spi_fd, SPI_IOC_MESSAGE(1), &tr);
}

/* Alterado para retornar int e validar sucesso/falha */
int spi_read_multiple_registers(uint8_t reg_address, uint8_t *buffer, uint8_t length) {
    if (spi_fd < 0) return -1;

    /* Tamanho máximo fixado em 16 bytes para evitar alocação dinâmica no RTOS */
    if (length > 15) return -1;

    uint8_t tx_buf[16] = {0};
    uint8_t rx_buf[16] = {0};

    tx_buf[0] = reg_address;

    /* Inicializa a struct com zeros e atribui os campos para evitar erro de ordem e conversão */
    struct spi_ioc_transfer tr = {};
    tr.tx_buf = tx_buf;
    tr.rx_buf = rx_buf;
    tr.len = length + 1;
    tr.speed_hz = SENSOR_SPI_SPEED;
    tr.bits_per_word = 8;
    tr.cs_change = 0;

    if (ioctl(spi_fd, SPI_IOC_MESSAGE(1), &tr) < 0) {
        return -1; /* Falha na leitura ioctl */
    }

    memcpy(buffer, &rx_buf[1], length);

    return 0; /* Sucesso */
}