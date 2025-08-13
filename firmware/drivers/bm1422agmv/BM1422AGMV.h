/*****************************************************************************
  BM1422AGMV.h  (versão adaptada para C)

  Baseado no código original da ROHM (c) 2018
******************************************************************************/

#ifndef _BM1422AGMV_H_
#define _BM1422AGMV_H_

#include <stdint.h>

/* Endereços I²C */
#define BM1422AGMV_DEVICE_ADDRESS_0E   (0x0E)    // 7-bit Address
#define BM1422AGMV_DEVICE_ADDRESS_0F   (0x0F)    // 7-bit Address

/* Valores de identificação */
#define BM1422AGMV_WIA_VAL             (0x41)

/* Registradores */
#define BM1422AGMV_WIA                 (0x0F)
#define BM1422AGMV_DATAX               (0x10)
#define BM1422AGMV_STA1                (0x18)
#define BM1422AGMV_CNTL1               (0x1B)
#define BM1422AGMV_CNTL2               (0x1C)
#define BM1422AGMV_CNTL3               (0x1D)
#define BM1422AGMV_AVE_A               (0x40)
#define BM1422AGMV_CNTL4               (0x5C)

/* Bits */
#define BM1422AGMV_STA1_RD_DRDY        (1 << 6)

#define BM1422AGMV_CNTL1_FS1           (1 << 1)
#define BM1422AGMV_CNTL1_ODR_10Hz      (0 << 3)
#define BM1422AGMV_CNTL1_RST_LV        (1 << 5)
#define BM1422AGMV_CNTL1_OUT_BIT       (1 << 6)
#define BM1422AGMV_CNTL1_PC1           (1 << 7)

#define BM1422AGMV_CNTL2_DRP           (1 << 2)
#define BM1422AGMV_CNTL2_DREN          (1 << 3)

#define BM1422AGMV_CNTL3_FORCE         (1 << 6)

#define BM1422AGMV_AVE_A_AVE4          (0 << 2)

/* Valores padrão de configuração */
#define BM1422AGMV_CNTL1_VAL           (BM1422AGMV_CNTL1_FS1 | BM1422AGMV_CNTL1_OUT_BIT | BM1422AGMV_CNTL1_PC1)
#define BM1422AGMV_CNTL2_VAL           (BM1422AGMV_CNTL2_DREN)
#define BM1422AGMV_CNTL3_VAL           (BM1422AGMV_CNTL3_FORCE)
#define BM1422AGMV_CNTL4_VAL           (0x0000)
#define BM1422AGMV_AVE_A_VAL           (BM1422AGMV_AVE_A_AVE4)

/* Sensibilidade (LSB/µT) */
#define BM1422AGMV_14BIT_SENS          (24)
#define BM1422AGMV_12BIT_SENS          (6)

/* Estrutura do driver */
typedef struct {
    int device_address;
    float sens;
} BM1422AGMV_t;

/* Protótipos */
int BM1422AGMV_init(BM1422AGMV_t *dev);
int BM1422AGMV_get_rawval(BM1422AGMV_t *dev, int *data);
int BM1422AGMV_get_val(BM1422AGMV_t *dev, float *data);
void    BM1422AGMV_convert_uT(BM1422AGMV_t *dev, int16_t *rawdata, float *data);
int BM1422AGMV_write(BM1422AGMV_t *dev, int reg, int *data, int size);
int BM1422AGMV_read(BM1422AGMV_t *dev, int reg, int *data, int size);


#endif // _BM1422AGMV_H_
