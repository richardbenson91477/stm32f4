#include "l2.h"
#include "img/img_logo.h"
const uint8_t logo_w = 128;
const uint8_t logo_h = 128;
const uint16_t logo_bg_col = 0x0;

const uint16_t frame_t = 33;

int main () {
    IO_init ();

  #ifdef SERIAL_CON
    IO_USART_init ();
    IO_debug ("+ serial");
  #endif

    LCD_init ();
    IO_debug ("+ LCD");

    LCD_clear (logo_bg_col);
    LCD_blit (LCD_HW - (logo_w / 2), LCD_HH - (logo_h / 2), logo_w, logo_h, img_logo);
  #ifdef F4EMU
    emu_render ();
  #endif

    while (1) {
        if (IO_tch_ ()) {
            uint16_t tx, ty;
            if (tch_read_px_ (&tx, &ty)) {
                misc_srand (((uint32_t)tx << 16) + ty);
                break;
            }
        }
        else
            misc_mdelay (frame_t);
    }

    tiles_init ();
    rays_init ();
    ents_init ();

    IO_debug ("> main loop");

    uint16_t tx_prev = 0, ty_prev = 0;
    while (1) {
        rays_tick ();
        ents_tick ();

        if (IO_but1_ ()) {
            IO_debug ("- but1");
            misc_mdelay (100);
        }
        
        if (IO_but2_ ()) {
            IO_debug ("- but2");
            rays_clear ();
            rays_boost_power ();
            misc_mdelay (100);
        }

        if (IO_tch_ ()) {
            uint16_t tx, ty;
            if (tch_read_px_ (&tx, &ty)) {
                if (! rays_tch_)
                    rays_tch_begin ();
                else
                    rays_add (tx_prev, ty_prev, tx, ty);
                tx_prev = tx;
                ty_prev = ty;
            }
        }
        else {
            if (rays_tch_)
                rays_tch_end ();
        }

        tiles_render ();
        ents_render ();
        rays_render ();

      #ifdef F4EMU
        emu_render ();
      #endif

        misc_mdelay (frame_t);
    }
}

