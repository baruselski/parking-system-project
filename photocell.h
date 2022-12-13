#include "MKL05Z4.h"

#define F1 3 //PORT B 
#define F2 4
#define F1_MASK	(1<<3)
#define F2_MASK	(1<<4)

extern uint16_t time;
extern uint16_t totaltime;
extern uint8_t F1_broken;
extern uint8_t F2_broken;
extern uint8_t test;

void Photocell_Init(void);
//void F1_init(void);
//void F2_init(void);
