//--------------------------------------------------
// Интерфейс к модулю HC-05
//--------------------------------------------------
#include "globals.h"
#include "hc-05.h"
#include "ste2007.h"
#include "debug.h"

#include <util/delay.h>

//--------------------------------------------------
#define HC_SET_RESET PORTD &= ~(1<<PD7)
#define HC_CLR_RESET PORTD |=  (1<<PD7)

#define HC_COMM_MODE PORTD &= ~(1<<PD5)
#define HC_AT_MODE   PORTD |=  (1<<PD5)

//--------------------------------------------------
const uint8_t max_at_try=5;
const uint8_t max_bind_try=3;

//--------------------------------------------------
static volatile enum hc05_state state;

//--------------------------------------------------
volatile char hc05_buff[HC_BUFF_SIZE];
volatile uint8_t buff_fill, hc05_i;

//--------------------------------------------------
enum hc05_state hc05_init(void)/*{{{*/
{
uint8_t i=0;
state=HC05_OFFLINE;

for(i=0; i<HC_BUFF_SIZE; i++)
    hc05_buff[i]=0;
buff_fill=0;
hc05_i=0;

// pins
DDRD |= (1<<DDD3) | (1<<DDD5) | (1<<DDD7);
PORTD |= (1<<PD2) | (1<<PD3) | (1<<PD7);

// start out at 38400 8N1
UBRR1L = 12;
UBRR1H = 0;

UCSR1B = (1<<TXEN1)  | (1<<RXEN1);
UCSR1C = (1<<UCSZ10) | (1<<UCSZ11);

_delay_ms(10);
HC_SET_RESET;
_delay_ms(10);
HC_CLR_RESET;
_delay_ms(1000);

HC_AT_MODE;
for(i=0; i<max_at_try; i++)/*{{{*/
    {
    _delay_ms(100);

    hc05_send("AT\r\n");
    if(!hc05_recv())
	{
	if(i % 2 == 0)
	    { // try 9600
	    UBRR1L = 51;
	    UBRR1H = 0;
	    }
	else
	    { // try 38400
	    UBRR1L = 12;
	    UBRR1H = 0;
	    }
	continue;
	}

    if(hc05_buff[0] == 'O' &&
       hc05_buff[1] == 'K')
	break;
    }

if(i >= max_at_try)
    return HC05_OFFLINE;
/*}}}*/
/*
hc05_send("AT+VERSION?\r\n");
if(!hc05_recv())
    return HC05_OFFLINE;
if(!hc05_recv())
    return HC05_OFFLINE;
    */

//DBG1_SET;
hc05_send("AT+UART=9600,0,0\r\n");
if(!hc05_recv())
    return HC05_OFFLINE;

hc05_send("AT+NAME=\"Scoreboard-remote\"\r\n");
if(!hc05_recv())
    return HC05_OFFLINE;

hc05_send("AT+RMAAD\r\n");
if(!hc05_recv())
    return HC05_OFFLINE;

hc05_send("AT+ROLE=1\r\n");
if(!hc05_recv())
    return HC05_OFFLINE;

hc05_send("AT+CMODE=1\r\n");
if(!hc05_recv())
    return HC05_OFFLINE;

// pc
//hc05_send("AT+BIND=001b,10,002624\r\n");
// E61
//hc05_send("AT+BIND=0012,d1,0bb16a\r\n");
// galaxy s
//hc05_send("AT+BIND=0cdf,a4,78e7a4\r\n");
// scoreboard-board
/*
for(i=0; i<max_bind_try; i++)
    {
    hc05_send("AT+BIND=0014,02,280224\r\n");
    if(!hc05_recv())
	return HC05_OFFLINE;
    if(hc05_recv())
	return HC05_PAIRED;
    }

hc05_send("AT+BIND?\r\n");
if(!hc05_recv())
    return HC05_OFFLINE;
if(!hc05_recv())
    return HC05_OFFLINE;
    */

// pc
//hc05_send("AT+PAIR=001b,10,002624,20\r\n");
// E61
//hc05_send("AT+PAIR=0012,d1,0bb16a,20\r\n");
// galaxy s
//hc05_send("AT+PAIR=0cdf,a4,78e7a4,20\r\n");
// scoreboard-board
/*
hc05_send("AT+PAIR=0014,02,280224\r\n");
if(!hc05_recv())
    return HC05_OFFLINE;

if(hc05_buff[0] == 'O')
    {
    HC_COMM_MODE;

    // switch to 9600 now
    UBRR1L = 51;
    UBRR1H = 0;
    return HC05_PAIRED;
    }
    */

// stay in HC_AT_MODE
state=HC05_NOT_PAIRED;
return HC05_NOT_PAIRED;
}
/*}}}*/
//--------------------------------------------------
enum hc05_state hc05_task(void)/*{{{*/
{
if(state == HC05_NOT_PAIRED)
    {
    hc05_send("AT+PAIR=0014,02,280224,1\r\n");
    hc05_recv();

    state=HC05_PAIRED;
    HC_COMM_MODE;
    }

return state;
}
/*}}}*/
//--------------------------------------------------
void hc05_send(char *msg)/*{{{*/
{
uint8_t i=0;

buff_fill=0;
do
    {
    while(~UCSR1A & (1<<UDRE1)) {};
    UDR1=msg[i];
    
    i++;
    if(msg[i] == '\0')
	break;
    }
while(1);

UCSR1A |= (1<<TXC1);
while(~UCSR1A & (1<<TXC1)) {}

hc05_i=0;
}
/*}}}*/
//--------------------------------------------------
void hc05_put(char c)/*{{{*/
{
while(~UCSR1A & (1<<UDRE1)) {};
UDR1=c;
}
/*}}}*/
//--------------------------------------------------
bool hc05_recv(void)/*{{{*/
{
const uint16_t max_to=1000;

uint8_t rcv;
uint16_t to;

UCSR1A |= (1<<RXC1);
while(1)
    {
    to=0;
    while(~UCSR1A & (1<<RXC1))
	{
	_delay_us(500);
	to++;
	if(to >= max_to)
	    break;
	};

    if(to >= max_to)
	break;

    rcv=UDR1;
    hc05_buff[hc05_i]=rcv;

    hc05_i++;
    if(hc05_i > HC_BUFF_SIZE)
	hc05_i=0;
    if(rcv == '\n')
	{
	buff_fill=hc05_i;
	break;
	}
    }

if(to >= max_to)
    return false;
else
    return true;
}
/*}}}*/
//--------------------------------------------------
