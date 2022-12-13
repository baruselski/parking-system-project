#ifndef DISPLAY_H
#define DISPLAY_H

#include "MKL05Z4.h"
/*
#define a_MASK	(1<<2)		//PORTA
#define b_MASK	(1<<3)		
#define c_MASK	(1<<4)		
#define d_MASK	(1<<5)
#define e_MASK	(1<<6)
#define f_MASK	(1<<7)
#define g_MASK	(1<<12)
#define digit1_MASK	(1<<12) //PORTB
#define digit2_MASK	(1<<12)
*/
#define a	0		// Segments on PORTB			
#define b	1				
#define c	2			
#define d	5		// Sprobowac na B5
#define e	6		 
#define f	7		
#define g	8		
#define digit1	6 		// Digits on PORTA
#define digit2	7		

void display_init(void);
void display_func();


#endif  /* DISPLAY_H */
