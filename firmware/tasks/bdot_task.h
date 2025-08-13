#ifndef BDOT_TASK_H
#define BDOT_TASK_H

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "bfield.h"  // Para MagnetometerData e calculate_derivative()

// Prioridade e stack da task
#define BDOT_TASK_PRIORITY   (tskIDLE_PRIORITY + 4)
#define BDOT_TASK_STACK_SIZE (configMINIMAL_STACK_SIZE * 2)

// Protótipo da função de criação da task
void bdot_task_create(void);

#endif // BDOT_TASK_H
