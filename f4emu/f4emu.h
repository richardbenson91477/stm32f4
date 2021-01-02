#ifndef F4EMU_H
#define F4EMU_H

#include <stdint.h>
#include <unistd.h>

#define LCD_W 240
#define LCD_H 320
#define LCD_HW 120
#define LCD_HH 160
#define LCD_RES_N 76800

#define ABS(x) (x < 0 ? -x : x)

void emu_render ();

extern void LCD_init ();
extern void LCD_clear (uint16_t col);
extern void LCD_pt (uint16_t x0, uint16_t y0, uint16_t col);
extern void LCD_rect (uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t col);
extern void LCD_blit (uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, const uint16_t *_p);
extern void LCD_blit_a (uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, const uint16_t *_p);
extern void LCD_line (uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t col);

extern uint8_t tch_read_px_ (uint16_t *tx, uint16_t *ty);

extern void misc_mdelay (uint32_t n);
extern void misc_udelay (uint32_t n);

extern void IO_init ();
extern void IO_USART_init ();
extern void IO_USART_putchar (char ch);
extern void IO_USART_puts (const char *_s);
extern void IO_debug (const char *_s);
extern uint8_t IO_but1_ ();
extern uint8_t IO_but2_ ();
extern uint8_t IO_tch_ ();

#endif

