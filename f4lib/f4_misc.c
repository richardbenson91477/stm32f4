#include "f4lib.h"

#pragma GCC push_options
#pragma GCC optimize ("O0")

//  based on 168Mhz cpu frequency
void misc_mdelay (uint32_t n) {
    while (n--) {
        for (uint32_t c = 16800; c; c--)
            asm ("nop");
    }
}

//  based on 168Mhz cpu frequency
void misc_udelay (uint32_t n) {
    while (n--) {
        for (uint8_t c = 17; c; c--)
            asm ("nop");
    }
}

#pragma GCC pop_options
 
