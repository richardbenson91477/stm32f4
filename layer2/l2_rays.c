#include "l2.h"
#include "blaze/blaze.h"

struct ray {
    uint16_t x1, y1, x2, y2;
    uint16_t age;
};
uint16_t rays_n;
blaze rays;

uint8_t rays_tch_;
const uint16_t rays_pow_n = 3;
const uint16_t rays_col [] = {0xfbe2, 0xffe2, 0xffff};
const uint16_t rays_age_max [] = {5, 10, 15};
const uint16_t rays_pow_t_max [] = {0, 60, 120};
uint16_t rays_pow;
uint16_t rays_pow_t;

void rays_remove_first ();
void rays_lose_power ();

void rays_init () {
    blaze_cons (&rays);
    rays.blk_len = 32;

    rays_tch_ = 0;
    rays_n = 0;
    rays_pow = 0;
}

void rays_tick () {
    if (! rays_n)
        return;

    for (struct ray *_r = blaze_first (&rays); _r; _r = blaze_next (&rays))
        _r->age += 1;

    struct ray *_r = blaze_first (&rays);
    if (_r->age >= rays_age_max [rays_pow])
        rays_remove_first ();

    if (rays_pow > 0) {
        rays_pow_t ++;
        if (rays_pow_t == rays_pow_t_max[rays_pow])
            rays_lose_power ();
    }
}

void rays_render () {
    if (! rays_n)
        return;
    
    for (struct ray *_r = blaze_first (&rays); _r; _r = blaze_next (&rays)) {
        LCD_line (_r->x1, _r->y1, _r->x2, _r->y2, rays_col[rays_pow]);
        LCD_line (_r->x1, _r->y1 + 1, _r->x2, _r->y2 + 1, rays_col[rays_pow]);
        LCD_line (_r->x1 + 1, _r->y1, _r->x2 + 1, _r->y2, rays_col[rays_pow]);
    }
}

void rays_boost_power () {
    if (rays_pow < (rays_pow_n - 1))
        rays_pow ++;

    rays_pow_t = 0;
}

void rays_lose_power () {
    rays_pow = 0;
}

uint8_t rays_add (uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    if (rays_n >= rays_age_max[rays_pow])
        rays_remove_first ();
    
    struct ray *_r = malloc (sizeof (struct ray));
    if (! _r)
        return 0;
    _r->x1 = x1;
    _r->y1 = y1;
    _r->x2 = x2;
    _r->y2 = y2;
    _r->age = 0;

    blaze_add (&rays, _r);
    rays_n ++;

    return 1;
}

void rays_remove_first () {
    if (! rays_n)
        return;

    blaze_del (&rays, NULL, 0, free);
    rays_n --;
}

void rays_clear () {
    blaze_clear (&rays, free);
    rays_n = 0;
}

void rays_tch_begin () {
    rays_tch_ = 1;
}

void rays_tch_end () {
    rays_tch_ = 0;
}

