#include <stdio.h>
#include <stdlib.h>		/*
				   for random() 
				 */
#include <math.h>

#include "syre.h"

bit16 sawtooth[SINTAB_LEN];
bit16 squarewave[SINTAB_LEN];
bit16 sinwave[SINTAB_LEN];
bit16 wnwave[SINTAB_LEN];

void generate_squarewave(void)
{
	int i;

	for (i = 0; i < SINTAB_LEN / 2; i++) {
		squarewave[i] = 32768 / 2;
	}
	for (; i < SINTAB_LEN; i++) {
		squarewave[i] = -(32768 / 2);
	}
}

void generate_sawtooth(void)
{
	int i;

	for (i = 0; i < SINTAB_LEN; i++) {
		sawtooth[i] = (i * 32768) / SINTAB_LEN - (32768 / 2);
	}
}

void generate_sinwave(void)
{
	int i;

	for (i = 0; i < (SINTAB_LEN); i++) {
		sinwave[i] =
		    (bit16) (32768.0 * sin(2.0 * M_PI * i / (SINTAB_LEN)));
	}
}

void generate_wnwave(void)
{
	int i;

	for (i = 0; i < SINTAB_LEN; i++) {
		wnwave[i] = (bit16) ((random() % (1 << 16)) - (1 << 15));
	}
}
