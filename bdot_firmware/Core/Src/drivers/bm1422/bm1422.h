#ifndef BM1422_H
#define BM1422_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"            // Inclusão essencial para tipos HAL
#include <stdint.h>

// Declaração antecipada para resolver dependência circular
typedef struct I2C_HandleTypeDef I2C_HandleTypeDef;

#define BM1422_I2C_ADDR        (0x0F << 1)  // Endereço I2C padrão

// Registros do BM1422
#define BM1422_REG_WHO_AM_I    0x0F
#define BM1422_REG_CTRL1       0x20
#define BM1422_REG_DATA_X      0x28

// Protótipos de funções
HAL_StatusTypeDef BM1422_Init(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef BM1422_ReadData(I2C_HandleTypeDef *hi2c, float *magn_x, float *magn_y, float *magn_z);
HAL_StatusTypeDef BM1422_ReadRegister(I2C_HandleTypeDef *hi2c, uint8_t reg, uint8_t *data, uint8_t len);
HAL_StatusTypeDef BM1422_WriteRegister(I2C_HandleTypeDef *hi2c, uint8_t reg, uint8_t *data, uint8_t len);

#ifdef __cplusplus
}
#endif

#endif // BM1422_H
