#ifdef __cplusplus
extern "C" {
#endif

// serviços básicos
#define PUS_SERVICE_TC_VERIFICATION 1
#define PUS_SERVICE_HK              3
#define PUS_SERVICE_TEST            17
#define PUS_SERVICE_COMMAND         8

// subtypes exemplo
#define SUBTYPE_SET_MODE            1
#define SUBTYPE_SET_BDOT_GAIN       2


void send_ack(uint8_t result);
void send_hk(void);
void send_test(void);
void pus_dispatch(pus_packet_t *pkt);

#ifdef __cplusplus
}
#endif


