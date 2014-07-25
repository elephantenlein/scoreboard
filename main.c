//--------------------------------------------------
#include "globals.h"
#include "ste2007.h"
#include "symbols.h"
#include "display.h"
#include "keyboard.h"
#include "hc-05.h"

#include "debug.h"

#include <avr/sleep.h>

//--------------------------------------------------
enum _cmd {/*{{{*/
    CMD_IDLE=0,
    CMD_RESET,
    CMD_LIGHT,

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
volatile enum hc05_state bluetooth_ok;

//--------------------------------------------------
static void master_init(void);
static enum _cmd get_command(void);
static void submit_score(uint8_t, uint8_t, uint8_t, uint8_t);

//--------------------------------------------------
int main(void)
{
DBG_INIT;
DBG1_CLR;

master_init();
ste_init();
keyboard_init();
bluetooth_ok=hc05_init();
sei();

small_dots();
submit_score(0, 0, 0, 0);
show_service(14, 4);
set_serve(SERVE_START);
if(bluetooth_ok == HC05_PAIRED)
    show_bluetooth(88, 1);
ste_render();

enum _cmd command=CMD_IDLE, old_command=CMD_IDLE;
enum _serve serve=SERVE_START;
int8_t a=0, b=0, ga=0, gb=0;
uint8_t state=0, lights=0;
while(1)
    {
    bluetooth_ok=hc05_task();
    if(bluetooth_ok == HC05_PAIRED)
	show_bluetooth(88, 1);
    command=get_command();

    if(command == old_command)/*{{{*/
	continue;
    old_command=command;

    if(command == CMD_IDLE)
	continue;
/*}}}*/
    if(command == CMD_RESET)/*{{{*/
	{
	a=b=ga=gb=0;
	state=0;
	serve=SERVE_START;
	submit_score(a, b, ga, gb);
	show_service(14, 4);
	set_serve(SERVE_START);
	ste_render();
	continue;
	}
/*}}}*/
    if(command == CMD_LIGHT)/*{{{*/
	{
	if(lights != 0)
	    {
	    ste_light(0);
	    lights=0;
	    }
	else
	    {
	    ste_light(1);
	    lights=1;
	    }
	continue;
	}
/*}}}*/
    if(command == CMD_GA_UP)/*{{{*/
	{
	ga++;
	if(ga > 9)
	    ga=9;
	set_grand_count(ga, gb);
	ste_render();
	continue;
	}

    if(command == CMD_GB_UP)
	{
	gb++;
	if(gb > 9)
	    gb=9;
	set_grand_count(ga, gb);
	ste_render();
	continue;
	}

    if(command == CMD_GA_DN)
	{
	ga--;
	if(ga < 0)
	    ga=0;
	set_grand_count(ga, gb);
	ste_render();
	continue;
	}

    if(command == CMD_GB_DN)
	{
	gb--;
	if(gb < 0)
	    gb=0;
	set_grand_count(ga, gb);
	ste_render();
	continue;
	}
/*}}}*/
    switch(state)
	{
	default:
	case 0:
	    if(command == CMD_A_UP || command == CMD_A_DN)/*{{{*/
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
/*}}}*/
	case 1:
	    if(command == CMD_A_UP)/*{{{*/
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
/*}}}*/
	case 2: // tie-brake
	    if(command == CMD_A_UP)/*{{{*/
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
/*}}}*/
	};

    submit_score(a, b, ga, gb);
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
static enum _cmd get_command(void)/*{{{*/
{
static uint8_t state=0;

uint8_t keys=0;
uint8_t cmd=CMD_IDLE;

keys=get_keys();
switch(state)
    {
    default:
    case 0:
	if(keys == (1<<SHIFT))
	    state=2;

	if(keys == (1<<RESET))
	    {
	    cmd=CMD_RESET;
	    state=1;
	    }

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
	break;

    case 1:
	if(keys == 0)
	    state=0;
	break;

    case 2:
	if(keys == 0)
	    {
	    cmd=CMD_LIGHT;
	    state=0;
	    }

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
	break;

    case 3:
	if(keys == 0)
	    state=0;

	if( (keys & ((1<<A_UP) | (1<<A_DN) | (1<<B_UP) | (1<<B_DN))) == 0)
	    state=4;
	break;

    case 4:
	if(keys == 0)
	    state=0;

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
	break;
    };

return cmd;
}
/*}}}*/
//--------------------------------------------------
static void submit_score(uint8_t a, uint8_t b, uint8_t ga, uint8_t gb)/*{{{*/
{
char message[10];

set_curr_count(a, b);
set_grand_count(ga, gb);

message[0]=':';
message[1]=a/10 + '0';
message[2]=a%10 + '0';
message[3]=ga+'0';
message[4]=gb+'0';
message[5]=b/10 + '0';
message[6]=b%10 + '0';

message[7]='\r';
message[8]='\n';

// send via bluetooth
if(bluetooth_ok == HC05_PAIRED)
    hc05_send(message);
}
/*}}}*/
//--------------------------------------------------
