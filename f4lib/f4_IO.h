#ifndef F4_IO_H
#define F4_IO_H

extern void IO_init ();

extern void IO_USART_init ();
extern void IO_USART_putchar (char ch);
extern void IO_USART_puts (const char *_s);

extern void IO_debug (const char *_s);

extern void IO_LCD_init ();
extern void IO_LCD_backlight (uint8_t en_);
extern void IO_LCD_reset (uint8_t en_);

extern uint8_t IO_but1_ ();
extern uint8_t IO_but2_ ();

extern uint8_t IO_tch_ ();
extern void IO_tch_get_pt (uint16_t *_pt_x, uint16_t *_pt_y);

#endif

