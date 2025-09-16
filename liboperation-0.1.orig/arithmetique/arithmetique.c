#include "arithmetique.h"
void *r_square(struct nbr *num, unsigned long int bscale, int scale){
	#define DESTROY_MOD(mod) \
		destroy_nbr(mod); \
		mod = NULL;
	/*
		Heron's Method:
		Xn+1 = 1/2 * ( Xn+S/Xn )
	*/
	struct bin _cent = INIT_BIN(100, 3, NULL, NULL), _un = INIT_BIN( 1, 1, NULL, NULL ), _two = INIT_BIN( 2, 1, NULL, NULL ), *dot, *rm
		/*, _dix = INIT_BIN( 10, 2, NULL, NULL)*/;
	struct nbr *temp, *n, *f, *r, *m, *div, *add, *half, *fac = NULL, *mul, 
		cent = INIT_NBR( 0, 0, 3, 0, 0, NULL, NULL ),
		un = INIT_NBR( 0, 0, 1, 0, 0, NULL, NULL ), two = INIT_NBR( 0, 0, 1, 0, 0, NULL, NULL )/*,
		dix = INIT_NBR( 0, 0, 2, 0, 0, NULL, NULL )*/;
	/*fac = spuissance(&dix, bscale, scale);
	mul = multiplication(fac, fac);
	n = multiplication(num, mul);*/
	r = &un;
	un.num = &_un;
	two.num = &_two;
	/*dix.num = &_dix;*/
	cent.num = &_cent;
	mul = dup_nbr(&un);
	n = dup_nbr(num);
	while(equal(n, &two) < 0){
		temp = multiplication(n, &cent, NULL);
		fac = bymul10(mul, 10, 1);
		destroy_nbr(n);
		n = temp;
	}
	half = division(&un, &two, NULL, 0, 1, 0);
	div = division( n, &un, NULL, bscale, scale, 0 );
	add = addition( div, &un, NULL );
	f = multiplication( add, half, NULL );
	f->num->num /= 10;
	f->num->nmemb--;
	if(f->num->full){
		f->num->full = 0;
		f->bdot--;
		f->dot = BLK-1;
	}else
		f->dot--;
	if(f->num->nmemb == 0){
		rm = f->num;
		f->num = f->num->next;
		f->num->prev = rm->prev;
		free(rm);
	}
	/*f = division( add, &two, NULL, bscale, scale, 0 );*/
	/*printf("*\n");*/
	/*rm = f->num;
	f->num = f->num->next;
	f->num->prev = rm->prev;
	f->dot = 0;
	free(rm);*/
	destroy_nbr(div);
	destroy_nbr(add);
	if(bscale > 0 || scale > 0 || fac){
		while(equal(f, r) != 0){
			if(r != &un){
				destroy_nbr(r);
			}
			r = f;
			/*print_nbr(n);
			printf("\n%lu :: %i, %lu :: %i\n", n->bval, n->val, n->bdot, n->dot);
			print_nbr(r);
			printf("\n%lu :: %i, %lu :: %i\n", r->bval, r->val, r->bdot, r->dot);*/
			div = division( n, r, NULL, bscale, scale, 0 );
			/*printf("<<<<add>>>>\n");*/
			add = addition( div, r, NULL );
			/*print_nbr(add);
			printf("/2 = ");*/
			/*f = multiplication( add, half );*/
			f = multiplication( add, half, NULL );
			f->num->num /= 10;
			f->num->nmemb--;
			if(f->num->full){
				f->num->full = 0;
				f->bdot--;
				f->dot = BLK-1;
			}else
				f->dot--;
			if(f->num->nmemb == 0){
				rm = f->num;
				f->num = f->num->next;
				f->num->prev = rm->prev;
				free(rm);
			}
			/*f = division( add, &two, NULL, bscale, scale, 0 );*/
			destroy_nbr(div);
			destroy_nbr(add);
		}
	}else{
		m = multiplication(f, f, NULL);
		while(equal(m, n) > 0){
			destroy_nbr(m);
			if(r != &un){
				destroy_nbr(r);
			}
			r = f;
			div = division( num, r, NULL, 0, 0, 0 );
			add = addition( div, r, NULL );
			/*print_nbr(add);
			putchar('\n');
			print_nbr(&two);
			putchar('\n');*/
			f = multiplication( add, half, NULL );
			/*f = division( add, &two, NULL, 0, 0, 0 );*/
			rm = f->num;
			f->num = f->num->next;
			f->num->prev = rm->prev;
			free(rm);
			f->dot = 0;
			destroy_nbr(div);
			destroy_nbr(add);
			m = multiplication(f, f, NULL);
		}
		destroy_nbr(m);
	}
	if(r != &un){
		destroy_nbr(r);
	}
	if(fac){
		r = division(f, mul, NULL, bscale, scale, 0);
		destroy_nbr(fac);
		destroy_nbr(f);
		f = r;
	}else
		destroy_nbr(mul);
	destroy_nbr(n);
	/*if(scale || bscale){
		n = bymin10(f, bscale, scale);
		destroy_nbr(f);
	}else{
		n = f;
	}*/
	/*destroy_nbr(fac);
	destroy_nbr(mul);*/
	destroy_nbr(half);
	/*destroy_nbr(r);*/
	/*print_nbr(n);
	exit(0);*/
	return f;
	/*DOT(f, dot);*/
	r = division(f, &un, NULL, bscale, scale, 0);
	/*DESTROY_MOD(mod);*/
	destroy_nbr(f);
	DOT(r, dot);
	return r;
}
