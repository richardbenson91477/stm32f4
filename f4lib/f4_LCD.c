#include "f4lib.h"

inline void LCD_reg (uint16_t val) {
    *((volatile uint16_t *)0x6c0007fe) = val;
}
inline void LCD_data (uint16_t val) {
    *((volatile uint16_t *)0x6c000800) = val;
}

void LCD_init () { 
    IO_LCD_init ();

    IO_LCD_reset (0);
    misc_mdelay (150);

    IO_LCD_reset (1);
    misc_mdelay (50);
    
    // ili9341
    LCD_reg (0xEF); LCD_data (0x03); LCD_data (0x80); LCD_data (0x02);
    LCD_reg (0xCF); LCD_data (0x00); LCD_data (0xC1); LCD_data (0X30);
    LCD_reg (0xED); LCD_data (0x64); LCD_data (0x03); LCD_data (0X12);
        LCD_data (0X81); 
    LCD_reg (0xE8); LCD_data (0x85); LCD_data (0x10); LCD_data (0x7A);
    LCD_reg (0xCB); LCD_data (0x39); LCD_data (0x2C); LCD_data (0x00);
        LCD_data (0x34); LCD_data (0x02);
    LCD_reg (0xF7); LCD_data (0x20); 
    LCD_reg (0xEA); LCD_data (0x00); LCD_data (0x00); 
    LCD_reg (0xC0); LCD_data (0x1B);
    LCD_reg (0xC1); LCD_data (0x01);
    LCD_reg (0xC5); LCD_data (0x30); LCD_data (0x30);
    LCD_reg (0xC7); LCD_data (0XB7);
    LCD_reg (0x36); LCD_data (0x48);
    LCD_reg (0x3A); LCD_data (0x55);
    LCD_reg (0xB1); LCD_data (0x00); LCD_data (0x1A);
    LCD_reg (0xB6); LCD_data (0x0A); LCD_data (0xA2);
    LCD_reg (0xF2); LCD_data (0x00);
    LCD_reg (0x26); LCD_data (0x01);
    LCD_reg (0xE0); LCD_data (0x0F); LCD_data (0x2A); LCD_data (0x28);
        LCD_data (0x08); LCD_data (0x0E); LCD_data (0x08); LCD_data (0x54);
        LCD_data (0XA9); LCD_data (0x43); LCD_data (0x0A); LCD_data (0x0F);
        LCD_data (0x00); LCD_data (0x00); LCD_data (0x00); LCD_data (0x00);
    LCD_reg (0XE1); LCD_data (0x00); LCD_data (0x15); LCD_data (0x17);
        LCD_data (0x07); LCD_data (0x11); LCD_data (0x06); LCD_data (0x2B);
        LCD_data (0x56); LCD_data (0x3C); LCD_data (0x05); LCD_data (0x10);
        LCD_data (0x0F); LCD_data (0x3F); LCD_data (0x3F); LCD_data (0x0F);
    LCD_reg (0x2B); LCD_data (0x00); LCD_data (0x00); LCD_data (0x01);
        LCD_data (0x3f);
    LCD_reg (0x2A); LCD_data (0x00); LCD_data (0x00); LCD_data (0x00);
        LCD_data (0xef);
    LCD_reg (0x11);
    misc_mdelay (120);
    LCD_reg (0x29);
    LCD_reg (0x36);
    LCD_data (0xC9);      
 
    IO_LCD_backlight (1);
}

void LCD_win (uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    LCD_reg (0x2A); 
    LCD_data (x0 >> 8);
    LCD_data (0x00FF & x0);       
    LCD_data (x1 >> 8);
    LCD_data (0x00FF & x1);

    LCD_reg (0x2B);
    LCD_data (y0 >> 8);
    LCD_data (0x00FF & y0);       
    LCD_data (y1 >> 8);
    LCD_data (0x00FF & y1);

    LCD_reg (0x2C);
}

void LCD_clear (uint16_t col) {
    LCD_win (0, 0, LCD_W - 1, LCD_H - 1);
    for (uint32_t c = 0; c < LCD_RES_N; c ++)
        LCD_data (col);
}

void LCD_pt (uint16_t x0, uint16_t y0, uint16_t col) {
    LCD_win (x0, y0, x0, y0);
    LCD_data (col);
}

void LCD_rect (uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t col) {
    LCD_win (x0, y0, x1, y1);
    for (int32_t c = 0; c < ((x1 - x0 + 1) * (y1 - y0 + 1)); c++)
        LCD_data (col);
}

void LCD_blit (uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, const uint16_t *_p) {
    uint16_t *_p2 = (uint16_t *)_p;
    LCD_win (x0, y0, x0 + w - 1, y0 + h - 1);
    for (uint32_t i = 0; i < (w * h); i ++)
        LCD_data (*_p2++);
}

void LCD_blit_a (uint16_t x0, uint16_t y0, uint16_t w, uint16_t h,
        const uint16_t *_p) {
    uint16_t p, *_p2 = (uint16_t *)_p;
    for (uint16_t y = y0; y < (y0 + h); y ++) {
        for (uint16_t x = x0; x < (x0 + w); x ++) {
            p = *_p2++;
            // black (0x0) = full transparency 
            if (p) {
                LCD_win (x, y, x, y);
                LCD_data (p);
            }
        }
    }
}

void LCD_line (uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t col) {
    int16_t dx = x1 - x0;
    int16_t dy = y1 - y0;
    if ((! dx) && (! dy))
        return;

    float fdx = (float)dx;
    float fdy = (float)dy;

    float fdxm = ABS(fdx);
    float fdym = ABS(fdy);

    float t_step = 1.0 / (1.00001 * (fdxm > fdym ? fdxm : fdym));

    for (float t = 0.0; t <= 1.0f; t += t_step) {
        uint16_t x = (uint16_t)(fdx * t + (float)x0);
        uint16_t y = (uint16_t)(fdy * t + (float)y0);
        if ((x < LCD_W) && (y < LCD_H)) {
            LCD_win (x, y, x, y);
            LCD_data (col);
        }
    }
}

