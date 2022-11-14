#include "display.h"

void display_init(void){
		SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
		PORTB->PCR[a] |= PORT_PCR_MUX(1);
		PORTB->PCR[b] |= PORT_PCR_MUX(1);
		PORTB->PCR[c] |= PORT_PCR_MUX(1);
		PORTB->PCR[d] |= PORT_PCR_MUX(1);
		PORTB->PCR[e] |= PORT_PCR_MUX(1);
		PORTB->PCR[f] |= PORT_PCR_MUX(1);
		PORTB->PCR[g] |= PORT_PCR_MUX(1);
	
		PORTA->PCR[digit1] |= PORT_PCR_MUX(1);
		PORTA->PCR[digit2] |= PORT_PCR_MUX(1);
		
		PTB->PDDR |= (1<<a)|(1<<b)|(1<<c)|(1<<d)|(1<<e)|(1<<f)|(1<<g);
		PTA->PDDR |= (1<<digit1)|(1<<digit2);
}

void display_func(n,seg){
		if(seg==1){								// Segment choice
			PTB->PDOR |= (1<<a)|(1<<b)|(1<<c)|(1<<d)|(1<<e)|(1<<f)|(1<<g);
			PTA->PCOR |= (1<<digit2);
			PTA->PDOR |= (1<<digit1);
		}
		else if(seg==2){
			PTB->PDOR |= (1<<a)|(1<<b)|(1<<c)|(1<<d)|(1<<e)|(1<<f)|(1<<g);
			PTA->PCOR |= (1<<digit1);
			PTA->PDOR |= (1<<digit2);
		}
	
		switch(n){								// Choosing correct segments for every number
			
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
