#ifndef SRC_CORE_UTILS_STRING_H_
#define SRC_CORE_UTILS_STRING_H_

#include <uv.h>

typedef struct {
    size_t len;
    char *data;
} jstr_t;

#define jstring(str)     { sizeof(str) - 1, (char *) str }
#define jstring_null     { 0, NULL }

typedef int (*jstr_vslp_cb)(void *, char **, char **);
int
jstr_vslprintf(uint8_t *buf, uint8_t *last, size_t maxn, jstr_vslp_cb cb, void *ctx, const char *fmt, va_list args);


#endif // SRC_CORE_UTILS_STRING_H_