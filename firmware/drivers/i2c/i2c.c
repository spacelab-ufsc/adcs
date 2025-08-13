/*
 * i2c.c
 * 
 * Copyright The ADCS Contributors.
 * 
 * This file is part of ADCS.
 * 
 * SLCam is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * SLCam is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with ADCS. If not, see <http:/\/www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief I2C driver implementation.
 * 
 * \author Felipe Juliano <felipecostajuliano@live.com>
 * 
 * \version 0.0.1
 * 
 * \date 2025/08/12
 * 
 * \addtogroup i2c
 * \{
 */

#include "i2c.h" // A sua interface de driver, que permanece a mesma.

// Headers da libopencm3 que vamos usar
#include <hal/include/libopencm3/stm32/f1/rcc.h>
#include <hal/include/libopencm3/stm32/f1/gpio.h>
#include <hal/include/libopencm3/stm32/f1/i2c.h>

int i2c_init(i2c_port_t port, i2c_config_t config)
{
    uint32_t i2c_periph;

    // Associa a nossa porta lógica ao periférico correto da libopencm3
    if (port == I2C_PORT_0) {
        i2c_periph = I2C1;
    } else if (port == I2C_PORT_1) {
        i2c_periph = I2C2;
    } else {
        return -1; // Porta inválida
    }

    // 1. Habilita os clocks necessários
    if (i2c_periph == I2C1) {
        // Para I2C1, comumente nos pinos PB6/PB7
        rcc_periph_clock_enable(RCC_GPIOB);
        rcc_periph_clock_enable(RCC_I2C1);
    } else { // I2C2
        // Para I2C2, comumente nos pinos PB10/PB11
        rcc_periph_clock_enable(RCC_GPIOB);
        rcc_periph_clock_enable(RCC_I2C2);
    }
    
    // 2. Configura os pinos GPIO para a função I2C
    // É importante que o pino seja configurado como "Alternate Function Open-Drain"
    if (i2c_periph == I2C1) {
        gpio_set_mode(GPIOB, GPIO_MODE_AF_OD, GPIO_CNF_OUTPUT_ALTFN_OPENDRAIN, GPIO6 | GPIO7);
    } else { // I2C2
        gpio_set_mode(GPIOB, GPIO_MODE_AF_OD, GPIO_CNF_OUTPUT_ALTFN_OPENDRAIN, GPIO10 | GPIO11);
    }

    // 3. Reseta e configura o periférico I2C
    i2c_peripheral_disable(i2c_periph);
    i2c_reset(i2c_periph);
    
    // Define a frequência do clock do barramento (APB1) no registrador do I2C.
    // IMPORTANTE: rcc_apb1_frequency deve ser a frequência real do seu clock APB1.
    // Isso é configurado no início do seu main.c com rcc_clock_setup_in_hse_8mhz_out_72mhz() ou similar.
    // Aqui, vamos assumir um valor comum de 36MHz.
    const uint32_t apb1_freq_mhz = 36;
    i2c_set_clock_frequency(i2c_periph, apb1_freq_mhz);

    // Configura a velocidade (Standard 100kHz ou Fast 400kHz)
    // A libopencm3 calcula o valor do registrador CCR para nós.
    i2c_set_speed(i2c_periph, i2c_speed_sm_100k, apb1_freq_mhz);

    // Define nosso próprio endereço (não relevante para o modo mestre)
    i2c_set_own_address7(i2c_periph, 0x00);
    
    // Habilita o periférico
    i2c_peripheral_enable(i2c_periph);

    return 0; // Sucesso
}


int i2c_write(i2c_port_t port, i2c_slave_adr_t adr, uint8_t *data, uint16_t len)
{
    uint32_t i2c_periph = (port == I2C_PORT_0) ? I2C1 : I2C2;
    uint32_t reg32;
    int i;

    // 1. Envia a condição de START
    i2c_send_start(i2c_periph);

    // 2. Espera até que o START seja enviado e o modo mestre seja selecionado
    // O evento I2C_EVENT_MASTER_MODE_SELECT verifica as flags SR1_SB, SR2_MSL, SR2_BUSY
    while (!((I2C_SR1(i2c_periph) & I2C_SR1_SB)
             && (I2C_SR2(i2c_periph) & (I2C_SR2_MSL | I2C_SR2_BUSY))));

    // 3. Envia o endereço do escravo com o bit de Escrita (WRITE)
    i2c_send_7bit_address(i2c_periph, adr, I2C_WRITE);

    // 4. Espera até que o endereço seja enviado e o escravo dê ACK
    // O evento I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED verifica se a flag ADDR foi setada
    while (!(I2C_SR1(i2c_periph) & I2C_SR1_ADDR)) {
        // Adicione um timeout aqui em um código de produção
    }
    // Limpa a flag ADDR lendo SR1 e depois SR2
    reg32 = I2C_SR2(i2c_periph);
    (void)reg32;

    // 5. Envia os dados, byte a byte
    for (i = 0; i < len; i++) {
        i2c_send_data(i2c_periph, data[i]);
        // Espera o byte ser transmitido
        while (!(I2C_SR1(i2c_periph) & (I2C_SR1_BTF)));
    }

    // 6. Envia a condição de STOP
    i2c_send_stop(i2c_periph);

    return 0;
}

int i2c_read(i2c_port_t port, i2c_slave_adr_t adr, uint8_t *data, uint16_t len)
{
    uint32_t i2c_periph = (port == I2C_PORT_0) ? I2C1 : I2C2;
    uint32_t reg32;
    int i;

    // 1. Envia START
    i2c_send_start(i2c_periph);
    while (!((I2C_SR1(i2c_periph) & I2C_SR1_SB)
             && (I2C_SR2(i2c_periph) & (I2C_SR2_MSL | I2C_SR2_BUSY))));

    // 2. Envia o endereço do escravo com o bit de Leitura (READ)
    i2c_send_7bit_address(i2c_periph, adr, I2C_READ);

    // 3. Espera o ACK do escravo
    while (!(I2C_SR1(i2c_periph) & I2C_SR1_ADDR)) {
        // Adicione um timeout aqui em um código de produção
    }
    // Limpa a flag ADDR
    reg32 = I2C_SR2(i2c_periph);
    (void)reg32;

    // 4. Lê os dados, byte a byte
    for (i = 0; i < len; i++) {
        // Para o último byte, precisamos desabilitar o ACK antes de ler
        if (i == len - 1) {
            i2c_disable_ack(i2c_periph);
        }

        // Espera até que o buffer de recepção tenha dados
        while (!(I2C_SR1(i2c_periph) & I2C_SR1_RxNE));
        data[i] = i2c_get_data(i2c_periph);
    }

    // 5. Envia a condição de STOP e reabilita o ACK para a próxima transação
    i2c_send_stop(i2c_periph);
    i2c_enable_ack(i2c_periph);

    return 0;
}