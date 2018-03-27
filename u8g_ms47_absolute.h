#ifndef _U8G_MS47_ABSOLUTE_H_
#define _U8G_MS47_ABSOLUTE_H_


extern 	uint8_t ms47_init_done;

void	ms47_object_drawing_task	(u8g_uint_t x, u8g_uint_t y, u8g_uint_t cnt, u8g_uint_t h, const uint8_t *bitmap, uint8_t out);
void	ms47_drawing_task_next_page	(void);
void	ms47_drawing_task_first_page(void);
uint8_t ms47_get_str_pixel_width	(const char *s);
void 	ms47_set_font				(const u8g_fntpgm_uint8_t  *font);
void 	ms47_init					(void);
void 	ms47_draw_str				(uint8_t x, uint8_t y, const char *s);

#endif
