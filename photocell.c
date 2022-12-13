#include "photocell.h"
void TPM1_IRQHandler(void);
void PORTB_IRQHandler(void);

uint16_t time=0;
uint16_t totaltime=0;
uint8_t F1_broken=0;
uint8_t F2_broken=0;
uint8_t test=0;
static uint16_t start = 0;
static uint16_t stop = 0;
volatile uint8_t tryb = 0;


void Photocell_Init(void){
	PORTB->PCR[F1] |= PORT_PCR_MUX(1);  //IRQ
	PORTB->PCR[F2] |= PORT_PCR_MUX(1); 	//IRQ
	
	PORTB->PCR[F1] |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;
	PORTB->PCR[F2] |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;
/*	
	PORTB->PCR[F1] |= PORT_PCR_PE_MASK;  //Rezystor podciagajacy
	PORTB->PCR[F1] &= ~PORT_PCR_PS_MASK;	// Podpiety do gnd
	PORTB->PCR[F2] |= PORT_PCR_PE_MASK;
	PORTB->PCR[F2] &= ~PORT_PCR_PS_MASK;
*/
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
	PORTB -> PCR[F1] |= PORT_PCR_IRQC(0xb);		//0xb - both edge
	PORTB -> PCR[F2] |= PORT_PCR_IRQC(0xa);		//0xa - falling edge
	NVIC_ClearPendingIRQ(PORTB_IRQn);
//	NVIC_EnableIRQ(PORTB_IRQn);
	
	}


void PORTB_IRQHandler(void)	
{
	uint32_t buf;
	buf=PORTB->ISFR & (F1_MASK | F2_MASK);
switch(buf)
	{	
	case F1_MASK:
				if(F1_broken==0){
						start=time;
						F1_broken=1;
				}
				else{
					stop=time;
					totaltime=stop-start;
					if(totaltime>14){   // Predkosc max 10km/h (2,77m/s) srednia dlugosc samochodu czas ok. 1,66s 
						test=1;
						F1_broken=0;
					}					
				}																		
				break;
	case F2_MASK: 
		
			if(F2_broken){
					F2_broken =0;
							}
			else{
					F2_broken=1;
							}	
				break;
	default: break;
						}					
	PORTB->ISFR |=  F1_MASK | F2_MASK;	// Deleton of all bits· ‰SF
	NVIC_ClearPendingIRQ(PORTB_IRQn);
}
