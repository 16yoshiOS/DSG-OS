/* gui/gui.h */
#ifndef DSG_GUI_H
#define DSG_GUI_H
#include "drivers/framebuffer.h"

void gui_init(fb_info_t *fb);
void gui_present(void);
void gui_draw_desktop(void);
void gui_draw_menu_bar(void);
void gui_draw_dock(void);
void gui_draw_window(int x, int y, int w, int h, const char *title);
#endif