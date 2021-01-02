#include "f4lib.h"

uint8_t tch_read_px_ (uint16_t *_tx, uint16_t *_ty) {
    uint16_t pt_xa [TCH_SAMPLES_MAX], pt_ya [TCH_SAMPLES_MAX];
    uint32_t pt_x_avg, pt_y_avg, pt_x_avg2, pt_y_avg2;
    uint16_t pt_x, pt_y;
    uint8_t pt_n, pt_n2;

    if (! IO_tch_ ())
        return 0;

    pt_x_avg = pt_y_avg = 0;
    pt_n = 0;
    while (pt_n < TCH_SAMPLES_MAX) {
        IO_tch_get_pt (&pt_x, &pt_y);
        if ((pt_x < TCH_MIN_X) || (pt_x > TCH_MAX_X) ||
                (pt_y < TCH_MIN_Y) || (pt_y > TCH_MAX_Y))
            break;
        misc_udelay (200);

        pt_xa [pt_n] = pt_x;
        pt_ya [pt_n] = pt_y;
        pt_x_avg += pt_x;
        pt_y_avg += pt_y;
        pt_n += 1;

        if (! IO_tch_ ())
            break;
    }

    if (! pt_n)
        return 0;

    pt_x_avg /= pt_n;
    pt_y_avg /= pt_n;
 
    // discount outliers beyond threshold
    pt_x_avg2 = pt_y_avg2 = 0;
    pt_n2 = 0;
    for (uint8_t c = 0; c < pt_n; c ++) {
        if (((ABS(pt_xa [c] - (uint16_t)pt_x_avg)) < TCH_PT_OUTLIER) &&
                ((ABS(pt_ya [c] - (uint16_t)pt_y_avg)) < TCH_PT_OUTLIER)) {
            pt_x_avg2 += pt_xa [c];
            pt_y_avg2 += pt_ya [c];
            pt_n2 += 1;
        }
    }

    if (! pt_n2)
        return 0;

    pt_x_avg2 /= pt_n2;
    pt_y_avg2 /= pt_n2;
  
    *_tx = ((pt_x_avg2 - TCH_MIN_X) * LCD_W) / TCH_RES_X;
    *_ty = ((pt_y_avg2 - TCH_MIN_Y) * LCD_H) / TCH_RES_Y;

    if ((*_tx < LCD_W) && (*_ty < LCD_H))
        return 1;

    return 0;
}

