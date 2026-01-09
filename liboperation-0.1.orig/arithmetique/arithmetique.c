#include "arithmetique.h"
void *ispuissance(struct nbr *num, int pui){
	struct bin _un_ = { 1, 1, 0, 0, NULL, NULL };
	struct nbr *r[2], *n = num, *res = NULL, un = INIT_NBR( 0, 0, 1, 0, 0, NULL, NULL ), *pseudo[2];
	int comp = pui, i, j;
	unsigned long int n_bdot = num->bdot, n_dot = num->dot, n_bval = num->bval, n_val = num->val;
	un.num = &_un_;
	if(pui == 0){
		if((res = calloc(1, sizeof(struct nbr))) == NULL){
			perror("calloc()");
			exit(EXIT_FAILURE);
		}
		memcpy(res, &un, sizeof(struct nbr));
		if((res->num = calloc(1, sizeof(struct bin))) == NULL){
			perror("calloc()");
			exit(EXIT_FAILURE);
		}
		memcpy(res->num, &_un_, sizeof(struct bin));
		res->num->alloc = 1;
		return res;
	}
	n_dot *= pui;
	n_bdot *= pui;
	n_bval *= pui;
	n_val *= pui;
	while(n_dot >= BLK){
		n_dot -= BLK;
		n_bdot++;
	}
	while(n_val >= BLK){
		n_val -= BLK;
		n_bval++;
	}
	for(i = 0; i < 2; i++){
		if((r[i] = calloc(1, sizeof(struct nbr))) == NULL){
			perror("calloc()");
			exit(EXIT_FAILURE);
		}
		if((pseudo[i] = calloc(1, sizeof(struct nbr))) == NULL){
			perror("calloc()");
			exit(EXIT_FAILURE);
		}
		r[i]->num = new_num(n_bval + (n_val > 0), n_bdot + (n_dot > 0));
		pseudo[i]->num = new_num(n_bval + (n_val > 0), n_bdot + (n_dot > 0));
	}
	num_cpy(pseudo[0], &un);
	num_cpy(r[0], num);
	n = r[0];
	while(comp > 1){
		if(comp%2){
			comp--;
			j = (pseudo[0]->num->nmemb == 0);
			(void)multiplication(n, pseudo[j], pseudo[!j]);
			reset_num(pseudo[j]);
		}else{
			comp /= 2;
			i = (r[0]->num->nmemb == 0);
			res = multiplication(n, n, r[!i]);
			n = r[!i];
			reset_num(r[i]);
		}
	}
	j = (pseudo[0]->num->nmemb == 0);
	if(equal(pseudo[j], &un) != 0){
		res = multiplication(pseudo[j], n, pseudo[!j]);
	}else{
		if(pui == 1)
			res = dup_nbr(num);
	}
	for(i = 0; i < 2;i++){
		if(res != r[i])
			destroy_nbr(r[i]);
		if(res != pseudo[i])
			destroy_nbr(pseudo[i]);
	}
	return res;
}
void *spuissance(struct nbr *num, unsigned long int bpui, int pui){
	struct bin _un_ = { 1, 1, 0, 0, NULL, NULL };
	struct nbr *r[2] = { NULL, NULL }, *pseudo[2], *n = num, *in, *res = NULL, un = INIT_NBR( 0, 0, 1, 0, 0, NULL, NULL );
	unsigned long int bcomp = bpui, b_total, db_total;
	int i, j;
	un.num = &_un_;
	if(pui == 0 && bpui == 0){
		if((res = calloc(1, sizeof(struct nbr))) == NULL){
			perror("calloc()");
			exit(EXIT_FAILURE);
		}
		memcpy(res, &un, sizeof(struct nbr));
		if((res->num = calloc(1, sizeof(struct bin))) == NULL){
			perror("calloc()");
			exit(EXIT_FAILURE);
		}
		memcpy(res->num, &_un_, sizeof(struct bin));
		res->num->alloc = 1;
		return res;
	}
	in = ispuissance(num, pui);
	if(!bpui){
		return in;
	}
	b_total = n->val * bpui + n->bval * bpui * BLK;
	db_total = n->dot *bpui + n->bdot * bpui * BLK;
	for(i = 0; i < 2; i++){
		if((r[i] = calloc(1, sizeof(struct nbr))) == NULL){
			perror("calloc()");
			exit(EXIT_FAILURE);
		}
		r[i]->num = new_num(b_total, db_total);
		if((pseudo[i] = calloc(1, sizeof(struct nbr))) == NULL){
			perror("calloc()");
			exit(EXIT_FAILURE);
		}
		pseudo[i]->num = new_num(b_total, db_total);
	}
	num_cpy(pseudo[0], &un);
	n = ispuissance(num, BLK);
	num_cpy(r[0], n);
	destroy_nbr(n);
	n = r[0];
	while(bcomp > 1){
		if(bcomp%2){
			bcomp--;
			j = (pseudo[0]->num->nmemb == 0);
			(void)multiplication(n, pseudo[j], pseudo[!j]);
			reset_num(pseudo[j]);
		}else{
			bcomp /= 2;
			i = (r[0]->num->nmemb == 0);
			res = multiplication(n, n, r[!i]);
			n = r[!i];
			reset_num(r[i]);
		}
	}
	j = (pseudo[0]->num->nmemb == 0);
	if(equal(pseudo[j], &un) != 0){
		res = multiplication(pseudo[j], n, pseudo[!j]);
		n = pseudo[!j];
	}else{
		if(pui == 1)
			res = num;
	}
	res = multiplication(n, in, NULL);
	for(i = 0; i < 2;i++){
		if(res != r[i])
			destroy_nbr(r[i]);
		if(res != pseudo[i])
			destroy_nbr(pseudo[i]);
	}
	destroy_nbr(in);
	return res;
}
void *puissance(struct nbr *num1, struct nbr *num2, struct nbr **modulo, unsigned long int bscale, int scale, int approximation){
	struct bin _two_ = INIT_BIN(2, 1, NULL, NULL ), _un_ = INIT_BIN( 1, 1, NULL, NULL ), _zero_ = INIT_BIN( 0, 1, NULL, NULL );
	struct nbr two = INIT_NBR( 0, 0 , 1, 0, 0, NULL, NULL ), un = INIT_NBR( 0, 0, 1, 0, 0, NULL, NULL ), zero = INIT_NBR( 0, 0, 1, 0, 0, NULL, NULL ), *pseudo = &un, *p, *div, *d, *mod = NULL, *n, *res = NULL;
	int neg = 0;
	two.num = &_two_;
	un.num = &_un_;
	zero.num = &_zero_;
	if(equal(num2, &zero) == 0){
		if((res = calloc(1, sizeof(struct nbr))) == NULL){
			perror("calloc()");
			exit(EXIT_FAILURE);
		}
		if((res->num = calloc(1, sizeof(struct bin))) == NULL){
			perror("calloc()");
			exit(EXIT_FAILURE);
		}
		memcpy(res, &un, sizeof(struct nbr));
		memcpy(res->num, &_un_, sizeof(struct bin));
		res->num->alloc = 1;
		return res;
	}
	if(num2->neg){
		neg = num2->neg;
		num2->neg = 0;
	}
	n = num1;
	div = num2;
	while(equal(div, &un) != 0){
		d = edivision(div, &two, &mod, 0, 0, 0, NULL);
		if(div != num2)
			destroy_nbr(div);
		if(equal(mod, &un) == 0){
			p = multiplication(n, pseudo, NULL);
			if(pseudo != &un)
				destroy_nbr(pseudo);
			pseudo = p;
		}
		if(equal(d, &zero) != 0){
			res = multiplication(n, n, NULL);
			if(n != num1)
				destroy_nbr(n);
			n = res;
		}
		if(n->dot > BLK){
			exit(0);
		}
		div = d;
		destroy_nbr(mod);
		mod = NULL;
	}
	if(pseudo != &un){
		res = multiplication(pseudo, n, NULL);
		destroy_nbr(pseudo);
		if(n != num1)
			destroy_nbr(n);
	}else{
		if(n == num1)
			res = dup_nbr(n);
	}
	if(div != num2)
		destroy_nbr(div);
	if(neg){
		div = edivision(&un, res, modulo, bscale, scale, approximation, NULL);
		num2->neg = neg;
		destroy_nbr(res);
		res = div;
	}
	return res;
}
/*
	DIVISEUR,
	DIVIDENDE,
	QUOTIENT
*/

