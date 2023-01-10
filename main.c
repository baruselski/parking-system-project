
#include "MKL05Z4.h"
#include "klaw.h"
#include "display.h"
#include "serwo.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "photocell.h"

//#include "uart0.h"
//#include "tsi.h"
#include "lcd1602.h"
#include "frdm_bsp.h"

#define Free_space_max 99
#define Free_space_min 1


static uint8_t counter=88; 		// Number of free spaces
static uint8_t D;							// Cyfra czesci dziesiatek
static uint8_t J;							// Cyfra czesci jednosci
static uint8_t IN;						// IN = 1 fotokomorka F1 byla przerwana jako pierwsza IN = 0 to F2 byla przerwana jako 1


void Counter_display(void){						// Wyswietlenie liczby wolnych miejsc na wyswietlaczu
						D=counter/10;							// Obliczenie cyfry dziesiatek
						J=counter-(10*D);					// Obliczenie cyfry jednostek
						display_func(J,1);				// Wyswietlenie cyfry jednostek
						for(int i=0;i<400;i++){};	// Delay
						display_func(D,2);				// Wyswietlenie cyfry dziesiatek
						for(int i=0;i<400;i++){};	// Delay

}
void SysTick_Handler(void)	// Podprogram obslugi przerwania od SysTick'a
{ 
	time++;										// Pomiar czasu od poczatku otwarcia szlabanu, Przerwania co 100ms
}


