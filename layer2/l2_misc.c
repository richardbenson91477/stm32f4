#include "l2.h"

uint64_t misc_random_seed;

void misc_srand (uint64_t s) {
    misc_random_seed = s + 0xdeadbeefdeadbeef;
    (void) misc_random ();
    (void) misc_random ();
}
 
// Middle Square Weyl Sequence Random Number Generator
// Copyright (c) 2014-2019 Bernard Widynski
uint32_t misc_random () {
    static uint64_t x = 0, w = 0;
    x *= x;
    x += (w += misc_random_seed);
    return x = (x>>32) | (x<<32);
}

float misc_random_f () {
    return (float)misc_random() / (uint32_t) -1;
}

