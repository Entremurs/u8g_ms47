#include "include.h"



//=============================================================================
void u8g_DrawBitmapSecondScreen(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, u8g_uint_t cnt, u8g_uint_t h, const uint8_t *bitmap)
{	
	u8g_pb_t *pb = (u8g_pb_t *)(u8g->dev->dev_mem);
	
	uint8_t *ptr = pb->buf;
	
	draw_4gs_s_bitmap_on_pos(ptr, x, y, cnt, h, pb->width/2, bitmap);
}

//=============================================================================
void draw_4gs_s_bitmap_on_pos(uint8_t *ptr, u8g_uint_t x, u8g_uint_t y, u8g_uint_t cnt, u8g_uint_t h, uint8_t width, const uint8_t *bitmap)
{
	uint8_t all_byte_complete = 0;
	
	uint8_t offset = 0;
	if((x % 2) == 0)
	{
		offset 	= 1;	
	}
	else
	{
		offset 	= 0;	
		
	}
	
	x 	/= 2;
	
	for(uint8_t j = 0; j<h; j++)
	{	
		for(uint8_t k = 0; k < cnt*2; k++)//px
		{
			uint8_t x1 = k/2; //to byte
			uint8_t z = cnt - x1;
			
			uint32_t addr = x + (y*width) - z + 1;
			
			if(k % 2 == 0) //to byte
			{
				if(offset)
				{					
					drawing_4gs_pixel(&ptr[addr], (*bitmap&0xF0), 1);
				}
				else
				{					
					drawing_4gs_pixel(&ptr[addr], (*bitmap&0xF0) >> 4, 0);
				}
				all_byte_complete++;
			}
			else
			{
				if(offset)
				{					
					drawing_4gs_pixel(&ptr[addr - offset], (*bitmap&0x0F), 0);
				}
				else
				{					
					drawing_4gs_pixel(&ptr[addr], (*bitmap&0x0F) << 4, 1);
				}
				all_byte_complete++;
			}
			
			
			
			if(all_byte_complete == 2)
			{
				all_byte_complete = 0;
				bitmap--;
			}
			
			#ifdef DEBUG_MODE_U8G_MS47_ABSOLUTE
				//dprintf("%X\t", *bitmap);
				dprintf("%d,%d,%X\t", i, j, *bitmap);
			#endif
		}
		
		#ifdef DEBUG_MODE_U8G_MS47_ABSOLUTE
			dprintf("\n");
			delay_ms(10);
		#endif

		y++;
	}
}

//=============================================================================
void u8g_Draw8Pixel_4bit_mode_second_screen(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, uint8_t pixel)
{
	uint8_t z = 0;
	u8g_pb_t *pb = (u8g_pb_t *)(u8g->dev->dev_mem);
	uint8_t *ptr = pb->buf;
	
	for(uint8_t k = 0; k < 4; k++)
	{
		uint32_t offset =  x+(y*pb->width/2);
		
		uint8_t h = pixel&(1<<(z)) 	? 0xF0 : 0x00;
		uint8_t l = pixel&(1<<(z+1))? 0x0F : 0x00;
		
		ptr[k+offset] = h | l;
		
		z+= 2;
	}
}

