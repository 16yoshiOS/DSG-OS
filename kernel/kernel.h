#ifndef DSG_KERNEL_H
#define DSG_KERNEL_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Вызов паники ядра */
void kernel_panic(const char *msg);

/* Функции для работы с портами ввода-вывода x86 (пригодятся для драйверов) */
static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ( "inb %1, %0" : "=a"(ret) : "Nd"(port) );
    return ret;
}

/* Отключение и включение прерываний */
static inline void cli(void) { __asm__ volatile ("cli"); }
static inline void sti(void) { __asm__ volatile ("sti"); }

#endif