#include <stdint.h>
#include "kernel/kernel.h"
#include "kernel/meminfo.h"
#include "kernel/string.h"
#include "drivers/multiboot.h"
#include "drivers/framebuffer.h"
#include "gui/gui.h"
#include "network/netstack.h"
#include "network/tls.h"
#include "browser/browser.h"

void kernel_panic(const char *msg) {
    (void)msg;
    cli();
    for (;;) __asm__ volatile("hlt");
}

/* 
 * Внимание: QEMU передает адрес multiboot_info в регистре EBX.
 * В boot.S мы делаем "mov %ebx, %edi", поэтому сюда приходит 32-битный адрес.
 */
void kernel_main(uint32_t mbi_addr) {
    /* Приводим 32-битный адрес к 64-битному указателю */
    multiboot_info_t *mbi = (multiboot_info_t *)(uintptr_t)mbi_addr;
    
    /* Проверяем, что QEMU выдал нам видеорежим */
    if (!(mbi->flags & MULTIBOOT_INFO_FRAMEBUFFER_INFO)) {
        kernel_panic("No framebuffer available from Multiboot!");
    }
    
    /* 1. Инициализация памяти */
    meminfo_init();

    /* 2. Инициализация фреймбуфера */
    fb_info_t fb = {0};
    /* mbi->framebuffer_addr имеет тип uint64_t, приводим к указателю */
    int fb_status = fb_init((uint32_t *)(uintptr_t)mbi->framebuffer_addr, 
                            mbi->framebuffer_width, 
                            mbi->framebuffer_height, 
                            mbi->framebuffer_pitch);
    if (fb_status != 0) {
        kernel_panic("Framebuffer init failed!");
    }

    /* 3. Инициализация подсистем */
    gui_init(&fb);
    net_init();
    tls_init();
    browser_init();

    /* 4. Отрисовка рабочего стола */
    gui_draw_desktop();
    
    /* 5. Эмуляция работы браузера */
    char html_buffer[1024];
    kmemset(html_buffer, 0, sizeof(html_buffer));
    
    /* Приводим sizeof к int, так как browser_navigate ожидает int */
    browser_navigate("https://dsg-os.local", html_buffer, (int)sizeof(html_buffer));
    
    /* Рисуем окно браузера с полученным HTML */
    browser_render(100, 100, 600, 400, html_buffer);
    
    /* Еще одно окно для красоты */
    gui_draw_window(450, 300, 300, 200, "Settings");
    
    /* Обновляем экран */
    gui_present();

    /* Уходим в бесконечное ожидание */
    for (;;) {
        __asm__ volatile("hlt");
    }
}