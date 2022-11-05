#ifndef KLAW_H
#define KLAW_H

#include "MKL05Z4.h"
#define S1_MASK	(1<<10)		// Maska dla klawisza S1 - otworzenie szlabanu
#define S2_MASK	(1<<11)		// Maska dla klawisza S2 - przycisk do wlaczenie funkcji zmiany liczby na wyswietlaczu
#define S3_MASK	(1<<8)		// Maska dla klawisza S4 - inkrementacja 
#define S4_MASK	(1<<9)		// Maska dla klawisza S4 - dekrementacja
#define S1	10					// Numer bitu dla klawisza S1
#define S2	11					// Numer bitu dla klawisza S2
#define S3	8					// Numer bitu dla klawisza S3
#define S4	9					// Numer bitu dla klawisza S4

void Klaw_Init(void);
void Klaw_S1_S2_Int(void);

#endif  /* KLAW_H */
