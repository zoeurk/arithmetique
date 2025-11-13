#include "operation.h"
/*Voir BUG*/
void *reset_num(struct nbr *n){
	struct bin *delete;
	for(delete = n->num;delete;delete = delete->next)
		delete->num = delete->nmemb = delete->full = 0;
	n->neg = n->val = n->dot = n->bval = n->bdot = 0;
	return n;
}
int num_cpy(struct nbr *res, struct nbr *num){
	struct bin *result = res->num, *n = num->num;
	for(;n && result && n->nmemb > 0; n = n->next, result = result->next){
		result->num = n->num;
		result->nmemb = n->nmemb;
		result->full = n->full;
	}
	if(n && n->nmemb > 0)
		return -1;
	res->val = num->val;
	res->bval = num->bval;
	res->dot = num->dot;
	res->bdot = num->bdot;
	return 0;
}
char *parse_nbr(char *n){
	char *ret, *pret, *pn, *cp = NULL;
	size_t len = 0;
	int neg = 0, zero = 1, num = 0, dot = 0;
	for(pn = n;*pn != 0;pn++)
		if((*pn == '-' || *pn == '+') && num == 0){
			if(*pn == '-')
				neg = !neg;
		}else{
			num = 1;
			if(*pn == '0' && zero == 1){
				cp = pn;
				len = 1;
				continue;
			}else{
				if(zero == 1 && len == 1){
					len = 0;
					cp = NULL;
				}
				zero = 0;
				len++;
				if(cp == NULL)
					cp = pn;
				if(*pn == '.'){
					if(dot == 0){
						dot = 1;
					}else{
						fprintf(stderr, "Invalid Number\n");
						return NULL;
					}
				}else{
					if(*pn < '0' || *pn > '9'){
						fprintf(stderr, "Invalid Number\n");
						return NULL;
					}
				}
			}
		}
	if(dot){
		for(;*(pn-1) == '0'; pn--, len--);
		if(*(pn-1) == '.'){
			pn--;
			len--;
		}
	}
	if(cp){
		if(*cp == '.')
			len++;
	}else{
		cp = n;
		len = 1;
	}
	if(neg)
		len++;
	if((pret = ret = calloc(len+1, sizeof(char))) == NULL){
		perror("calloc()");
		exit(EXIT_FAILURE);
	}
	if(neg){
		*pret = '-';
		pret++;
		len--;
	}
	if(cp && *cp == '.'){
		*pret = '0';
		pret++;
		len--;
	}
	memcpy(pret, cp, len);
	return ret;
}

