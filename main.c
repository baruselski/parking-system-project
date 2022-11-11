
#include "MKL05Z4.h"
#include "klaw.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define Free_space_max 99
#define Free_space_min 0

uint8_t counter=0; // Number of free spaces

volatile uint8_t S1_press=0;	
volatile uint8_t S2_press=0;

void PORTA_IRQHandler(void)	// Subprogram for handling S1 i S2
{
	uint32_t buf;
	buf=PORTA->ISFR & (S1_MASK | S2_MASK);

	switch(buf)
	{
		case S1_MASK:											
									if(!(PTA->PDIR&S1_MASK))		// Minimalisation of contact vibration					
										{
										if(!(PTA->PDIR&S1_MASK))	// cont.
										{
											if(S1_press){
												S1_press =0;
											}
											else{
											S1_press=1;
											}
										}
										}
									break;
									
		case S2_MASK:											
									if(!(PTA->PDIR&S2_MASK))		// Minimalisation of contact vibration			
										{
										if(!(PTA->PDIR&S2_MASK))	// cont.
										{
											if(S2_press){
												S2_press =0;
											}
											else{
											S2_press=1;
											}
										
										}
										}									
									break;
										
										
		default:			break;
	}	
	PORTA->ISFR |=  S1_MASK | S2_MASK;	// Deleton of all bits· ‰SF
	NVIC_ClearPendingIRQ(PORTA_IRQn);
}



int main(){
	Klaw_Init();								// Buttons initlalization
	Klaw_S1_S2_Int();						// S1 and S2 report interrupts
	
	while(1){
	
		if(S1_press){
				// Functions called by S1 (opening the barrier)
				
		}
		if(S2_press){  // Additional function for barrier operator (manual display control) 
									 // After pressing S2, change "counter" value 

				if(!(PTA->PDIR&S3_MASK))	// Check if S3 is pressed (increment counter)
				{
					if(counter<Free_space_max){
						counter +=1;
					}
					}	
				if(!(PTA->PDIR&S4_MASK))	// Check if S4 is pressed (decrement counter)
				{
					if(counter>Free_space_min){
					counter -= 1; 
					}
					}	
				
			}	
		
		}
	
}
