// testing with stm32f407zgt6 and ili9341 16bit i/o FSMC
#ifndef F4EMU
  #include "f4lib/f4lib.h"
#else
  #include "f4emu/f4emu.h"
#endif

#include "img/img1.h"

int main () {
    IO_init ();

  #ifdef SERIAL_CON
    IO_USART_init ();
    IO_debug ("+ serial");
  #endif

    LCD_init ();
    IO_debug ("+ LCD");
    
    IO_debug ("> main loop");

    while (1) {
        if (IO_tch_ ())
            break;

        LCD_clear (0x0);

        // ~1500 lps
        IO_debug ("line 1");
        for (uint16_t x = 0; x < LCD_W; x ++)
            LCD_line (0, 0, x, LCD_H - 1, 0xa000);
        for (uint16_t x = 0; x < LCD_W; x ++)
            LCD_line (0, 0, x, LCD_H - 1, 0x0);

        for (uint16_t y = 0; y < LCD_H; y ++)
            LCD_line (0, 0, LCD_W - 1, y, 0xa000);
        for (uint16_t y = 0; y < LCD_H; y ++)
            LCD_line (0, 0, LCD_W - 1, y, 0x0);

        for (uint16_t y = 0; y < LCD_H; y ++)
            LCD_line (LCD_W - 1, 0, 0, y, 0xa000);
        for (uint16_t y = 0; y < LCD_H; y ++)
            LCD_line (LCD_W - 1, 0, 0, y, 0x0);

        for (uint16_t x = 0; x < LCD_W; x ++)
            LCD_line (0, LCD_H - 1, x, 0, 0xa000);
        for (uint16_t x = 0; x < LCD_W; x ++)
            LCD_line (0, LCD_H - 1, x, 0, 0x0);

        IO_debug ("line 2240");

        // ~128 fps (256f/2s)
        IO_debug ("frame 1");
        for (uint16_t d = 0; d < 0x100; d ++) {
            LCD_clear (0xa000 + d);
        }
        IO_debug ("frame 256");
 
        // ~80 fps (240f/3s) 
        IO_debug ("frame 1");
        for (uint16_t d = 0; d < 240; d ++) {
            LCD_blit (0, 0, LCD_W, LCD_H, img1);
        }
        IO_debug ("frame 240");

      #ifdef F4EMU
        emu_render ();
      #endif
        misc_mdelay (1000);
    }
}

