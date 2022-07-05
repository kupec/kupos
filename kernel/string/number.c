#include "number.h"
#include "../error.h"

#define MAX_UINT32_DIGITS 10

int num_to_str(uint32 num, uint32 max_length, char* result) {
    uint8 digits[MAX_UINT32_DIGITS];

    uint32 length = 0;
    for (int i = 0; i < MAX_UINT32_DIGITS; i++) {
        digits[i] = num % 10;
        num /= 10;
        if (num == 0) {
            length = i + 2;
            break;
        }
    }

    if (length > max_length) {
        return E_SIZE_EXCEED;
    }

    for (int i = 0; i < length; i++) {
        result[length - 2 - i] = '0' + digits[i];
    }
    result[length - 1] = 0;

    return E_OK;
}

int inum_to_str(int32 num, uint32 max_length, char* result) {
    if (max_length < 1) {
        return E_SIZE_EXCEED;
    }

    if (num >= 0) {
        return num_to_str(num, max_length, result);
    }
    else {
        result[0] = '-';
        return num_to_str(-num, max_length - 1, result + 1);
    }
}
