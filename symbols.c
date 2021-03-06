//--------------------------------------------------
// Знакогенератор для пульта управления
// табло
//--------------------------------------------------
#include "globals.h"
#include "symbols.h"
#include "ste2007.h"

//--------------------------------------------------
const uint8_t small_w=6, small_h=11;
const uint8_t big_w=17, big_h=32;
const uint8_t ball_r=10;
const uint8_t service_w=66, service_h=12;
const uint8_t blue_w=8, blue_h=16;

// packed small digits
//--------------------------------------------------
const char small_0[] PROGMEM = {/*{{{*/
    0xDE,0x8D,0x03,0x03,0x8D,0xDE,
    0x03,0x05,0x06,0x06,0x05,0x03
};
const char small_1[] PROGMEM = {
    0x00,0x00,0x00,0x00,0x8C,0xDE,
    0x00,0x00,0x00,0x00,0x01,0x03
};
const char small_2[] PROGMEM = {
    0xC0,0xA1,0x73,0x73,0x2D,0x1E,
    0x03,0x05,0x06,0x06,0x04,0x00
};
const char small_3[] PROGMEM = {
    0x00,0x21,0x73,0x73,0xAD,0xDE,
    0x00,0x04,0x06,0x06,0x05,0x03
};
const char small_4[] PROGMEM = {
    0x1E,0x2C,0x70,0x70,0xAC,0xDE,
    0x00,0x00,0x00,0x00,0x01,0x03
};
const char small_5[] PROGMEM = {
    0x1E,0x2D,0x73,0x73,0xA1,0xC0,
    0x00,0x04,0x06,0x06,0x05,0x03
};
const char small_6[] PROGMEM = {
    0xDE,0xAD,0x73,0x73,0xA1,0xC0,
    0x03,0x05,0x06,0x06,0x05,0x03
};
const char small_7[] PROGMEM = {
    0x00,0x01,0x03,0x03,0x8D,0xDE,
    0x00,0x00,0x00,0x00,0x01,0x03
};
const char small_8[] PROGMEM = {
    0xDE,0xAD,0x73,0x73,0xAD,0xDE,
    0x03,0x05,0x06,0x06,0x05,0x03
};
const char small_9[] PROGMEM = {
    0x1E,0x2D,0x73,0x73,0xAD,0xDE,
    0x00,0x04,0x06,0x06,0x05,0x03
};
/*}}}*/
// packed large digits
//--------------------------------------------------
const char big_0[] PROGMEM = {/*{{{*/
    0xFE,0xFD,0xFB,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0xFB,0xFD,0xFE,
    0x7F,0x3F,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0x3F,0x7F,
    0xFF,0xFE,0xFC,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0xFE,0xFF,
    0x7F,0xDF,0xBF,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xBF,0xDF,0x7F
};
const char big_1[] PROGMEM = {
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0xFC,0xFE,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0x3F,0x7F,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0xFE,0xFF,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0x3F,0x7F
};
const char big_2[] PROGMEM = {
    0x00,0x01,0x03,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0xFB,0xFD,0xFE,
    0x00,0x80,0xC0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xDF,0xBF,0x7F,
    0xFF,0xFE,0xFD,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x01,0x00,0x00,
    0x7F,0xDF,0xBF,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xC0,0x80,0x00
};
const char big_3[] PROGMEM = {
    0x00,0x01,0x03,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0xFB,0xFD,0xFE,
    0x00,0x80,0xC0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xDF,0xBF,0x7F,
    0x00,0x00,0x01,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0xFD,0xFE,0xFF,
    0x00,0x80,0xC0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xBF,0xDF,0x7F
};
const char big_4[] PROGMEM = {
    0xFE,0xFC,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0xFC,0xFE,
    0x7F,0xBF,0xDF,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xDF,0xBF,0x7F,
    0x00,0x00,0x01,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0xFD,0xFE,0xFF,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0x3F,0x7F
};
const char big_5[] PROGMEM = {
    0xFE,0xFD,0xFB,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x03,0x01,0x00,
    0x7F,0xBF,0xDF,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xC0,0x80,0x00,
    0x00,0x01,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0xFD,0xFE,0xFF,
    0x00,0x80,0xC0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xBF,0xDF,0x7F
};
const char big_6[] PROGMEM = {
    0xFE,0xFD,0xFB,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x03,0x01,0x00,
    0x7F,0xBF,0xDF,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xC0,0x80,0x00,
    0xFF,0xFE,0xFD,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0xFD,0xFE,0xFF,
    0x7F,0xDF,0xBF,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xBF,0xDF,0x7F
};
const char big_7[] PROGMEM = {
    0x00,0x01,0x03,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0xFB,0xFD,0xFE,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0x3F,0x7F,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0xFE,0xFF,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0x3F,0x7F
};
const char big_8[] PROGMEM = {
    0xFE,0xFD,0xFB,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0xFB,0xFD,0xFE,
    0x7F,0xBF,0xDF,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xDF,0xBF,0x7F,
    0xFF,0xFE,0xFD,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0xFD,0xFE,0xFF,
    0x7F,0xDF,0xBF,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xBF,0xDF,0x7F
};
const char big_9[] PROGMEM = {
    0xFE,0xFD,0xFB,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0xFB,0xFD,0xFE,
    0x7F,0xBF,0xDF,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xDF,0xBF,0x7F,
    0x00,0x00,0x01,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0xFD,0xFE,0xFF,
    0x00,0x80,0xC0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xBF,0xDF,0x7F
};
/*}}}*/
// a ball
//--------------------------------------------------
const char ball[] PROGMEM = {/*{{{*/
    0x78,0xFE,0xFE,0xFF,0xFF,0xFF,0xFF,0xFE,0xFE,0x78,
    0x00,0x01,0x01,0x03,0x03,0x03,0x03,0x01,0x01,0x00
};
/*}}}*/
// service
//--------------------------------------------------
const char service[] PROGMEM = {/*{{{*/
    0xFE,0xFE,0xFE,0x06,0x06,0xFE,0xFE,0xFE,0x00,0x00,0xF8,0xFC,0xFE,0x8E,0x06,0x06,0x8E,0xFE,0xFC,0xF8,0x00,0x00,0x00,0x80,0xFE,0xFE,0x7E,0x06,0x06,0xFE,0xFE,0xFE,0x00,0x00,0x00,0xE0,0xF6,0xF6,0x36,0x36,0xB6,0xFE,0xFC,0xF8,0x00,0x00,0x3E,0x7E,0x7E,0x60,0x60,0xFE,0xFE,0xFE,0x00,0x00,0x00,0xE0,0xF6,0xF6,0x36,0x36,0xB6,0xFE,0xFC,0xF8,
    0x03,0x03,0x03,0x00,0x00,0x03,0x03,0x03,0x00,0x00,0x00,0x01,0x03,0x03,0x03,0x03,0x03,0x03,0x01,0x00,0x00,0x00,0x0F,0x0F,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x0F,0x0F,0x00,0x00,0x01,0x03,0x03,0x03,0x03,0x01,0x03,0x03,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x03,0x03,0x00,0x00,0x00,0x01,0x03,0x03,0x03,0x03,0x01,0x03,0x03,0x03
};
/*}}}*/
// bluetooth availability
//--------------------------------------------------
const char bluetooth[] PROGMEM = {/*{{{*/
    0x10, 0x20, 0x40, 0xFF, 0x81, 0x42, 0x24, 0x18,
    0x08, 0x04, 0x02, 0xFF, 0x81, 0x42, 0x24, 0x18
};
/*}}}*/
//--------------------------------------------------
void small_dots(void)/*{{{*/
{
framebuffer[47+96*3] |= 0x1B;
framebuffer[48+96*3] |= 0x1B;
framebuffer[47+96*3] &=~0x04;
framebuffer[48+96*3] &=~0x04;
}
/*}}}*/
//--------------------------------------------------
void small_digit(uint8_t d, uint8_t x, uint8_t y)/*{{{*/
{
// small_y - rows into small_0
// small_y1 - dots into row in small_0
uint8_t small_y=0, small_y1=0;
uint8_t pixel=0;

if(d < 0 || d > 9)
    return;

for(uint8_t i=0; i<9; i++)
    { // rows
    for(uint8_t j=0; j<8; j++)
	{ // dots within rows
	if(j+8*i <  y)
	    continue;
	if(j+8*i >= y+small_h)
	    continue;

	for(uint8_t k=0; k<96; k++)
	    { // columns
	    if(k <  x)
		continue;
	    if(k >= x+small_w)
		continue;

	    // framebuffer is packed
	    switch(d)
		{
		case 0:
		    pixel=pgm_read_byte(&small_0[k-x + small_y*small_w]);
		    break;
		case 1:
		    pixel=pgm_read_byte(&small_1[k-x + small_y*small_w]);
		    break;
		case 2:
		    pixel=pgm_read_byte(&small_2[k-x + small_y*small_w]);
		    break;
		case 3:
		    pixel=pgm_read_byte(&small_3[k-x + small_y*small_w]);
		    break;
		case 4:
		    pixel=pgm_read_byte(&small_4[k-x + small_y*small_w]);
		    break;
		case 5:
		    pixel=pgm_read_byte(&small_5[k-x + small_y*small_w]);
		    break;
		case 6:
		    pixel=pgm_read_byte(&small_6[k-x + small_y*small_w]);
		    break;
		case 7:
		    pixel=pgm_read_byte(&small_7[k-x + small_y*small_w]);
		    break;
		case 8:
		    pixel=pgm_read_byte(&small_8[k-x + small_y*small_w]);
		    break;
		case 9:
		    pixel=pgm_read_byte(&small_9[k-x + small_y*small_w]);
		    break;
		};
	    if(pixel & (1<<small_y1))
		framebuffer[k+96*i] = framebuffer[k+96*i] |  (1<<j);
	    else
		framebuffer[k+96*i] = framebuffer[k+96*i] & ~(1<<j);
	    }

	small_y1++;
	if(small_y1 >= 8)
	    {
	    small_y1=0;
	    small_y++;
	    if(small_y >= 4)
		small_y=3;
	    }
	}
    }
}
/*}}}*/
//--------------------------------------------------
void big_digit(uint8_t d, uint8_t x, uint8_t y)/*{{{*/
{
// big_y - rows into big_0
// big_y1 - dots into row in big_0
uint8_t big_y=0, big_y1=0;
uint8_t pixel=0;

if(d < 0 || d > 9)
    return;

for(uint8_t i=0; i<9; i++)
    { // rows
    for(uint8_t j=0; j<8; j++)
	{ // dots within rows
	if(j+8*i <  y)
	    continue;
	if(j+8*i >= y+big_h)
	    continue;

	for(uint8_t k=0; k<96; k++)
	    { // columns
	    if(k <  x)
		continue;
	    if(k >= x+big_w)
		continue;

	    // framebuffer is packed
	    switch(d)
		{
		case 0:
		    pixel=pgm_read_byte(&big_0[k-x + big_y*big_w]);
		    break;
		case 1:
		    pixel=pgm_read_byte(&big_1[k-x + big_y*big_w]);
		    break;
		case 2:
		    pixel=pgm_read_byte(&big_2[k-x + big_y*big_w]);
		    break;
		case 3:
		    pixel=pgm_read_byte(&big_3[k-x + big_y*big_w]);
		    break;
		case 4:
		    pixel=pgm_read_byte(&big_4[k-x + big_y*big_w]);
		    break;
		case 5:
		    pixel=pgm_read_byte(&big_5[k-x + big_y*big_w]);
		    break;
		case 6:
		    pixel=pgm_read_byte(&big_6[k-x + big_y*big_w]);
		    break;
		case 7:
		    pixel=pgm_read_byte(&big_7[k-x + big_y*big_w]);
		    break;
		case 8:
		    pixel=pgm_read_byte(&big_8[k-x + big_y*big_w]);
		    break;
		case 9:
		    pixel=pgm_read_byte(&big_9[k-x + big_y*big_w]);
		    break;
		};
	    if(pixel & (1<<big_y1))
		framebuffer[k+96*i] = framebuffer[k+96*i] |  (1<<j);
	    else
		framebuffer[k+96*i] = framebuffer[k+96*i] & ~(1<<j);
	    }

	big_y1++;
	if(big_y1 >= 8)
	    {
	    big_y1=0;
	    big_y++;
	    if(big_y >= 4)
		big_y=3;
	    }
	}
    }
}
/*}}}*/
//--------------------------------------------------
void set_ball(uint8_t x, uint8_t y)/*{{{*/
{
// big_y - rows into big_0
// big_y1 - dots into row in big_0
uint8_t big_y=0, big_y1=0;
uint8_t pixel=0;

for(uint8_t i=0; i<9; i++)
    { // rows
    for(uint8_t j=0; j<8; j++)
	{ // dots within rows
	if(j+8*i <  y)
	    continue;
	if(j+8*i >= y+ball_r)
	    continue;

	for(uint8_t k=0; k<96; k++)
	    { // columns
	    if(k <  x)
		continue;
	    if(k >= x+ball_r)
		continue;

	    // framebuffer is packed
	    pixel=pgm_read_byte(&ball[k-x + big_y*ball_r]);
	    if(pixel & (1<<big_y1))
		framebuffer[k+96*i] = framebuffer[k+96*i] |  (1<<j);
	    else
		framebuffer[k+96*i] = framebuffer[k+96*i] & ~(1<<j);
	    }

	big_y1++;
	if(big_y1 >= 8)
	    {
	    big_y1=0;
	    big_y++;
	    if(big_y >= 4)
		big_y=3;
	    }
	}
    }
}
/*}}}*/
//--------------------------------------------------
void show_service(uint8_t x, uint8_t y)/*{{{*/
{
// big_y - rows into big_0
// big_y1 - dots into row in big_0
uint8_t big_y=0, big_y1=0;
uint8_t pixel=0;

for(uint8_t i=0; i<9; i++)
    { // rows
    for(uint8_t j=0; j<8; j++)
	{ // dots within rows
	if(j+8*i <  y)
	    continue;
	if(j+8*i >= y+service_h)
	    continue;

	for(uint8_t k=0; k<96; k++)
	    { // columns
	    if(k <  x)
		continue;
	    if(k >= x+service_w)
		continue;

	    // framebuffer is packed
	    pixel=pgm_read_byte(&service[k-x + big_y*service_w]);
	    if(pixel & (1<<big_y1))
		framebuffer[k+96*i] = framebuffer[k+96*i] |  (1<<j);
	    else
		framebuffer[k+96*i] = framebuffer[k+96*i] & ~(1<<j);
	    }

	big_y1++;
	if(big_y1 >= 8)
	    {
	    big_y1=0;
	    big_y++;
	    if(big_y >= 4)
		big_y=3;
	    }
	}
    }
}
/*}}}*/
//--------------------------------------------------
void show_bluetooth(uint8_t x, uint8_t y)/*{{{*/
{
// big_y - rows into big_0
// big_y1 - dots into row in big_0
uint8_t big_y=0, big_y1=0;
uint8_t pixel=0;

for(uint8_t i=0; i<9; i++)
    { // rows
    for(uint8_t j=0; j<8; j++)
	{ // dots within rows
	if(j+8*i <  y)
	    continue;
	if(j+8*i >= y+blue_h)
	    continue;

	for(uint8_t k=0; k<96; k++)
	    { // columns
	    if(k <  x)
		continue;
	    if(k >= x+blue_w)
		continue;

	    // framebuffer is packed
	    pixel=pgm_read_byte(&bluetooth[k-x + big_y*blue_w]);
	    if(pixel & (1<<big_y1))
		framebuffer[k+96*i] = framebuffer[k+96*i] |  (1<<j);
	    else
		framebuffer[k+96*i] = framebuffer[k+96*i] & ~(1<<j);
	    }

	big_y1++;
	if(big_y1 >= 8)
	    {
	    big_y1=0;
	    big_y++;
	    if(big_y >= 4)
		big_y=3;
	    }
	}
    }
}
/*}}}*/
//--------------------------------------------------
void erase(uint8_t x, uint8_t y, uint8_t w, uint8_t h)/*{{{*/
{
// blank_y - rows into blank_0
// blank_y1 - dots into row in blank_0
uint8_t blank_y=0, blank_y1=0;

for(uint8_t i=0; i<9; i++)
    { // rows
    for(uint8_t j=0; j<8; j++)
	{ // dots within rows
	if(j+8*i <  y)
	    continue;
	if(j+8*i >= y+h)
	    continue;

	for(uint8_t k=x; k<x+w; k++)
	    { framebuffer[k+96*i] = framebuffer[k+96*i] & ~(1<<j); }

	blank_y1++;
	if(blank_y1 >= 8)
	    {
	    blank_y1=0;
	    blank_y++;
	    if(blank_y >= 4)
		blank_y=3;
	    }
	}
    }
}
/*}}}*/
//--------------------------------------------------
