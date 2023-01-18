#include "klaw.h"
uint8_t S1_press=0;
uint8_t S2_press=0;

void Klaw_Init(void)
{
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;		// Turning on port A
	PORTA->PCR[S1] |= PORT_PCR_MUX(1);
	PORTA->PCR[S2] |= PORT_PCR_MUX(1);
	PORTA->PCR[S3] |= PORT_PCR_MUX(1);
	PORTA->PCR[S4] |= PORT_PCR_MUX(1);
	PORTA->PCR[S1] |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;  // Attach a pull-up resistor and connect it to VCC
	PORTA->PCR[S2] |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;
	PORTA->PCR[S3] |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;
	PORTA->PCR[S4] |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;
}
void Klaw_S1_S2_Int(void)
{
	PORTA -> PCR[S1] |= PORT_PCR_IRQC(0xa);		//0xa - falling edge
	PORTA -> PCR[S2] |= PORT_PCR_IRQC(0xa);		
	NVIC_ClearPendingIRQ(PORTA_IRQn);
	NVIC_EnableIRQ(PORTA_IRQn);
}
void PORTA_IRQHandler(void)	// Subprogram for handling S1 i S2
{
	uint32_t buf;
	buf=PORTA->ISFR & (S1_MASK | S2_MASK);

	switch(buf)
	{
		case S1_MASK:	for(uint32_t i=0;i<(100000);i++)__nop(); 										
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
									
		case S2_MASK:	for(uint32_t i=0;i<(100000);i++)__nop(); 																				
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