struct nbr *encode_nbr(char *n){
	struct nbr *r;
	struct bin *pnum, *sv;
	unsigned long int c = 1, i,k;
	int j;
	char *pn = n;
	if((r = calloc(1, sizeof(struct nbr))) == NULL){
		perror("calloc()");
		exit(EXIT_FAILURE);
	}
	if(*n == '-' || *n == '+'){
		r->neg = (*n == '-') ? 1 : 0;
		pn++;
		n++;
	}
	if((pnum = r->num = calloc(1, sizeof(struct bin))) == NULL){
		perror("calloc()");
		exit(EXIT_FAILURE);
	}
	r->num->alloc = 1;
	for(pn = pn + strlen(pn)-1;pn >= n; pn--){
		if(*pn == '.'){
			r->dot = r->val;
			r->bdot = r->bval;
			r->bval = r->val = 0;
			c = 1;
			if((pnum->next = calloc(1, sizeof(struct bin))) == NULL){
				perror("calloc()");
				exit(EXIT_FAILURE);
			}
			pnum->next->alloc = 1;
			pnum->next->prev = pnum;
			r->num->prev = pnum->next;
			pnum = pnum->next;
			continue;
		}
		if(++pnum->nmemb == BLK+1){
			pnum->nmemb = BLK;
			c = 1;
			if((pnum->next = calloc(1, sizeof(struct bin))) == NULL){
				perror("calloc()");
				exit(EXIT_FAILURE);
			}
			pnum->next->alloc = 1;
			pnum->next->prev = pnum;
			r->num->prev = pnum->next;
			pnum = pnum->next;
			pnum->nmemb = 1;
		}
		i = (*pn - (3 << 4)) * c;
		c *= 10;
		/*r->val++;*/
		if(++r->val == BLK){
			pnum->full = 1;
			r->bval++;
			r->val = 0;
		}
		pnum->num += i;
	}
	if(r->val == BLK){
		r->val--;
		r->bval++;
	}
	if(r->dot && r->bdot){
		for(pnum = r->num, i = r->bdot+1;i > 0; i--, pnum = pnum->next);
		pnum = pnum->prev;
		if(pnum != r->num){
			for(j = 0, i = 1; j < pnum->nmemb; j++, i *= 10);
			k = D_MAX/(i/10);
			for(sv = pnum; pnum != r->num; pnum = pnum->prev){
				pnum->num *= k;
				pnum->num += (pnum->prev->num/i);
				pnum->prev->num -= ((pnum->prev->num/(i)*i));
			}
			sv->nmemb = BLK;
			sv->full = 1;
			r->num->nmemb = j;
			r->num->full = (j == BLK) ? 1 : 0;
		}
	}
	if(r->bval == 0 && r->val == 1 && r->bdot == 0 && r->dot == 0 && r->num->num == 0 && r->neg){
		r->neg = 0;
	}
	return r;
}
void print_nbr(struct nbr *n){
	const size_t jj = D_MAX;
	struct bin *b;
	unsigned long int blen = n->bval, j, cnum;
	int dot = (n->dot > 0 || n->bdot > 0), i, k, num, len = n->val/*, mul[C_BLK] = COEFS*/;
	if(n->neg)
		putchar('-');
	for(b = (n->num->prev) ? n->num->prev : n->num; b != NULL;b = (b == n->num) ? NULL : b->prev){
		if(b->full){
			j = jj;
			/*printf("%lu", b->num);*/
		}else{
			if(b->nmemb == 0)
				continue;
			/*printf("%lu", b->num%mul[b->nmemb]);*/
			for(j = 1, i = 0, k = b->nmemb-1; i < k; i++)
				j *= 10;
		}
		for(j = j, i = 0, cnum = b->num; j; i++){
			num = cnum/j;
			putchar(num | (3 << 4));
			cnum -= num*j;
			j /= 10;
		}
		if(b->full)
			blen--;
		else
			len -= b->nmemb;
		if(dot == 1 && len == 0 && blen == 0){
			putchar('.');
			dot = 0;
			len = n->dot;
			blen = n->bdot;
		}
	}
}
void *destroy_nbr(struct nbr *n){
	struct bin *b, *next;
	for(b = n->num; b != NULL;){
		if(b->alloc == 1){
			for(next = b->next; next && next->alloc == 0; next = next->next);
			/*pb = b->next;*/
			free(b);
			b = next;
		}else
			b = b->next;
	}
	if(n->n)
		free(n->n);
	free(n);
	return NULL;
}
int equal(struct nbr *num1, struct nbr *num2){
	struct bin *b1, *b2;
	unsigned long int blen, blen1, blen2, sbdot, dmul, n1, n2;
	int i, ret, len, len1, len2, sdot;
	if(num1->neg == 1 && num2->neg == 1)
		ret = -1;
	else
		if(num1->neg == 0 && num2->neg == 0)
			ret = 1;
		else
			if(num1->neg == 0 && num2->neg == 1)
				return 1;
			else
				return -1;
	if(num1->bval > num2->bval || (num1->val > num2->val && num1->bval == num2->bval))
		return 1*ret;
	else
		if(num1->bval < num2->bval || (num1->val < num2->val && num1->bval == num2->bval)){
			return -1*ret;
		}
	len1 = num1->val + num1->dot;
	blen1 = num1->bval + num1->bdot;
	len2 = num2->val + num2->dot;
	blen2 = num2->bval + num2->bdot;
	b1 = num1->num;
	b2 = num2->num;
	b1 = (!b1->prev) ? b1 : b1->prev;
	b2 = (!b2->prev) ? b2 : b2->prev;
	len = (len1 > len2)? len1 : len2;
	blen = (blen1 > blen2) ? blen1 : blen2;
	sdot = num1->val;
	sbdot = num1->bval;
	while(b1->nmemb == 0)
		b1 = b1->prev;
	while(b2->nmemb == 0)
		b2 = b2->prev;
	/*printf("%i (%lu):: %i (%lu)\n", b1->nmemb, b1->num, b2->nmemb, b2->num);*/
	do{
		if(b1->num > b2->num)
			return 1*ret;
		else
			if(b1->num < b2->num)
				return -1*ret;
		if(len)
			len -= b1->nmemb;
		else
			blen--;
		if(sdot && b1->nmemb != BLK){
			sdot -= b1->nmemb;
		}
		if(sbdot && b1->full){
			sbdot--;
		}
		/*printf("%lu :: %lu\n", b1->num, b2->num);*/
		b1 = b1->prev;
		b2 = b2->prev;
		/*if(!sbdot && !sdot)
			break;*/
	}while(sdot || sbdot);
	/*while(len != 0 || blen != 0);*/
	if(((blen || (blen1 != blen2)) || (len || (len1 != len2))) && b1 && b2)
		do{
			if(b1->nmemb != b2->nmemb){
				if(b1->nmemb > b2->nmemb){
					for(dmul = 1, i = b1->nmemb - b2->nmemb; i > 0; i--)
						dmul *= 10;
					n1 = b1->num;
					n2 = b2->num * dmul;
				}else{
					if(b1->nmemb < b2->nmemb){
						for(dmul = 1, i = b2->nmemb - b1->nmemb; i > 0; i--)
							dmul *= 10;
						n1 = b1->num * dmul;
						n2 = b2->num;
					}
				}
				if(n1 > n2){
					return 1*ret;
				}else{
					return -1*ret;
				}
			}
			/*printf("DOT = %lu :: %lu\n", b1->num, b2->num);*/
			if(b1->num > b2->num)
				return 1*ret;
			else
				if(b1->num < b2->num)
					return -1*ret;
			/*if(!b1->full && len)
				len -= (len1 < len2) ? b1->nmemb : b2->nmemb;
			else
				if(blen)
					blen--;*/
			b1 = b1->prev;
			b2 = b2->prev;
			/*if(b1->next == NULL || b2->next == NULL)
				break;*/
		}while(b1->next != NULL && b2->next != NULL);
		/*while(len != 0 || blen != 0);*/
	if((b1 && b1 != num1->num->prev) && (b2 && b2 != num2->num->prev)){
		return 0;
	}
	if(b1 && b1 != num1->num->prev){
		for(b1 = b1; b1->num == 0; b1 = b1->prev){
			if(b1 == num1->num)
				return 0;
		}
		return 1*ret;
	}else{
		if(b2 && b2 != num2->num->prev){
			for(b2 = b2; b2->num == 0; b2 = b2->prev){
				if(b2 == num2->num)
					return 0;
			}
			return -1*ret;
		}
	}
	return 0;
}
struct bin *new_num(unsigned long int val, unsigned long int dot){
	struct bin *new = NULL, *pnew, *pdot = NULL, *pval, *plval;
	unsigned long int lval = val, ldot = dot;
	#define TST 1
	#if TST == 1
	if((pnew = pval = plval = calloc(lval, sizeof(struct bin))) == NULL){
		perror("calloc()");
		exit(EXIT_FAILURE);
	}
	pval->alloc = 1;
	if(ldot){
		if((pnew = pdot = calloc(ldot, sizeof(struct bin))) == NULL){
			perror("calloc()");
			exit(EXIT_FAILURE);
		}
		pdot->alloc = 1;
		for(;ldot > 1; ldot--, pdot++){
			pdot->next = (pdot+1);
			pdot->next->prev = pdot;
		}
		pdot->next = pval;
		pdot->next->prev = pdot;
	}
	for(plval = NULL;lval > 1;lval--, pval++){
		pnew->prev = pval;
		pnew->prev->next = NULL;
		pval->next = (pval + 1);
		pval->next->prev = pval;
		plval = pval;
	}
	if(plval){
		plval->next = pval;
		plval->next->prev = plval;
	}
	pnew->prev = pval;
	pnew->prev->next = NULL;
	/*for(pdot = pnew->prev; pdot; pdot = (pdot == pnew) ? NULL : pdot->prev)
		printf("%lu :: %i :: %i (%lu, %lu)\n", pdot->num, pdot->nmemb, pdot->full, val, dot);
	for(pdot = pnew; pdot; pdot = pdot->next)
		printf("%lu :: %i :: %i (%lu, %lu)\n", pdot->num, pdot->nmemb, pdot->full, val, dot);*/
	/*exit(0);*/
	return pnew;
	#else
	/*pval->prev->next = pval;
	pval->next = NULL;*/
	while(lval || ldot){
		if(new == NULL){
			if((pnew = new = calloc(1, sizeof(struct bin))) == NULL){
				perror("calloc()");
				exit(EXIT_FAILURE);
			}
		}else{
			if((pnew->next = calloc(1, sizeof(struct bin))) == NULL){
				perror("calloc()");
				exit(EXIT_FAILURE);
			}
			pnew->next->prev = pnew;
			new->prev = pnew->next;
			pnew = pnew->next;
		}
		pnew->alloc = 1;
		if(lval > 0)
			lval--;
		else
			ldot--;
	}
	#endif
	return new;
}
void *dup_nbr(struct nbr *num){
	struct bin *b1, *b2;
	struct nbr *res;
	if((res = calloc(1, sizeof(struct nbr))) == NULL){
		perror("calloc()");
		exit(EXIT_FAILURE);
	}
	res->num = new_num(num->bval + (num->val > 0), num->bdot + (num->dot > 0));
	for(b1 = num->num, b2 = res->num; b1 && b2; b1 = b1->next, b2 = b2->next){
		b2->num = b1->num;
		b2->nmemb = b1->nmemb;
		b2->full = b1->full;
	}
	res->bval = num->bval;
	res->bdot = num->bdot;
	res->val = num->val;
	res->dot = num->dot;
	res->neg = num->neg;
	return res;
}
void *bymul10(struct nbr *num, int fac, int faclen){
	struct bin *nw = NULL, *r, z = ZERO_BIN/*z = INIT_BIN( 0, 1, NULL, NULL )*/;
	struct nbr zero = INIT_NBR( 0, 0, 1, 0, 0, NULL, NULL );
	int n;
	zero.num = &z;
	if(equal(num, &zero) == 0){
		return num;
	}
	if(num->num->nmemb == 0 && num->val == 0 && num->bval == 0){
		num->val = num->num->nmemb = 1;
		return num;
	}
	r = (num->num->prev) ? num->num->prev : num->num;
	if(r->prev){
		while(r->nmemb == 0){
			r = r->prev;
		}
	}
	if(r->nmemb+faclen > BLK){
		if(r->next == NULL){
			if((nw = calloc(1, sizeof(struct bin))) == NULL){
				perror("calloc()");
				exit(EXIT_FAILURE);
			}
			nw->alloc = 1;
			r->next = nw;
			r->next->prev = r;
			nw->next = NULL;
			num->num->prev = nw;
		}else
			nw = r->next;
		nw->nmemb = r->nmemb + faclen - BLK;
	}else
		r->nmemb += faclen;
	if(r->nmemb == BLK)
		r->full = 1;
	for(;r;r = r->prev){
		r->num *= fac;
		if((n = r->num / D_MAX_LIMIT) ){
			r->num %= D_MAX_LIMIT;
			if(r->next == nw){
				if(nw == NULL)
					printf("NULL\n");
				r->next->num = n;
			}else{
				r->next->num += n;
			}
		}
		if(nw){
			r->nmemb = BLK;
			r->full = 1;
		}
		if(r == num->num)
			break;
	}
	num->val += faclen;
	if(num->val >= BLK){
		num->val -= BLK;
		num->bval++;
	}
	return num;
}
struct retbcpy *nbytescpy(struct bin **b2, struct bin **b1, int *bstart, unsigned long int lbytes, unsigned long int bytes){
	static struct retbcpy ret;
	unsigned long int  letbread = lbytes;
	int mul[C_BLK] = COEFS, letread = bytes, toread, blkbytes, bisread = 0;
	letread = bytes;
	if(bytes)
		toread = bytes;
	else{
		toread = BLK;
		letread = BLK;
		letbread--;
	}
	ret.rblk = 0;
	ret.rbytes = 0;
	bisread = toread;
	blkbytes = (*b1)->nmemb - *bstart;
	do{
		if((*b1)->nmemb - *bstart >= toread){
			(*b2)->num *= mul[toread];
			(*b2)->num += ((*b1)->num%mul[(*b1)->nmemb - *bstart])/mul[(*b1)->nmemb - *bstart - toread];
			bisread = toread;
		}else{
			(*b2)->num *= mul[(*b1)->nmemb - *bstart];
			(*b2)->num += (*b1)->num%mul[(*b1)->nmemb - *bstart];
			bisread = (*b1)->nmemb - *bstart;
			*bstart = 0;
		}
		(*b2)->nmemb += bisread;
		if((*b2)->nmemb == BLK)
			(*b2)->full = 1;
		ret.rbytes += bisread;
		if(ret.rbytes >= BLK){
			ret.rbytes -= BLK;
			ret.rblk++;
		}
		letread -= bisread;
		blkbytes -= bisread;
		toread -= bisread;
		if(toread == 0 && letbread){
			letread = BLK;
			letbread--;
		}
		if(blkbytes == 0){
			if(!(*b1)->prev)
				break;
			(*b1) = (*b1)->prev;
			blkbytes = (*b1)->nmemb;
			*bstart = 0;
		}else
			*bstart = (*bstart + bisread);
		if(toread == 0 && letread){
			toread = BLK;
			(*b2) = (*b2)->prev;
		}
	}while(letread || letbread);
	return &ret;
}
void *align_dot(struct nbr *_num, unsigned long int blk, int bytes){
	struct bin *pb2, *pbr, *pbl, *new, bdix = INIT_BIN(10, 2, NULL, NULL);
	struct nbr *num, *dres = NULL, *dval, *pui, *nadd = NULL, dix = INIT_NBR(0, 0, 2, 0, 0, NULL, "10");
	unsigned long int cbdot = blk;
	int start = 0, cdot = bytes;
	dix.num = &bdix;
	num = dup_nbr(_num);
	if((dres = calloc(1, sizeof(struct nbr))) == NULL){
		perror("calloc()");
		exit(EXIT_FAILURE);
	}
	if((dval = calloc(1, sizeof(struct nbr))) == NULL){
		perror("calloc()");
		exit(EXIT_FAILURE);
	}
	pb2 = num->num;
	pbl = new_num(num->bdot + (num->dot > 0), 0);
	dval->num = new_num(num->bval + (num->val > 0), 0);
	dres->bdot = num->bval;
	if(num->val)
		pb2 = pb2->prev;
	for(;dres->bdot > 0; dres->bdot--, pb2 = pb2->prev);
	dres->num = pbl;
	dres->bval = num->bdot;
	dres->val = num->dot;
	pbr = (pbl->prev) ? pbl->prev : pbl;
	nbytescpy(&pbr, &pb2->prev, &start, num->bdot, num->dot);
	pbr = (dval->num->prev) ? dval->num->prev : dval->num;
	pb2 = num->num->prev;
	start = 0;
	nbytescpy(&pbr, &pb2, &start, num->bval, num->val);
	pui = spuissance(&dix, cbdot, cdot);
	nadd = multiplication(dres, pui, NULL);
	/**************/
	if(!nadd->num->prev){
		new = calloc(1, sizeof(struct bin));
		nadd->num->next = new;
		nadd->num->prev = new;
	}
	nadd->num->prev->next = dval->num;
	pb2 = dval->num->prev;
	dval->num->prev = nadd->num->prev;
	nadd->num->prev = (pb2) ? pb2 : dval->num;
	/**************/
	nadd->bdot = nadd->bval;
	nadd->dot = nadd->val;
	nadd->val = num->val;
	nadd->bval = num->bval;
	pb2 = nadd->num;
	destroy_nbr(num);
	destroy_nbr(dres);
	destroy_nbr(pui);
	free(dval);
	return nadd;
}
void *addition(struct nbr *num1, struct nbr *num2, struct nbr *result){
	struct bin *pb1, *pb2, *pbr;
	struct nbr *res, *n1, *n2;
	unsigned long int bdot, bval, cbdot = 0;
	unsigned long int b1n, b2n;
	int i, j, nmemb1, nmemb2, add, retenue = 0,
		cf, full = 0, full1 = 0, full2 = 0, dot, val, cdot = 0, mul[C_BLK] = COEFS;
	if(num1->neg && !num2->neg){
		num1->neg = 0;
		res = soustraction(num2, num1, result);
		num1->neg = 1;
		return res;
	}else{
		if(!num1->neg && num2->neg){
			num2->neg = 0;
			res = soustraction(num1, num2, result);
			num2->neg = 1;
			return res;
		}
	}
	if(num1->bval > num2->bval || (num1->bval == num2->bval && num1->val > num2->val)){
		val = num1->val;
		bval = num1->bval;
	}else{
		val = num2->val;
		bval = num2->bval;
	}
	if(num1->bdot > num2->bdot || (num1->bdot == num2->bdot && num1->dot > num2->dot)){
		dot = num1->dot;
		bdot = num1->bdot;
		n1 = num1;
		n2 = num2;
	}else{
		dot = num2->dot;
		bdot = num2->bdot;
		n1 = num2;
		n2 = num1;
	}
	pb1 = n1->num;
	pb2 = n2->num;
	/*dot = (num1->dot > num2->dot) ? num1->dot : num2->dot;
	val = (num1->val > num2->val) ? num1->val : num2->val;
	bdot = (num1->bdot > num2->bdot) ? num1->bdot : num2->bdot;
	bval = (num1->bval > num2->bval) ? num1->bval : num2->bval;*/
	if(!result){
		if((res = calloc(1, sizeof(struct nbr))) == NULL){
			perror("calloc()");
			exit(EXIT_FAILURE);
		}
		res->dot = dot;
		res->val = val;
		res->bval = bval;
		res->bdot = bdot;
		if(res->bval + (res->val > 0) < res->bval || res->bdot + (res->dot > 0) < res->bdot){
			fprintf(stderr, "Result can be too high: %i\n\t%lu + (%i > 0)= %lu\n\t%lu + (%i > 0)== %lu\n",
				(res->bval + res->val <= res->bval || res->bdot + res->dot <= res->bdot),
				res->bval, res->val, res->bval + res->val,
				res->bdot, res->dot, res->bdot + res->dot
			);
			exit(EXIT_FAILURE);
		}
		pbr = res->num = new_num(bval + 1, bdot + (dot > 0));
	}else{
		res = result;
		result->dot = dot;
		result->val = val;
		result->bval = bval;
		result->bdot = bdot;
		pbr = result->num;
	}
	cbdot = n1->bdot - n2->bdot;
	if(n1->dot || n2->dot){
		if(n1->dot >= n2->dot)
			cdot = n1->dot - n2->dot;
		else{
			cdot = BLK + n1->dot - n2->dot;
			cbdot--;
		}
	}
	if(cdot || cbdot){
		pb1 = n1->num;
		pb2 = n2->num;
		for(i = cbdot; i > 0; i--){
			pbr->num = pb1->num;
			pbr->nmemb = pb1->nmemb;
			pbr->full = pb1->full;
			if(pbr->full)
				cbdot--;
			else{
				cbdot--;
				cdot += BLK;
				cdot -= pb1->nmemb;
			}
			pb1 = pb1->next;
			pbr = pbr->next;
		}
		if(cdot >= pb1->nmemb){
			pbr->nmemb = pb1->nmemb;
			pbr->full = pb1->full;
			pbr->num = pb1->num;
			cdot -= pbr->nmemb;
			pbr = pbr->next;
			pb1 = pb1->next;
		}
		if(cdot){
			pbr->nmemb = pb1->nmemb;
			pbr->full = pb1->full;
			cf = mul[cdot];
			i = mul[pbr->nmemb];
			pbr->num = pb1->num;
			pbr->num -= (pbr->num/cf)*cf;
			pbr->num += (pb1->num/cf)*cf + pb2->num*cf;
			if(pbr->num >= (unsigned long int) i){
				retenue = 1;
				pbr->num -= i;
			}
			pbr = pbr->next;
			pb1 = pb1->next;
			pb2 = pb2->next;
		}
	}
	for(	nmemb1 = pb1->nmemb,
		nmemb2 = pb2->nmemb,
		full1 = pb1->full,
		full2 = pb2->full,
		b1n = pb1->num,
		b2n = pb2->num,
		i = 0;
		pbr;
		i = 0,
		pbr = pbr->next
	){
		pbr->num = b1n + b2n + retenue;
		if(full1 || full2){
			if(pbr->num >= D_MAX_LIMIT){
				pbr->num -= D_MAX_LIMIT;
				retenue = 1;
			}else
				retenue = 0;
			add = i = BLK;
			full = 1;
		}else{
			for(	cf = 1,
				j = add = i = (nmemb1 > nmemb2) ? nmemb1 : nmemb2;
				j > 0;
				j--,
				cf *= 10
			);
			if(pbr->num >= (unsigned long int)cf){
				if(pbr->next){
					pbr->num -= cf;
					retenue = 1;
				}else{
					add++;
					if(++res->val == BLK){
						res->bval++;
						res->val = 0;
					}
					retenue = 0;
				}
			}else
				retenue = 0;
			full = 0;
		}
		if(dot){
			dot -= i;
		}else{
			if(val){
				val -= i;
			}else{
				if(i != add)
					add = 1;
			}
		}
		pbr->nmemb = add;
		pbr->full = full;
		if(pb1){
			pb1 = pb1->next;
			if(pb1){
				b1n = pb1->num;
				nmemb1 = pb1->nmemb;
				full1 = pb1->full;
			}else
				full1 = nmemb1 = b1n = 0;
		}else
			full1 = nmemb1 = b1n = 0;
		if(pb2){
			pb2 = pb2->next;
			if(pb2){
				b2n = pb2->num;
				nmemb2 = pb2->nmemb;
				full2 = pb2->full;
			}else
				full2 = nmemb2 = b2n = 0;
		}else
			full2 = nmemb2 = b2n = 0;
	}
	/*if(res->num->prev && res->num->prev->nmemb == 0 && res->num->prev->num == 0){
		pb1 = res->num->prev;
		res->num->prev = pb1->prev;
		pb1->prev->next = NULL;
		free(pb1);
	}*/
	res->neg = (num1->neg && num2->neg);
	/*if(dres)
		destroy_nbr(dres);*/
	/*if(nadd)
		destroy_nbr(nadd);*/
	return res;
}
void *soustraction(struct nbr *num1, struct nbr *num2, struct nbr *result){
	struct bin *pb1, *pb2, *pbr;
	struct nbr *res, *pn1, *pn2;
	unsigned long int n1, n2, cbdot = 0;
	int i, nmemb1, nmemb2, neg = 0, cf, mul[C_BLK] = COEFS, retenue = 0, cdot = 0;
	/*printf("SOUSTRACTION:");
	print_nbr(num1);
	putchar('\n');
	print_nbr(num2);
	putchar('=');*/
	/*printf("= = = = = = = =\n");
	printf("%lu :: %i, %lu :: %i\n", num1->bdot, num1->dot, num2->bdot, num2->dot);
	for(b = num1->num;b;b = b->next)
		printf("%lu :: %i :: %i\n", b->num, b->nmemb, b->full);
	printf("<<<<<>>>>>\n");
	for(b = num2->num;b;b = b->next)
		printf("%lu :: %i :: %i\n", b->num, b->nmemb, b->full);
	printf("= = = = = = = =\n");*/
	/*print_nbr(num1);
	putchar('-');
	print_nbr(num2);
	putchar('=');*/
	if(num1->neg && !num2->neg){
		num1->neg = 0;
		res = addition(num2, num1, result);
		num1->neg = res->neg = 1;
		return res;
	}else{
		if(!num1->neg && num2->neg){
			num2->neg = 0;
			res = addition(num2, num1, result);
			num2->neg = 1;
			return res;
		}else{
			if(num1->neg && num2->neg){
				num2->neg = 0;
				num1->neg = 0;
				res = soustraction(num2, num1, result);
				num2->neg = 1;
				num1->neg = 1;
				return res;
			}
		}
	}
	if(!result){
		if((res = calloc(1, sizeof(struct nbr))) == NULL){
			perror("calloc()");
			exit(EXIT_FAILURE);
		}
	}else
		res = result;
	if(equal(num1, num2) == 0){
		if(!result){
			if((res->num = calloc(1, sizeof(struct bin))) == NULL){
				perror("calloc()");
				exit(EXIT_FAILURE);
			}
		}
		res->num->alloc = 1;
		res->num->num = res->num->full = 0;
		res->num->nmemb = 1;
		res->val = 1;
		res->bval = res->bdot = res->dot = 0;
		for(pbr = res->num->next;pbr; pbr = pbr->next)
			pbr->num = pbr->nmemb = pbr->full = 0;
		return res;
	}
	/*res->bval = (num1->bval > num2->bval) ? num1->bval : num2->bval;
	res->bdot = (num1->bdot > num2->bdot) ? num1->bdot : num2->bdot;
	res->val = (num1->val > num2->val) ? num1->val : num2->val;
	res->dot = (num1->dot > num2->dot) ? num1->dot : num2->dot;
	pbr = res->num = new_num(res->bval + (res->val > 0), res->bdot + (res->dot > 0));*/
	if(equal(num1, num2) < 0){
		res->bval = num2->bval;
		res->val = num2->val;
		pb1 = num2->num;
		pb2 = num1->num;
		pn1 = num2;
		pn2 = num1;
		neg = 1;
	}else{
		res->bval = num1->bval;
		res->val = num1->val;
		pb1 = num1->num;
		pb2 = num2->num;
		pn1 = num1;
		pn2 = num2;
	}
	if(num1->bdot > num2->bdot || (num1->bdot == num2->bdot && num1->dot > num2->dot)){
		res->bdot = num1->bdot;
		res->dot = num1->dot;
	}else{
		res->bdot = num2->bdot;
		res->dot = num2->dot;
	}
	if(!result)
		pbr = res->num = new_num(res->bval + (res->val > 0), res->bdot + (res->dot > 0));
	else
		pbr = res->num;
	if(pn1->bdot > pn2->bdot || (pn1->bdot == pn2->bdot && pn1->dot > pn2->dot)){
		cbdot = pn1->bdot - pn2->bdot;
		if(pn1->dot >= pn2->dot)
			cdot = pn1->dot - pn2->dot;
		else{
			cdot = BLK + pn1->dot - pn2->dot;
			cbdot--;
		}
		/*cdot = pn1->dot - pn2->dot;*/
		for(i = cbdot; i > 0; i--){
			pbr->num = pb1->num;
			pbr->nmemb = pb1->nmemb;
			pbr->full = pb1->full;
			if(pbr->full)
				cbdot--;
			else{
				cdot += BLK;
				cbdot--;
				cdot -= pbr->nmemb;
			}
			pb1 = pb1->next;
			pbr = pbr->next;
		}
		if(cdot >= pb1->nmemb){
			pbr->full = pb1->full;
			pbr->nmemb = pb1->nmemb;
			pbr->num = pb1->num;
			cdot -= pbr->nmemb;
			pbr = pbr->next;
			pb1 = pb1->next;
		}
		if(cdot){
			pbr->nmemb = pb1->nmemb;
			pbr->full = pb1->full;
			cf = mul[cdot];
			cdot = mul[(pb2->nmemb > pb1->nmemb) ? pb2->nmemb : pb1->nmemb];
			pbr->num = pb1->num;
			pbr->num -= (pbr->num/cf)*cf;
			if((n1 = (pb1->num/cf)*cf) >= (n2 = pb2->num*cf)){
				pbr->num += n1 - n2;
			}else{
				pbr->num += n1 - n2 + cdot;
				retenue = 1;
			}
			pb2 = pb2->next;
			pbr = pbr->next;
			pb1 = pb1->next;
		}
	}else{
		if(pn1->bdot < pn2->bdot || (pn1->dot < pn2->dot && pn1->bdot == pn2->bdot)){
			cbdot = pn2->bdot - pn1->bdot;
			if(pn2->dot >= pn1->dot)
				cdot = pn2->dot - pn1->dot;
			else{
				cdot = BLK + pn2->dot - pn1->dot;
				cbdot--;
			}
			for(i = cbdot; i > 0; i--){
				pbr->nmemb = pb2->nmemb;
				pbr->full = pb2->full;
				if(pbr->full)
					cbdot--;
				else{
					cdot += BLK;
					cbdot--;
					cdot -= pbr->nmemb;
				}
				cf = mul[pbr->nmemb];
				pbr->num = cf - pb2->num - retenue;
				retenue = 1;
				pb2 = pb2->next;
				pbr = pbr->next;
			}
			if(cdot >= pb2->nmemb){
				pbr->nmemb = pb2->nmemb;
				pbr->full = pb2->full;
				cf = mul[pbr->nmemb];
				pbr->num = cf - pb2->num - retenue;
				retenue = 1;
				cdot -= pbr->nmemb;
				pbr = pbr->next;
				pb2 = pb2->next;
			}
			if(cdot){
				pbr->nmemb = pb2->nmemb;
				pbr->full = pb2->full;
				cf = mul[cdot];
				cdot = mul[(pb2->nmemb > pb1->nmemb) ? pb2->nmemb : pb1->nmemb];
				pbr->num = pb2->num;
				pbr->num -= (pbr->num/cf)*cf;
				pbr->num = cf - pbr->num - retenue;
				retenue = 1;
				if((n1 = pb1->num*cf) > (n2 = ((pb2->num/cf))*cf)){
					pbr->num += n1 - n2 - cf;
					retenue = 0;
				}else{
					pbr->num += n1 - n2 + cdot - cf;
					/*pbr->num += cdot - n2 + n1 - cf;*/
					retenue = 1;
				}
				pbr = pbr->next;
				pb1 = pb1->next;
				pb2 = pb2->next;
			}
		}
	}
	for(	nmemb1 = pb1->nmemb,
		nmemb2 = pb2->nmemb,
		n1 = pb1->num,
		n2 = pb2->num;
		pbr;
		pbr = pbr->next
	){
		/*printf("%lu :: %lu\n", n1, n2);*/
		if(n1 >= n2 + retenue){
			pbr->num = n1 - n2 - retenue;
			pbr->nmemb = (nmemb1 > nmemb2) ? nmemb1 : nmemb2;
			retenue = 0;
		}else{
			pbr->nmemb = (nmemb1 > nmemb2) ? nmemb1 : nmemb2;
			cf = mul[pbr->nmemb];
			pbr->num = cf + n1 - n2 - retenue;
			retenue = 1;
		}
		if(pbr->nmemb == BLK)
			pbr->full = 1;
		if(pb1){
			pb1 = pb1->next;
			if(pb1){
				n1 = pb1->num;
				nmemb1 = pb1->nmemb;
			}else
				nmemb1 = n1 = 0;
		}else
			nmemb1 = n1 = 0;
		if(pb2){
			pb2 = pb2->next;
			if(pb2){
				n2 = pb2->num;
				nmemb2 = pb2->nmemb;
			}else
				nmemb2 = n2 = 0;
		}else
			nmemb2 = n2 = 0;
	}
	if(retenue){
		if(res->num->prev){
			res->num->prev->num += retenue*mul[res->num->prev->nmemb-1];
		}else
			res->num->num += retenue*mul[res->num->nmemb-1];
	}
	/*print_nbr(res);
	putchar('\n');*/
	/*for(pb1 = res->num; pb1; pb1 = pb1->next)
		printf("%lu :: %i :: %i\n", pb1->num, pb1->nmemb, pb1->full);
	print_nbr(res);
	printf("=>%lu :: %i, %lu :: %i\n", res->bval, res->val, res->bdot, res->dot);*/
	/*if(res->num->prev && res->num->prev->nmemb == 0 && res->num->prev->num == 0){
		pb1 = res->num->prev;
		res->num->prev = pb1->prev;
		pb1->prev->next = NULL;
		free(pb1);
	}*/
	/*if(dres)
		destroy_nbr(dres);*/
	/*if(nadd)
		destroy_nbr(nadd);*/
	ADJUST_0(res, pb1, pb2);
	res->neg = neg;
	/*print_nbr(res);
	printf("\nEND SOUSTRACTION\n");*/
	/*print_nbr(res);
	printf("~>%lu :: %i\n", res->bval, res->val);*/
	/*for(pb1 = num1->num;pb1;pb1 = pb1->next)
		printf("%lu :: %i :: %i\n", pb1->num, pb1->nmemb, pb1->full);
	for(pb1 = num2->num;pb1;pb1 = pb1->next)
		printf("%lu :: %i :: %i\n", pb1->num, pb1->nmemb, pb1->full);
	for(pb1 = res->num;pb1;pb1 = pb1->next)
		printf("%lu :: %i :: %i\n", pb1->num, pb1->nmemb, pb1->full);
	printf("%lu :: %i, %lu, %i\n", num1->bval, num1->val, num2->bval, num2->val);*/
	/*if(nadd)
		destroy_nbr(nadd);*/
	return res;
}
void *multiplication(struct nbr *num1, struct nbr *num2, struct nbr *result){
	struct bin *bn1, *bn2, *pbr1, *pbr2, *pbr3, z = ZERO_BIN;
	struct nbr *res, zero = ZERO( NULL );
	unsigned long int n1, n2, n3, dot, bdot, dot_, val, bval, retenue;
	int ret, nmemb1, rnmemb1, nmemb2, rnmemb2, nmemb3, rnmemb3, mul[C_BLK] = COEFS;
	zero.num = &z;
	/*printf("MULTIPLICATION\n");
	print_nbr(num1);
	putchar('*');
	print_nbr(num2);
	putchar('\n');*/
	if(!result){
		if((res = calloc(1, sizeof(struct nbr))) == NULL){
			perror("calloc()");
			exit(EXIT_FAILURE);
		}
	}else
		res = result;
	if(equal(num1, &zero) == 0 || equal(num2, &zero) == 0){
		if(!result){
			if((res->num = calloc(1, sizeof(struct bin))) == NULL){
				perror("calloc()");
				exit(EXIT_FAILURE);
			}
			res->num->alloc = 1;
		}else
			res = result;
		res->num->num = 0;
		res->num->nmemb = 1;
		res->val = 1;
		return res;
	}
	bval = num1->bval + num2->bval;
	val = num1->val + num2->val;
	if(val >= BLK){
		val -= BLK;
		bval++;
	}
	bdot = res->bdot = num1->bdot + num2->bdot;
	dot = res->dot = num1->dot + num2->dot;
	if(dot >= BLK){
		dot -= BLK;
		res->dot -= BLK;
		res->bdot = ++bdot;
	}
	if(!result)
		pbr1 = res->num = new_num(bval + (val > 0), res->bdot + (res->dot > 0));
	else
		pbr1 = result->num;
	for(	dot_ = dot,
		ret = rnmemb1 = nmemb1 = 0,
		bn1 = num1->num,
		pbr2 = pbr1 = res->num;
		n1 = 0,
		bn1;
		pbr2 = pbr1
	){
		while(bn1 && nmemb1 < BLK){
			if(nmemb1 + bn1->nmemb - rnmemb1 > BLK){
				rnmemb1 = nmemb1 + bn1->nmemb - BLK;
				n1 += (bn1->num%mul[bn1->nmemb-rnmemb1])*mul[nmemb1];
				nmemb1 = BLK;
				break;
			}else{
				if(rnmemb1){
					n1 = bn1->num/mul[bn1->nmemb - rnmemb1];
					nmemb1 = rnmemb1;
				}else{
					n1 += bn1->num*mul[nmemb1];
					nmemb1 += bn1->nmemb;
				}
				rnmemb1 = 0;
			}
			bn1 = bn1->next;
		}
		for(	bn2 = num2->num,
			dot = dot_,
			rnmemb2 = rnmemb3 = nmemb2 = nmemb3 = 0,
			retenue = 0;
			n2 = n3 = 0,
			bn2;
		){
			while(bn2 && nmemb2 < BLK){
				if(nmemb2 + bn2->nmemb -rnmemb2 > BLK){
					rnmemb2 = nmemb2 + bn2->nmemb -BLK;
					n2 += (bn2->num%mul[bn2->nmemb - rnmemb2])*mul[nmemb2];
					nmemb2 = BLK;
					break;
				}else{
					if(rnmemb2){
						n2 = bn2->num/mul[bn2->nmemb-rnmemb2];
						nmemb2 = rnmemb2;
					}else{
						n2 += bn2->num*mul[nmemb2];
						nmemb2 += bn2->nmemb;
					}
					rnmemb2 = 0;
				}
				bn2 = bn2->next;
			}
			for(pbr3 = pbr2; pbr3 && nmemb3 < BLK;){
				n3 += ((pbr3->num)*mul[nmemb3]);
				nmemb3 += pbr3->nmemb;
				if(n3 == 0){
					n3 = pbr3->num;
					nmemb3 = pbr3->nmemb;
				}
				pbr3 = pbr3->next;
			}
			pbr2->num = n1 * n2 + n3;
			retenue = ret = 0;
			if(nmemb2+nmemb1 != 0)
				pbr2->nmemb = nmemb2 + nmemb1;
			if(pbr2->nmemb > BLK){
				ret = pbr2->nmemb - BLK;
				retenue = pbr2->num/mul[BLK];
				pbr2->num -= retenue * mul[BLK];
				pbr2->nmemb = BLK;
				if(retenue/mul[ret-1] == 0){
					--ret;
				}
			}
			if(pbr2->nmemb == BLK)
				pbr2->full = 1;
			if(nmemb2 == BLK){
				nmemb2 = 0;
			}
			nmemb3 = 0;
			if(pbr2->next){
				pbr2 = pbr2->next;
				pbr2->num += retenue;
				if(pbr2->nmemb < ret)
					pbr2->nmemb = ret;
				if(pbr2->num/mul[pbr2->nmemb] != 0)
					pbr2->nmemb++;
				retenue = pbr2->prev->num/mul[BLK-1];
				if(retenue == 0)
					ret = 0;
			}
			/*if(pbr2->num == 61492332 && pbr2->nmemb == 0)
				exit(0);*/
		}
		if(nmemb1 == BLK){
			nmemb1 = 0;
		}
		ret = 0;
		retenue = 0;
		pbr1 = pbr1->next;
	}
	/*print_nbr(res);
	putchar('\n');*/
	/*for(pbr1 = res->num;pbr1;pbr1 = pbr1->next)
		printf(">%lu :: %i :: %i\n", pbr1->num, pbr1->nmemb, pbr1->full);*/
	if(dot){
		if(result){
			pbr1 = res->num->prev;
			while(pbr1->nmemb == 0)
				pbr1 = pbr1->prev;
			/*if(pbr1->next && pbr1->nmemb+dot > BLK)*/
			if(pbr1->next)
				pbr2 = pbr1 = pbr1->next;
		}else
			pbr2 = pbr1 = res->num->prev;
		pbr1->num *= mul[pbr1->prev->nmemb - dot];
		pbr1->num += pbr1->prev->num/mul[dot];
		pbr1->prev->num -= (pbr1->prev->num/mul[dot])*mul[dot];
		pbr1->nmemb += pbr1->prev->nmemb - dot;
		if(pbr1->num == 0)
			pbr1->nmemb = 0;
		pbr1->prev->full = 0;
		for(pbr1 = pbr1->prev; pbr1 && pbr1 != res->num; pbr1 = pbr1->prev){
			pbr1->num *= mul[BLK-dot];
			pbr1->num += pbr1->prev->num/mul[dot];
			pbr1->nmemb = BLK;
			pbr1->full = 1;
			pbr1->prev->num -= (pbr1->prev->num/mul[dot])*mul[dot];
		}
		pbr1->num %= mul[dot];
		pbr1->nmemb = dot;
		pbr1->full = 0;
	}
	for(res->val = 0, pbr1 = res->num; pbr1; pbr1 = pbr1->next){
		if((pbr1->full = (pbr1->nmemb == BLK))){
			res->bval++;
		}else
			res->val += pbr1->nmemb;
	}
	res->bval -= res->bdot;
	res->dot = dot;
	res->bdot = bdot;
	if(res->val >= res->dot)
		res->val -= res->dot;
	else{
		res->val = BLK + res->val - res->dot;
		res->bval--;
	}
	if(res->val == 0 && res->bval == 0)
		res->val = pbr2->nmemb = 1;
	/*for(pbr1 = res->num;pbr1;pbr1 = pbr1->next)
		printf("%lu :: %i :: %i\n", pbr1->num, pbr1->nmemb, pbr1->full);
	print_nbr(res);
	putchar('\n');*/
	/*exit(0);*/
	ADJUST_0(res, pbr1, pbr2);
	if(num1->neg != num2->neg)
		res->neg = 1;
	/*printf("FIN MULTIPLICATION\n");*/
	/*print_nbr(res);
	putchar('\n');*/
	return res;
}
#define DEBUG 0
#define SPOW 1
#define ISPOW 1
#if ISPOW == 0
void *ispuissance(struct nbr *num, int pui){
	struct bin _un_ = { 1, 1, 0, 0, NULL, NULL };
	struct nbr *n = num, *res = NULL, un = INIT_NBR( 0, 0, 1, 0, 0, NULL, NULL ), *pseudo = &un, *p;
	int comp = pui;
	un.num = &_un_;
	#if DEBUG == 1
	printf("ISPUI = %i\n", pui);
	#endif
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
		/*print_nbr(res);
		putchar('\n');*/
		return res;
	}
	while(comp > 1){
		if(comp%2){
			comp--;
			p = multiplication(n, pseudo, NULL);
			if(pseudo != &un)
				destroy_nbr(pseudo);
			pseudo = p;
		}else{
			comp /= 2;
			res = multiplication(n, n, NULL);
			if(n != num)
				destroy_nbr(n);
			n = res;
		}
	}
	if(pseudo != &un){
		res = multiplication(pseudo, n, NULL);
		destroy_nbr(pseudo);
		if(n != num)
			destroy_nbr(n);
	}else{
		if(n == num)
			res = dup_nbr(n);
	}
	#if DEBUG == 1
	print_nbr(res);
	putchar('\n');
	#endif
	return res;
}
#else
void *ispuissance(struct nbr *num, int pui){
	struct bin _un_ = { 1, 1, 0, 0, NULL, NULL };
	struct nbr *r[2], *n = num, *res = NULL, un = INIT_NBR( 0, 0, 1, 0, 0, NULL, NULL ), *pseudo[2];
	int comp = pui, i, j;
	unsigned long int n_bdot = num->bdot, n_dot = num->dot, n_bval = num->bval, n_val = num->val;
	un.num = &_un_;
	#if DEBUG == 1
	printf("ISPUI = %i\n", pui);
	#endif
	/*printf("%i\n", pui);*/
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
		/*printf(">>%i\n", comp);*/
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
	/*if(equal(pseudo[j], &un) == 0){
		res = multiplication(pseudo[j], n, pseudo[!j]);
	}else{
		if(pui == 1){
			res = dup_nbr(num);
		}
	}*/
	for(i = 0; i < 2;i++){
		if(res != r[i])
			destroy_nbr(r[i]);
		if(res != pseudo[i])
			destroy_nbr(pseudo[i]);
	}
	#if DEBUG == 1
	print_nbr(res);
	putchar('\n');
	#endif
	return res;
}
#endif
#if SPOW == 0
void *spuissance(struct nbr *num, unsigned long int bpui, int pui){
	struct bin _un_ = { 1, 1, 0, 0, NULL, NULL };
	struct nbr *n = num/*, *tn*/, *in, *res = NULL, un = INIT_NBR( 0, 0, 1, 0, 0, NULL, NULL ), *pseudo = &un, *p;
	unsigned long int bcomp = bpui;
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
	n = ispuissance(num, BLK);
	while(bcomp > 1){
		if(bcomp%2){
			bcomp--;
			p = multiplication(n, pseudo, NULL);
			if(pseudo != &un)
				destroy_nbr(pseudo);
			pseudo = p;
		}else{
			bcomp /= 2;
			res = multiplication(n, n, NULL);
			if(n != num)
				destroy_nbr(n);
			n = res;
		}
	}
	if(pseudo != &un){
		res = multiplication(pseudo, n, NULL);
		destroy_nbr(pseudo);
		if(n != num){
			destroy_nbr(n);
			n = NULL;
		}
	}else{
		if(n == num)
			res = n;
	}
	if(n == NULL)
		n = res;
	res = multiplication(n, in, NULL);
	destroy_nbr(n);
	destroy_nbr(in);
	return res;
}
#else
void *spuissance(struct nbr *num, unsigned long int bpui, int pui){
	struct bin _un_ = { 1, 1, 0, 0, NULL, NULL };
	struct nbr *r[2] = { NULL, NULL }, *pseudo[2], *n = num/*, *tn*/, *in, *res = NULL, un = INIT_NBR( 0, 0, 1, 0, 0, NULL, NULL )/*, *pseudo = &un, *p*/;
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
#endif
#define BYMIN10 1
#if BYMIN10 == 0
void *bymin10(struct nbr *num, unsigned long int bscale, int scale){
	struct nbr *res = NULL;
	struct bin *bs, *bt;
	unsigned long int bval, blen;
	int x, val, len, mul[C_BLK] = COEFS;
	if(num->bval < bscale || (num->bval == bscale && num->val <= scale)){
		if((res = calloc(1,sizeof(struct nbr))) == NULL){
			perror("calloc()");
			exit(EXIT_FAILURE);
		}
		res->num = new_num(1, bscale + (scale > 0));
		for(	bt = (num->num->prev) ? num->num->prev : num->num,
			bs = res->num->prev->prev,
			bval = bscale - num->bval,
			(scale >= num->val && (val = scale - num->val) >= 0) || (bval--, val = BLK + scale - num->val),
			len = num->val,
			blen = num->bval;
			;
		){
			if(val || bval){
				if(bval){
					bs->nmemb = BLK;
					bs->full = 1;
					bval--;
					res->bdot++;
				}else{
					bs->nmemb = val;
					val -= bs->nmemb;
					res->dot += bs->nmemb;
				}
			}else{
				res->dot += bt->nmemb;
				if(res->dot >= BLK){
					res->dot -= BLK;
					res->bdot++;
				}
				bs->num *= mul[bt->nmemb];
				bs->num += bt->num;
				bs->nmemb += bt->nmemb;
				if(bs->nmemb > BLK){
					bs->prev->num = bs->num%mul[bs->nmemb - BLK];
					bs->num /= mul[bs->nmemb - BLK];
					bs->prev->nmemb = bs->nmemb - BLK;
					bs->nmemb = BLK;
					bs->full = 1;
				}
				if(bt->full){
					blen--;
				}else{
					len -= bt->nmemb;
				}
				if(bt == num->num)
					break;
				bt = bt->prev;
			}
			if(bs->nmemb >= BLK)
				bs = bs->prev;
		}
		res->val = res->num->prev->nmemb = 1;
	}else{
		if(bscale || scale){
			if((res = calloc(1,sizeof(struct nbr))) == NULL){
				perror("calloc()");
				exit(EXIT_FAILURE);
			}
			if(num->val > scale)
				res->num = new_num(num->bval - bscale +1, bscale + (scale > 0));
			else
				res->num = new_num(num->bval - bscale /*+ (BLK + num->val - scale > 0)*/ , bscale + (scale > 0));
			x = scale;
			bt = num->num;
			bs = res->num;
			if(x){
				bs->num = bt->num%mul[x];
				bs->nmemb = x;
				bs->full = 0;
				printf("%lu :: %lu\n", bs->num, bt->num);
				bs = bs->next;
			}
			for(blen = bscale; bs;blen--, bs = bs->next){
				bs->num = bt->num/mul[x];
				bs->nmemb = bt->nmemb-x;
				printf("=> %lu :: %lu\n", bs->num, bt->num);
				if(!(bt = bt->next))
					break;
				bs->num += (bt->num%mul[x])*mul[BLK-x];
				bs->nmemb = BLK;
				bs->full = 1;
				bs->num -= (bs->num/mul[BLK]) * mul[BLK];
				printf("-> %lu :: %lu\n", bs->num, bt->num);
			}
			exit(0);
			if(num->val > scale){
				res->num->prev->nmemb = res->val = num->val - scale;
				res->bval = num->bval - bscale;
			}else{
				res->num->prev->nmemb = res->val = BLK + num->val - scale;
				res->bval = num->bval - bscale - 1;
			}
			if(res->val >= BLK){
				res->val -= BLK;
				res->bval++;
			}
			res->dot = scale;
			res->bdot = bscale;
			if(res->num->prev)
				res->num->prev->full = (res->num->prev->nmemb == BLK);
			if(res->num->prev->nmemb == 0)
				res->num->prev->nmemb = res->val = 1;
		}else{
			res = num;
		}
	}
	return res;
}
#else
void *bymin10(struct nbr *num, unsigned long int bscale, int scale){
	struct nbr *res = NULL;
	struct bin *bs, *bt, *br = NULL;
	unsigned long int bval, rtemp[3] = { 0, 0, 0 }, rtemp_, btemp;
	int val, mul[C_BLK] = COEFS, mb[3] = { 0, 0, 0 }, init = 0;
	res = num;
	bval = num->bval;
	val = num->val;
	if(num->bval > bscale || (num->bval == bscale && num->val > scale)){
		if(bval > 1 || (bval == 1 && val > 0)){
			res->bval -= bscale;
			res->bdot = bscale;
			if(res->val < scale){
				res->val += BLK;
				res->val -= scale;
				res->bval--;
			}else
				res->val -= scale;
			bt = num->num;
			bs = res->num;
			if(scale == 0)
				return res;
			rtemp[0] = bs->num;
			res->dot = scale;
			rtemp[1] = rtemp[0]%mul[scale];
			mb[1] = scale;
			mb[0] = BLK;
			bs->nmemb = scale;
			bs->full = (bt->nmemb == BLK);
			for(rtemp_ = rtemp[0]/mul[scale],bs = bs->next, rtemp_ = bt->num;;){
				rtemp[0] = rtemp_;
				if(bs)
					rtemp_ = bs->num;
				if(mb[2]){
					if(br == NULL)
						br = res->num;
					else{
						br = br->next;
					}
					br->num = rtemp[2];
					if(!br || mb[1] == 0){
						br->nmemb = res->val;
						br->full = (br->nmemb == BLK);
						break;
					}else{
						br->nmemb = mb[2];
						br->full = (br->nmemb == BLK);
					}
				}
				mb[2] = mb[1];
				mb[1] = mb[0];
				mb[0] = (!bs || bs->nmemb < scale) ? 0 : BLK;
				rtemp[2] = rtemp[1];
				rtemp[1] = rtemp_%mul[scale]*mul[BLK-scale] + rtemp[0]/mul[scale];
				if(bs)
					bs = bs->next;
				if(bt)
					bt = bt->next;
			}
		}else{
			if(res->val < scale){
				res->val += BLK;
				res->val -= scale;
				res->bval--;
			}else
				res->val -= scale;
			rtemp_ = res->num->num;
			res->num->nmemb = res->dot = scale;
			res->num->num %= mul[scale];
			res->num->full = 0;
			res->num->next->num = rtemp_/mul[scale];
			res->num->next->nmemb = res->val;
		}
	}else{
		if(res->num->next){
			res->bval = 0;
			res->bdot = bscale;
			res->val = 1;
			res->dot = scale;
			bs = num->num;
			bt = res->num;
			rtemp[0] = bs->num;
			rtemp[1] = rtemp[0]%mul[scale];
			bt->num = rtemp[1];
			bt->nmemb = scale;
			bt->full = (bt->nmemb == BLK);
			if(bt->nmemb != 0)
				bt = bt->next;
			for(bs = bs->next, btemp = bscale ;bs->next && bs->nmemb != 0;btemp--,bs = bs->next, bt = bt->next){
				if(rtemp[1])
					rtemp[2] = rtemp[1];
				rtemp[1] = (rtemp[0] / mul[scale]) + bs->num%mul[scale]*mul[BLK-scale];
				rtemp[0] = bs->num;
				bt->nmemb = BLK;
				bt->full = 1;
				bt->num = rtemp[1];
			}
			if(btemp > 0){
				bt->num = rtemp[0]/mul[scale];
				bt->nmemb = BLK;
				bt->full = 1;
				bt = bt->next;
				btemp--;
			}
			for(;btemp > 0;btemp--, bt = bt->next){
				bt->nmemb = BLK;
				bt->full = 1;
				bt->num = 0;
			}
		}else{
			bt = res->num;
		}
		bt->nmemb = 1;
		bt->num = 0;
	}
	return res;
}
#endif
void *mv_dot(struct nbr *num, struct nbr *result, unsigned long int bscale, int scale){
	struct bin *bn, *nmv, *end;
	struct nbr *n;
	unsigned long int _bscale = 0, ubscale = 0, r_val, bs, stbdot, stbval;
	int _scale = 0, uscale = 0, rb_val, init = 0, mul[C_BLK] = COEFS, s, stdot, stval;
	stval = num->val;
	stbval = num->bval;
	stdot = num->dot;
	stbdot = num->bdot;
	if(scale == 0 && bscale <= stbdot){
		if(result == NULL)
			n = dup_nbr(num);
		else{
			n = result;
			if(n != num)
			num_cpy(n, num);
		}
		n->bval += bscale;
		n->bdot -= bscale;
		ADJUST_0(n, nmv, bn);
		return n;
	}
	if(bscale < stbdot || (bscale == stbdot && scale <= stdot)){
		if(bscale < stbdot || scale < stdot){
			init = 2;
		}
		if(result == NULL){
			if((n = calloc(1, sizeof(struct nbr))) == NULL){
				perror("calloc()");
				exit(EXIT_FAILURE);
			}
			n->num = new_num(stbval + (stval > 0) +2, stbdot + (stdot > 0));
		}else
			n = result;
		if(n != num)
			num_cpy(n, num);
		for(bn = n->num;bn->next && bn->next->nmemb != 0; bn = bn->next);
		n->bval += bscale;
		n->bdot -= bscale;
		n->val += scale;
		if(n->val >= BLK){
			n->val -= BLK;
			n->bval++;
		}
		if(n->dot >= scale){
			n->dot -= scale;
		}else{
			n->dot = BLK + n->dot - scale;
			n->bdot--;
		}
		s = scale;
		bs = bscale;
		end = bn;
	}else{
		s = stdot;
		bs = stbdot;
		_bscale = stbval + bscale;
		_scale = stval + scale;
		if(_scale >= BLK){
			_scale -= BLK;
			_bscale++;
		}
		if(result == NULL){
			if((n = calloc(1, sizeof(struct nbr))) == NULL){
				perror("calloc()");
				exit(EXIT_FAILURE);
			}
			n->num = new_num(_bscale + (_scale > 0) + stbval, 1);
		}else
			n = result;
		if(result != num)
			num_cpy(n, num);
		n->dot = n->bdot = 0;
		for(bn = n->num;bn->next && bn->next->nmemb != 0; bn = bn->next);
		n->bval = stbval + bs;
		n->val = stval + s;
		end = bn;
		if(n->val >= BLK){
			n->val -= BLK;
			n->bval++;
		}
		uscale = scale + stval;
		ubscale = bscale + stbval;
		if(uscale < 0){
			uscale += BLK;
			ubscale--;
		}else
			if(uscale >= BLK){
				ubscale++;
				uscale -= BLK;
			}
		r_val = uscale;
		rb_val = ubscale;
	}
	if(stdot || stbdot){
		if(bn->nmemb + s > BLK){
			nmv = bn->next;
			nmv->num = bn->num / mul[BLK-s];
			nmv->nmemb = n->val;
			nmv->full = 0;
			nmv = nmv->prev;
			nmv->num = (bn->num % mul[BLK-s]) * mul[s] + bn->prev->num / mul[BLK-s];
			nmv->nmemb = BLK;
			nmv->full = 1;
			if(init == 0){
				for(nmv = nmv->prev, bn = bn->prev; nmv->prev->next != NULL; bn = bn->prev, nmv = nmv->prev){
					nmv->num = (bn->num % mul[bn->nmemb-s]) * mul[s] + bn->prev->num / mul[bn->prev->nmemb-s];
					nmv->nmemb = BLK;
					nmv->full = 1;
				}
				if(n->dot == 0 && n->bdot == 0)
					nmv->num = nmv->nmemb = 0;
			}else{
				for(nmv = nmv->prev, bn = bn->prev, bs+=2; bs; bs--, bn = bn->prev, nmv = nmv->prev){
					nmv->num = (bn->num % mul[bn->nmemb-s]) * mul[s] + bn->prev->num / mul[bn->prev->nmemb-s];
					nmv->nmemb = BLK;
					nmv->full = 1;
				}
				for(;bn->prev->next != NULL && bn->prev->nmemb >= s; bn = bn->prev){
					bn->num = (bn->num % mul[bn->nmemb-s])
							* mul[s] + bn->prev->num / mul[bn->prev->nmemb-s];
					bn->nmemb = BLK;
					bn->full = 1;
				}
				bn->num %= mul[BLK-s];
				bn->num *= mul[bn->prev->nmemb];
				bn->num += bn->prev->num;
				n->dot = bn->nmemb = BLK - s + bn->prev->nmemb;
				if(bn->nmemb == BLK){
					n->dot = 0;
					n->bdot++;
					bn->full = 1;
				}
				if(bn->nmemb >= BLK){
					bn->prev->num = bn->num%mul[bn->nmemb - BLK];
					bn->num/=mul[bn->nmemb-BLK];
					n->dot = bn->prev->nmemb = bn->nmemb - BLK;
					bn->nmemb = BLK;
					bn->full = 1;
					bn->prev->full = 0;
					bn->prev->num = bn->prev->nmemb = 0;
				}else{
					bn->full = 0;
					bn->prev->full = 0;
					bn->prev->nmemb = 0;
					bn->prev->num = 0;
					if(stdot >= s){
						n->dot = bn->nmemb = stdot - s;
						bn->num %= mul[bn->nmemb];
					}
				}
			}
		}else{
			for(nmv = bn->prev;nmv != end && nmv->nmemb != 0;bn = bn->prev, nmv = nmv->prev){
				if(nmv->nmemb < s){
					goto end;
				}
				bn->num *= mul[s];
				bn->num += nmv->num / mul[nmv->nmemb-s];
				nmv->num %= mul[nmv->nmemb-s];
				nmv->nmemb -= s;
				bn->nmemb += s;
				bn->full = (bn->nmemb == BLK);
			}
			goto after;
			end:;
			bn->num *= mul[bn->prev->nmemb];
			bn->num += bn->prev->num;
			bn->nmemb += bn->prev->nmemb;
			bn->full = (bn->nmemb == BLK);
			bn->prev->num = 0;
			bn->prev->nmemb = 0;
			bn->prev->full = 0;
			after:;
		}
	}
	if(n->num->nmemb == 0){
		for(nmv = bn = n->num;bn->nmemb == 0; bn = bn->next);
		for(;bn;nmv = nmv->next, bn = bn->next){
			nmv->full = bn->full;
			nmv->nmemb = bn->nmemb;
			nmv->num = bn->num;
		}
		nmv->nmemb = nmv->full = nmv->num = 0;
	}
	if(uscale || ubscale){
		ubscale -= n->bval;
		uscale -= n->val;
		if(uscale < 0){
			ubscale--;
			uscale += BLK;
		}
		for(nmv = bn = n->num; bn->next && bn->next->nmemb != 0; nmv = bn = bn->next);
		for(;ubscale > 0; ubscale--, bn = bn->next);
		if(n->val == 0 || n->val + uscale >= BLK){
			bn->next->num = nmv->num * mul[uscale] / mul[BLK];
			bn->next->nmemb = r_val;
			bn->num %= mul[BLK-uscale];
		}else{
			bn->num = nmv->num * mul[uscale] + nmv->prev->num / mul[BLK-uscale];
			bn->prev->num %= mul[BLK - uscale];
			bn->nmemb = r_val;
			bn = bn->prev;
			nmv = nmv->prev;
		}
		if(nmv != bn){
			for(init = 1;nmv != n->num->prev;nmv->num = 0, nmv->nmemb = BLK, nmv->full = 1,bn = bn->prev, nmv = nmv->prev){
				bn->num = (nmv->num * mul[uscale] + nmv->prev->num / mul[BLK-uscale])%mul[BLK];
				bn->prev->num %= mul[BLK - uscale];
				bn->full = 1;
				bn->nmemb = BLK;
			}
		}else{
			for(;nmv != n->num->prev;bn = bn->prev, nmv = nmv->prev){
				bn->num = (nmv->num * mul[uscale] + nmv->prev->num / mul[BLK-uscale])%mul[BLK];
				bn->prev->num %= mul[BLK - uscale];
				bn->full = 1;
				bn->nmemb = BLK;
			}
			for(;bn->next;bn->num = 0, bn = bn->prev);
		}
		n->val = r_val;
		n->bval = rb_val;
		if(init == 1){
			for(bn = bn;bn->next != NULL;bn = bn->prev){
				bn->num = 0;
				bn->full = 1;
				bn->nmemb = BLK;
			}
		}
	}
	ADJUST_0(n, nmv, bn);
	return n;
}
#define PRINT_NBR(n) \
	print_nbr(n); \
	putchar('\n');
#define M_BLK 2
void *division(struct nbr *num1, struct nbr *num2, struct nbr **modulo, unsigned long int bscale, int scale, int approximation){
	struct retbcpy *bcpy;
	struct bin *bdividende, *breste, *b,
		bdix = { 10, 2, 0, 0, NULL, NULL }, bsingle = { 0, 1, 0, 0, NULL, NULL }, *bs = NULL, *bt, bx = { 0, 1, 0, 0, NULL, NULL };
	struct nbr *res = NULL, *diviseur, *dividende, *quotient, *mod = NULL, *reste = NULL,
		dix = INIT_NBR( 0, 0, 2, 0, 0, NULL, "10" ), *temp, *temp_,
		nx = INIT_NBR( 0, 0, 1, 0, 0, NULL, NULL );
	unsigned long int blk, bval, blen, mbdot,bdot_0 = 0, cbscale = 0, binit, lsc;
	int neg1, neg2, x, a = 0, start = 0, val, len, dot_0 = 0, cscale = 0, sclen, mdot, mul[C_BLK] = COEFS, init, sc;
	dix.num = &bdix;
	nx.num = &bx;
	if(equal(num2, &nx) == 0){
		fprintf(stderr, "Division by 0\n");
		return NULL;
	}
	if(equal(num1, &nx) == 0){
		if((res = calloc(1, sizeof(struct nbr))) == NULL){
			perror("calloc()");
			exit(EXIT_FAILURE);
		}
		res->num = new_num(1, 0);
		if(modulo){
			if((*modulo = calloc(1, sizeof(struct nbr))) == NULL){
				perror("calloc()");
				exit(EXIT_FAILURE);
			}
			(*modulo)->num = new_num(1, 0);
			res->val = res->num->nmemb = (*modulo)->val = (*modulo)->num->nmemb = 1;
		}else
			res->val = res->num->nmemb = 1;
		return res;
	}
	if((neg1 = num1->neg)){
		num1->neg = 0;
	}
	if((neg2 = num2->neg)){
		num2->neg = 0;
	}
	/*if(num1->bval > bscale || (num1->bval == bscale && num1->val < scale)){
		cbscale = num1->bval;
		cscale = num1->val;
	}else{
		cbscale = bscale + num2->bval;
		cscale = scale + num2->val;
	}*/
	if(num2->dot || num2->bdot){
		bdot_0 = num2->bdot;
		dot_0 = num2->dot;
		diviseur = mv_dot(num2, NULL, bdot_0, dot_0);
		DOT(diviseur, bs, b);
	}else{
		diviseur = dup_nbr(num2);
	}
	if(scale || bscale || bdot_0 || dot_0){
		mbdot = bdot_0 + bscale;
		mdot = dot_0 + scale;
		if(mdot >= BLK){
			mdot -= BLK;
			mbdot++;
		}
		temp = mv_dot(num1, NULL, mbdot, mdot);
		dividende = temp;
	}else{
		dividende = dup_nbr(num1);
	}
	/*
		dividende
		diviseur
	*/
	/*PRINT_NBR(num1);
	PRINT_NBR(dividende);
	PRINT_NBR(num2)
	PRINT_NBR(diviseur);
	printf("==========\n");*/
	if(dividende->bdot || dividende->dot){
		if((mod = calloc(1, sizeof(struct nbr))) == NULL){
			perror("calloc()");
			exit(EXIT_FAILURE);
		}
		mod->num = new_num(dividende->bval + (dividende->val > 0)+1, num1->bdot + (num1->dot > 0));
		num_cpy(mod, dividende);
		for(bt = mod->num->prev;mod->val || mod->bval;bt = bt->prev){
			bt->num = 0;
			if(bt->full){
				mod->bval--;
				bt->nmemb = bt->full = 0;
			}else{
				mod->val -= bt->nmemb;
				bt->nmemb = 0;
			}
		}
		bt->next->nmemb = 1;
		mod->bval = 0;
		mod->val = 1;
		dot_0 += mod->dot;
		bdot_0 += mod->bdot;
		if(dot_0 >= BLK){
			dot_0 -= BLK;
			bdot_0++;
		}
	}
	bt = dividende->num;
	for(bt = dividende->num; dividende->bdot > 0 || dividende->dot > 0; bt = bt->next){
		if(dividende->dot){
			dividende->dot -= dividende->num->nmemb;
		}else{
			dividende->bdot--;
		}
		a = bt->num/mul[bt->nmemb-1];
	}
	for(bs = dividende->num, bt = bt; bt; bt = bt->next, bs = bs->next){
		bs->num = bt->num;
		bs->nmemb = bt->nmemb;
		bs->full = bt->full;
	}
	for(bs = bs;bs;bs = bs->next)
		bs->num = bs->nmemb = bs->full = 0;
	for(bdividende = (dividende->num->prev) ? dividende->num->prev: dividende->num; bdividende->nmemb == 0 && bdividende != dividende->num;bdividende = bdividende->prev);
	bval = dividende->bval;
	val = dividende->val;
	blen = diviseur->bval;
	len = diviseur->val;
	sc = scale + dot_0;
	lsc = bscale + bdot_0;
	if(sc >= BLK){
		sc -= BLK;
		lsc++;
	}
	if(blen < bval || (blen == bval && len <= val)){
		if((reste = calloc(1, sizeof(struct nbr))) == NULL){
			perror("calloc()");
			exit(EXIT_FAILURE);
		}
		binit = diviseur->bval;
		init = diviseur->val;
		if(lsc < num1->bdot || (lsc == num1->bdot && sc == num1->dot)){
			lsc = num1->bdot + bdot_0;
			sc = num1->dot + dot_0;
			if(sc >= BLK){
				sc -= BLK;
				lsc++;
			}
		}
		sclen = M_BLK;
		/*if(mod){
			dot_0 += mod->dot;
			bdot_0 += mod->bdot;
			if(dot_0 >= BLK){
				dot_0 -= BLK;
				bdot_0++;
			}
		}*/
		reste->num = new_num(num1->bval + num1->bdot + (num1->val > 0) + (num1->dot > 0), lsc + (sc > 0) + bdot_0 + (dot_0 > 0) + bdot_0 +(dot_0 > 0));
		/*reste->num = new_num(num1->bval + num1->bdot + (num1->val > 0) + (num1->dot > 0), lsc + (sc > 0));*/
		for(blk = binit + (init > 0), bt = reste->num; blk > 1; blk--, bt = bt->next);
		breste = bt;
		bcpy = nbytescpy(&breste, &bdividende, &start, binit, init);
		reste->val = bcpy->rbytes;
		reste->bval = bcpy->rblk;
		if(equal(reste, diviseur) < 0){
			if(reste->bval < dividende->bval || (reste->bval == dividende->bval && reste->val < dividende->val)){
				bs = &bsingle;
				bsingle.num = bsingle.nmemb = 0;
				nbytescpy(&bs, &bdividende, &start, 0, 1);
				bymul10(reste, mul[1], 1);
				reste->num->num += bs->num;
				val--;
			}
		}
		if((quotient = calloc(1, sizeof(struct nbr))) == NULL){
			perror("calloc()");
			exit(EXIT_FAILURE);
		}
		quotient->num = new_num(num1->bval + (num1->val > 0), bscale + (scale > 0));
		for(;;){
			for(bx.nmemb = 1, bx.num = 0, x = 0; (int)bx.num < mul[M_BLK]; bx.num++){
				if(equal(reste, diviseur) < 0){
					x = bx.num;
					break;
				}
				soustraction(reste, diviseur, reste);
			}
			quotient = bymul10(quotient, mul[sclen], sclen);
			quotient->num->num += x;
			if(val || bval){
				if((binit || init)){
					if(val >= init){
						val -= init;
						bval -= binit;
						binit = init = 0;
					}else{
						val += BLK - init;
						bval -= (binit+1);
						binit = 0;
						init = 0;
					}
				}else{
					if(val < sclen){
						if(bval > 0){
							val += BLK - sclen;
							bval--;
						}else{
							val -= sclen;
						}
					}else{
						val -= sclen;
					}
				}
			}
			if(!bval && !val)
				break;
			bs = &bsingle;
			bsingle.num = bsingle.nmemb = 0;
			if(val >= M_BLK || bval > 0){
				sclen = M_BLK;
			}else{
				sclen = val;
			}
			bcpy = nbytescpy(&bs, &bdividende, &start, 0, sclen);
			if(reste->bval == 0 && reste->val == 1 && reste->num->num == 0){
				while(bs->nmemb > 1 && bs->num / mul[bs->nmemb-1] == 0){
					bs->nmemb--;
				}
				reste->num->num = bs->num;
				reste->val = reste->num->nmemb = bs->nmemb;
			}else{
				bymul10(reste, mul[sclen], sclen);
				reste->num->num += bs->num;
			}
		}
	}else{
		if((quotient = calloc(1, sizeof(struct nbr))) == NULL){
			perror("calloc()");
			exit(EXIT_FAILURE);
		}
		if((quotient->num = calloc(1, sizeof(struct bin))) == NULL){
			perror("calloc()");
			exit(EXIT_FAILURE);
		}
		quotient->num->alloc = 1;
		quotient->val = quotient->num->nmemb = 1;
		if(modulo){
			if((reste = calloc(1, sizeof(struct nbr))) == NULL){
				perror("calloc()");
				exit(EXIT_FAILURE);
			}
			if(lsc < num1->bdot || (lsc == num1->bdot && sc == num1->dot)){
				lsc = num1->bdot + bdot_0;
				sc = num1->dot + dot_0;
				if(sc >= BLK){
					sc -= BLK;
					lsc++;
				}
			}
			reste->num = new_num(num1->bval + num1->bdot + (num1->val > 0) + (num1->dot > 0)+1, lsc + (sc > 0));
			num_cpy(reste, dividende);
		}
	}
	if(modulo && reste && !*modulo){
		*modulo = reste;
	}
	if(approximation && reste){
		temp = multiplication(reste, &dix, NULL);
		temp->num->num += a;
		for(bx.nmemb = 1, bx.num = 0, x = 0; (int)bx.num < 10; bx.num++){
			
			if(equal(temp, diviseur) < 0){
				destroy_nbr(temp);
				x = bx.num;
				break;
			}
			(void)soustraction(temp, diviseur, temp);
		}
		if(bx.num >= 5){
			bx.num = 1;
			(void)addition(&nx, quotient, quotient);
		}
	}
	if(bscale || scale){
		res = bymin10(quotient, bscale, scale);
		/*destroy_nbr(quotient);*/
	}else{
		res = quotient;
	}
	if(modulo){
		if(mod){
			/*dot_0 += mod->dot;
			bdot_0 += mod->bdot;
			if(dot_0 >= BLK){
				dot_0 -= BLK;
				bdot_0++;
			}*/
			/*temp = mv_dot(reste, NULL, mod->bdot, mod->dot);*/
			/*temp_ = mv_dot(mod, NULL, mod->bdot, mod->dot);*/
			(void)mv_dot(reste, reste, mod->bdot, mod->dot);
			(void)mv_dot(mod, mod, mod->bdot, mod->dot);
			(void)addition(reste, mod, mod);
			/*(void)addition(temp, temp_, mod);*/
			/*destroy_nbr(temp);*/
			/*destroy_nbr(temp_);*/
			destroy_nbr(reste);
			*modulo = mod;
		}
		bx.num = 0;
		/*if(sc || lsc){
			mod = bymin10(*modulo, lsc, sc);
			destroy_nbr(*modulo);
			*modulo = mod;
		}*/
		if(scale || bscale || dot_0 || bdot_0){
			cscale = scale + dot_0;
			cbscale = bscale + bdot_0;
			if(cscale >= BLK){
				cscale -= BLK;
				cbscale++;
			}
			mod = bymin10(*modulo, cbscale, cscale);
			/*destroy_nbr(*modulo);
			*modulo = mod;*/
		}
		if(neg1){
			if(equal(*modulo, &nx) != 0)
				(*modulo)->neg = 1;
		}
	}
	if(neg1 != neg2){
		if(equal(res, &nx) != 0)	
			res->neg = 1;
	}
	num1->neg = neg1;
	num2->neg = neg2;
	destroy_nbr(diviseur);
	destroy_nbr(dividende);
	ADJUST_0(res, bs, bt);
	if(modulo){
		ADJUST_0((*modulo), bs, bt);
	}else
		destroy_nbr(reste);
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
		d = division(div, &two, &mod, 0, 0, 0);
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
		div = division(&un, res, modulo, bscale, scale, approximation);
		num2->neg = neg;
		destroy_nbr(res);
		res = div;
	}
	return res;
}

