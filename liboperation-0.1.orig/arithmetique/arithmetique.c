#include "arithmetique.h"
void *r_square(struct nbr *num, unsigned long int bscale, int scale){
	#define DESTROY_MOD(mod) \
		destroy_nbr(mod); \
		mod = NULL;
	/*
		Heron's Method:
		Xn+1 = 1/2 * ( Xn+S/Xn )
	*/
	struct bin _un = INIT_BIN( 1, 1, NULL, NULL ), _two = INIT_BIN( 2, 1, NULL, NULL ), *dot;
	struct nbr *f, *r, *m, *div, *add, *half,
		un = INIT_NBR( 0, 0, 1, 0, 0, NULL, NULL ), two = INIT_NBR( 0, 0, 1, 0, 0, NULL, NULL ),
		*mod = NULL;
	unsigned long int bs = 2;
	int s = 7;
	r = &un;
	un.num = &_un;
	two.num = &_two;
	half = division(&un, &two, NULL, 0, 1, 0);
	/*DESTROY_MOD(mod);*/
	div = division( num, &un, NULL, bscale, scale-1, 0 );
	DOT(div, dot);
	/*DESTROY_MOD(mod);*/
	add = addition( div, &un );
	/*f = division( add, &two, &mod, bscale, scale, 0 );*/
	f = multiplication( add, half );
	DOT(f, dot);
	destroy_nbr(div);
	/*DESTROY_MOD(mod);*/
	destroy_nbr(add);
	if(bscale > 0 || scale > 0){
		do{
			while(equal(f, r) != 0){
				if(r != &un){
					destroy_nbr(r);
				}
				r = f;
				div = division( num, r, NULL, bscale, scale-1, 0 );
				DOT(div, dot);
				/*DESTROY_MOD(mod);*/
				add = addition( div, r );
				f = multiplication( add, half );
				DOT(f, dot);
				/*f = division( add, &two, &mod, bscale, scale, 0 );*/
				destroy_nbr(div);
				/*DESTROY_MOD(mod);*/
				destroy_nbr(add);
			}
			s += 25%BLK;
			bs += 25/BLK;
			if(s >= BLK){
				s -= BLK;
				bs++;
			}
		}while(s < scale || bs < bscale);
	}else{
		m = multiplication(f, f);
		while(equal(m, num) > 0){
			destroy_nbr(m);
			if(r != &un){
				destroy_nbr(r);
			}
			r = f;
			div = division( num, r, NULL, bscale, scale, 0 );
			DESTROY_MOD(mod);
			add = addition( div, r );
			f = division( add, &two, NULL, bscale, scale, 0 );
			destroy_nbr(div);
			DESTROY_MOD(mod);
			destroy_nbr(add);
			m = multiplication(f, f);
			print_nbr(f);
			putchar('\n');
		}
		destroy_nbr(m);
	}
	destroy_nbr(half);
	if(r != &un){
		destroy_nbr(r);
	}
	/*destroy_nbr(r);*/
	/*return f;*/
	/*DOT(f, dot);*/
	r = division(f, &un, NULL, bscale, scale, 0);
	/*DESTROY_MOD(mod);*/
	destroy_nbr(f);
	DOT(r, dot);
	return r;
}
