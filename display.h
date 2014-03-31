//--------------------------------------------------
// Высокоуровневый модуль управления дисплеем
//--------------------------------------------------
#ifndef DISPLAY_H
#define DISPLAY_H

//--------------------------------------------------
#include <stdint.h>

//--------------------------------------------------
enum _serve {/*{{{*/
    SERVE_START,
    SERVE_A1,
    SERVE_A2,
    SERVE_B1,
    SERVE_B2
};
/*}}}*/
//--------------------------------------------------
extern void set_grand_count(uint8_t a, uint8_t b);
extern void set_curr_count(uint8_t a, uint8_t b);
extern void set_serve(enum _serve s);
extern enum _serve serve_next(enum _serve);
extern enum _serve serve_prev(enum _serve);

#endif
//--------------------------------------------------
