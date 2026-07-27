#include <rtems.h>
#include <stdint.h>
#include <stdbool.h>

extern "C" {
    #include "../../pus/pus_packet.h"
    #include "../../drivers/telemetry/adcs_telemetry.h"
    #include "../queues/queues.h"
}

// protótipo da sua função em C
extern "C" int check_telemetry(pus_packet_t *pkt, bool hk, uint8_t subtype);

// --------------------------------------
// TASK ADCS
// --------------------------------------
static inline void send_pkt(pus_packet_t *pkt)
{
    if (!pkt) return;

    rtems_status_code status;

    status = rtems_message_queue_send(queue_tx, pkt, sizeof(*pkt));

    if (status != RTEMS_SUCCESSFUL) {
        // erro (fila cheia)
    }
}

rtems_task adcs_task(rtems_task_argument arg)
{
    pus_packet_t pkt;
    uint32_t tick = 1;

    static bool last_fault = false;

    while (1) {

        if (tick % 10 == 0) {
            if (check_telemetry(&pkt, false, ADCS_TM_GYRO) == 0)
                send_pkt(&pkt);
        }

        if (tick % 20 == 0) {
            if (check_telemetry(&pkt, false, ADCS_TM_BFIELD) == 0)
                send_pkt(&pkt);
        }

        if (tick % 100 == 0) {
            if (check_telemetry(&pkt, true, HK_ADCS_STATUS) == 0)
                send_pkt(&pkt);
        }

        if (tick % 100 == 50) {
            if (check_telemetry(&pkt, true, HK_ADCS_POWER) == 0)
                send_pkt(&pkt);
        }

        //bool fault = adcs_has_fault();

        //if (fault && !last_fault) {
       //     if (check_telemetry(&pkt, true, HK_ADCS_FAULTS) == 0)
       //         send_pkt(&pkt);
       // }

        //last_fault = fault;

        tick++;
        if (tick >= 1000) tick = 0;

        rtems_task_wake_after(10);
    }
}