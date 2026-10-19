/* boot.S - x86_64 Multiboot Entry with Video Mode */

.section .multiboot
.align 4
.long 0x1BADB002            /* MAGIC */
.long 0x00000007            /* FLAGS: align + mem info + video mode */
.long -(0x1BADB002 + 0x00000007) /* CHECKSUM */
/* 5 полей (не используются без бита 16) */
.long 0, 0, 0, 0, 0
/* Запрос видеорежима */
.long 0                     /* 0 = линейный графический режим */
.long 1024                  /* Ширина */
.long 768                   /* Высота */
.long 32                    /* Глубина цвета (32-bit) */

.section .bss
.align 16
stack_bottom:
.skip 65536                 /* 64 KiB стек */
stack_top:

/* Таблицы страниц для Long Mode */
.align 4096
pml4_table:
.skip 4096
pdpt_table:
.skip 4096
pd_table_low:
.skip 4096
pd_table_high:
.skip 4096

.section .text
.global _start
.extern kernel_main

.code32
_start:
    cli
    mov $stack_top, %esp

    /* Очищаем BSS */
    mov $__bss_start, %edi
    mov $__bss_end, %ecx
    sub %edi, %ecx
    xor %eax, %eax
    rep stosb

    /* 1. Настраиваем PML4 -> PDPT */
    mov $pdpt_table, %eax
    or $0x03, %eax
    mov %eax, (pml4_table)

    /* 2. Настраиваем PDPT[0] -> pd_table_low (0-1GB) */
    mov $pd_table_low, %eax
    or $0x03, %eax
    mov %eax, (pdpt_table)

    /* 3. Настраиваем PDPT[3] -> pd_table_high (3-4GB, где висит видеопамять) */
    mov $pd_table_high, %eax
    or $0x03, %eax
    mov %eax, (pdpt_table + 24) /* 3 * 8 */

    /* 4. Заполняем pd_table_low (0-1GB) */
    mov $pd_table_low, %edi
    mov $0x83, %eax
    mov $512, %ecx
1:
    mov %eax, (%edi)
    add $0x200000, %eax
    add $8, %edi
    loop 1b

    /* 5. Заполняем pd_table_high (3-4GB) */
    mov $pd_table_high, %edi
    mov $0xC0000083, %eax
    mov $512, %ecx
2:
    mov %eax, (%edi)
    add $0x200000, %eax
    add $8, %edi
    loop 2b

    /* 6. Загружаем PML4 в CR3 */
    mov $pml4_table, %eax
    mov %eax, %cr3

    /* 7. Включаем PAE */
    mov %cr4, %eax
    or $(1 << 5), %eax
    mov %eax, %cr4

    /* 8. Устанавливаем LME */
    mov $0xC0000080, %ecx
    rdmsr
    or $(1 << 8), %eax
    wrmsr

    /* 9. Включаем Paging */
    mov %cr0, %eax
    or $(1 << 31), %eax
    mov %eax, %cr0

    /* 10. Загружаем GDT */
    lgdt gdt64_pointer

    /* 11. Прыгаем в 64-битный код */
    ljmp $0x08, $long_mode_entry

.code64
long_mode_entry:
    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss

    mov $stack_top, %rsp

    /* Сохраняем указатель на multiboot_info (в ebx) в первый аргумент (rdi) */
    mov %ebx, %edi

    call kernel_main

    cli
1:  hlt
    jmp 1b

.section .rodata
.align 16
gdt64:
    .quad 0
    .quad 0x00209A0000000000
    .quad 0x0000920000000000
gdt64_pointer:
    .word gdt64_pointer - gdt64 - 1
    .quad gdt64