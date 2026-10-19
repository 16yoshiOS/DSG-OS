#include "kernel/string.h"

size_t kstrlen(const char *s) { size_t i = 0; while (s[i]) i++; return i; }
void *kmemset(void *s, int c, size_t n) { uint8_t *p = s; while (n--) *p++ = c; return s; }
void *kmemcpy(void *dest, const void *src, size_t n) { uint8_t *d = dest; const uint8_t *s = src; while (n--) *d++ = *s++; return dest; }
int kstrncmp(const char *s1, const char *s2, size_t n) {
    while (n && *s1 && (*s1 == *s2)) { s1++; s2++; n--; }
    if (n == 0) return 0;
    return *(unsigned char *)s1 - *(unsigned char *)s2;
}