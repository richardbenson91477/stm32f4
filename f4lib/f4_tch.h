#ifndef F4_TCH_H
#define F4_TCH_H

#define TCH_MIN_X 400
#define TCH_MAX_X 3995
#define TCH_MIN_Y 300
#define TCH_MAX_Y 3995
#define TCH_RES_X (TCH_MAX_X - TCH_MIN_X)
#define TCH_RES_Y (TCH_MAX_Y - TCH_MIN_Y)

#define TCH_SAMPLES_MAX 16
#define TCH_PT_OUTLIER 8

extern uint8_t tch_read_px_ (uint16_t *tx, uint16_t *ty);

#endif

