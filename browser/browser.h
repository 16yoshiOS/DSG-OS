#ifndef DSG_BROWSER_H
#define DSG_BROWSER_H

void browser_init(void);
void browser_navigate(const char *url, char *buffer, int max_len);
void browser_render(int x, int y, int w, int h, const char *html);

#endif