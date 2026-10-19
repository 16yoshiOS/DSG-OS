#ifndef DSG_MEMINFO_H
#define DSG_MEMINFO_H

#include <stdint.h>
#include <stddef.h>

typedef struct {
    uint64_t total;   /* Всего ОЗУ в байтах */
    uint64_t used;    /* Занято */
    uint64_t free;    /* Свободно */
} meminfo_t;

/* Инициализация (вызывается 1 раз при старте) */
void meminfo_init(void);

/* Получить текущую статистику ОЗУ */
meminfo_t meminfo_get(void);

/* Простейший аллокатор (bump allocator) */
void *kmalloc(size_t size);
void kfree(void *ptr);

#endif