
#ifndef QUEUES_H
#define QUEUES_H

#include <rtems.h>
#include "../../pus/pus_packet.h"

// IDs das filas (globais)
extern rtems_id queue_tx;     // ADCS → PUS (pkt)
extern rtems_id queue_uart;   // PUS → UART (bytes)

// init centralizado
void queues_init(void);

#endif