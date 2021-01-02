#ifndef L2_RAYS_H
#define L2_RAYS_H

extern uint8_t rays_tch_;

extern void rays_init ();
extern void rays_tick ();
extern void rays_render ();
extern void rays_boost_power ();
extern uint8_t rays_add (uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
extern void rays_clear ();
extern void rays_tch_begin ();
extern void rays_tch_end ();

#endif

