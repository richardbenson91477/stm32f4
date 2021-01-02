#ifndef L2_TILES_H
#define L2_TILES_H

#define TILE_W 16
#define TILE_H 16

#define TILES_XN (LCD_W / TILE_W)
#define TILES_YN (LCD_H / TILE_H)

extern uint8_t tiles_screen [TILES_YN][TILES_XN];

void tiles_init ();
void tiles_render ();

#endif