void *r_square(struct nbr *num, unsigned long int bscale, int scale){
	/*
		Heron's Method:
		Xn+1 = 1/2 * ( Xn+S/Xn )
	*/
	struct bin 	_cent = INIT_BIN(100, 3, NULL, NULL),
			_un = INIT_BIN( 1, 1, NULL, NULL ),
			_two = INIT_BIN( 2, 1, NULL, NULL ),
			_half[2] = { INIT_BIN( 0, 1, NULL, NULL ), INIT_BIN( 0, 1, NULL, NULL ) },
			*dot, *rm;
	struct nbr *temp = NULL, *n, *f, *r, *m, *div, *add, *fmul, *fac = NULL, *mul, *nmul, 
		cent = INIT_NBR( 0, 0, 3, 0, 0, NULL, NULL ),
		un = INIT_NBR( 0, 0, 1, 0, 0, NULL, NULL ),
		two = INIT_NBR( 0, 0, 1, 0, 0, NULL, NULL ),
		half = INIT_NBR( 0, 0, 1, 0, 1, NULL, "0.5" ),
		*for_div[5] = { NULL, NULL, NULL, NULL, NULL };
	unsigned long int bdmv = 0;
	int dmv = 0;
	un.num = &_un;
	two.num = &_two;
	cent.num = &_cent;
	mul = dup_nbr(&un);
	/*r = dup_nbr(&un);*/
	r = &un;
	n = dup_nbr(num);
	half.num = &_half[0];
	_half[0].num = 5;
	_half[1].num = 0;
	_half[0].next = &_half[1];
	_half[0].next->prev = &_half[0];
	_half[0].prev = &_half[1];
	_half[1].next = NULL;
	while(equal(n, &two) < 0){
		temp = multiplication(n, &cent, NULL);
		fac = bymul10(mul, 10, 1);
		destroy_nbr(n);
		n = temp;
	}
	if(bscale || scale){
		if((temp = calloc(1, sizeof(struct nbr))) == NULL){
			perror("calloc()");
			exit(EXIT_FAILURE);
		}
		dmv = 2*scale;
		bdmv = 2*bscale;
		if(dmv >= BLK){
			dmv-=BLK;
			bdmv++;
		}
		temp->num = new_num(n->bval + (n->val > 0), bdmv + (dmv > 0));
		temp = mv_dot(n, temp, bdmv, dmv);
		destroy_nbr(n);
		n = temp;
	}
	if((add = calloc(1, sizeof(struct nbr))) == NULL){
		perror("calloc()");
		exit(EXIT_FAILURE);
	}
	add->num = new_num(n->bval + (n->val > 0), bscale + (scale > 0));
	if((fmul = calloc(1, sizeof(struct nbr))) == NULL){
		perror("calloc()");
		exit(EXIT_FAILURE);
	}
	fmul->num = new_num(n->bval + (n->val > 0) +1, bdmv + (dmv > 0));

	/*half = division(&un, &two, NULL, 0, 1, 0, NULL);*/

	/*div = division( n, &un, NULL, 0, 0, 0, NULL );

	(void)addition( div, &un, add );*/
	(void)addition( n, &un, add );
	f = multiplication( add, &half, fmul );

	f->num->num -= (f->num->num%10);
	DOT(f, dot, rm);
	/*destroy_nbr(div);*/
	if((nmul = calloc(1, sizeof(struct nbr))) == NULL){
		perror("calloc()");
		exit(EXIT_FAILURE);
	}
	nmul->num = new_num(f->bval *2 + (f->val > 0) *2, 0);
	m = multiplication(f, f, nmul);
	
	if((for_div[DIVISEUR] = calloc(1, sizeof(struct nbr))) == NULL){
		perror("calloc()");
		exit(EXIT_FAILURE);
	}
	for_div[DIVISEUR]->num = new_num(f->bval + (f->val > 0), 0);
	if((for_div[DIVIDENDE] = calloc(1, sizeof(struct nbr))) == NULL){
		perror("calloc()");
		exit(EXIT_FAILURE);
	}
	for_div[DIVIDENDE]->num = new_num(n->bval + (n->val > 0), 0);
	if((for_div[QUOTIENT] = calloc(1, sizeof(struct nbr))) == NULL){
		perror("calloc()");
		exit(EXIT_FAILURE);
	}
	for_div[QUOTIENT]->num = new_num(n->bval + (n->val > 0), bscale + (scale > 0));
	if((for_div[MODULO] = for_div[RESTE] = calloc(1, sizeof(struct nbr))) == NULL){
		perror("calloc()");
		exit(EXIT_FAILURE);
	}
	for_div[MODULO]->num = for_div[RESTE]->num = new_num(n->bval + (n->val > 0), bscale + (scale > 0));

	/*for_div[RESTE] = NULL;*/
	while(equal(m, n) > 0){
		/*destroy_nbr(m);*/
		r = f;
		div = edivision( n, r, NULL, 0, 0, 0, for_div );
		reset_num(add);
		(void)addition( div, r, add );
		/*destroy_nbr(fmul);
		fmul = multiplication( add, &half, NULL );
		f = fmul;*/
		reset_num(fmul);
		f = multiplication( add, &half, fmul );
		f->num->num -= (f->num->num%10);
		DOT(f, dot, rm);
		/*destroy_nbr(div);*/
		reset_num(for_div[DIVISEUR]);
		reset_num(for_div[DIVIDENDE]);
		reset_num(for_div[QUOTIENT]);
		reset_num(for_div[MODULO]);
		reset_num(nmul);
		m = multiplication(f, f, nmul);
	}
	destroy_nbr(for_div[DIVISEUR]);
	destroy_nbr(for_div[DIVIDENDE]);
	destroy_nbr(for_div[QUOTIENT]);
	destroy_nbr(for_div[MODULO]);
	/*destroy_nbr(m);*/
	destroy_nbr(nmul);
	/*if(r != &un && r != f){
		destroy_nbr(r);
	}*/
	if(fac){
		r = edivision(f, mul, NULL, bscale, scale, 0, NULL);
		destroy_nbr(fac);
		f = r;
	}
	destroy_nbr(mul);
	destroy_nbr(add);
	destroy_nbr(n);
	if(scale || bscale)
		bymin10(f, f, bscale, scale);
	/*destroy_nbr(half);*/
	return f;
}
