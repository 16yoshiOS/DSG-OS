#include "drivers/pci.h"
#include "kernel/kernel.h"
#include "drivers/virtio_net.h"

uint32_t pci_read(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset) {
    uint32_t address = (uint32_t)((bus << 16) | (slot << 11) | (func << 8) | (offset & 0xfc) | 0x80000000);
    outb(0xCF8, address);
    return (uint32_t)inb(0xCFC);
}

void pci_scan(void) {
    for (uint16_t bus = 0; bus < 256; bus++) {
        for (uint8_t slot = 0; slot < 32; slot++) {
            uint32_t vendor = pci_read(bus, slot, 0, 0);
            if ((vendor & 0xFFFF) == 0x1AF4) { /* VirtIO Vendor ID */
                uint32_t device = pci_read(bus, slot, 0, 2) >> 16;
                if (device == 0x1000 || device == 0x1041) { /* VirtIO Net */
                    virtio_net_init(bus, slot);
                    return;
                }
            }
        }
    }
}