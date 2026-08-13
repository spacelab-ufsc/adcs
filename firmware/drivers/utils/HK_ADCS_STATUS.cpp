#include <stdint.h>
#include <stdbool.h>      // para bool, true, false
#include "../i3g4250d/i3g4250d.h"

#define ROTATION_THRESHOLD_DPS  5   // 5 graus por segundo

uint8_t get_status(void) {
    float x, y, z;   // variáveis para receber os valores em dps

    // Tenta ler o giroscópio; se falhar, retorna código de erro 0xFF
    if (!gyro_i3g4250d_read_dps(&x, &y, &z)) {
        return 0xFF;   // erro de comunicação com o sensor
    }

    // Calcula a magnitude ao quadrado (evita sqrt)
    float magnitude_sq = x*x + y*y + z*z;
    float threshold_sq = (float)ROTATION_THRESHOLD_DPS * ROTATION_THRESHOLD_DPS;

    if (magnitude_sq < threshold_sq) {
        return 0x01;   // parado – detumbling concluído
    } else {
        return 0x02;   // girando – detumbling ativo
    }
}