//--------------------------------------------------
// Буфер кадра для дисплея Nokia 1202 на базе
// контроллера ste2007
//--------------------------------------------------
#include "globals.h"
#include "ste2007.h"

#include <string.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

//--------------------------------------------------
char framebuffer[96*9];

//--------------------------------------------------
static void send_byte(uint8_t cd, uint8_t byte);

//--------------------------------------------------
void ste_init()/*{{{*/
{
// lcd control lines
PORTB |= (1<<PB1) | (1<<PB2);
PORTE |= (1<<PE2) | (1<<PE3);

DDRB |= (1<<PB1) | (1<<PB2) | (1<<PB4);
DDRE |= (1<<PE2) | (1<<PE3);

// turn led off
PORTB &= ~(1<<PB4);

// drop clock
PORTB &= ~(1<<PB1);

// reset lcd
_delay_ms(100);
PORTE &= ~(1<<PE2);
_delay_us(3);
PORTE |=  (1<<PE2);
_delay_us(3);

// init sequence
send_byte(0x00, 0x20);
send_byte(0x00, 0x90);
send_byte(0x00, 0xAF);
send_byte(0x00, 0xA4);
send_byte(0x00, 0x2F);

// futher initialization
_delay_ms(100);
send_byte(0x00, 0xA6);
send_byte(0x00, 0xB0);
send_byte(0x00, 0x10);
send_byte(0x00, 0x00);
send_byte(0x00, 0x40);

// orientate the device
send_byte(0x00, 0xA1);
send_byte(0x00, 0xC8);
}
/*}}}*/
//--------------------------------------------------
static void send_byte(uint8_t cd, uint8_t byte)/*{{{*/
{
// drop cs to start communication
PORTE &= ~(1<<PE3);

// command/data
if(cd)
    PORTB |=  (1<<PB2);
else
    PORTB &= ~(1<<PB2);

// clock
PORTB |=  (1<<PB1);

// data
for(uint8_t i=0; i<8; i++)
    {
    PORTB &= ~(1<<PB1);

    // data
    if(byte & 0x80)
	PORTB |=  (1<<PB2);
    else
	PORTB &= ~(1<<PB2);

    // clock
    PORTB |=  (1<<PB1);
    byte = byte << 1;
    }

// clock
PORTB &= ~(1<<PB1);

// rise cs to stop communication
PORTE |=  (1<<PE3);
}
/*}}}*/
//--------------------------------------------------
void ste_pack(const char *src)/*{{{*/
{
for(uint8_t k=0; k<8; k++)
    { // lines of 8 pixels
    for(uint8_t j=0; j<8; j++)
	{ // pixels within line
	for(uint8_t i=0, l=0; i<97; i++)
	    { // all columns
	    switch(pgm_read_byte(&src[i+97*j+97*8*k]))
		{
		case '.': // space
		    framebuffer[l+96*k] &= ~(1<<j);
		    l++;
		    break;
		case '+': // mark
		    framebuffer[l+96*k] |=  (1<<j);
		    l++;
		    break;
		default:
		    break;
		};
	    }
	}
    }

// last line is 4 pixels
for(uint8_t j=0; j<4; j++)
    { // pixels within line
    for(uint8_t i=0, l=0; i<97; i++)
	{ // all columns
	switch(pgm_read_byte(&src[i+97*j+97*8*8]))
	    {
	    case '.': // space
		framebuffer[l+96*8] &= ~(1<<j);
		l++;
		break;
	    case '+': // mark
		framebuffer[l+96*8] |=  (1<<j);
		l++;
		break;
	    default:
		break;
	    };
	}
    }
}
/*}}}*/
//--------------------------------------------------
void ste_render()/*{{{*/
{
for(uint16_t i=0; i<96*9; i++)
    send_byte(0x01, framebuffer[i]);
}
/*}}}*/
//--------------------------------------------------
void ste_light(uint8_t on)/*{{{*/
{
if(on != 0)
    PORTB |= (1<<PB4);
else
    PORTB &= ~(1<<PB4);
}
/*}}}*/
//--------------------------------------------------
