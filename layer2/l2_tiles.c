#include "l2.h"
#include "img/t_floor1.h"
#include "img/t_floor2.h"

uint8_t tiles_screen [TILES_YN][TILES_XN];

enum {t_floor1_id = 0, t_floor2_id};
const uint16_t *tiles [] = {t_floor1, t_floor2};

void tiles_init () {
    for (uint16_t yc = 0; yc < TILES_YN; yc ++)
        for (uint16_t xc = 0; xc < TILES_XN; xc ++) {
            if ((yc == 0) || (xc == 0) ||\
                    (yc == TILES_YN - 1) || (xc == TILES_XN - 1))
                tiles_screen [yc][xc] = t_floor1_id;
            else
                tiles_screen [yc][xc] = t_floor2_id;
        }
    // mark the upper left
    tiles_screen [0][0] = t_floor2_id;
}

void tiles_render () {
    uint16_t x = 0, y = 0;
    for (uint16_t yc = 0; yc < TILES_YN; yc ++) {
        for (uint16_t xc = 0; xc < TILES_XN; xc ++) {
            LCD_blit (x, y, TILE_W, TILE_H, \
                tiles [tiles_screen [yc][xc]]);
            x += TILE_W;
        }
        x = 0;
        y += TILE_H;
    }
}

