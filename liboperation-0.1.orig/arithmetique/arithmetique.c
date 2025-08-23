#include "arithmetique.h"
void *r_square(struct nbr *num, unsigned long int bscale, int scale){
	#define DESTROY_MOD(mod) \
		destroy_nbr(mod); \
		mod = NULL;
	/*
		Heron's Method:
		Xn+1 = 1/2 * ( Xn+S/Xn )
	*/
	struct bin _un = INIT_BIN( 1, 1, NULL, NULL ), _two = INIT_BIN( 2, 1, NULL, NULL ), _dix = INIT_BIN( 10, 2, NULL, NULL), *dot, *rm;
	struct nbr *n, *f, *r, *m, *div, *add, *half, *fac, *mul, 
		un = INIT_NBR( 0, 0, 1, 0, 0, NULL, NULL ), two = INIT_NBR( 0, 0, 1, 0, 0, NULL, NULL ),
		dix = INIT_NBR( 0, 0, 2, 0, 0, NULL, NULL );
	unsigned long int i = 0;
	dix.num = &_dix;
	fac = spuissance(&dix, bscale, scale);
	mul = multiplication(fac, fac);
	/*print_nbr(mul);
	putchar('\n');*/
	n = multiplication(num, mul);

	/*exit(0);*/
	r = &un;
	un.num = &_un;
	two.num = &_two;
	half = division(&un, &two, NULL, 0, 1, 0);
	div = division( n, &un, NULL, 0, 0, 0 );
	/*DOT(div, dot);*/
	add = addition( div, &un );
	f = multiplication( add, half );
	
	rm = f->num;
	f->num = f->num->next;
	f->num->prev = rm->prev;
	f->dot = 0;
	free(rm);
	/*DOT(f, dot);*/
	/*print_nbr(f);
	putchar('\n');
	exit(0);*/
	destroy_nbr(div);
	destroy_nbr(add);
	if(bscale > 0 || scale > 0){
		/*m = multiplication(f, f);*/
		while(equal(f, r) != 0){
			if(r != &un){
				destroy_nbr(r);
			}
			r = f;
			div = division( n, r, NULL, 0, 0, 0 );
			/*DOT(div, dot);*/
			add = addition( div, r );
			f = multiplication( add, half );
			rm = f->num;
			f->num = f->num->next;
			f->num->prev = rm->prev;
			free(rm);
			f->dot = 0;
			/*DOT(f, dot);*/
			destroy_nbr(div);
			destroy_nbr(add);
			/*printf("%lu\n", i++);*/
			/*m = multiplication(f, f);*/
		}
	}else{
		m = multiplication(f, f);
		while(equal(m, n) > 0){
			destroy_nbr(m);
			if(r != &un){
				destroy_nbr(r);
			}
			r = f;
			div = division( num, r, NULL, 0, 0, 0 );
			/*DESTROY_MOD(mod);*/
			add = addition( div, r );
			f = division( add, &two, NULL, 0, 0, 0 );
			rm = f->num;
			f->num = f->num->next;
			f->num->prev = rm->prev;
			free(rm);
			f->dot = 0;
			destroy_nbr(div);
			/*DESTROY_MOD(mod);*/
			destroy_nbr(add);
			m = multiplication(f, f);
		}
		destroy_nbr(m);
	}
	destroy_nbr(fac);
	destroy_nbr(mul);
	destroy_nbr(n);
	destroy_nbr(half);
	if(r != &un){
		destroy_nbr(r);
	}
	/*destroy_nbr(r);*/
	return f;
	/*DOT(f, dot);*/
	r = division(f, &un, NULL, bscale, scale, 0);
	/*DESTROY_MOD(mod);*/
	destroy_nbr(f);
	DOT(r, dot);
	return r;
}
