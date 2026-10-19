#ifndef DSG_FRAMEBUFFER_H
#define DSG_FRAMEBUFFER_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint32_t *pixels;
    uint32_t  width;
    uint32_t  height;
    uint32_t  pitch;
} fb_info_t;

int      fb_init(uint32_t *pixels, uint32_t width, uint32_t height, uint32_t pitch);
fb_info_t *fb_get(void);

void     fb_draw_pixel(int x, int y, uint32_t color);
void     fb_fill_rect(int x, int y, int w, int h, uint32_t color);
void     fb_fill_round_rect(int x, int y, int w, int h, int r, uint32_t color);
void     fb_draw_string(int x, int y, const char *s, uint32_t color);
void     fb_draw_u64(int x, int y, uint64_t v, uint32_t color);

#endif