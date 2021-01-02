#include "f4emu.h"
#include <SDL2/SDL.h>
    
SDL_Window *_win;
SDL_Surface *_win_surf;
SDL_Rect win_rc;
uint32_t *_win_p;

uint8_t tch_, but1_, but2_;
uint16_t m_x, m_y;

void emu_tick () {
    SDL_Event ev;
    uint8_t done = 0;

    while (SDL_PollEvent (&ev) != 0) {
        if (ev.type == SDL_QUIT) {
            done = 1;
            break;
        }
        else if (ev.type == SDL_KEYDOWN) {
            switch (ev.key.keysym.sym) {
                case SDLK_ESCAPE:
                    done = 1;
                    break;
                case SDLK_1:
                    but1_ = 1;
                    break;
                case SDLK_2:
                    but2_ = 1;
                    break;
            }
            if (done)
                break;
        }
        else if (ev.type == SDL_KEYUP) {
            switch (ev.key.keysym.sym) {
                case SDLK_1:
                    but1_ = 0;
                    break;
                case SDLK_2:
                    but2_ = 0;
                    break;
            }
        }
        else if (ev.type == SDL_MOUSEBUTTONDOWN) {
            if (ev.button.button == 1)
                tch_ = 1;
        }
        else if (ev.type == SDL_MOUSEBUTTONUP) {
            if (ev.button.button == 1)
                tch_ = 0;
        }
        else if (ev.type == SDL_MOUSEMOTION) {
            m_x = ev.motion.x;
            m_y = ev.motion.y;
        }
    }
    if (done) {
        SDL_DestroyWindow (_win);
        SDL_Quit ();
        return;
    }
}

void emu_render () {
    SDL_UpdateWindowSurface (_win);
}

static inline uint32_t to_rgb888 (uint16_t col) {
    return
        ((uint32_t)(col & 0xf800) << 8) +\
        ((uint32_t)(col & 0x7e0) << 5) +\
        ((uint32_t)(col & 0x1f) << 3);
}

void LCD_init () { 
    SDL_Init (SDL_INIT_VIDEO);

    _win = SDL_CreateWindow (LCD_S,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        LCD_W, LCD_H,
        SDL_WINDOW_SHOWN);
    _win_surf = SDL_GetWindowSurface (_win);
    _win_p = (uint32_t *) _win_surf->pixels;

    win_rc.x = 0;
    win_rc.y = 0;
    win_rc.w = LCD_W;
    win_rc.h = LCD_H;
}

void LCD_clear (uint16_t col) {
    SDL_FillRect (_win_surf, &win_rc, to_rgb888(col));
}

void LCD_pt (uint16_t x0, uint16_t y0, uint16_t col) {
    _win_p[y0 * LCD_W + x0] = to_rgb888(col);
}

void LCD_rect (uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t col) {
    SDL_Rect rc;
    rc.x = x0;
    rc.y = y0;
    rc.w = x1 - x0;
    rc.h = y1 - y0;
    SDL_FillRect (_win_surf, &rc, to_rgb888(col));
}

void LCD_blit (uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, const uint16_t *_p) {
    uint16_t scan_n = LCD_W - w;
    uint32_t *_pd = _win_p + (y0 * LCD_W) + x0;

    for (uint16_t y = 0; y < h; y ++) {
        for (uint16_t x = 0; x < w; x ++)
            *_pd++ = to_rgb888(*_p++);
        _pd += scan_n;
    }
}

void LCD_blit_a (uint16_t x0, uint16_t y0, uint16_t w, uint16_t h,
        const uint16_t *_p) {
    uint16_t scan_n = LCD_W - w;
    uint32_t p, *_pd = _win_p + (y0 * LCD_W) + x0;

    for (uint16_t y = 0; y < h; y ++) {
        for (uint16_t x = 0; x < w; x ++) {
            p = *_p;
            if (p)
                *_pd = to_rgb888(p);
            _pd ++;
            _p ++;
        }
        _pd += scan_n;
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
        if ((x < LCD_W) && (y < LCD_H))
            _win_p[y * LCD_W + x] = to_rgb888(col);
    }
}

uint8_t tch_read_px_ (uint16_t *_tx, uint16_t *_ty) {
    emu_tick ();
    if (! tch_)
        return 0;

    *_tx = m_x;
    *_ty = m_y;

    return 1;
}

void misc_mdelay (uint32_t n) {
    SDL_Delay (n);
}

void misc_udelay (uint32_t n) {
    n > 1000 ? SDL_Delay (n / 1000) : SDL_Delay (1);
}

void IO_init () {
    but1_ = but2_ = tch_ = 0;
    m_x = m_y = 0;
}

void IO_USART_init () {                                                
}

void IO_USART_putchar (char ch) {
    (void) write (1, &ch, 1);
}

void IO_USART_puts (const char *_s) {
    while (*_s)
        IO_USART_putchar (*(_s++));
}

extern void IO_debug (const char *_s) {
    IO_USART_puts ("debug: ");
    IO_USART_puts (_s);
    IO_USART_putchar ('\n');
}

uint8_t IO_but1_ () {
    emu_tick ();
    return but1_;
}

uint8_t IO_but2_ () {
    emu_tick ();
    return but2_;
}

uint8_t IO_tch_ () {
    emu_tick ();
    return tch_;
}

