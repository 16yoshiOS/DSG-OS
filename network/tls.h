#ifndef DSG_TLS_H
#define DSG_TLS_H
#include <stdint.h>

void tls_init(void);
int  tls_connect(const char *host, uint16_t port);
int  tls_read(int conn, void *buf, uint32_t max_len);

#endif