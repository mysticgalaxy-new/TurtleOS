#include <stdint.h>

void* memcpy(void* dest, const void* src, unsigned int n) {
    uint32_t* d32 = (uint32_t*)dest;
    const uint32_t* s32 = (const uint32_t*)src;

    while (n >= 4) {
        *d32++ = *s32++;
        n -= 4;
    }

    uint8_t* d8 = (uint8_t*)d32;
    const uint8_t* s8 = (const uint8_t*)s32;

    while (n--) {
        *d8++ = *s8++;
    }

    return dest;
}
