#include "photocell.h"
void TPM1_IRQHandler(void);
void PORTB_IRQHandler(void);

uint16_t time=0;
uint8_t F1_first=0;
uint8_t F1_broken=0;
uint8_t F2_broken=0;
uint8_t test=0;
static uint16_t totaltime=0;			// czas okreslajacy jak dlugo byla przerwana fotokomorka F1 (totaltime=stop-start)
static uint16_t start = 0;				// Wykrycie przerwania z F1 start=time 
static uint16_t stop = 0;					// Wykrycie nastepnego przerwania z F1 stop=time 


void Photocell_Init(void){
	PORTB->PCR[F1] |= PORT_PCR_MUX(1);  //MUX funkcja przerwania IRQ
	PORTB->PCR[F2] |= PORT_PCR_MUX(1); 	
	
	PORTB->PCR[F1] |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;	// Dolaczenie rezystora pull-up i podpiecie go do VCC
	PORTB->PCR[F2] |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;

	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;				// Turning on port B
	PORTB -> PCR[F1] |= PORT_PCR_IRQC(0xb);		//0xb - both edge
	PORTB -> PCR[F2] |= PORT_PCR_IRQC(0xa);		//0xa - falling edge
	NVIC_ClearPendingIRQ(PORTB_IRQn);
	
	}


void PORTB_IRQHandler(void)						// Subprogram for handling F1 i F2
{
	uint32_t buf;
	buf=PORTB->ISFR & (F1_MASK | F2_MASK);
switch(buf)
	{	
	case F1_MASK:												// Przerwanie F1 
				if(F1_broken==0){							// Przerwanie zbocze opadajace (pojazd przerywa fotokomorke)
						start=time;								// Start pomiaru czasu
						F1_first=1;								// Sygnalizacja przerwania do wykrycia, ktora fotokomorka byla przerwana jako pierwsza
						F1_broken=1;							// Sygnalizacja przerwania fotokomorki F1
				}
				else{													// Przerwanie zbocze narastajace (pojazd nie przerywa fotokomorki)
					stop=time;									// Koniec pomiaru czasu
					totaltime=stop-start;				// Wyznaczenie czasu przerwania
					if(totaltime>10){   				// Jesli czas byl wystarczajaco dlugi, zeby zaliczyc przerwanie jako pojazd to nastepuje 
						test=1;										// Predkosc max 10km/h (2,77m/s) srednia dlugosc samochodu czas ok. 1,66s 
					}														// Przyjeto poprawny czas jako wiekszy od 1s (time=1 to 100ms) 
				F1_broken=0;									// Sygnalizacja braku przerwania fotokomorki F1
				}																		
				break;
	case F2_MASK: 

					F2_broken=1;								// Sygnalizacja przerwania fotokomorki F2
					
				break;
	default: break;
						}					
	PORTB->ISFR |=  F1_MASK | F2_MASK;	// Deleton of all bits· ISF
	NVIC_ClearPendingIRQ(PORTB_IRQn);
}
