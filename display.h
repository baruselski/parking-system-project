#ifndef DISPLAY_H
#define DISPLAY_H

#include "MKL05Z4.h"

#define a	0		// PIN number Segments on PORTB			
#define b	1				
#define c	2			
#define d	9		
#define e	6		 
#define f	7		
#define g	8		
#define digit1	6 		// PIN number Digits on PORTA
#define digit2	7		

void display_init(void);					// Initialization of the display
void display_func(int n,int seg); // Display function (exact number)

#endif  /* DISPLAY_H */
