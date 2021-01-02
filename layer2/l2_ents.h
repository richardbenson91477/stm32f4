#ifndef L2_ENTS_H
#define L2_ENTS_H

#define ENT_TYPE_FRAMES_MAX 8
#define ENT_TYPE_SEQ_MAX 8

enum {ent_type_bug1_id = 0, ent_type_pow1_id};

void ents_init ();
void ents_tick ();
void ents_render ();
uint8_t ents_add (uint8_t ent_type_id, uint16_t x, uint16_t y);

#endif

