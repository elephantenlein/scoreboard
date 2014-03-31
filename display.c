//--------------------------------------------------
// Высокоуровневый модуль управления дисплеем
//--------------------------------------------------
#include "display.h"
#include "symbols.h"

//--------------------------------------------------
void set_grand_count(uint8_t a, uint8_t b)/*{{{*/
{
small_digit(a, 40, 21);
small_digit(b, 50, 21);
}
/*}}}*/
//--------------------------------------------------
void set_curr_count(uint8_t a, uint8_t b)/*{{{*/
{
uint8_t d;
if(a == b && b == 0)
    {
    big_digit(0,  0, 21);
    big_digit(0, 20, 21);
    big_digit(0, 59, 21);
    big_digit(0, 79, 21);
    return;
    }

d = a/10;
if(d != 0)
    big_digit(d,  0, 21);
d = a-10*d;
big_digit(d, 20, 21);

d = b/10;
if(d != 0)
    big_digit(d, 59, 21);
d = b-10*d;
big_digit(d, 79, 21);
}
/*}}}*/
//--------------------------------------------------
void set_serve(enum _serve s)/*{{{*/
{
erase( 1,58,10,10);
erase(13,58,10,10);
erase(74,58,10,10);
erase(86,58,10,10);
switch(s)
    {
    default:
    case SERVE_START:
	break;
    case SERVE_A1:
	set_ball( 1, 58);
	break;
    case SERVE_A2:
	set_ball( 1, 58);
	set_ball(13, 58);
	break;
    case SERVE_B1:
	set_ball(86, 58);
	break;
    case SERVE_B2:
	set_ball(74, 58);
	set_ball(86, 58);
	break;
    };
}
/*}}}*/
//--------------------------------------------------
enum _serve serve_next(enum _serve s)/*{{{*/
{
s++;
if(s > SERVE_B2)
    s=SERVE_A1;

return s;
}
/*}}}*/
//--------------------------------------------------
enum _serve serve_prev(enum _serve s)/*{{{*/
{
s--;
if(s < SERVE_A1)
    s=SERVE_B2;

return s;
}
/*}}}*/
//--------------------------------------------------
