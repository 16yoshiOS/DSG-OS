#include "network/netstack.h"
#include "drivers/pci.h"

void net_init(void) {
    pci_scan(); // Ищем VirtIO-Net
}

int net_tcp_connect(const char *host, uint16_t port) {
    (void)host; (void)port;
    return 1; // Возвращаем фиктивный сокет (успешно подключено)
}

int net_tcp_send(int socket, const void *buf, uint32_t len) {
    (void)socket; (void)buf; (void)len;
    return 0;
}

int net_tcp_recv(int socket, void *buf, uint32_t max_len) {
    (void)socket; (void)buf; (void)max_len;
    return -1;
}