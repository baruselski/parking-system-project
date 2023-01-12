#include "serwo.h"

void serwo_init(void){
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;			// Turning on port B
	PORTB->PCR[serwo] |= PORT_PCR_MUX(2);		// PIN MUX TPM0_CH0
	
	SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;			// Attach clock signal to TPM0
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);			// Choose source clock for TPM0 MCGFLLCLK=41943040Hz
	
	TPM0->SC &= ~TPM_SC_CPWMS_MASK;					//	TPM0 in "count forward" mode
	TPM0->SC |= TPM_SC_PS(6);								//	Divider of input clock equal 64; clock=655360Hz
	TPM0->MOD = 0x0888;	 										//  Register MOD=2184 f=300Hz
	TPM0->CONTROLS[0].CnSC = TPM_CnSC_MSB_MASK|TPM_CnSC_ELSB_MASK;		// High-true pulses mode
	TPM0->CONTROLS[0].CnSC = ~TPM_CnSC_ELSA_MASK;
	TPM0->CONTROLS[0].CnV = 0x0000;					// Initial value
}


void serwo_open_func(){
	
	TPM0->CONTROLS[0].CnV =0x0423; 					// Impulse width for opening (set velocity)
	TPM0->SC |= TPM_SC_CMOD(1);							// Enable TPM counter

	}
void serwo_close_func(){
		
	TPM0->CONTROLS[0].CnV =0x036F;					// Impulse width for closing (set velocity)
	TPM0->SC |= TPM_SC_CMOD(1);							// Enable TPM counter
	
	}
void serwo_stop_func(){
	
	TPM0->CONTROLS[0].CnV =0x03D6;					// Impulse width for stopping (set velocity)
	TPM0->SC |= TPM_SC_CMOD(1);							// Enable TPM counter

	}
