#ifndef __GYRO_H
#define __GYRO_H

#include <stdint.h>

typedef struct {
  void (*Init)(uint16_t);
  void (*DeInit)(void);
  uint8_t (*ReadID)(void);
  void (*RebootCmd)(void);
  void (*LowPower)(uint16_t);
  void (*INT1InterruptConfig)(uint16_t);
  void (*EnableIT)(uint8_t);
  void (*DisableIT)(uint8_t);
  void (*FilterConfig)(uint8_t);
  void (*FilterCmd)(uint8_t);
  void (*GetXYZ)(float *);
} GYRO_DrvTypeDef;

#endif /* __GYRO_H */
