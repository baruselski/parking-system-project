#include "MKL05Z4.h"

#define F1 3								// bit number for F1 PORTB
#define F2 4								// bit number for F2 PORTB
#define F1_MASK	(1<<3)			// Mask for F1
#define F2_MASK	(1<<4)			// Mask for F2

// Zmienne globalne
extern uint16_t time;					// Czas dla funkcji SyStick
extern uint8_t F1_first;			// Zmienna sygnalizacyjna wykorzystywana do wykrycia, ktora fotokomorka byla przerwana jako pierwsza
extern uint8_t F1_broken;			// Sygnalizacja przerwania fotokomorki F1
extern uint8_t F2_broken;			// Sygnalizacja przerwania fotokomorki F2
extern uint8_t test;					// Sygnalizacja wykrycia pojazdu (test=1 gdy przerwanie fotokomorki F1 jest dluzsze od 1s)

void Photocell_Init(void);		// Inicjalizacja pinow i przerwania od pinow

