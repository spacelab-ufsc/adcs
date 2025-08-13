#include "bdot_task.h"
#include "bfield.c"
#include <stdio.h>  
#include <time.h>
#include <queue.h>

// Mutex para proteger acesso aos dados do magnetômetro
static SemaphoreHandle_t bdot_mutex = NULL;

// Dados armazenados
static MagnetometerData previous_data = {0};
static MagnetometerData current_data  = {0};
QueueHandle_t fila_bdot;

// Função mock de leitura de magnetômetro (troque pelo seu driver real)
static MagnetometerData read_magnetometer(void) {
    MagnetometerData data;
    data.x = (rand() % 200) - 100; // Simula leitura em µT
    data.y = (rand() % 200) - 100;
    data.z = (rand() % 200) - 100;
    data.timestamp = time(NULL); // Marca tempo atual
    return data;
}

// Task que processa o BDOT
static void bdot_task(void *pvParameters) {
    (void)pvParameters;

    previous_data = read_magnetometer();

    for (;;) {
        vTaskDelay(pdMS_TO_TICKS(100)); // 100 ms entre leituras

        // Lê o magnetômetro
        current_data = read_magnetometer();

        // Calcula derivada (BDOT)
        if (xSemaphoreTake(bdot_mutex, portMAX_DELAY) == pdTRUE) {
            MagnetometerData bdot = calculate_derivative(current_data, previous_data);
            xSemaphoreGive(bdot_mutex);
            xQueueSend(fila_bdot, &bdot, portMAX_DELAY);

            // Exibe resultado (bdot.x, bdot.y, bdot.z)
            // Atualiza previous_data para próxima iteração
            previous_data = current_data;
        }
    }
}

// Função para criar a task
void bdot_task_create(void) {
    // Cria o mutex
    bdot_mutex = xSemaphoreCreateMutex();
    if (bdot_mutex == NULL) {
        return;
    }

    // Cria a task do BDOT
    if (xTaskCreate(
            bdot_task,
            "BDOT_Task",
            BDOT_TASK_STACK_SIZE,
            NULL,
            BDOT_TASK_PRIORITY,
            NULL
        ) != pdPASS) {
    }
}
