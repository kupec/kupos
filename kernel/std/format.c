#include "format.h"
#include "../types/types.h"
#include "string.h"
#include <stdarg.h>

int fd_format(FileDescriptor fd, const char* fmt, ...) {
    int result = E_OK;

    va_list args;
    va_start(args, fmt);

    char s[80];
    result = vformat(fmt, LEN(s), s, args);
    if (result < 0) {
        return result;
    }

    fd_write(fd, str_len(s), s);
    if (result < 0) {
        return result;
    }

    va_end(args);
    return result;
}
