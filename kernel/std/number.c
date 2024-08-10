#include "number.h"
#include "../types/error.h"
#include "../std/math.h"

#define MAX_UINT32_DIGITS 10

char digit_to_char(int digit) {
    return digit <= 9 ? '0' + digit : 'A' + (digit - 10);
}

int num_to_digits(uint32 num, uint32 base, uint8* digits, uint32 *p_length) {
    *p_length = 0;
    for (int i = 0; i < MAX_UINT32_DIGITS; i++) {
        digits[i] = num % base;
        num /= base;
        if (num == 0) {
            *p_length = i + 1;
            break;
        }
    }

    if (num != 0) {
        return E_SIZE_EXCEED;
    }

    return E_OK;
}

int num_to_str(uint32 num, uint32 base, int padding, uint32 max_length, char* result) {
    uint8 digits[MAX_UINT32_DIGITS];
    uint32 length = 0;
    int err = E_OK;
    if ((err = num_to_digits(num, base, digits, &length)) < 0) {
        return err;
    }

    if (length + 1 > max_length || padding + 1 > max_length) {
        return E_SIZE_EXCEED;
    }

    int prefix_len = padding < length ? 0 : padding - length;
    while (prefix_len--) {
        *result++ = '0';
    }
    for (int i = 0; i < length; i++) {
        result[length - 1 - i] = digit_to_char(digits[i]);
    }
    result[length] = 0;

    return E_OK;
}

int inum_to_str(int32 num, uint32 base, int padding, uint32 max_length, char* result) {
    if (max_length < 1) {
        return E_SIZE_EXCEED;
    }

    if (num >= 0) {
        return num_to_str(num, base, padding, max_length, result);
    }
    else {
        result[0] = '-';
        return num_to_str(-num, base, padding - 1, max_length - 1, result + 1);
    }
}
