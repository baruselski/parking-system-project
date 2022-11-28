
#include "MKL05Z4.h"
#include "klaw.h"
#include "display.h"
#include "serwo.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "frdm_bsp.h"
#include "lcd1602.h"

#define Free_space_max 99
#define Free_space_min 0

uint8_t counter=0; // Number of free spaces
uint8_t D;						// Cyfra czesci dziesiatek
uint8_t J;						// Cyfra czesci jednosci

volatile uint16_t czas=0;
volatile uint16_t start=0;
volatile uint16_t stop=0;
volatile uint8_t tryb=0;
uint8_t test=0;

volatile uint8_t S1_press=0;
volatile uint8_t S2_press=0;	
/*
void TPM1_IRQHandler(void) {
	
	if(tryb==0){
		TPM1->CONTROLS[1].CnSC |= TPM_CnSC_ELSB_MASK; //Falling edge
		TPM1->CONTROLS[1].CnSC &= ~TPM_CnSC_ELSA_MASK;
		start=TPM1->CONTROLS[1].CnV & 0xFFFF;
		tryb=1;
		TPM1->CONTROLS[1].CnSC |= TPM_CnSC_CHF_MASK;
		test=10;
		czas=start;
	}
	else{
		TPM1->CONTROLS[1].CnSC |= TPM_CnSC_ELSA_MASK; //Risiing edge
		TPM1->CONTROLS[1].CnSC &= ~TPM_CnSC_ELSB_MASK;
		tryb=0;
		test=11;
		stop=TPM1->CONTROLS[1].CnV & 0xFFFF;
		//czas=10;
		czas=stop-start;
		TPM1->CONTROLS[1].CnSC |= TPM_CnSC_CHF_MASK;
	}
	
	
//	tpm1Old = tpm1New;
//	tpm1New = TPM1->CONTROLS[X].CnV & 0xFFFF;  // ToDo 2.4 Enable saving counter value
//	tpm1Diff = tpm1New - tpm1Old;	/* calculate difference */
	
//	LED_Ctrl(LED_RED, LED_TOGGLE); // ToDo 2.1.7: Enable red led toggle
//	TPM1->CONTROLS[X].CnSC |= TPM_CnSC_CHF_MASK; // ToDo 2.1.8: Clear channel flag
//}
void PORTA_IRQHandler(void)	// Subprogram for handling S1 i S2
{
	uint32_t buf;
	buf=PORTA->ISFR & (S1_MASK | S2_MASK);

	switch(buf)
	{
		case S1_MASK:	DELAY(10)										
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
									
		case S2_MASK:	DELAY(10)																				
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
	PORTA->ISFR |=  S1_MASK | S2_MASK;	// Deleton of all bits· ‰SF
	NVIC_ClearPendingIRQ(PORTA_IRQn);
}



int main(){
	Klaw_Init();								// Buttons initlalization
	Klaw_S1_S2_Int();						// S1 and S2 report interrupts
	display_init();
	serwo_init();
	
	/*
	TPM1_init();
	char display[]={0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20};
	LCD1602_Init();		 					// Inicjalizacja LCD
	LCD1602_Backlight(TRUE);  	// Wlaczenie podswietlenia
	LCD1602_ClearAll();					// Wyczysc ekran
	
	*/
	
	
	while(1){
//	serwo_open_func();
//		LCD1602_SetCursor(0,0);
//		sprintf(display,"%d",czas);
//		LCD1602_Print(display);
	
		D=counter/10;
		J=counter-(10*D);
		
		display_func(J,1);
		for(int i=0;i<300;i++){};
		display_func(D,2);
		
		
		if(S1_press){     // Functions called by S1 (opening the barrier)
			
			serwo_open_func();
			for(int i=0;i<10150;i++){
				display_func(J,1);
				for(int i=0;i<300;i++){};
				display_func(D,2);
			}
			serwo_stop_func();
				for(int i=0;i<3000051;i++){}
			
			serwo_close_func();
			for(int i=0;i<10140;i++){
					display_func(J,1);
					for(int i=0;i<300;i++){};
					display_func(D,2);
			}
			serwo_stop_func();
				S1_press=0;
				}
				
		
		if(S2_press){  // Additional function for barrier operator (manual display control) 
									 // After pressing S2, change "counter" value 
					
				if(!(PTA->PDIR&S3_MASK))	// Check if S3 is pressed (increment counter)
				{
					for(int i=0;i<3000000;i++){};
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
					for(int i=0;i<3000000;i++){};
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
