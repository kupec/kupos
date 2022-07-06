#include "../types/types.h"
#include "../error.h"
#include <stdarg.h>

#define LEN(x) (sizeof x / sizeof x[0])

uint32 str_len(const char*);
int str_cat(const char*, const char*, uint32, char*);
int format(const char*, uint32, char*, ...);
int vformat(const char*, uint32, char*, va_list args);
