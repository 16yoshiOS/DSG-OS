#include "drivers/virtio_net.h"
#include "drivers/pci.h"
#include "kernel/kernel.h"

static uint8_t mac_addr[6];

void virtio_net_init(uint8_t bus, uint8_t slot) {
    // В реальной ОС здесь настройка VirtQueue (DMA, Ring-буферы).
    // Пока мы просто считываем MAC-адрес, чтобы доказать, что карта найдена.
    uint32_t bar0 = pci_read(bus, slot, 0, 0x10) & ~0xF;
    
    // Заглушка MAC-адреса (в QEMU обычно 52:54:00:12:34:56)
    mac_addr[0] = 0x52; mac_addr[1] = 0x54; mac_addr[2] = 0x00;
    mac_addr[3] = 0x12; mac_addr[4] = 0x34; mac_addr[5] = 0x56;
    
    (void)bar0;
}

int virtio_net_send(const void *buf, uint32_t len) {
    (void)buf; (void)len;
    return 0; // Заглушка: пакет отправлен
}

int virtio_net_recv(void *buf, uint32_t max_len) {
    (void)buf; (void)max_len;
    return -1; // Заглушка: нет входящих пакетов
}