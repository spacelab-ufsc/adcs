/*
 * i2c_dma_driver.c
 * * Copyright The ADCS Contributors.
 * * This file is part of ADCS.
 * * ADCS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * * ADCSis distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * * You should have received a copy of the GNU General Public License
 * along with ADCS. If not, see <http:/\/www.gnu.org/licenses/>.
 * */

/**
 * \brief I2C DMA driver implementation.
 * * \author Felipe Juliano <felipecostajuliano@live.com>
 * * \version 0.0.1
 * * \date 2025/12/13
 * * \addtogroup i2c
 * \{
 */
#include "i2c_dma_driver.h"
#include "mutex.h"

/* libopencm3 */
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/i2c.h>
#include <libopencm3/stm32/dma.h>
#include <libopencm3/cm3/nvic.h>

/* FreeRTOS */
#include <FreeRTOS.h>
#include <semphr.h>

/* ========================================================================== */
/* Estado interno do driver                                                    */
/* ========================================================================== */

static struct mutex i2c_mutex;

/* Semáforo para sincronizar task ↔ DMA IRQ */
static SemaphoreHandle_t i2c_done_sem;

/* Estado simples do driver */
typedef enum {
    I2C_IDLE = 0,
    I2C_BUSY,
    I2C_ERROR
} i2c_state_t;

static volatile i2c_state_t i2c_state;

/* ========================================================================== */
/* Inicialização                                                               */
/* ========================================================================== */

int i2c_dma_init(void)
{
    /* Inicializa mutex (OS abstraction) */
    if (mutex_init(&i2c_mutex) != 0)
        return -1;

    /* Semáforo binário para conclusão do DMA */
    i2c_done_sem = xSemaphoreCreateBinary();
    if (!i2c_done_sem)
        return -1;

    /* Clocks */
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_I2C1);
    rcc_periph_clock_enable(RCC_DMA1);

    /* GPIO I2C: PB6 = SCL, PB7 = SDA */
    gpio_set_mode(GPIOB,
                  GPIO_MODE_OUTPUT_50_MHZ,
                  GPIO_CNF_OUTPUT_ALTFN_OPENDRAIN,
                  GPIO6 | GPIO7);

    /* Reset e configuração do I2C */
    i2c_reset(I2C1);
    i2c_peripheral_disable(I2C1);

    i2c_set_clock_frequency(I2C1, 36); /* APB1 = 36 MHz */
    i2c_set_standard_mode(I2C1);       /* 100 kHz */
    i2c_set_ccr(I2C1, 180);
    i2c_set_trise(I2C1, 37);

    /* Habilita DMA no periférico I2C */
    i2c_enable_dma(I2C1);
    i2c_peripheral_enable(I2C1);

    /* IRQ do DMA TX (I2C1_TX = DMA1 Channel 6) */
    nvic_enable_irq(NVIC_DMA1_CHANNEL6_IRQ);

    i2c_state = I2C_IDLE;
    return 0;
}

/* ========================================================================== */
/* Escrita I2C usando DMA                                                      */
/* ========================================================================== */

int i2c_dma_write(uint8_t addr,
                  const uint8_t *data,
                  size_t len,
                  uint32_t timeout_ms)
{
    if (!data || len == 0)
        return -1;

    /* Exclusão mútua do barramento */
    if (mutex_lock_with_timeout(&i2c_mutex, timeout_ms) != 0)
        return -1;

    /* Garante que o semáforo está limpo */
    xSemaphoreTake(i2c_done_sem, 0);

    i2c_state = I2C_BUSY;

    /* ------------------ Configuração do DMA ------------------ */

    dma_channel_reset(DMA1, DMA_CHANNEL6);

    dma_set_peripheral_address(DMA1,
                               DMA_CHANNEL6,
                               (uint32_t)&I2C_DR(I2C1));

    dma_set_memory_address(DMA1,
                            DMA_CHANNEL6,
                            (uint32_t)data);

    dma_set_number_of_data(DMA1,
                           DMA_CHANNEL6,
                           len);

    dma_set_read_from_memory(DMA1, DMA_CHANNEL6);
    dma_enable_memory_increment_mode(DMA1, DMA_CHANNEL6);

    dma_set_peripheral_size(DMA1, DMA_CHANNEL6, DMA_CCR_PSIZE_8BIT);
    dma_set_memory_size(DMA1, DMA_CHANNEL6, DMA_CCR_MSIZE_8BIT);

    dma_enable_transfer_complete_interrupt(DMA1, DMA_CHANNEL6);

    /* ------------------ START + Endereço ------------------ */

    i2c_send_start(I2C1);

    while (!(I2C_SR1(I2C1) & I2C_SR1_SB));
    (void)I2C_SR1(I2C1);

    /* Endereço de 7 bits */
    i2c_send_7bit_address(I2C1, addr & 0x7F, I2C_WRITE);

    while (!(I2C_SR1(I2C1) & I2C_SR1_ADDR));
    (void)I2C_SR1(I2C1);
    (void)I2C_SR2(I2C1);

    /* ------------------ Dispara DMA ------------------ */

    dma_enable_channel(DMA1, DMA_CHANNEL6);

    /* ------------------ Aguarda conclusão ------------------ */

    if (xSemaphoreTake(i2c_done_sem,
                       pdMS_TO_TICKS(timeout_ms)) != pdTRUE)
    {
        /* Timeout: aborta transação */
        dma_disable_channel(DMA1, DMA_CHANNEL6);
        i2c_send_stop(I2C1);

        i2c_state = I2C_ERROR;
        mutex_unlock(&i2c_mutex);
        return -1;
    }

    /* ------------------ Finalização ------------------ */

    i2c_send_stop(I2C1);

    dma_disable_channel(DMA1, DMA_CHANNEL6);
    dma_clear_interrupt_flags(DMA1, DMA_CHANNEL6, DMA_TCIF);

    i2c_state = I2C_IDLE;

    mutex_unlock(&i2c_mutex);
    return 0;
}

/* ========================================================================== */
/* IRQ DMA I2C TX                                                              */
/* ========================================================================== */

void dma1_channel6_isr(void)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    if (dma_get_interrupt_flag(DMA1, DMA_CHANNEL6, DMA_TCIF))
    {
        dma_clear_interrupt_flags(DMA1, DMA_CHANNEL6, DMA_TCIF);

        xSemaphoreGiveFromISR(i2c_done_sem,
                              &xHigherPriorityTaskWoken);
    }

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
