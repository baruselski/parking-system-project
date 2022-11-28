#include "serwo.h"
#include "frdm_bsp.h"
#include "lcd1602.h"
void serwo_init(void){
	PORTB->PCR[serwo] |= PORT_PCR_MUX(2);
	
	SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;			// Dolaczenie sygnalu zegara do TPM0
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);			// Wybierz zr�dlo taktowania TPMx MCGFLLCLK=41943040Hz
	
	TPM0->SC &= ~TPM_SC_CPWMS_MASK;				//	TPM0 w trybie zliczanie "w prz�d"
	TPM0->SC |= TPM_SC_PS(6);							//	Dzielnik zegara wejsciowegoo r�wny 64; zegar=655360Hz
	//TPM0->MOD = 0x3332;	 //  Register MOD=13106 f=50Hz
	TPM0->MOD = 0x0888;	 //  Register MOD=2184 f=300Hz
	TPM0->CONTROLS[0].CnSC = TPM_CnSC_MSB_MASK|TPM_CnSC_ELSB_MASK;
	TPM0->CONTROLS[0].CnSC = ~TPM_CnSC_ELSA_MASK;
	TPM0->CONTROLS[0].CnV = 0x0000;
	TPM0->SC |= TPM_SC_CMOD(1);	
	
	
}

void TPM1_init(void){
	SIM->SCGC6 |= SIM_SCGC6_TPM1_MASK;
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
	PORTB->PCR[13] = PORT_PCR_MUX(2);
	
	TPM1->SC &= ~TPM_SC_CPWMS_MASK;				//	TPM0 w trybie zliczanie "w prz�d"
	TPM1->SC |= TPM_SC_PS(6);			
	
	TPM1->CONTROLS[1].CnSC &= ~(TPM_CnSC_MSA_MASK|TPM_CnSC_MSB_MASK);

	TPM1->CONTROLS[1].CnSC |= TPM_CnSC_ELSA_MASK; //Fall edge
	TPM1->CONTROLS[1].CnSC &= ~TPM_CnSC_ELSB_MASK;

	TPM1->SC |= TPM_SC_CMOD(1);	
	/*cap
	TPM1->CONTROLS[1].CnSC |= (TPM_CnSC_ELSA_MASK | TPM_CnSC_ELSB_MASK); 
	TPM1->CONTROLS[1].CnSC |= TPM_CnSC_CHIE_MASK;
	*/
	NVIC_SetPriority(TPM1_IRQn, 1);  /* TPM1 interrupt priority level  */

	NVIC_ClearPendingIRQ(TPM1_IRQn); 
	NVIC_EnableIRQ(TPM1_IRQn);
}
void serwo_open_func(){
	
	//TPM0->CONTROLS[0].CnV =0x049C;
	TPM0->CONTROLS[0].CnV =0x0423;
	TPM0->SC |= TPM_SC_CMOD(1);
	//for(int i=0;i<3000280;i++){}
	
	//TPM0->CONTROLS[0].CnV =0x03D6;
//	TPM0->SC |= TPM_SC_CMOD(1);

	}
void serwo_close_func(){
	
	TPM0->CONTROLS[0].CnV =0x036F;
	TPM0->SC |= TPM_SC_CMOD(1);
	//for(int i=0;i<3000280;i++){}
	
	//TPM0->CONTROLS[0].CnV =0x03D6;
	//TPM0->SC |= TPM_SC_CMOD(1);
	}
void serwo_stop_func(){
	
	TPM0->CONTROLS[0].CnV =0x03D6;
	TPM0->SC |= TPM_SC_CMOD(1);	

	}
