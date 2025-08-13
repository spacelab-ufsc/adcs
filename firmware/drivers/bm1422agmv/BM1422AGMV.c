#include <stdio.h>
#include <stdint.h>
#include <string.h>     // <-- ADICIONAR: Para usar memcpy
#include "BM1422AGMV.h"
#include <drivers/i2c/i2c.h>     // <-- ADICIONAR: Header do seu driver I2C
#include <system/sys_log/sys_log.h>

#define MAGNETOMETER_I2C_PORT   I2C_PORT_0 // Porta I2c

extern void delay_ms(uint32_t ms);

typedef struct {
    int device_address;
    float sens;
} BM1422AGMV_t;

// Prototipos
int BM1422AGMV_init(BM1422AGMV_t *dev);
int BM1422AGMV_get_rawval(BM1422AGMV_t *dev, int *data);
int BM1422AGMV_get_val(BM1422AGMV_t *dev, float *data);
void BM1422AGMV_convert_uT(BM1422AGMV_t *dev, int16_t *rawdata, float *data);
int BM1422AGMV_write(BM1422AGMV_t *dev, int reg, int *data, int size);
int BM1422AGMV_read(BM1422AGMV_t *dev, int reg, int *data, int size);

// Funções I²C 
extern int i2c_write(int dev_addr, int reg_addr, int *data, int size);
extern int i2c_read(int dev_addr, int reg_addr, int *data, int size);
extern void    delay_ms(uint32_t ms);

int BM1422AGMV_init(BM1422AGMV_t *dev)
{
    int rc;
    int reg;
   int buf[2];

    rc = BM1422AGMV_read(dev, BM1422AGMV_WIA, &reg, sizeof(reg));
    if (rc != 0) {

        sys_log_print_event_from_module(SYS_LOG_ERROR, BM1422AGMV_init, "Can't access BM1422AGMV");
        sys_log_new_line();        
        
        return rc;
    }

    if (reg != BM1422AGMV_WIA_VAL) {

        sys_log_print_event_from_module(SYS_LOG_ERROR, BM1422AGMV_init, "Can't find BM1422AGMV");
        sys_log_new_line(); 
        
        return 1;
    }

    // Step1
    reg = BM1422AGMV_CNTL1_VAL;
    rc = BM1422AGMV_write(dev, BM1422AGMV_CNTL1, &reg, sizeof(reg));
    if (rc != 0){ 

        sys_log_print_event_from_module(SYS_LOG_ERROR, BM1422AGMV_init, "Can't write BM1422AGMV_CNTL1 Register");
        sys_log_new_line(); 

        return rc;
    }

    // Checa resolução 12 ou 14 bits
    buf[0] = (BM1422AGMV_CNTL1_VAL & BM1422AGMV_CNTL1_OUT_BIT);
    if (buf[0] == BM1422AGMV_CNTL1_OUT_BIT) {
        dev->sens = BM1422AGMV_14BIT_SENS;
    } else {
        dev->sens = BM1422AGMV_12BIT_SENS;
    }

    delay_ms(1);

    buf[0] = (BM1422AGMV_CNTL4_VAL >> 8) & 0xFF;
    buf[1] = (BM1422AGMV_CNTL4_VAL & 0xFF);
    rc = BM1422AGMV_write(dev, BM1422AGMV_CNTL4, buf, sizeof(buf));
    if (rc != 0) {

        sys_log_print_event_from_module(SYS_LOG_ERROR, BM1422AGMV_init, "Can't write BM1422AGMV_CNTL4 Register");
        sys_log_new_line(); 

        return rc;
    }

    // Step2
    reg = BM1422AGMV_CNTL2_VAL;
    rc = BM1422AGMV_write(dev, BM1422AGMV_CNTL2, &reg, sizeof(reg));
    if (rc != 0) {

        sys_log_print_event_from_module(SYS_LOG_ERROR, BM1422AGMV_init, "Can't write BM1422AGMV_CNTL2 Register");
        sys_log_new_line(); 

        return rc;
    }

    // Step3 (Option)
    reg = BM1422AGMV_AVE_A_VAL;
    rc = BM1422AGMV_write(dev, BM1422AGMV_AVE_A, &reg, sizeof(reg));
    if (rc != 0) {

        sys_log_print_event_from_module(SYS_LOG_ERROR, BM1422AGMV_init, "Can't write BM1422AGMV_AVE_A Register");
        sys_log_new_line(); 

        return rc;
    }

    return 0;
}

int BM1422AGMV_get_rawval(BM1422AGMV_t *dev, int *data)
{
    int rc;
    int reg;

    // Step4
    reg = BM1422AGMV_CNTL3_VAL;
    rc = BM1422AGMV_write(dev, BM1422AGMV_CNTL3, &reg, sizeof(reg));
    if (rc != 0) {

        sys_log_print_event_from_module(SYS_LOG_ERROR, BM1422AGMV_init, "Can't write BM1422AGMV_CNTL3 Register");
        sys_log_new_line(); 

        return rc;
    }

    delay_ms(2);

    rc = BM1422AGMV_read(dev, BM1422AGMV_DATAX, data, 6);
    if (rc != 0) {

        sys_log_print_event_from_module(SYS_LOG_ERROR, BM1422AGMV_init, "Can't get BM1422AGMV magnet values");
        sys_log_new_line(); 

    }

    return rc;
}

int BM1422AGMV_get_val(BM1422AGMV_t *dev, float *data)
{
    int rc;
    int val[6];
    int16_t mag[3];

    rc = BM1422AGMV_get_rawval(dev, val);
    if (rc != 0) {
        return rc;
    }

    mag[0] = ((int16_t)val[1] << 8) | (val[0]);
    mag[1] = ((int16_t)val[3] << 8) | (val[2]);
    mag[2] = ((int16_t)val[5] << 8) | (val[4]);

    BM1422AGMV_convert_uT(dev, mag, data);

    return 0;
}

void BM1422AGMV_convert_uT(BM1422AGMV_t *dev, int16_t *rawdata, float *data)
{
    data[0] = (float)rawdata[0] / dev->sens;
    data[1] = (float)rawdata[1] / dev->sens;
    data[2] = (float)rawdata[2] / dev->sens;
}

// DEPOIS (versão corrigida)
int BM1422AGMV_write(BM1422AGMV_t *dev, int reg, int *data, int size)
{
    int buffer[size + 1];

    // O primeiro byte do buffer deve ser o endereço do registrador
    buffer[0] = reg;
    // Copia os dados a serem escritos logo em seguida
    memcpy(&buffer[1], data, size);

    // Chama a função do SEU driver, passando a porta, o endereço do dispositivo,
    // o buffer completo e o tamanho total (size + 1 byte do registrador).
    return i2c_write(MAGNETOMETER_I2C_PORT, dev->device_address, buffer, size + 1);
}

// DEPOIS (versão corrigida)
int BM1422AGMV_read(BM1422AGMV_t *dev, int reg, int *data, int size)
{
    int status;

    // Etapa 1: Escreve o endereço do registrador que queremos ler, sem enviar dados adicionais.
    status = i2c_write(MAGNETOMETER_I2C_PORT, dev->device_address, &reg, 1);
    if (status != 0) {
        // Se a escrita falhar, não adianta tentar ler.
        return status;
    }

    // Etapa 2: Lê a quantidade de bytes solicitada do dispositivo.
    return i2c_read(MAGNETOMETER_I2C_PORT, dev->device_address, data, size);
}