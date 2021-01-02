#ifndef F4_LCD_H
#define F4_LCD_H

#define LCD_W 240
#define LCD_H 320
#define LCD_HW 120
#define LCD_HH 160
#define LCD_RES_N 76800

extern void LCD_init ();
extern void LCD_clear (uint16_t col);
extern void LCD_pt (uint16_t x0, uint16_t y0, uint16_t col);
extern void LCD_rect (uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t col);
extern void LCD_blit (uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, const uint16_t *_p);
extern void LCD_blit_a (uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, const uint16_t *_p);
extern void LCD_line (uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t col);

#endif

