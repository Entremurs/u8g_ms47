#include "include.h"


/*
*	ms47 have 2 pixel per bytes. 4 grayscale.
*	Presentation of ms47 with two ssd1322. ms47 have two screen each 256*64 px.
*
*	Absolute coordinate of each screen:
*y	-----------------------------
*	|	first screen	(255,63)|
*	|                           |
*	|                           |
*	|(0,0)	start point	 (255,0)|
*y	-----------------------------x	if use u8g_DrawBitmapSecondScreen
*	|(255,0)	start point(0,0)| 	|(0,63)						| 					
*   |                           |	|                           |				
*   |                           |	|                           |					
*	|(255,63)  second screen    |	|0,0    start point         |
*   -----------------------------x	-----------------------------x				
*
*
*	Absolute coord of all screen for this module:
*   -----------------------------y
*   |(0,0)  first screen(255,0) |
*   |                           |
*   |                           |
*   |(0,63)				(255,63)|
*   -----------------------------
*   |(0, 64)					|
*   |                           |
*   |                           |
*   |	second screen  (255,127)|
*   -----------------------------
*	x
*
*
*/


/********************DEBUG MODE***********************/
//#define DEBUG_MODE_U8G_MS47_ABSOLUTE
//#define DEBUG_MS47_OBJECT_DRAWING_TASK
/********************DEBUG MODE***********************/


typedef struct
{
	uint8_t x;
	uint8_t y;
}u8g_pos_t;

#define SIZE_Y		128
#define SIZE_X		256
static uint8_t 	u8g_get_absolute_coor_end_bitmap		(u8g_uint_t x, u8g_uint_t y, u8g_uint_t cnt, u8g_uint_t h, u8g_pos_t *pos);
static uint8_t 	u8g_check_borders						(u8g_uint_t y);
static uint16_t u8g_get_param_bitmap_belongs_first_lcd	(uint8_t y_start, u8g_uint_t h);
static uint16_t u8g_get_y_second_lcd					(uint8_t y_start, u8g_uint_t h);
static void 	convert_abs_pos_to_screen_pos			(uint8_t x, uint8_t y, uint8_t screen_number, u8g_pos_t *pos_rel);


//*************************************************************************************************************//
static uint8_t  object_belongs_to_screen				(u8g_uint_t x, u8g_uint_t y, u8g_uint_t cnt, u8g_uint_t h);
static uint8_t 	object_belongs_both_screen				(uint8_t y, uint8_t y_end);
static uint8_t 	object_belongs_only_second_screen		(uint8_t y);
//*************************************************************************************************************//

enum
{
	FIRST_SCREEN  = 1,
	SECOND_SCREEN = 2,
	BOTH_SCREEN	  = 3
};


#define QTY_SCREEN		2
static u8g_t *u8g_list[QTY_SCREEN] = 
{	
	&u8g1,
	&u8g
};

uint8_t ms47_init_done = 0;

//=============================================================================
void ms47_init(void)
{
	u8g_InitComFn(u8g_list[1], 	&u8g_dev_ssd1322_ms47_half_4x_gr_sw_spi, 	u8g_com_stm32_software_spi_fn);		
	u8g_InitComFn(u8g_list[0], 	&u8g_dev_ssd1322_ms47_half_4x_gr_sw_spi1, 	u8g_com_stm32_software_spi_fn1);		
	ms47_init_done = 1;
}

//=============================================================================
void  ms47_drawing_task_first_page(void)
{	
	for(uint32_t i = 0; i < QTY_SCREEN; i++)
	{
		u8g_FirstPage(u8g_list[i]);
	}
}

//=============================================================================
void  ms47_drawing_task_next_page(void)
{	
	for(uint32_t i = 0; i < QTY_SCREEN; i++)
	{			
		u8g_NextPage(u8g_list[i]);
	}
}

