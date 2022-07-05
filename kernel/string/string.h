#include "../types/types.h"
#include "../error.h"

uint32 strlen(char *s) {
    char *t = s;
    while (*t != 0) {
        t++;
    }
    return t - s;
}

int strcat(char* s1, char* s2, uint32 max_length, char* result) {
    uint32 l1 = strlen(s1);
    uint32 l2 = strlen(s2);
    if (max_length < l1 + l2 + 1) {
        return E_SIZE_EXCEED;
    }

    for (int i = 0; i < l1; i++) {
        result[i] = s1[i];
    }
    for (int i = 0; i < l1; i++) {
        result[l1 + i] = s2[i];
    }
    result[l1+l2] = 0;

    return 0;
}
