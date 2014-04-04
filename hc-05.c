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
volatile char hc05_buff[HC_BUFF_SIZE];
volatile uint8_t buff_fill, hc05_i;

//--------------------------------------------------
void hc05_init(void)/*{{{*/
{
uint8_t i=0;
for(i=0; i<HC_BUFF_SIZE; i++)
    hc05_buff[i]=0;
buff_fill=0;
hc05_i=0;

// pins
DDRD |= (1<<DDD3) | (1<<DDD5) | (1<<DDD7);
PORTD |= (1<<PD2) | (1<<PD3) | (1<<PD7);

// uart 38400 8N1
UBRR1L = 12;
UBRR1H = 0;

UCSR1C = (1<<UCSZ10) | (1<<UCSZ11);
UCSR1B = (1<<TXEN1)  | (1<<RXEN1)  | (1<<RXCIE1);

// reset interrupts
UCSR1A |= (1<<RXC1);

HC_COMM_MODE;

// init proc
HC_SET_RESET;
_delay_ms(10);
HC_CLR_RESET;
_delay_ms(10);

HC_AT_MODE;
hc05_send("AT\r\n");
_delay_ms(600);

hc05_send("AT\r\n");
hc05_recv();
if(hc05_buff[0] != 'O' ||
   hc05_buff[1] != 'K')
    HALT;

hc05_send("AT+VERSION?\r\n");
hc05_recv();
hc05_recv();

hc05_send("AT+INIT\r\n");
hc05_recv();

hc05_send("AT+NAME=\"Scoreboard\"\r\n");
hc05_recv();

hc05_send("AT+RMAAD\r\n");
hc05_recv();

hc05_send("AT+UART=38400,0,0\r\n");
hc05_recv();

hc05_send("AT+ROLE=1\r\n");
hc05_recv();

hc05_send("AT+CMODE=1\r\n");
hc05_recv();

hc05_send("AT+IAC=9e8b33\r\n");
hc05_recv();

hc05_send("AT+CLASS=0\r\n");
hc05_recv();

hc05_send("AT+INQM=1,5,48\r\n");
hc05_recv();

HC_COMM_MODE;
_delay_ms(10);

hc05_send("AT+STATE?\r\n");
hc05_recv();
hc05_recv();

// pc
//hc05_send("AT+BIND=001b,10,002624\r\n");
// E61
//hc05_send("AT+BIND=0012,d1,0bb16a\r\n");
// galaxy s
hc05_send("AT+BIND=0cdf,a4,78e7a4\r\n");
hc05_recv();

hc05_send("AT+BIND?\r\n");
hc05_recv();
hc05_recv();

hc05_send("AT+STATE?\r\n");
hc05_recv();
hc05_recv();

// pc
//hc05_send("AT+PAIR=001b,10,002624,20\r\n");
// E61
//hc05_send("AT+PAIR=0012,d1,0bb16a,20\r\n");
// galaxy s
hc05_send("AT+PAIR=0cdf,a4,78e7a4,20\r\n");
hc05_recv();
}
/*}}}*/
//--------------------------------------------------
void hc05_send(char *msg)/*{{{*/
{
uint8_t i=0;

buff_fill=0;
hc05_i=0;
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
while(~UCSR1A & (1<<TXC1)) {};
}
/*}}}*/
//--------------------------------------------------
void hc05_recv(void)/*{{{*/
{
uint8_t rcv;

UCSR1A |= (1<<RXC1);
while(1)
    {
    while(~UCSR1A & (1<<RXC1)) {};

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
}
/*}}}*/
//--------------------------------------------------
uint8_t hc05_rcv_complete(void)/*{{{*/
{
if(buff_fill != 0)
    return 1;

return 0;
}
/*}}}*/
//--------------------------------------------------
ISR(USART1_RX_vect)/*{{{*/
{
char rcv=UDR1;
hc05_buff[hc05_i]=rcv;

hc05_i++;
if(rcv == '\n' ||
   rcv == '\r')
    {
    buff_fill=hc05_i;
    hc05_i=0;
    }
}
/*}}}*/
//--------------------------------------------------
