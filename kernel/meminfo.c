#include "kernel/kernel.h"
#include "kernel/meminfo.h"

/* 
 * Для QEMU по умолчанию даем 128 МБ ОЗУ.
 * В будущем здесь нужно будет читать Multiboot memory map.
 */
#define RAM_TOTAL (128ULL * 1024 * 1024) 

/* Символ из linker.ld */
extern uint8_t __bss_end[];

static uintptr_t heap_ptr;
static uintptr_t heap_end;
static meminfo_t s_info;

void meminfo_init(void) {
    /* Куча начинается сразу после конца BSS (и стека), выравниваем по 4КБ */
    uintptr_t kernel_end = (uintptr_t)__bss_end;
    heap_ptr = (kernel_end + 0xFFF) & ~0xFFFULL;
    
    /* Конец доступной памяти */
    heap_end = RAM_TOTAL;

    s_info.total = RAM_TOTAL;
    s_info.used  = heap_ptr; /* Всё, что от 0 до heap_ptr уже занято ядром */
    s_info.free  = s_info.total - s_info.used;
}

meminfo_t meminfo_get(void) { 
    return s_info; 
}

void *kmalloc(size_t size) {
    if (!size) return NULL;
    
    /* Выравнивание по 16 байт */
    size = (size + 0xF) & ~0xFULL; 
    
    if (heap_ptr + size > heap_end) {
        return NULL; /* Out of memory */
    }
    
    void *p = (void *)heap_ptr;
    heap_ptr += size;
    
    s_info.used += size;
    s_info.free -= size;
    
    return p;
}

void kfree(void *ptr) { 
    (void)ptr; 
    /* Заглушка. Позже сделаем полноценный менеджер (buddy allocator) */
}