int main(){
	Klaw_Init();								// Buttons initlalization
	Klaw_S1_S2_Int();						// S1 and S2 report interrupts
	display_init();							// Display initlalization
	serwo_init();								// Servo initlalization
	Photocell_Init();						// Photocell initlalization
	

	
	while(1){										// Petla glowna
		Counter_display();				// Wyswietlenie wartosci counter na wyswietlaczu

		if(S1_press){     // Functions called by S1 (opening the barrier)
			
			SysTick_Config(SystemCoreClock/10 );	// Start SysTick
			NVIC_ClearPendingIRQ(PORTB_IRQn);
			NVIC_EnableIRQ(PORTB_IRQn);						// Start przerwania od fotokomorek		
			
			serwo_open_func();						//Open start
			for(int i=0;i<5850;i++){					// Opoznienie sluzace jako czas otwarcia
				display_func(J,1);							// W trakcie opoznienia wyswietlana jest wartosc na wyswietlaczu
				for(int i=0;i<400;i++){};
				display_func(D,2);
				for(int i=0;i<400;i++){};
			}
			serwo_stop_func();
																			//Open stop
															
																					// Waiting
					time=0;													// Wyzerowanie czasu
					while(1){
						Counter_display();						// Wyswietlenie wartosci counter na wyswietlaczu
		// (start) Jesli czas otwarcia jest wiekszy od 10s i fotokomorka F1 nie jest przerwana (brak przszkody pod szlabanem)			
						if(time>=100&&F1_broken==0){			
							if(test && F2_broken){					// Sprawdzenie, czy jakikolwiek pojazd przejechal przez obie fotokomorki
								time=0;												// Wyzerowanie czasu
									
									if(IN){														// Jesli IN =1 oznacza, ze F1 przerwana jako pierwsza, czyli pojazd wjechal
											if(counter>Free_space_min){		// Jesli wartoac counter jest wieksza niz wartosc minimalna
													counter--;								// nastepuje zmniejszenie liczby wolnych miejsc parkingowych
												}
												IN=0;												// Wyzerowanie wartosci IN
											}
									else{															// Jesli IN =0 oznacza, ze F2 przerwana jako pierwsza, czyli pojazd wyjechal
											if(counter<Free_space_max){		// Jesli wartoac counter jest mniejsza niz wartosc maksymalna
												counter++;									// nastepuje zwiekszenie liczby wolnych miejsc parkingowych
												}
											}
											
								
								while(1){														// Oczekiwanie 2s po przejezdzie zeby zamknac szlaban
										if(time>=20&&F1_broken==0){			// Jesli czas jest wiekszy niz 2s i fotokomorka F1 nie jest przerwana (brak przszkody pod szlabanem)
											break;
											}
										Counter_display();							// Wyswietlenie wartosci counter na wyswietlaczu
										}
									}
							time=0;																// Wyzerowanie czasu
							break;																// Wyjscie z petli przejscie do funkcji zamykajacej szlaban
						}
		// (stop) Jesli czas otwarcia jest wiekszy od 10s i fotokomorka F1 nie jest przerwana (brak przszkody pod szlabanem)
		
		// (start) Jesli czas otwarcia jest mniejszy niz 10s
						if(F1_first && F2_broken==0){       		// Jesli F1 jest przerwana jako pierwsza ustawia wartosc IN = 1
							IN=1;
							}
						
						if(test && F2_broken){									// Sprawdzenie czy jakis pojazd przejechal (poprawnie przerwane 2 fotokomorki)
							time=0;																// Wyzerowanie czasu
							if(IN){																// Jesli IN =1 oznacza, ze F1 przerwana jako pierwsza, czyli pojazd wjechal
									if(counter>Free_space_min){				// Jesli wartoac counter jest wieksza niz wartosc minimalna
											counter--;										// nastepuje zmniejszenie liczby wolnych miejsc parkingowych
											}
											IN=0;													// Wyzerowanie wartosci IN
										}
							else{																	// Jesli IN =0 oznacza, ze F2 przerwana jako pierwsza, czyli pojazd wyjechal
									if(counter<Free_space_max){				// Jesli wartoac counter jest mniejsza niz wartosc maksymalna
											counter++;										// nastepuje zwiekszenie liczby wolnych miejsc parkingowych
											}
											
									}
								
								while(1){														// Oczekiwanie 2s po przejezdzie zeby zamknac szlaban
										if(time>=20&&F1_broken==0){			// Jesli czas jest wiekszy niz 2s i fotokomorka F1 nie jest przerwana (brak przszkody pod szlabanem)
											break;
											}
										Counter_display();							// Wyswietlenie wartosci counter na wyswietlaczu
										}
							time=0;																// Wyzerowanie czasu
							break;																// Wyjscie z petli przejscie do funkcji zamykajacej szlaban
						}
			// (stop) Jesli czas otwarcia jest mniejszy niz 10s
					}
		
						test=0;																	// Wyzerowanie wszystkich zmiennych sygnalizacyjnych
						F1_first=0;
						F2_broken=0;
						F1_broken=0;
																							// Close start
							serwo_close_func();
						for(int i=0;i<6540;i++){					// Opoznienie sluzace jako czas zamykania
								display_func(J,1);						// W trakcie opoznienia wyswietlana jest wartosc na wyswietlaczu
								for(int i=0;i<400;i++){};
								display_func(D,2);
								for(int i=0;i<400;i++){};
						}
						serwo_stop_func();				
																							// Close stop
					
					S1_press=0;													// Wyzerowanie zmiennej S1_press warunek if(S1_press) wykonuje sie tylko raz
					SysTick_Config(1); 									// Stop SysTick
					NVIC_DisableIRQ(PORTB_IRQn);				// Wylaczenie przerwan z PORTB (fotokomorki)
					time=0;
				}
						
		if(S2_press){  // Additional function for barrier operator (manual display control) 
									 // After pressing S2, change "counter" value 
					
				if(!(PTA->PDIR&S3_MASK))	// Check if S3 is pressed (increment counter)
				{
					for(int i=0;i<1500000;i++){}
					if(counter<Free_space_max){
						counter =counter+1;
						}
						
				}	
				if(!(PTA->PDIR&S4_MASK))	// Check if S4 is pressed (decrement counter)
				{
					for(int i=0;i<1500000;i++){}
					if(counter>Free_space_min){
						counter = counter - 1; 
						}
					}	
		
			}	

		}
	
}
