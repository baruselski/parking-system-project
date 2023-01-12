#include "photocell.h"
void TPM1_IRQHandler(void);
void PORTB_IRQHandler(void);

uint16_t time=0;
uint8_t F1_first=0;
uint8_t F1_broken=0;
uint8_t F2_broken=0;
uint8_t test=0;
static uint16_t totaltime=0;			// variable for checking how long was F1 interrupted (totaltime=stop-start)
static uint16_t start = 0;				// Interrupt detection with F1 start=time 
static uint16_t stop = 0;					// Second interrupt detecion with F1 stop=time 


void Photocell_Init(void){
	PORTB->PCR[F1] |= PORT_PCR_MUX(1);  //MUX interrupt function IRQ
	PORTB->PCR[F2] |= PORT_PCR_MUX(1); 	
	
	PORTB->PCR[F1] |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;	// Attaching a pull-up resistor and connecting it to VCC
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
	case F1_MASK:												// F1 Interrupt
				if(F1_broken==0){							// Interrupt on falling edge (vehicle interrupts the photocell)
						start=time;								// Start measuring time
						F1_first=1;								// Sygnalize interrupt to determine which photocell was interrupted first
						F1_broken=1;							// Sygnalize F1 interrupt
				}
				else{													// Interrupt on rising edge (vehicle does not interrupt the photocell)
					stop=time;									// Stop measuring time
					totaltime=stop-start;				// Determine time of the interrupt
					if(totaltime>10){   				// If interrupt time was long enough, to count it as a vehicle
						test=1;										// Max speed 10km/h (2,77m/s) average car length, time around 1,66s 
					}														// We established correct time higher than 1s(time=1 is 100ms) 
				F1_broken=0;									// Sygnalize lack of interrupt from F1
				}																		
				break;
	case F2_MASK: 

					F2_broken=1;								// Sygnalize interrupt of F2
					
				break;
	default: break;
						}					
	PORTB->ISFR |=  F1_MASK | F2_MASK;	// Deleton of all bitsˇ ISF
	NVIC_ClearPendingIRQ(PORTB_IRQn);
}
