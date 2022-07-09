#include "number.h"
#include "../types/error.h"

#define MAX_UINT32_DIGITS 10

char digit_to_char(int digit) {
    return digit <= 9 ? '0' + digit : 'A' + (digit - 10);
}

int num_to_str(uint32 num, uint32 base, uint32 max_length, char* result) {
    uint8 digits[MAX_UINT32_DIGITS];

    uint32 length = 0;
    for (int i = 0; i < MAX_UINT32_DIGITS; i++) {
        digits[i] = num % base;
        num /= base;
        if (num == 0) {
            length = i + 2;
            break;
        }
    }

    if (length > max_length) {
        return E_SIZE_EXCEED;
    }

    for (int i = 0; i < length - 1; i++) {
        result[length - 2 - i] = digit_to_char(digits[i]);
    }
    result[length - 1] = 0;

    return E_OK;
}

int inum_to_str(int32 num, uint32 base, uint32 max_length, char* result) {
    if (max_length < 1) {
        return E_SIZE_EXCEED;
    }

    if (num >= 0) {
        return num_to_str(num, base, max_length, result);
    }
    else {
        result[0] = '-';
        return num_to_str(-num, base, max_length - 1, result + 1);
    }
}
