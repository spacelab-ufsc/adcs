#include <rtems.h>

/* ========================================================= */
/* 1. Drivers Básicos da Placa (BSP)                         */
/* ========================================================= */
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER

// Não ative CONSOLE DRIVER, o projeto utiliza driver de UART fora do BSP
//#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER

/* ========================================================= */
/* 2. Configuração de Tempo                                  */
/* ========================================================= */
#define CONFIGURE_MICROSECONDS_PER_TICK 1000 /* 1 ms por tick */

/* ========================================================= */
/* 3. Limites de Recursos do Sistema Operacional             */
/* ========================================================= */
#define CONFIGURE_MAXIMUM_TASKS 4        /* Init + ACS + WatchDog + 1 extra */
#define CONFIGURE_MAXIMUM_SEMAPHORES 5   
#define CONFIGURE_MAXIMUM_TIMERS 2

// ADICIONADO: O RTEMS precisa alocar memória para suas Message Queues (vistas no task.cpp)
#define CONFIGURE_MAXIMUM_MESSAGE_QUEUES 5 

/* ========================================================= */
/* 4. Configuração de Memória                                */
/* ========================================================= */
#define CONFIGURE_EXTRA_TASK_STACKS (3 * RTEMS_MINIMUM_STACK_SIZE)

/* ========================================================= */
/* 5. Tarefa de Inicialização                                */
/* ========================================================= */
// ADICIONADO: Isso instrui o linker a iniciar o sistema através da task 'Init'
#define CONFIGURE_RTEMS_INIT_TASKS_TABLE 

/* ========================================================= */
/* Geração das Estruturas do RTEMS (Obrigatório no final)    */
/* ========================================================= */
/* A macro CONFIGURE_INIT diz ao RTEMS para instanciar os objetos de configuração aqui. */
#define CONFIGURE_INIT
#include <rtems/confdefs.h>