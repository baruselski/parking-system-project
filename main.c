
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
static uint8_t D;							// Number of tens
static uint8_t J;							// Number of ones
static uint8_t IN;						// IN = 1 photocell F1 was interrupted first 
								// IN = 0 - F2 was interrupted first


void Counter_display(void){						// Display free spaces on the 7-seg display
						D=counter/10;							// Calculate number of ones
						J=counter-(10*D);					// Calculate number of tens
						display_func(J,1);				// Display ones
						for(int i=0;i<400;i++){};	// Delay
						display_func(D,2);				// Display tens
						for(int i=0;i<400;i++){};	// Delay

}
void SysTick_Handler(void)	// Subprogram for handling SysTick interrupts
{ 
	time++;										// Count time up from the moment of opening the barrier, every 100ms
}


int main(){
	Klaw_Init();								// Buttons initlalization
	Klaw_S1_S2_Int();						// S1 and S2 report interrupts
	display_init();							// Display initlalization
	serwo_init();								// Servo initlalization
	Photocell_Init();						// Photocell initlalization
	

	
	while(1){										// Main program loop
		Counter_display();				// Display the counter value 

		if(S1_press){     // Functions called by S1 (opening the barrier)
			
			SysTick_Config(SystemCoreClock/10 );	// Start SysTick
			NVIC_ClearPendingIRQ(PORTB_IRQn);
			NVIC_EnableIRQ(PORTB_IRQn);						// Start photocell interrupts		
			
			serwo_open_func();						// Open start
			for(int i=0;i<5850;i++){					// Delay used as barrier opening time
				display_func(J,1);							// During the delay show current value on the display
				for(int i=0;i<400;i++){};
				display_func(D,2);
				for(int i=0;i<400;i++){};
			}
			serwo_stop_func();
																			// Open stop
															
																					// Waiting
					time=0;													// Set time to 0
					while(1){
						Counter_display();						// Display counter value
	
					// (start) If opening time higher than 10s, photocell F1 is not interrupted (no obstacle under the barrier)
						if(time>=100&&F1_broken==0){			
							if(test && F2_broken){					// Check if the vehicle drove under both photocells
								time=0;												// Set time to 0
									
									if(IN){														// If IN =1, F1 was interrupted first, so the vehicle drove in
											if(counter>Free_space_min){		// If counter is higher than min value
													counter--;								// the amount of parking spots gets lowered
												}
												IN=0;												// set IN to 0
											}
									else{															// If IN = 0, F2 was interrupted first, so the vehicle drove out
											if(counter<Free_space_max){		// If counter is lower than max value
												counter++;									// the amount of parking spots gets increased
												}
											}
											
								
								while(1){														// Wait 2 seconds before closing
										if(time>=20&&F1_broken==0){			// If time is bigger than 2s and photocell F1 was not interrupted (no obstacle detected)
											break;
											}
										Counter_display();							// Display counter value
										}
									}
							time=0;																// Set time to 0
							break;																// Exit the loop, go to closing of the barrier
						}
		// (stop) If opening time is bigger than 10s and photocell F1 was not interrupted (no obstacle was detected)
		
		// (start) If opening time lower than 10s
						if(F1_first && F2_broken==0){       		// If F1 was interrupted first, set IN = 1
							IN=1;
							}
						
						if(test && F2_broken){									// Check if a vehicle passed both photocells correctly
							time=0;																// Set time to 0
							if(IN){																// If IN =1, F1 was interrupted first, the vehicle drove in
									if(counter>Free_space_min){				// If counter is higher than min value
											counter--;										// the amount of parking spots gets lowered
											}
											IN=0;													// Set IN to 0
										}
							else{																	// If IN =0, F2 was interrupted first, the vehicle drove out
									if(counter<Free_space_max){				// If counter is lower than max value
											counter++;										// the amount of parking spots gets increased
											}
											
									}
								
								while(1){														// Wait 2 seconds after the vehicle has passed to close the barrier
										if(time>=20&&F1_broken==0){			// If time is higher than 2s and photocell F1 was not interrupted (no obstacle detected)
											break;
											}
										Counter_display();							// Display the counter value
										}
							time=0;																// Set time to 0
							break;																// Exit the loop
						}
			// (stop) If opening time is lower than 10s
					}
		
						test=0;																	// Wyzerowanie wszystkich zmiennych sygnalizacyjnych
						F1_first=0;
						F2_broken=0;
						F1_broken=0;
																							// Close start
							serwo_close_func();
						for(int i=0;i<6540;i++){					// Delay used as closing time
								display_func(J,1);						// During the delay the counter value is displayed
								for(int i=0;i<400;i++){};
								display_func(D,2);
								for(int i=0;i<400;i++){};
						}
						serwo_stop_func();				
																							// Close stop
					
					S1_press=0;													// Set variable S1 so the if(S1_press) condition happens only once
					SysTick_Config(1); 									// Stop SysTick
					NVIC_DisableIRQ(PORTB_IRQn);				// Turn off PORTB interrupts (photocells)
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
