#ifndef DSG_VIRTIO_NET_H
#define DSG_VIRTIO_NET_H
#include <stdint.h>

void virtio_net_init(uint8_t bus, uint8_t slot);
int  virtio_net_send(const void *buf, uint32_t len);
int  virtio_net_recv(void *buf, uint32_t max_len);

#endif