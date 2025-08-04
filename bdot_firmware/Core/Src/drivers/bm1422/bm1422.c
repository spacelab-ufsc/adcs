#include "bm1422.h"
#include "stm32f1xx_hal.h"

// Definições específicas para STM32F1
#if defined(STM32F1xx)
  #include "stm32f1xx_hal_i2c.h"

  // Mapeamento para o padrão F1
  #ifndef I2C_MEMADD_SIZE_8BIT
    #define I2C_MEMADD_SIZE_8BIT  I2C_MEMADD_SIZE_8BIT
  #endif
#endif

HAL_StatusTypeDef BM1422_Init(I2C_HandleTypeDef *hi2c) {
    uint8_t ctrl_reg = 0x01; // Modo de medição contínua
    return BM1422_WriteRegister(hi2c, BM1422_REG_CTRL1, &ctrl_reg, 1);
}

HAL_StatusTypeDef BM1422_ReadData(I2C_HandleTypeDef *hi2c, float *magn_x, float *magn_y, float *magn_z) {
    uint8_t data[6];
    HAL_StatusTypeDef status;

    status = BM1422_ReadRegister(hi2c, BM1422_REG_DATA_X, data, 6);
    if(status != HAL_OK) return status;

    // Converter dados brutos (16 bits signed)
    int16_t raw_x = (int16_t)((data[1] << 8) | data[0]);
    int16_t raw_y = (int16_t)((data[3] << 8) | data[2]);
    int16_t raw_z = (int16_t)((data[5] << 8) | data[4]);

    // Conversão para microtesla (ajustar conforme sensibilidade)
    const float sensitivity = 0.00015f; // 0.15 µT/LSB para ±1200µT
    *magn_x = raw_x * sensitivity;
    *magn_y = raw_y * sensitivity;
    *magn_z = raw_z * sensitivity;

    return HAL_OK;
}

HAL_StatusTypeDef BM1422_WriteRegister(I2C_HandleTypeDef *hi2c, uint8_t reg, uint8_t *data, uint8_t len) {
    return HAL_I2C_Mem_Write(
        hi2c,
        BM1422_I2C_ADDR,
        reg,
        data,
        len,
        100
    );
}

HAL_StatusTypeDef BM1422_ReadRegister(I2C_HandleTypeDef *hi2c, uint8_t reg, uint8_t *data, uint8_t len) {
    return HAL_I2C_Mem_Read(
        hi2c,
        BM1422_I2C_ADDR,
        reg,
        data,
        len,
        100
    );
}
