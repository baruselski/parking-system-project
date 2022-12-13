#include "serwo.h"

void serwo_init(void){
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
	PORTB->PCR[serwo] |= PORT_PCR_MUX(2);
	
	SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;			// Dolaczenie sygnalu zegara do TPM0
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);			// Wybierz zródlo taktowania TPMx MCGFLLCLK=41943040Hz
	
	TPM0->SC &= ~TPM_SC_CPWMS_MASK;				//	TPM0 w trybie zliczanie "w przód"
	TPM0->SC |= TPM_SC_PS(6);							//	Dzielnik zegara wejsciowegoo równy 64; zegar=655360Hz
	//TPM0->MOD = 0x3332;	 //  Register MOD=13106 f=50Hz
	TPM0->MOD = 0x0888;	 //  Register MOD=2184 f=300Hz
	TPM0->CONTROLS[0].CnSC = TPM_CnSC_MSB_MASK|TPM_CnSC_ELSB_MASK;
	TPM0->CONTROLS[0].CnSC = ~TPM_CnSC_ELSA_MASK;
	TPM0->CONTROLS[0].CnV = 0x0000;
	TPM0->SC |= TPM_SC_CMOD(1);	
	
	
}


void serwo_open_func(){
	
	TPM0->CONTROLS[0].CnV =0x0423;
	TPM0->SC |= TPM_SC_CMOD(1);

	}
void serwo_close_func(){
	
	TPM0->CONTROLS[0].CnV =0x036F;
	TPM0->SC |= TPM_SC_CMOD(1);
	
	}
void serwo_stop_func(){
	
	TPM0->CONTROLS[0].CnV =0x03D6;
	TPM0->SC |= TPM_SC_CMOD(1);	

	}
