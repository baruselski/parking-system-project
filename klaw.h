#ifndef KLAW_H
#define KLAW_H

#include "MKL05Z4.h"
#define S1_MASK	(1<<10)		// Mask for S1 - opening the barrier
#define S2_MASK	(1<<11)		// Mask for S2 - button to start function that allows user to change values with S3 and S4
#define S3_MASK	(1<<8)		// Mask for S4 - increment
#define S4_MASK	(1<<9)		// Mask for S4 - decrement
#define S1	10					// bit number for S1
#define S2	11					// bit number for S2
#define S3	8					// bit number for S3
#define S4	9					// bit number for S4

void Klaw_Init(void);
void Klaw_S1_S2_Int(void);

#endif  /* KLAW_H */
