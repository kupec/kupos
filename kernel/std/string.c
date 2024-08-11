#include "string.h"
#include "number.h"
#include <stdbool.h>
#include <stdarg.h>

uint32 str_len(const char *s) {
    const char *t = s;
    while (*t != 0) {
        t++;
    }
    return t - s;
}

int str_cat(const char* s1, const char* s2, uint32 max_length, char* result) {
    uint32 l1 = str_len(s1);
    uint32 l2 = str_len(s2);
    if (max_length < l1 + l2 + 1) {
        return E_SIZE_EXCEED;
    }

    for (int i = 0; i < l1; i++) {
        result[i] = s1[i];
    }
    for (int i = 0; i < l2; i++) {
        result[l1 + i] = s2[i];
    }
    result[l1+l2] = 0;

    return E_OK;
}

int append_str(const char* suffix, uint32 *p_max_length, char** p_result) {
    while (true) {
        char ch = *suffix++;
        if (!ch) {
            break;
        }

        if ((*p_max_length)-- == 0) {
            return E_SIZE_EXCEED;
        }

        *(*p_result)++ = ch;
    }

    return E_OK;
}

int format(const char *fmt, uint32 max_length, char* result, ...) {
    va_list args;
    va_start(args, result);
    int error = vformat(fmt, max_length, result, args);
    va_end(args);
    return error;
}

int vformat(const char *fmt, uint32 max_length, char* result, va_list args) {
    while (true) {
        char ch = *fmt++;
        if (!ch) {
            break;
        }

        if (ch != '%') {
            if (max_length-- == 0) {
                return E_SIZE_EXCEED;
            }
        
            *result++ = ch;
            continue;
        }

        int padding = 0;
        while (true) {
            ch = *fmt++;
            if (ch < '0' || ch > '9') {
                break;
            }

            padding = padding * 10 + ch - '0';
        }

        if (ch == 'd' || ch == 'x') {
            int num = va_arg(args, int);
            char s[10];
            inum_to_str(num, ch == 'd' ? 10 : 16, padding, LEN(s), s);
            int res = append_str(s, &max_length, &result);
            if (res < 0) {
                return res;
            }
        }
        else if (ch == 's') {
            const char *s = va_arg(args, const char*);
            int res = append_str(s, &max_length, &result);
            if (res < 0) {
                return res;
            }
        }
    }

    if (max_length-- == 0) {
        return E_SIZE_EXCEED;
    }

    *result++ = 0;
    return E_OK;
}
