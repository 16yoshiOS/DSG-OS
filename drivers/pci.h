#ifndef DSG_PCI_H
#define DSG_PCI_H
#include <stdint.h>

uint32_t pci_read(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);
void pci_scan(void);

#endif