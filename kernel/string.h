#ifndef DSG_STRING_H
#define DSG_STRING_H
#include <stddef.h>
#include <stdint.h>

size_t kstrlen(const char *s);
void *kmemset(void *s, int c, size_t n);
void *kmemcpy(void *dest, const void *src, size_t n);
int kstrncmp(const char *s1, const char *s2, size_t n);

#endif