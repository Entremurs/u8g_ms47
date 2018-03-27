#ifndef _U8G_MS47_S_LCD_H_
#define _U8G_MS47_S_LCD_H_

void u8g_DrawBitmapSecondScreen				(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, u8g_uint_t cnt, u8g_uint_t h, const uint8_t *bitmap);
void u8g_Draw8Pixel_4bit_mode_second_screen	(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, uint8_t pixel);
void draw_4gs_s_bitmap_on_pos				(uint8_t *ptr, u8g_uint_t x, u8g_uint_t y, u8g_uint_t cnt, u8g_uint_t h, uint8_t width, const uint8_t *bitmap);

#endif
