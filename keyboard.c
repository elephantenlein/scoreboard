//--------------------------------------------------
// Модуль опроса клавиатуры
//--------------------------------------------------
#include "globals.h"
#include "keyboard.h"

//--------------------------------------------------
#define CHECK_A_UP (~PINF & (1<<PF4))/*{{{*/
#define CHECK_A_DN (~PINF & (1<<PF5))
#define CHECK_B_UP (~PINF & (1<<PF3))
#define CHECK_B_DN (~PINF & (1<<PF6))
#define CHECK_SHIFT (~PINB & (1<<PB6))
#define CHECK_RESET (~PINB & (1<<PB5))
/*}}}*/
//--------------------------------------------------
enum _key_states {/*{{{*/
    IDLE,
    PRESSED,
};
/*}}}*/
struct _key {/*{{{*/
    uint8_t state;
    uint8_t timer;
};
/*}}}*/
//--------------------------------------------------
const uint16_t keyboard_poll_interval=1; // ~60 Гц
const uint16_t debounce_interval=3;

//--------------------------------------------------
volatile struct _key keyboard[KEY_COUNT];

volatile uint8_t  active_keys;
volatile uint16_t keyboard_poll;

//--------------------------------------------------
static uint8_t check_key(enum _keys);

//--------------------------------------------------
void keyboard_init(void)/*{{{*/
{
for(uint8_t i=0; i<KEY_COUNT; i++)
    {
    keyboard[i].state=IDLE;
    keyboard[i].timer=0;
    }

active_keys=0;
keyboard_poll=0;

// pins
PORTB |= (1<<PB5) | (1<<PB6);
PORTF |= (1<<PF3) | (1<<PF4) | (1<<PF5) | (1<<PF6);

// setup timer0 for power save mode
TCNT0  = 0x00;
TIFR  |= (1<<OCF0);
OCR0   = 0x7F;
TCCR0  = (1<<WGM01) | (1<<CS02) | (1<<CS01) | (1<<CS00);
TIMSK |= (1<<OCIE0);
}
/*}}}*/
//--------------------------------------------------
uint8_t get_keys(void)/*{{{*/
{
return active_keys;
}
/*}}}*/
//--------------------------------------------------
ISR(TIMER0_COMP_vect)/*{{{*/
{
keyboard_poll++;
if(keyboard_poll < keyboard_poll_interval)
    return;

for(uint8_t i=0; i<KEY_COUNT; i++)
    {
    switch(keyboard[i].state)
	{
	case IDLE:
	    if( check_key(i) )
		keyboard[i].timer++;
	    else
		keyboard[i].timer=0;

	    if(keyboard[i].timer > debounce_interval)
		{
		keyboard[i].timer=0;
		keyboard[i].state=PRESSED;
		active_keys |= (1<<i);
		}
	    break;

	case PRESSED:
	    if( !check_key(i) )
		keyboard[i].timer++;
	    else
		keyboard[i].timer=0;

	    if(keyboard[i].timer > debounce_interval)
		{
		keyboard[i].timer=0;
		keyboard[i].state=IDLE;
		active_keys &=~(1<<i);
		}
	    break;
	};
    }
}
/*}}}*/
//--------------------------------------------------
static uint8_t check_key(enum _keys k)/*{{{*/
{
switch(k)
    {
    case A_UP: return CHECK_A_UP;
    case A_DN: return CHECK_A_DN;
    case B_UP: return CHECK_B_UP;
    case B_DN: return CHECK_B_DN;

    case SHIFT: return CHECK_SHIFT;
    case RESET: return CHECK_RESET;

    default: return 0;
    };
}
/*}}}*/
//--------------------------------------------------
