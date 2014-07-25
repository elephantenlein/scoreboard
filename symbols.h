//--------------------------------------------------
// Знакогенератор для пульта управления
// табло
//--------------------------------------------------
#ifndef SYMBOLS_H
#define SYMBOLS_H

//--------------------------------------------------
extern void small_dots(void);
extern void small_digit(uint8_t,uint8_t,uint8_t);
extern void big_digit(uint8_t,uint8_t,uint8_t);
extern void set_ball(uint8_t,uint8_t);
extern void show_service(uint8_t,uint8_t);
extern void show_bluetooth(uint8_t, uint8_t);
extern void erase(uint8_t,uint8_t,uint8_t,uint8_t);

#endif
//--------------------------------------------------
