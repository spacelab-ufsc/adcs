#include <rtems.h>
#include <bsp.h>
#include <rtems/bspIo.h> /* Adicionado para resolver o erro do printk */

/*
 * Cabeçalho onde queues_init() está declarado.
 * Ajuste o nome da pasta ou arquivo se for diferente no seu projeto.
 */
#include "queues/queues.h"

/* ========================================================= */
/* Declarações de Tarefas                                    */
/* ========================================================= */

/* Protótipo da tarefa do Loop de Controle de Atitude (ACS) */
rtems_task acs_control_task(rtems_task_argument argument);


/* ========================================================= */
/* Tarefa Init - Ponto de Entrada da Aplicação RTEMS         */
/* ========================================================= */

extern "C" rtems_task Init(rtems_task_argument argument) {
    rtems_id acs_task_id;
    rtems_name acs_task_name = rtems_build_name('A', 'C', 'S', 'T');
    rtems_status_code status;

    printk("\n--- Iniciando Computador de Bordo (STM32F7) ---\n");

    queues_init();

    /* 1. Criação da tarefa do loop de controle */
    status = rtems_task_create(
        acs_task_name,
        10,                                 /* Prioridade (ex: 10, sendo 1 a mais alta) */
        RTEMS_MINIMUM_STACK_SIZE * 2,       /* Tamanho da pilha em RAM */
        RTEMS_DEFAULT_MODES,                /* Preempção habilitada por padrão */
        RTEMS_DEFAULT_ATTRIBUTES,           /* Tarefa local clássica */
        &acs_task_id
    );

    if (status != RTEMS_SUCCESSFUL) {
        printk("Erro Crítico: Falha ao criar a tarefa ACS (%s)\n", rtems_status_text(status));
        rtems_task_delete(RTEMS_SELF);
    }

    /* 2. Inicialização da tarefa */
    status = rtems_task_start(acs_task_id, acs_control_task, 0);

    if (status != RTEMS_SUCCESSFUL) {
        printk("Erro Crítico: Falha ao iniciar a tarefa ACS (%s)\n", rtems_status_text(status));
    }

    /* 3. Limpeza: A tarefa Init finalizou o escalonamento inicial,
          podemos deletá-la para liberar recursos do sistema. */
    printk("Setup concluído. Deletando tarefa Init.\n");
    rtems_task_delete(RTEMS_SELF);
}


/* ========================================================= */
/* Implementação das Tarefas da Aplicação                    */
/* ========================================================= */

rtems_task acs_control_task(rtems_task_argument argument) {
    printk("Tarefa de Controle de Atitude em execucao...\n");

    /* Define a frequência da malha de controle: 10 Hz */
    /* Assumindo que o tick configurado no rtems_config.c é de 1ms */
    rtems_interval ticks_per_period = rtems_clock_get_ticks_per_second() / 10;

    while(1) {
        /* Ponto de instrumentação/debug */
        // printk("Executando ciclo ACS...\n");

        /* ---------------------------------------------------
         * Lógica da Malha de Controle (Exemplo)
         * ---------------------------------------------------
         * 1. Aquisição de Sensores (ex: Leitura I2C/SPI do Magnetômetro)
         * 2. Algoritmo de Determinação de Atitude
         * 3. Cálculo da Lei de Controle
         * 4. Comando dos Atuadores (ex: PWM para Bobinas Magnéticas/Torquers)
         */

        /* Suspende a execução desta tarefa até o próximo período.
           Isso garante o determinismo temporal da malha e libera
           o processador para executar tarefas de menor prioridade. */
        rtems_task_wake_after(ticks_per_period);
    }
}

#include "rtems_config.h"