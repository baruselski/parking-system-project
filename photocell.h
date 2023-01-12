#include "MKL05Z4.h"

#define F1 3								// bit number for F1 PORTB
#define F2 4								// bit number for F2 PORTB
#define F1_MASK	(1<<3)			// Mask for F1
#define F2_MASK	(1<<4)			// Mask for F2

// Zmienne globalne
extern uint16_t time;					// time for SysTick 
extern uint8_t F1_first;			// Variable for determining which photocell was interrupt first
extern uint8_t F1_broken;			// Variable to sygnalize F1 interrupt
extern uint8_t F2_broken;			// Variable to sygnalize F2 interrupt
extern uint8_t test;					// Variable for checking if photocell interrupt was longer than 1s(test=1 if longer than 1s)

void Photocell_Init(void);		// Initialization
