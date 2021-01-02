#include "l2.h"
#include "blaze/blaze.h"
#include "img/s_bug1a.h"
#include "img/s_bug1b.h"
#include "img/s_pow1a.h"
#include "img/s_pow1b.h"
#include "img/s_pow1c.h"

struct ent_type {
    const uint8_t id;

    const uint16_t w, h;
    const uint16_t b_x, b_y, b_w, b_h;

    const uint8_t health_max;

    const uint8_t frames_n;
    const uint16_t *frames [ENT_TYPE_FRAMES_MAX];
    const uint8_t frames_seq_n;
    const uint8_t frames_seq [ENT_TYPE_SEQ_MAX];
    const uint8_t frames_seq_t_max;
};

const struct ent_type ent_type_bug1 = {ent_type_bug1_id,
    64, 64,
    0, 0, 64, 64,
    10,
    2, {s_bug1a, s_bug1b},
    2, {0, 1}, 8};

const struct ent_type ent_type_pow1 = {ent_type_pow1_id,
    64, 64,
    0, 0, 64, 64,
    4,
    3, {s_pow1a, s_pow1b, s_pow1c},
    4, {0, 1, 0, 2}, 4};

const struct ent_type *ent_types [] = {&ent_type_bug1, &ent_type_pow1};

struct ent {
    const struct ent_type *_ent_type;

    uint16_t x, y, w, h;

    uint8_t health;

    uint8_t frame_seq_c;
    uint8_t frame_seq_t;
    uint16_t *_frame;
};
uint16_t ents_n;
blaze ents;

void ents_init () {
    ents_n = 0;
    blaze_cons (&ents);
    ents.blk_len = 8;

    const struct ent_type *_et = ent_types [ent_type_bug1_id];
    ents_add (ent_type_bug1_id,
            (uint16_t) (misc_random_f () * (LCD_W - _et->w)),
            (uint16_t) (misc_random_f () * (LCD_H - _et->h)) );
}

void ents_tick () {
    if (! ents_n)
        return;

    for (struct ent *_e = blaze_first (&ents); _e; _e = blaze_next (&ents)) {
        const struct ent_type *_et = _e->_ent_type;
        _e->frame_seq_t += 1;
        if (_e->frame_seq_t >= _et->frames_seq_t_max) {
            _e->frame_seq_t = 0;
            _e->frame_seq_c += 1;
            if (_e->frame_seq_c >= _et->frames_seq_n)
                _e->frame_seq_c = 0;

            _e->_frame = (uint16_t *)_et->frames [_et->frames_seq [_e->frame_seq_c]];
        }
    }
}

void ents_render () {
    if (! ents_n)
        return;

    for (struct ent *_e = blaze_first (&ents); _e; _e = blaze_next (&ents))
        LCD_blit_a (_e->x, _e->y, _e->w, _e->h, _e->_frame);
}

uint8_t ents_add (uint8_t ent_type_id, uint16_t x, uint16_t y) {
    struct ent *_e = malloc (sizeof (struct ent));
    if (! _e)
        return 0;
 
    const struct ent_type *_et = ent_types [ent_type_id];
    _e->_ent_type = _et;
    _e->x = x;
    _e->y = y;
    _e->w = _et->w;
    _e->h = _et->h;
    _e->health = _et->health_max;
    _e->frame_seq_c = 0;
    _e->frame_seq_t = 0;
    _e->_frame = (uint16_t *)_et->frames [_et->frames_seq [0]];

    blaze_add (&ents, _e);
    ents_n += 1;

    return 1;
}

