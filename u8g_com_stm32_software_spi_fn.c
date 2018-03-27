/*
  
  u8g_com_stm32_software_spi.c

  Universal 8bit Graphics Library
  
  Copyright (c) 2012, olikraus@gmail.com
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification, 
  are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this list 
    of conditions and the following disclaimer.
    
  * Redistributions in binary form must reproduce the above copyright notice, this 
    list of conditions and the following disclaimer in the documentation and/or other 
    materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  
 
*/ 
 
#include "include.h"

uint8_t u8g_com_stm32_software_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr)
{
  switch(msg)
  {
	case U8G_COM_MSG_STOP:
	{
		break;
	}
	case U8G_COM_MSG_INIT:
	{	
		
		//pin_init(PIN_M47SP_RESET);
		//pin_init(PIN_M47SP_CS2);
		//pin_init(PIN_M47SP_CS1);  
		//pin_init(PIN_SPI_SCK);    
		//pin_init(PIN_SPI_D_C);
		//pin_clr(PIN_SPI_D_C);//always low for 3-wire SPI.
		//pin_init(PIN_SPI_SDIN);   
		
		break;
	}

   	case U8G_COM_MSG_ADDRESS:                     /* define cmd (arg_val = 0) or data mode (arg_val = 1) */
    { 
		if(arg_val)
		{
			pin_set(PIN_SPI_D_C);
		}
		else
		{
			pin_clr(PIN_SPI_D_C);
		}
		break;
	}
	
	case U8G_COM_MSG_RESET:
	{	
		if(arg_val)
		{
			pin_set(PIN_M47SP_RESET);
		}
		else
		{
			pin_clr(PIN_M47SP_RESET);
		}
		break;
	}
	
    case U8G_COM_MSG_CHIP_SELECT:
	{
      /* arg_val contains the chip number, which should be enabled */
		if(arg_val)
		{
			pin_clr(PIN_M47SP_CS2);
			//pin_clr(PIN_M47SP_CS1);
			if(!ms47_init_done)
			{
				pin_clr(PIN_M47SP_CS1);
			}
		}
		else
		{
			pin_set(PIN_M47SP_CS2);
			if(!ms47_init_done)
			{
				pin_set(PIN_M47SP_CS1);
			}
			//pin_set(PIN_M47SP_CS1);
		}
      break;
	}

    case U8G_COM_MSG_WRITE_BYTE:
	{
		spi_transfer(arg_val);
		break;
	}
    case U8G_COM_MSG_WRITE_SEQ:
	{
		spi_transfer_block(arg_ptr, arg_val);
		break;
	}
    case U8G_COM_MSG_WRITE_SEQ_P:
	{
		//spi_transfer_block();
		break;
	}
	
  }
  return 1;
}

