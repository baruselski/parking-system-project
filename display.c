#include "display.h"

void display_init(void){
		SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;					// Turning on port B
		PORTB->PCR[a] |= PORT_PCR_MUX(1);						// PIN MUX as PTBX
		PORTB->PCR[b] |= PORT_PCR_MUX(1);
		PORTB->PCR[c] |= PORT_PCR_MUX(1);
		PORTB->PCR[d] |= PORT_PCR_MUX(1);
		PORTB->PCR[e] |= PORT_PCR_MUX(1);
		PORTB->PCR[f] |= PORT_PCR_MUX(1);
		PORTB->PCR[g] |= PORT_PCR_MUX(1);
	
		PORTA->PCR[digit1] |= PORT_PCR_MUX(1);			// PIN MUX as PTAX
		PORTA->PCR[digit2] |= PORT_PCR_MUX(1);
		
		PTB->PDDR |= (1<<a)|(1<<b)|(1<<c)|(1<<d)|(1<<e)|(1<<f)|(1<<g);	// Set as outputs
		PTA->PDDR |= (1<<digit1)|(1<<digit2);														// Ustawienie jako wyjscia
}

void display_func(int n,int seg){
		if(seg==1){								// Choose seg=1 number of ones
			PTB->PDOR |= (1<<a)|(1<<b)|(1<<c)|(1<<d)|(1<<e)|(1<<f)|(1<<g);		// Clean the display
			PTA->PCOR |= (1<<digit2);																					// Wylaczenie cyfry dziesiatek
			PTA->PDOR |= (1<<digit1);																					// Wlaczenie cyfry jednosci
		}
		else if(seg==2){							// Choose seg=2 number of tens
			PTB->PDOR |= (1<<a)|(1<<b)|(1<<c)|(1<<d)|(1<<e)|(1<<f)|(1<<g);		// Clean the display
			PTA->PCOR |= (1<<digit1);																					// Wylaczenie cyfry jednosci
			PTA->PDOR |= (1<<digit2);																					// Wlaczenie cyfry dziesiatek
		}
	
		switch(n){								// Choose segments for each number
			
			case 0: PTB->PCOR |= (1<<a)|(1<<b)|(1<<c)|(1<<d)|(1<<e)|(1<<f); break;
			case 1: PTB->PCOR |= (1<<b)|(1<<c); break;
			case 2: PTB->PCOR |= (1<<a)|(1<<b)|(1<<d)|(1<<e)|(1<<g); break;
			case 3: PTB->PCOR |= (1<<a)|(1<<b)|(1<<c)|(1<<d)|(1<<g); break;
			case 4: PTB->PCOR |= (1<<b)|(1<<c)|(1<<f)|(1<<g); break;
			case 5: PTB->PCOR |= (1<<a)|(1<<c)|(1<<d)|(1<<f)|(1<<g); break;
			case 6: PTB->PCOR |= (1<<a)|(1<<c)|(1<<d)|(1<<e)|(1<<f)|(1<<g); break;
			case 7: PTB->PCOR |= (1<<a)|(1<<b)|(1<<c); break;
			case 8: PTB->PCOR |= (1<<a)|(1<<b)|(1<<c)|(1<<d)|(1<<e)|(1<<f)|(1<<g); break;
			case 9: PTB->PCOR |= (1<<a)|(1<<b)|(1<<c)|(1<<d)|(1<<f)|(1<<g); break;
			default: PTB->PCOR |= (1<<g); break;
	}
	
	
}
