
#include "MKL05Z4.h"
#include "klaw.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define Free_space_max 99
#define Free_space_min 0

uint8_t counter=0; // Liczba wolnych miejsc

volatile uint8_t S1_press=0;	
volatile uint8_t S2_press=0;

void PORTA_IRQHandler(void)	// Podprogram obslugi przerwania od klawiszy S1 i S2
{
	uint32_t buf;
	buf=PORTA->ISFR & (S1_MASK | S2_MASK);

	switch(buf)
	{
		case S1_MASK:											
									if(!(PTA->PDIR&S1_MASK))		// Minimalizacja drgan zestyków						
										{
										if(!(PTA->PDIR&S1_MASK))	// Minimalizacja drgan zestyków ¨c.d.)
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
									if(!(PTA->PDIR&S2_MASK))		// Minimalizacja drgan zestyków				
										{
										if(!(PTA->PDIR&S2_MASK))	// Minimalizacja drgan zestykó¨c.d.)
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
	PORTA->ISFR |=  S1_MASK | S2_MASK;	// Kasowanie wszystkich bitów· ‰SF
	NVIC_ClearPendingIRQ(PORTA_IRQn);
}



int main(){
	Klaw_Init();								// Inicjalizacja klawiatury
	Klaw_S1_S2_Int();						// Klawisze S1, S2 zglaszaja przerwanie
	
	while(1){
	
		if(S1_press){
				// funkcje wywolywane po wcisnieciu przycisku S1 (otwarcia szlabanu)
				
		}
		if(S2_press){  // Dodatkowa funkcja dla administratora korekta liczby wolnych miejsc 
									 // Tylko po wcisnieciu S2 mozliwosc zmiany zmiennej "counter" 

				if(!(PTA->PDIR&S3_MASK))	// Sprawdza czy przycisk S3 jest wcisniety (inkrementacja licznika)
				{
					if(counter<Free_space_max){
						counter +=1;
					}
					}	
				if(!(PTA->PDIR&S4_MASK))	// Sprawdza czy przycisk S4 jest wcisniety (dekrementacja licznika)
				{
					if(counter>Free_space_min){
					counter -= 1; 
					}
					}	
				
			}	
		
		}
	
}
