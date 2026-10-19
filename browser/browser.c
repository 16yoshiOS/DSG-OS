#include "browser/browser.h"
#include "gui/gui.h"
#include "network/tls.h"
#include "kernel/string.h"

void browser_init(void) { }

void browser_navigate(const char *url, char *buffer, int max_len) {
    int conn = tls_connect(url, 443);
    if (conn > 0) {
        tls_read(conn, buffer, max_len);
    }
}

void browser_render(int x, int y, int w, int h, const char *html) {
    gui_draw_window(x, y, w, h, "Browser");

    int cx = x + 20, cy = y + 50;
    int is_h1 = 0;
    int is_body = 0;

    // Примитивный парсер HTML 4.01
    for (int i = 0; html[i] != '\0' && cy < y + h - 20; i++) {
        if (html[i] == '<') {
            if (kstrncmp(&html[i], "<h1>", 4) == 0) { is_h1 = 1; i += 3; }
            else if (kstrncmp(&html[i], "</h1>", 5) == 0) { is_h1 = 0; i += 4; cy += 10; }
            else if (kstrncmp(&html[i], "<p>", 3) == 0) { is_body = 1; i += 2; }
            else if (kstrncmp(&html[i], "</p>", 4) == 0) { is_body = 0; i += 3; cy += 15; }
            else { // Пропускаем остальные теги
                while (html[i] != '>' && html[i] != '\0') i++;
            }
        } else if (is_h1 || is_body) {
            if (html[i] != '\r' && html[i] != '\n') {
                char str[2] = {html[i], '\0'};
                
                // Вывод текста (h1 - крупнее и цветом акцента, body - обычный)
                if (is_h1) {
                    // Временно рисуем h1 обычным шрифтом, но цветом акцента
                    fb_draw_string(cx, cy, str, 0xFF0A84FF); 
                    cx += 6;
                } else {
                    fb_draw_string(cx, cy, str, 0xFF333333);
                    cx += 6;
                }
            }
        }
    }
}