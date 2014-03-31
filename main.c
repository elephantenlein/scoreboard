//--------------------------------------------------
#include "globals.h"
#include "ste2007.h"
#include "symbols.h"
#include "display.h"
#include "keyboard.h"

#include <avr/sleep.h>

//--------------------------------------------------
enum _cmd {/*{{{*/
    CMD_IDLE=0,
    CMD_RESET,

    CMD_A_UP,
    CMD_A_DN,
    CMD_B_UP,
    CMD_B_DN,
    CMD_GA_UP,
    CMD_GA_DN,
    CMD_GB_UP,
    CMD_GB_DN
};
/*}}}*/
//--------------------------------------------------
static void master_init(void);
static void go_to_sleep(void);
static enum _cmd get_command(void);

//--------------------------------------------------
int main(void)
{
master_init();
ste_init();
keyboard_init();
sei();

small_dots();
set_curr_count(0,0);
set_grand_count(0,0);
show_service(14, 4);
set_serve(SERVE_START);
ste_render();

enum _cmd command=CMD_IDLE, old_command=CMD_IDLE;
enum _serve serve=SERVE_START;
uint8_t a=0, b=0, ga=0, gb=0;
uint8_t state=0;
while(1)
    {
    go_to_sleep();
    command=get_command();

    if(command == old_command)
	continue;
    old_command=command;

    if(command == CMD_IDLE)
	continue;

    if(command == CMD_RESET)
	{
	a=b=ga=gb=0;
	state=0;
	serve=SERVE_START;
	set_curr_count(a, b);
	set_grand_count(ga, gb);
	show_service(14, 4);
	set_serve(SERVE_START);
	ste_render();
	continue;
	}

    switch(state)
	{
	default:
	case 0:
	    if(command == CMD_A_UP || command == CMD_A_DN)
		{
		erase(14,4,66,12);
		serve=SERVE_A1;
		a=b=0;
		state=1;
		}
	    else if(command == CMD_B_UP || command == CMD_B_DN)
		{
		erase(14,4,66,12);
		serve=SERVE_B1;
		a=b=0;
		state=1;
		}
	    break;

	case 1:
	    if(command == CMD_A_UP)
		{
		a++;
		serve=serve_next(serve);
		}
	    if(command == CMD_A_DN)
		{
		if(a > 0)
		    a--;
		serve=serve_prev(serve);
		}
	    if(command == CMD_B_UP)
		{
		b++;
		serve=serve_next(serve);
		}
	    if(command == CMD_B_DN)
		{
		if(b > 0)
		    b--;
		serve=serve_prev(serve);
		}

	    if(a == 11 && b < 10)
		{
		ga++;
		a=b=0;
		state=0;
		serve=SERVE_START;
		show_service(14, 4);
		}

	    if(b == 11 && a < 10)
		{
		gb++;
		a=b=0;
		serve=SERVE_START;
		show_service(14, 4);
		state=0;
		}

	    if(a == b && b == 10)
		{
		if(serve == SERVE_A2)
		    serve=SERVE_B1;
		else if(serve == SERVE_B2)
		    serve=SERVE_A1;

		state=2;
		}
	    break;

	case 2: // tie-brake
	    if(command == CMD_A_UP)
		{
		a++;
		if(serve == SERVE_A1)
		    serve=SERVE_B1;
		else if(serve == SERVE_B1)
		    serve=SERVE_A1;
		}
	    else if(command == CMD_B_UP)
		{
		b++;
		if(serve == SERVE_A1)
		    serve=SERVE_B1;
		else if(serve == SERVE_B1)
		    serve=SERVE_A1;
		}

	    if(a > b && a-b > 1)
		{
		ga++;
		a=b=0;
		state=0;
		serve=SERVE_START;
		show_service(14, 4);
		}

	    if(b > a && b-a > 1)
		{
		gb++;
		a=b=0;
		state=0;
		serve=SERVE_START;
		show_service(14, 4);
		}
	    break;
	};

    set_curr_count(a, b);
    set_grand_count(ga, gb);
    set_serve(serve);
    ste_render();
    };
}

//--------------------------------------------------
static void master_init(void)/*{{{*/
{
MCUCR |= (1<<SM0);
}
/*}}}*/
//--------------------------------------------------
static void go_to_sleep(void)/*{{{*/
{
sleep_enable();
sleep_cpu();
sleep_disable();
}
/*}}}*/
//--------------------------------------------------
static enum _cmd get_command(void)/*{{{*/
{
uint8_t keys=0;
uint8_t state=0;
uint8_t cmd=CMD_IDLE;
keys=get_keys();
switch(state)
    {
    default:
    case 0:
	if(keys == (1<<SHIFT))
	    state=2;

	if(keys == (1<<RESET))
	    cmd=CMD_RESET;

	if(keys == (1<<A_UP))
	    {
	    cmd=CMD_A_UP;
	    state=1;
	    }
	if(keys == (1<<A_DN))
	    {
	    cmd=CMD_A_DN;
	    state=1;
	    }
	if(keys == (1<<B_UP))
	    {
	    cmd=CMD_B_UP;
	    state=1;
	    }
	if(keys == (1<<B_DN))
	    {
	    cmd=CMD_B_DN;
	    state=1;
	    }

	if(keys == 0)
	    {
	    cmd=CMD_IDLE;
	    state=0;
	    }
	break;

    case 1:
	if(keys == 0)
	    state=0;
	break;

    case 2:
	if(keys == ((1<<SHIFT) | (1<<A_UP)) )
	    {
	    cmd=CMD_GA_UP;
	    state=3;
	    }
	if(keys == ((1<<SHIFT) | (1<<A_DN)) )
	    {
	    cmd=CMD_GA_DN;
	    state=3;
	    }
	if(keys == ((1<<SHIFT) | (1<<B_UP)) )
	    {
	    cmd=CMD_GB_UP;
	    state=3;
	    }
	if(keys == ((1<<SHIFT) | (1<<B_DN)) )
	    {
	    cmd=CMD_GB_DN;
	    state=3;
	    }

	if(keys == 0)
	    {
	    cmd=CMD_IDLE;
	    state=0;
	    }
	break;

    case 3:
	if(~keys & ((1<<A_UP) | (1<<A_DN) | (1<<B_UP) | (1<<B_DN)))
	    {
	    cmd=CMD_IDLE;
	    state=0;
	    }
	break;
    };

return cmd;
}
/*}}}*/
//--------------------------------------------------
