#include "queues.h"

// filas globais
rtems_id queue_tx;
rtems_id queue_uart;

// construtor genérico
static rtems_id create_queue(
    char a, char b, char c, char d,
    uint32_t depth,
    size_t msg_size
) {
    rtems_id id;
    rtems_status_code status;

    status = rtems_message_queue_create(
        rtems_build_name(a, b, c, d),
        depth,
        msg_size,
        RTEMS_DEFAULT_ATTRIBUTES,
        &id
    );

    if (status != RTEMS_SUCCESSFUL) {
        return RTEMS_ID_NONE;
    }

    return id;
}

// inicialização centralizada
void queues_init(void) {

    // fila de pacotes (struct)
    queue_tx = create_queue(
        'A','D','C','Q',
        10,
        sizeof(pus_packet_t)
    );

    // fila de bytes (frame PUS)
    queue_uart = create_queue(
        'U','A','R','T',
        20,
        128   // buffer máximo TX
    );

    // validação básica
    if (queue_tx == RTEMS_ID_NONE || queue_uart == RTEMS_ID_NONE) {
        // erro crítico → travar sistema ou logar
        while (1);
    }
}