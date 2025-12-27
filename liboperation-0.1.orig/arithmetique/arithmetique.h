#ifndef ARITHMETIQUE_H
#define ARITHMETIQUE_H
#include "../operation/operation.h"
void *ispuissance(struct nbr *num, int pui);
void *spuissance(struct nbr *num, unsigned long int bpui, int pui);
void *puissance(struct nbr *num1, struct nbr *num2, struct nbr **modulo, unsigned long int bscale, int scale, int approximation);
void *r_square(struct nbr *num, unsigned long int bscale, int scale);
#endif
