/* gui/gui.c */
#include "gui/gui.h"

#define C_DESKTOP_TOP   0x002A2A3A
#define C_DESKTOP_BOT   0x00151822
#define C_MENU_BAR      0x20E8E8EE
#define C_MENU_TEXT     0xFF1C1C1E
#define C_DOCK          0x90F0F0F5
#define C_WINDOW        0xFFF7F7F9
#define C_WINDOW_TITLE  0xFFE9E9EE
#define C_TITLEBAR_TXT  0xFF333333
#define C_TRAFFIC_R     0xFFFF5F57
#define C_TRAFFIC_Y     0xFFFEBC2E
#define C_TRAFFIC_G     0xFF28C840

static fb_info_t *g_fb;

void gui_init(fb_info_t *fb) { g_fb = fb; }
void gui_present(void) { }

void gui_draw_desktop(void) {
    for (uint32_t y = 0; y < g_fb->height; y++) {
        int t = (int)(y * 255 / (g_fb->height ? g_fb->height : 1));
        uint32_t r = ((C_DESKTOP_TOP >> 16) & 0xFF) * (255 - t) / 255 + ((C_DESKTOP_BOT >> 16) & 0xFF) * t / 255;
        uint32_t g = ((C_DESKTOP_TOP >>  8) & 0xFF) * (255 - t) / 255 + ((C_DESKTOP_BOT >>  8) & 0xFF) * t / 255;
        uint32_t b = ( C_DESKTOP_TOP        & 0xFF) * (255 - t) / 255 + ( C_DESKTOP_BOT        & 0xFF) * t / 255;
        fb_fill_rect(0, (int)y, (int)g_fb->width, 1, 0xFF000000 | (r << 16) | (g << 8) | b);
    }
    gui_draw_menu_bar();
    gui_draw_dock();
}

void gui_draw_menu_bar(void) {
    fb_fill_rect(0, 0, (int)g_fb->width, 24, C_MENU_BAR);
    fb_fill_round_rect(10, 6, 12, 12, 3, 0xFF000000); /* Логотип */
    fb_draw_string(30, 8, "Finder", C_MENU_TEXT);
    fb_draw_string(110, 8, "File",   C_MENU_TEXT);
    fb_draw_string(150, 8, "Edit",   C_MENU_TEXT);
    fb_draw_string(190, 8, "View",   C_MENU_TEXT);
    fb_draw_string((int)g_fb->width - 80, 8, "DSG OS", C_MENU_TEXT);
}

void gui_draw_dock(void) {
    int dock_w = 360, dock_h = 64;
    int x = ((int)g_fb->width - dock_w) / 2;
    int y = (int)g_fb->height - dock_h - 12;
    fb_fill_round_rect(x, y, dock_w, dock_h, 14, C_DOCK);
    uint32_t tiles[] = {0xFF2D9CDB, 0xFF27AE60, 0xFFEB5757, 0xFFF2C94C, 0xFF9B51E0, 0xFF56CCF2, 0xFF6FCF97};
    for (int i = 0; i < 7; i++) {
        fb_fill_round_rect(x + 14 + i * 46, y + 10, 40, 40, 10, tiles[i]);
    }
}

void gui_draw_window(int x, int y, int w, int h, const char *title) {
    fb_fill_round_rect(x + 4, y + 6, w, h, 14, 0x30000000); /* Тень */
    fb_fill_round_rect(x, y, w, h, 14, C_WINDOW);
    fb_fill_round_rect(x, y, w, 28, 14, C_WINDOW_TITLE);
    fb_fill_round_rect(x + 12, y + 8, 12, 12, 6, C_TRAFFIC_R);
    fb_fill_round_rect(x + 30, y + 8, 12, 12, 6, C_TRAFFIC_Y);
    fb_fill_round_rect(x + 48, y + 8, 12, 12, 6, C_TRAFFIC_G);
    fb_draw_string(x + w / 2 - 20, y + 10, title, C_TITLEBAR_TXT);
}