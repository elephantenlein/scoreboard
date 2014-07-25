//--------------------------------------------------
// Буфер кадра для дисплея Nokia 1202 на базе
// контроллера ste2007
//--------------------------------------------------
#ifndef STE_2007
#define STE_2007

// 96x68 pixels, 96x9 packed
//--------------------------------------------------
extern char framebuffer[96*9];

//--------------------------------------------------
extern void ste_init(void);
extern void ste_pack(const char *);
extern void ste_render(void);
extern void ste_light(uint8_t);

#endif
//--------------------------------------------------
