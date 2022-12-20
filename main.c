
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


static uint8_t counter=88; // Number of free spaces
static uint8_t D;						// Cyfra czesci dziesiatek
static uint8_t J;						// Cyfra czesci jednosci
static uint8_t IN;


void SysTick_Handler(void)	// Podprogram obslugi przerwania od SysTick'a
{ 
	time++;
}


int main(){
	Klaw_Init();								// Buttons initlalization
	Klaw_S1_S2_Int();						// S1 and S2 report interrupts
	display_init();
	serwo_init();
	Photocell_Init();
	
	
	PORTB->PCR[10] |= PORT_PCR_MUX(1);
	PTB->PDDR |= (1<<10);
	PTB->PCOR |= (1<<10);
	

	
	while(1){


		D=counter/10;
		J=counter-(10*D);
		
		display_func(J,1);
		for(int i=0;i<100;i++){};
		display_func(D,2);

//		PTB->PDOR |= (1<<10);
//		for(int i=0;i<3000000*counter;i++){}

//		PTB->PCOR |= (1<<10);
//			for(int i=0;i<3000051;i++){}
			
	
		if(S1_press){     // Functions called by S1 (opening the barrier)
			
			SysTick_Config(SystemCoreClock/10 );	// Start SysTick
			NVIC_ClearPendingIRQ(PORTB_IRQn);
			NVIC_EnableIRQ(PORTB_IRQn);						// Start przerwania od fotokomorek		
			
			serwo_open_func();				//Open start
			for(int i=0;i<6150;i++){
				display_func(J,1);
				for(int i=0;i<300;i++){};
				display_func(D,2);
				for(int i=0;i<300;i++){};
			}
			serwo_stop_func();
											 //Open stop
															
															// Waiting
				
					while(1){
						D=counter/10;
						J=counter-(10*D);
						display_func(J,1);
						for(int i=0;i<300;i++){};
						display_func(D,2);
						
						if(time>=100&&F1_broken==0){
							time=0;
							break;
						}
					
						if(F1_first && F2_broken==0){
							IN=1;
					}
						
						if(test && F2_broken){
							time=0;
							if(IN){
									if(counter>Free_space_min){
											counter--;
											}
											IN=0;
										}
							else{
									if(counter<Free_space_max){
											counter++;
											}
											
									}
								
								while(1){
										if(time>=10&&F1_broken==0){
											time=0;
											break;
											}
										D=counter/10;
										J=counter-(10*D);
										display_func(J,1);
										for(int i=0;i<300;i++){};
										display_func(D,2);
										}
							
							break;
					}

					}
						test=0;
						F1_first=0;
						F2_broken=0;
						F1_broken=0;
																	//Close start
							serwo_close_func();
						for(int i=0;i<6840;i++){
								display_func(J,1);
								for(int i=0;i<300;i++){};
								display_func(D,2);
								for(int i=0;i<300;i++){};
						}
						serwo_stop_func();				
																//Close stop
					
					S1_press=0;				
					SysTick_Config(1); 		//Stop SysTick
					NVIC_DisableIRQ(PORTB_IRQn);
					time=0;
				}
						
		if(S2_press){  // Additional function for barrier operator (manual display control) 
									 // After pressing S2, change "counter" value 
					
				if(!(PTA->PDIR&S3_MASK))	// Check if S3 is pressed (increment counter)
				{
					for(int i=0;i<30000;i++){
						D=counter/10;
						J=counter-(10*D);
						display_func(J,1);
						for(int i=0;i<300;i++){};
						display_func(D,2);
									}
					if(counter<Free_space_max){
						counter =counter+1;
						D=counter/10;
						J=counter-(10*D);
						display_func(J,1);
						for(int i=0;i<300;i++){};
						display_func(D,2);
						}
						
				}	
				if(!(PTA->PDIR&S4_MASK))	// Check if S4 is pressed (decrement counter)
				{
					for(int i=0;i<30000;i++){
						D=counter/10;
						J=counter-(10*D);
						display_func(J,1);
						for(int i=0;i<300;i++){};
						display_func(D,2);
									}
					if(counter>Free_space_min){
						counter = counter - 1; 
						D=counter/10;
						J=counter-(10*D);
						display_func(J,1);
						for(int i=0;i<300;i++){};
						display_func(D,2);
					
						}
					}	
		
			}	

		}
	
}
