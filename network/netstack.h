#ifndef DSG_NETSTACK_H
#define DSG_NETSTACK_H
#include <stdint.h>

void net_init(void);
int  net_tcp_connect(const char *host, uint16_t port);
int  net_tcp_send(int socket, const void *buf, uint32_t len);
int  net_tcp_recv(int socket, void *buf, uint32_t max_len);

#endif