//=============================================================================
void ms47_object_drawing_task(u8g_uint_t x, u8g_uint_t y, u8g_uint_t cnt, u8g_uint_t h, const uint8_t *bitmap, uint8_t out)
{	
	u8g_pos_t 		pos_start_abs;
	
	cnt = (cnt / 2);//because ms47 have 2 pixel per bytes. 4 grayscale.

	pos_start_abs.x = x;
	pos_start_abs.y = y;
	u8g_pos_t 		start_draw;
		
	switch(object_belongs_to_screen(x, y, cnt, h))
	{
		case FIRST_SCREEN:
		{
			convert_abs_pos_to_screen_pos(x, y, FIRST_SCREEN, &start_draw);
		
			if(out)
			{
				u8g_DrawBitmapFirstScreen(u8g_list[0], start_draw.x, start_draw.y, cnt, h, bitmap+(cnt*h)-1);
			}
			else
			{
				uint8_t bitmap_symbol[4] = {0};
				
				eight_pixel_to_bitmap(&bitmap_symbol[0], *bitmap);
								
				u8g_DrawBitmapFirstScreen(u8g_list[0], start_draw.x, start_draw.y, cnt, h, &bitmap_symbol[3]);
			}
			break;
		}
		case SECOND_SCREEN:
		{		
			convert_abs_pos_to_screen_pos(x, y, SECOND_SCREEN, &start_draw);
			if(out)
			{
				u8g_DrawBitmapSecondScreen(u8g_list[1], start_draw.x, start_draw.y, cnt, h, bitmap+(cnt*h)-1);
			}
			else
			{									
				uint8_t bitmap_symbol[4] = {0};
				
				eight_pixel_to_bitmap(&bitmap_symbol[0], *bitmap);

				u8g_DrawBitmapSecondScreen(u8g_list[1], start_draw.x, start_draw.y, cnt, h, &bitmap_symbol[3]);			
			}
			
			break;
		}
		case BOTH_SCREEN:
		{						
			uint8_t h_first_screen 	= u8g_get_param_bitmap_belongs_first_lcd(pos_start_abs.y, h);
			
			convert_abs_pos_to_screen_pos(x, y, FIRST_SCREEN, &start_draw);
			
			if(out)
			{
				u8g_DrawBitmapFirstScreen(u8g_list[0], start_draw.x, start_draw.y, cnt, h_first_screen, (bitmap+(cnt*h)-1));				
			}
			else
			{
				u8g_Draw8Pixel_4bit_mode_first_screen(u8g_list[0], start_draw.x, start_draw.y, *bitmap);
			}
			
			uint32_t offset 		= cnt*(h-h_first_screen);
			
			convert_abs_pos_to_screen_pos(x, y+h_first_screen, SECOND_SCREEN, &start_draw);
			
			if(out)
			{
				u8g_DrawBitmapSecondScreen(u8g_list[1], start_draw.x, start_draw.y, cnt, h-h_first_screen, bitmap+offset-1);								
			}
			else
			{										
				u8g_Draw8Pixel_4bit_mode_second_screen(u8g_list[1], start_draw.x-1-cnt/2,  start_draw.y, *bitmap);
			}
						
			break;
		}
	}

}

//=============================================================================
uint8_t ms47_get_str_pixel_width(const char *s)
{
	return (u8g_GetStrPixelWidth(u8g_list[0], s)*2);
}

//=============================================================================
/* 
*	This function draw str from start pos (y) and to end pos (y-size) letter.
*/
void ms47_draw_str(uint8_t x, uint8_t y, const char *s)
{
	u8g_DrawStr(u8g_list[0],  x, y, s);
}

//=============================================================================
/*
*	Font must set before ms47_drawing_task_first_page();
*/
void ms47_set_font(const u8g_fntpgm_uint8_t  *font)
{
	u8g_SetFont(u8g_list[0], font);
	u8g_SetFont(u8g_list[1], font);
}

//=============================================================================
static uint8_t object_belongs_to_screen(u8g_uint_t x, u8g_uint_t y, u8g_uint_t cnt, u8g_uint_t h)
{
	u8g_pos_t pos_end_abs;
				
	uint8_t err = u8g_get_absolute_coor_end_bitmap(x, y, cnt, h, &pos_end_abs);
		
	if(object_belongs_both_screen(y, pos_end_abs.y))
	{
		return (BOTH_SCREEN);
	}
	else if(object_belongs_only_second_screen(y))
	{
		return (SECOND_SCREEN);
	}
	else
	{
		return (FIRST_SCREEN);
	}
}

//=============================================================================
static uint8_t object_belongs_both_screen(uint8_t y, uint8_t y_end)
{
	return (!u8g_check_borders(y) && u8g_check_borders(y_end));
}

//=============================================================================
static uint8_t object_belongs_only_second_screen(uint8_t y)
{
	return (u8g_check_borders(y));
}

//=============================================================================
static uint8_t u8g_get_absolute_coor_end_bitmap(u8g_uint_t x, u8g_uint_t y, u8g_uint_t cnt, u8g_uint_t h, u8g_pos_t *pos)
{
	pos->x = x+(cnt*2);
	pos->y = y+h-1;
	
	if(pos->y > SIZE_Y-1)
	{
		return (1);
	}
	if(pos->x > SIZE_X-1)
	{
		return (1);
	}
	
	return (0);
}

//=============================================================================
static uint8_t u8g_check_borders(u8g_uint_t y)
{	
	if(y >= SIZE_Y/2)
	{
		return (1);
	}
	return 		(0);
}

//=============================================================================
static uint16_t u8g_get_param_bitmap_belongs_first_lcd(uint8_t y_start, u8g_uint_t h)
{	
	#define END_FIRST_SCREEN	SIZE_Y/2
	
	if(y_start > END_FIRST_SCREEN)
	{
		return (0);
	}
	
	return (END_FIRST_SCREEN - y_start);
}

//=============================================================================
static uint16_t u8g_get_y_second_lcd(uint8_t y_start, u8g_uint_t h)
{
	
	#define END_FIRST_SCREEN	SIZE_Y/2
	
	/*if(y_start > END_FIRST_SCREEN)
	{
		return (0);
	}*/
	
	uint8_t d =  (y_start + h) - END_FIRST_SCREEN;
	
	return (d);
}

//=============================================================================
static void convert_abs_pos_to_screen_pos(uint8_t x, uint8_t y, uint8_t screen_number, u8g_pos_t *pos_rel)
{
	if(screen_number == 1)
	{
		pos_rel->y = (SIZE_Y/2 - 1) - y;
		pos_rel->x = x;
	}
	if(screen_number == 2)
	{		
		pos_rel->x = (SIZE_X-1) - x;
		pos_rel->y = y - (SIZE_Y/2);
	}
}




