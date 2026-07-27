#ifdef __cplusplus
extern "C" {
#endif

//int pus_parse(uint8_t *rx, pus_packet_t *pkt);
int pus_parse(const uint8_t *rx, uint16_t size, pus_packet_t *pkt);

#ifdef __cplusplus
}
#endif