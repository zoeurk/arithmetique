#include "operation.h"
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
	for(;result; result = result->next)
		result->num = result->nmemb = result->full = 0;
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
		if(res != num1)
			num1->neg = 1;
		return res;
	}else{
		if(!num1->neg && num2->neg){
			num2->neg = 0;
			res = soustraction(num1, num2, result);
			if(res != num2)
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
		if(cdot >= pb1->nmemb && cdot > 0){
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
			if(res != num2)
				num2->neg = 1;
			return res;
		}else{
			if(num1->neg && num2->neg){
				num2->neg = 0;
				num1->neg = 0;
				res = soustraction(num2, num1, result);
				if(res != num2)
					num2->neg = 1;
				if(res != num1)
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
		neg = 0;
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
		if(cdot > 0 && cdot >= pb1->nmemb){
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
			if(cdot > 0 && cdot >= pb2->nmemb){
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
	/*printf("MULTIPLICATION(%lu:%i,%lu:%i * %lu:%i,%lu:%i)\n",
		num1->bval, num1->val, num1->bdot, num1->dot, num2->bval, num2->val, num2->bdot, num2->dot);
	print_nbr(num1);
	putchar('*');
	print_nbr(num2);
	putchar('\n');*/
	/*for(pbr1 = num1->num;pbr1;pbr1 = pbr1->next)
		printf("%lu :: %i :: %i\n", pbr1->num, pbr1->nmemb, pbr1->full);
	for(pbr1 = num2->num;pbr1;pbr1 = pbr1->next)
		printf("%lu :: %i :: %i\n", pbr1->num, pbr1->nmemb, pbr1->full);*/
	/*printf("RESULT:\n");
	for(pbr1 = result->num;pbr1;pbr1 = pbr1->next)
		printf("%lu :: %i :: %i\n", pbr1->num, pbr1->nmemb, pbr1->full);
	printf("\n= = = = = = = = =\n");*/
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
	putchar('=');
	printf("%lu :: %i :: %lu :: %i\n", res->bval, res->val, res->bdot, res->dot);*/
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
	/*for(pbr1 = res->num;pbr1;pbr1 = pbr1->next)
		printf("%lu :: %i :: %i\n", pbr1->num, pbr1->nmemb, pbr1->full);
	print_nbr(res);
	putchar('=');
	printf("%lu :: %i :: %lu :: %i\n", res->bval, res->val, res->bdot, res->dot);
	if(res->bval < res->bdot){
		exit(255);
	}*/
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
	/*printf("FIN MULTIPLICATION(%lu:%i,%lu:%i) : ",
		res->bval, res->val, res->bdot, res->dot);
	print_nbr(res);
	putchar('\n');
	if(bval >= (unsigned long int)0-9)
		exit(0);*/
	return res;
}
void *bymin10(struct nbr *num, struct nbr *result, unsigned long int bscale, int scale){
	struct nbr *res = NULL;
	struct bin *bs, *bt, *br = NULL;
	unsigned long int bdot, j, rtemp[3] = { 0, 0, 0 }, rtemp_, btemp;
	int idx = 0, dot_, mul[C_BLK] = COEFS, mb[3] = { 0, 0, 0 };
	/*printf("BYMIN10\n");
	print_nbr(num);
	putchar('\n');*/
	/*printf("%lu :: %i, %lu :: %i => %lu :: %i\n", num->bval, num->val, num->bdot, num->dot, bscale, scale);*/
	/*for(bs = num->num;bs;bs = bs->next)
		printf("%lu :: %i\n", bs->num,bs->nmemb);*/
	/*print_nbr(num);
	putchar('\n');
	printf("%lu :: %i\n", bscale, scale);*/
	if(result == NULL){
		if((res = calloc(1,sizeof(struct nbr))) == NULL){
			perror("calloc()");
			exit(EXIT_FAILURE);
		}
		if(num->bdot > bscale || (num->bdot == bscale && num->dot > scale))
			res->num = new_num(num->bval + (num->val > 0), num->bdot + (num->dot > 0));
		else
			res->num = new_num(num->bval + (num->val > 0), bscale + (scale > 0));
		num_cpy(res, num);
	}else{
		res = result;
		if(res != num){
			num_cpy(res, num);
		}
	}
	dot_ = num->dot;
	bdot = num->bdot;
	if(num->bval > bscale || (num->bval == bscale && num->val > scale)){
			if(scale == 0){
				/*printf("* * * * * *(%lu), %i, %i\n", bscale, res->val = 8, res->dot = 0);*/
				res->bval -= bscale;
				res->bdot += bscale;
				/*print_nbr(res);
				putchar('\n');*/
				return res;
			}
			res->bval -= bscale;
			res->bdot += bscale;
			if(res->val < scale){
				res->val += BLK;
				res->val -= scale;
				res->bval--;
			}else
				res->val -= scale;
			bt = num->num;
			bs = res->num;
			if(dot_){
				bs->num *= mul[BLK-dot_];
				bs->nmemb += BLK-dot_;
				idx = BLK - dot_;
				bs->full = 1;
				res->bdot++;
				res->dot = 0;
			}
			res->dot += scale;
			rtemp[0] = bs->num;
			rtemp[1] = rtemp[0]%mul[scale];
			mb[1] = scale;
			mb[0] = BLK;
			/*printf(">>%lu :: %lu\n", rtemp[1], rtemp[0]/mul[scale]);*/
			if(dot_){
				if(rtemp[1]){
					/*ICI*/
					bs = bs->next;
					bt = res->num;
					mb[1] = scale;
					mb[0] = BLK;
				}else{
					/*bt = res->num->next;*/
					mb[1] = 0;
					mb[0] = scale;
					/*bs->num = 0;*/
				}

			}else{
				bs = bs->next;
				mb[1] = scale;
				mb[0] = BLK;
			}
			/*printf(">>%lu\n", bt->num);*/
			for(/*rtemp_ = rtemp[0]/mul[scale]*//*,bs = (idx && rtemp[1]) : bs->next : ,*/ rtemp_ = bt->num;;){
				rtemp[0] = rtemp_;
				if(bs){
					rtemp_ = bs->num;
					/*bs->num = bs->nmemb = bs->full = 0;*/
				}
				if(mb[2]){
					if(br == NULL){
						br = res->num;
					}else{
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
				/*printf("%lu :: %lu\n", rtemp_%mul[scale]*mul[BLK-scale], rtemp[0]/mul[scale]);*/
				rtemp[1] = rtemp_%mul[scale]*mul[BLK-scale] + rtemp[0]/mul[scale];
				if(bs)
					bs = bs->next;
				if(bt)
					bt = bt->next;
			}
			/*for(br = br->next;br;br = br->next)
				br->num = br->nmemb = br->full = 0;
			printf(">>%lu : %i, %lu :: %i\n", res->bval, res->val, res->bdot, res->dot);
			printf(">>");
			print_nbr(res);
			putchar('\n');*/
			/*for(bs = res->num; bs; bs = bs->next)
				printf("%lu :: %i :: %i\n", bs->num, bs->nmemb, bs->full);*/
	}else{
		/*if(scale == 0){
			res->bval -= bscale;
			res->bdot += bscale;
			for(bs = res->num;bs->nmemb > 0;bs->nmemb = BLK, bs->full = 1, bs = bs->next);
			res->val = bs->nmemb = 1;
			return res;
		}*/
		if(res->num->next){
			res->bval = 0;
			res->bdot += bscale;
			res->val = 1;
			res->dot += scale;
			bs = num->num;
			bt = res->num;
			j = bdot;
			if(dot_){
				bs->num *= mul[BLK-dot_];
				bs->nmemb += BLK-dot_;
				idx = BLK - dot_;
				bs->full = 1;
				bdot  = ++res->bdot;
				res->dot = 0;
			}
			rtemp[0] = bs->num;
			rtemp[1] = rtemp[0]%mul[scale];
			bt->num = rtemp[1];
			bt->nmemb = scale;
			bt->full = (bt->nmemb == BLK);
			if(bt->nmemb != 0 && bt->num != 0)
				bt = bt->next;
			else{
				if(idx){
					idx -= bt->nmemb;
					bt->nmemb = 0;
				}else
					bt = bt->next;
			}
			fprintf(stderr, "%lu :: %lu :: %i => %i\n", bscale, bdot, dot_, idx);
			for(bs = bs->next, btemp = bscale + j + (idx > 0), fprintf(stderr, "TEMP=%lu\n", btemp);bs && bs->nmemb > 0;btemp--,bs = bs->next, bt = bt->next){
				if(rtemp[1])
					rtemp[2] = rtemp[1];
				rtemp[1] = (rtemp[0] / mul[scale]) + bs->num%mul[scale]*mul[((btemp > 0)
							? BLK - scale
								: (scale <= bs->nmemb)
							? bs->nmemb-scale+bt->nmemb
								: scale-bs->nmemb+bt->nmemb)];
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
			for(;bt && btemp > 0;btemp--, bt = bt->next){
				bt->nmemb = BLK;
				bt->full = 1;
				bt->num = 0;
			}
		}else{
			bt = res->num;
		}
		if(bt){
			bt->nmemb = 1;
			bt->num = 0;
			/*for(bt = res->num; bt; bt = bt->next)
				printf("%lu :: %i :: %i\n", bt->num, bt->nmemb, bt->full);
			print_nbr(res);
			exit(0);*/
		}
	}
	if(idx > 0){
		for(bs = res->num;idx > 0;idx--){
			bs->num /= mul[1];
			bs->full = 0;
			if(--bs->nmemb == 0)
				bs = bs->next;
			if(res->dot > 0)
				res->dot--;
			else{
				res->bdot--;
				res->dot = BLK-1;
			}
			if(res->dot == 0 && res->bdot == 0)
				break;
		}
		/*print_nbr(res);
		putchar('\n');
		printf(">>%lu : %i, %lu :: %i\n", res->bval, res->val, res->bdot, res->dot);*/
		/*print_nbr(res);
		putchar('\n');
		printf(">>%lu : %i, %lu :: %i\n", res->bval, res->val, res->bdot, res->dot);*/
		/*res->num->num /= mul[idx];
		res->num->nmemb -= idx;
		res->num->full = 0;
		res->bdot--;*/
		/*print_nbr(res);
		putchar('\n');*/
		/*if(idx < res->num->nmemb)
			idx = res->num->nmemb - idx;*/
		/*printf("%i\n", idx);*/
		/*for(;idx && res->num->num%mul[1];idx--){
			res->num->num /= mul[1];
			res->num->nmemb--;
			res->num->full = 0;
			res->dot--;
		}*//*else{
			res->num->next->num /= mul[idx - res->num->nmemb];
			res->dot = res->num->next->nmemb = idx - res->num->nmemb,
			res->num->next->full = res->num->full = res->num->nmemb = 0;
			res->bdot--;
		}*/
		/*for(bt = res->num;bt->nmemb == 0;bt = bt->next);*/
		/*if(res->num->nmemb == 0)*/
		/*for(bs = res->num, bt = res->num->next;bt && bt->nmemb > 0;bs = bs->next, bt = bt->next){
			bs->num = bt->num;
			bs->nmemb = bt->nmemb;
			bs->full = bt->full;
		}*/
	}
	if(res->num->nmemb == 0){
		for(bs = res->num, bt = res->num->next;bt && bt->nmemb > 0;bs = bs->next, bt = bt->next){
			bs->num = bt->num;
			bs->nmemb = bt->nmemb;
			bs->full = bt->full;
		}
		for(;bs;bs->nmemb = bs->num = bs->full = 0, bs = bs->next);
	}
	/*if(res->num->nmemb == 0){
		for(bs = res->num, bt = res->num->next;bt && bt->nmemb > 0;bs = bs->next, bt = bt->next){
			bs->num = bt->num;
			bs->nmemb = bt->nmemb;
			bs->full = bt->full;
		}
	}
	if(idx){
		printf("IDX\n");
		for(;res->num->num % mul[1] == 0 && idx;idx--){
			res->num->num /= mul[1];
			res->num->nmemb--;
			res->num->full = 0;
			res->dot--;
		}
	}*/
	/*print_nbr(res);
	putchar('\n');*/
	return res;
}
void *mv_dot(struct nbr *num, struct nbr *result, unsigned long int bscale, int scale){
	static struct bin bzero = ZERO_BIN;
	static struct nbr zero = INIT_NBR(0, 0, 1, 0, 0, &bzero, "0");
	struct bin *bn, *nmv, *end;
	struct nbr *n;
	unsigned long int bs, blscale = 0, mem[2] = { 0, 0 };
	int mul[C_BLK] = COEFS, diff, idx = 0, lscale = 0, uscale = 0;
	/*printf("MV_DOT > ");
	print_nbr(num);
	putchar('\n');*/
	if(equal(&zero, num) == 0){
		if(result == NULL)
			n = dup_nbr(num);
		else{
			n = result;
			if(n != num)
				num_cpy(n, num);
		}
		return n;
	}
	if(scale == 0 && bscale <= num->bdot){
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
	if(bscale > num->bdot)
		bs = bscale - num->bdot;
	else
		bs = 0;
	if(result == NULL){
		if((n = calloc(1, sizeof(struct nbr))) == NULL){
			perror("calloc()");
			exit(EXIT_FAILURE);
		}
		n->num = new_num(num->bval + (num->val > 0), num->bdot + (num->dot > 0)+(scale > 0) + bscale +1);
	}else
		n = result;
	if(n != num)
		num_cpy(n, num);
	if((bscale > num->bdot || (bscale == num->bdot && scale > num->dot))){
		uscale = num->dot;
		blscale = bscale - num->bdot;
		if(scale < num->dot){
			blscale--,
			lscale = BLK + scale - num->dot;
		}else{
			lscale = scale - num->dot;
		}
	}else{
		uscale = scale;
	}
	if((lscale || bscale > num->bdot || (bscale == num->bdot && (scale >= num->dot))) && num->dot > 0){
		if(n->dot > 0){
			if(scale >= n->dot){
				idx = scale - n->dot;
				if(uscale != num->dot){
					if(lscale != idx){
						lscale = idx;
					}
				}
			}else{
				idx = n->dot - scale;
				if(lscale + idx >= BLK){
					lscale += (lscale+idx-BLK);
					bs--;
					idx = 0;
				}else{
					lscale += idx;
				}
			}
		}else{
			idx = lscale;
			lscale += idx;
		}
		if(n->num->nmemb == BLK){
			n->num->full = 1;
			n->bdot++;
			n->dot = 0;
		}else{
			if(n->num->nmemb > BLK){	
				for(diff = n->num->nmemb - BLK, nmv = n->num;
					nmv && nmv->nmemb > 0;
					nmv = nmv->next, nmv->num*=mul[diff], nmv->num += mem[0])
				{
					mem[0] = nmv->num / mul[BLK];
					nmv->num %= mul[BLK];
					nmv->nmemb = BLK;
				}
				nmv->nmemb += diff;
			}else
				n->dot = 0;
		}
	}
	n->bval += bscale;
	if(n->bdot >= bscale)
		n->bdot -= bscale;
	else
		n->bdot = 0;
	n->val += scale;
	if(n->val >= BLK){
		n->val -= BLK;
		n->bval++;
	}
	if(n->dot || n->bdot){
		if(n->dot >= scale){
			n->dot -= scale;
		}else{
			n->dot = BLK + n->dot - scale;
			n->bdot--;
		}
	}
	bn = n->num;
	if(uscale){
		if(bn->nmemb > uscale){
			if(n->dot || n->bdot){
				mem[0] = bn->num / mul[bn->nmemb-uscale];
				bn->num = bn->num % mul[bn->nmemb-uscale];
				if(bn->nmemb >= uscale)
					bn->nmemb = bn->nmemb - uscale;
				else
					bn->nmemb = uscale - bn->nmemb;
				bn->full = (bn->nmemb == BLK);
				bn = bn->next;
				for(;bn && bn->nmemb == BLK;bn = bn->next){
					mem[1] = mem[0] + (bn->num%mul[BLK-uscale])*mul[uscale];
					mem[0] = bn->num / mul[BLK-uscale];
					bn->num = mem[1];
					bn->nmemb = BLK;
					bn->full = 1;
				}
				if(bn){
					bn->num = mem[0]+bn->num*mul[uscale];
					bn->nmemb += uscale;
					if(bn->nmemb > BLK){
						bn->next->nmemb = bn->nmemb - BLK;
						bn->next->num = bn->num /mul[BLK];
						bn->next->full = 0;
						bn->full = 1;
						bn->nmemb = BLK;
						bn->num %= mul[BLK];
						bn = bn->next;
					}else{
						bn->full = (bn->nmemb == BLK);
					}
				}
			}
		}else{
			diff = uscale - bn->nmemb;
			bn->num += (bn->next->num%mul[BLK-bn->nmemb-diff])*mul[bn->nmemb];
			if(bn->next->nmemb == 1 && bn->next->num == 0){
				mem[0] = bn->num/mul[uscale];
				bn->next->num = mem[0];
				bn->next->nmemb = bn->nmemb-uscale;
				bn->num = bn->num%mul[uscale];
				bn->nmemb = (n->val) ? n->val : BLK;
				bn->full = (bn->nmemb == BLK);
			}else{
				bn->nmemb	= (n->dot)
						? n->dot : (n->bdot)
						? BLK : (bn->next && bn->next->nmemb == BLK)
						? BLK : (n->bval) ? BLK : n->val;
				bn->full = (bn->nmemb == BLK);
				for(	bn = bn->next, mem[0] = bn->num/mul[BLK-uscale];
					bn && bn->nmemb == BLK;
					bn = bn->next, mem[0] = bn->num/mul[BLK-uscale]
				){
					bn->num = mem[0]+(bn->next->num*mul[uscale])%mul[BLK];
					if((bn->next->nmemb+uscale) >= BLK){
						bn->nmemb = BLK;
						bn->full = 1;
					}else{
						bn->nmemb = bn->next->nmemb+uscale;
						bn->full = 0;
					}
				}
				if(bn){
					mem[0] = bn->num/mul[BLK-uscale];
					bn->num = bn->nmemb = bn->full = 0;
					if(bn->next && mem[0]+bn->next->num*mul[uscale]){
						bn->num = mem[0]+bn->next->num*mul[uscale];
						bn->nmemb = (n->val) ? n->val : BLK;
						bn->full = (bn->nmemb == BLK);
						bn->next->nmemb = bn->next->full = bn->next->num = 0;
					}else{
						if(mem[0] > 0){
							bn->num = mem[0];
							bn->nmemb = n->val;
							bn->full = 0;
						}else{
							if(bn->nmemb == 1 && mem[0] == 0){
								bn->num = mem[0];
								bn->nmemb = 0;
								bn->full = 0;
								bn->prev->nmemb = n->val;
								bn->prev->full = 0;
							}
						}
					}
				}
			}
		}
	}
	if(lscale){
		bn = n->num;
		bn->num *= mul[lscale];
		bn->nmemb += lscale;
		for(diff = bn->nmemb - BLK;
			bn && bn->nmemb;
			bn->nmemb = BLK, bn->full = 1,bn = bn->next, (bn) ? bn->num *= mul[diff] : 0, (bn) ? bn->num += mem[0] : 0)
		{
			mem[0] = bn->num / mul[BLK];
			bn->num %= mul[BLK];
		}
		if(bn && bn->num){
			if(n->val){
				bn->nmemb = n->val;
			}else{
				bn->nmemb = BLK;
				bn->full = 1;
			}
		}else{
			
			if(n->val && bn && bn->prev->num/mul[n->val] == 0){
				bn->prev->nmemb = n->val;
				bn->prev->full = 0;
			}else{
				n->val = 0;
			}
		}
	}
	if(bs){
		for(;bn->nmemb != 0; bn = bn->next);
		for(end = bn;bs > 0; bs--, end = end->next);
		for(;bn->next;bn = bn->prev, end = end->prev){
			end->num = bn->num;
			end->nmemb = bn->nmemb;
			end->full = bn->full;
			if(end != bn)
				bn->num = bn->nmemb = bn->full = 0;
		}
		for(;end && end->next;end = end->prev){
			end->full = 1;
			end->nmemb = BLK;
			end->num = 0;
		}
		n->bdot = n->dot = 0;
	}
	for(;idx && n->num->num%10 == 0 && n->dot > 0;idx--,n->dot--, n->num->nmemb--, n->num->num/=10);
	ADJUST_0(n, nmv, bn);
	return n;
}
#define PRINT_NBR(n) \
	print_nbr(n); \
	putchar('\n');
#define M_BLK 1
 
void *edivision(struct nbr *num1, struct nbr *num2, struct nbr **modulo,
		unsigned long int bscale, int scale, int approximation, struct nbr **sp
){
	struct retbcpy *bcpy;
	struct bin *bdividende, *breste, *b,
		bdix = { 10, 2, 0, 0, NULL, NULL }, bsingle = { 0, 1, 0, 0, NULL, NULL }, *bs = NULL, *bt, bx = { 0, 1, 0, 0, NULL, NULL };
	struct nbr *res = NULL, *diviseur, *dividende, *quotient, *mod = NULL, *reste = NULL,
		dix = INIT_NBR( 0, 0, 2, 0, 0, NULL, "10" ), *temp,
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
	if(num2->dot || num2->bdot){
		bdot_0 = num2->bdot;
		dot_0 = num2->dot;
		if(sp && sp[DIVISEUR])
			diviseur = mv_dot(num2, sp[DIVISEUR], bdot_0, dot_0);
		else
			diviseur = mv_dot(num2, NULL, bdot_0, dot_0);
		DOT(diviseur, bs, b);
	}else{
		if(sp && sp[DIVISEUR]){
			num_cpy(sp[DIVISEUR], num2);
			diviseur = sp[DIVISEUR];
		}else
			diviseur = dup_nbr(num2);
	}
	if(scale || bscale || bdot_0 || dot_0){
		mbdot = bdot_0 + bscale;
		mdot = dot_0 + scale;
		if(mdot >= BLK){
			mdot -= BLK;
			mbdot++;
		}
		if(sp && sp[DIVIDENDE])
			dividende = mv_dot(num1, sp[DIVIDENDE], mbdot, mdot);
		else
			dividende = mv_dot(num1, NULL, mbdot, mdot);
		/*temp = mv_dot(num1, NULL, mbdot, mdot);
		dividende = temp;*/
	}else{
		if(sp && sp[DIVIDENDE]){
			dividende = sp[DIVIDENDE];
			num_cpy(sp[DIVIDENDE], num1);
		}else
			dividende = dup_nbr(num1);
	}
	if(modulo){
		if(dividende->bdot || dividende->dot){
			if(sp && sp[RESTE]){
				mod = sp[RESTE];
			}else{
				if((mod = calloc(1, sizeof(struct nbr))) == NULL){
					perror("calloc()");
					exit(EXIT_FAILURE);
				}
				/*mod->num = new_num(dividende->bval + (dividende->val > 0)+1, num1->bdot + (num1->dot > 0));*/
				mod->num = new_num(dividende->bval + (dividende->val > 0)+1, dividende->bdot + (dividende->dot > 0));
			}
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
	for(bdividende 	= (dividende->num->prev)
			? dividende->num->prev
			: dividende->num;
		bdividende->nmemb == 0
			&& bdividende != dividende->num;
		bdividende = bdividende->prev
	);
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
		if(sp && sp[MODULO]){
			reste = sp[MODULO];
		}else{
			if((reste = calloc(1, sizeof(struct nbr))) == NULL){
				perror("calloc()");
				exit(EXIT_FAILURE);
			}
			/*reste->num = new_num(num1->bval + num1->bdot + (num1->val > 0) + (num1->dot > 0),
										lsc + (sc > 0) + bdot_0 + (dot_0 > 0) + bdot_0 +(dot_0 > 0));*/
			reste->num = new_num(num1->bval + (num1->val > 0),
							num1->bdot + (num1->dot > 0)
							+ num2->bdot + (num2->dot > 0)
							+ bscale + (scale > 0) + 3);
		}
		for(blk = binit + (init > 0), bt = reste->num; blk > 1; blk--, bt = bt->next);
		breste = bt;
		bcpy = nbytescpy(&breste, &bdividende, &start, binit, init);
		/*for(b = breste; b; b = b->next)
			printf("%lu :: %i\n", b->num, b->nmemb);*/
		reste->val = bcpy->rbytes;
		reste->bval = bcpy->rblk;
		/*PRINT_NBR(dividende);
		PRINT_NBR(reste);
		exit(0);*/
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
		if(sp && sp[QUOTIENT]){
			quotient = sp[QUOTIENT];
		}else{
			if((quotient = calloc(1, sizeof(struct nbr))) == NULL){
				perror("calloc()");
				exit(EXIT_FAILURE);
			}
			quotient->num = new_num(num1->bval + (num1->val > 0), bscale + (scale > 0)+1);
		}
		for(;;){
			for(bx.nmemb = 1, bx.num = 0, x = 0; (int)bx.num < mul[M_BLK]; bx.num++){
				if(equal(reste, diviseur) < 0){
					x = bx.num;
					break;
				}
				soustraction(reste, diviseur, reste);
			}
			/*quotient = bymul10(quotient, mul[sclen], sclen);*/
			quotient = mv_dot(quotient, quotient, 0, sclen);
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
				/*bymul10(reste, mul[sclen], sclen);*/
				mv_dot(reste, reste, 0, sclen);
				reste->num->num += bs->num;
			}
		}
	}else{
		if(sp && sp[QUOTIENT])
			quotient = sp[QUOTIENT];
		else{
			if((quotient = calloc(1, sizeof(struct nbr))) == NULL){
				perror("calloc()");
				exit(EXIT_FAILURE);
			}
			if((quotient->num = calloc(1, sizeof(struct bin))) == NULL){
				perror("calloc()");
				exit(EXIT_FAILURE);
			}
		}
		quotient->num->alloc = 1;
		quotient->val = quotient->num->nmemb = 1;
		if(modulo){
			if(lsc < num1->bdot || (lsc == num1->bdot && sc == num1->dot)){
				lsc = num1->bdot + bdot_0;
				sc = num1->dot + dot_0;
				if(sc >= BLK){
					sc -= BLK;
					lsc++;
				}
			}
			if(sp && sp[MODULO]){
				reste = sp[MODULO];
			}else{
				if((reste = calloc(1, sizeof(struct nbr))) == NULL){
					perror("calloc()");
					exit(EXIT_FAILURE);
				}
				reste->num = new_num(num1->bval + num1->bdot + (num1->val > 0) + (num1->dot > 0)+1, lsc + (sc > 0));
			}
			num_cpy(reste, dividende);
		}
	}
	/*PRINT_NBR(reste);
	PRINT_NBR(quotient);*/
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
		res = bymin10(quotient, quotient, bscale, scale);
	}else{
		res = quotient;
	}
	if(modulo){
		if(mod){
			(void)mv_dot(reste, reste, mod->bdot, mod->dot);
			(void)mv_dot(mod, mod, mod->bdot, mod->dot);
			/*(void)addition(reste, mod, mod);
			if(!sp && !sp[RESTE])
				destroy_nbr(reste);
			*modulo = mod;*/
			(void)addition(reste, mod, reste);
			if(!sp || !sp[MODULO])
				destroy_nbr(mod);
			*modulo = reste;
		}
		if(scale || bscale || dot_0 || bdot_0){
			cscale = scale + dot_0;
			cbscale = bscale + bdot_0;
			if(cscale >= BLK){
				cscale -= BLK;
				cbscale++;
			}
			/*mod = bymin10(*modulo, *modulo, cbscale, cscale);*/
			(void)bymin10(*modulo, *modulo, cbscale, cscale);
		}
		if(neg1){
			bx.num = 0;
			if(equal(*modulo, &nx) != 0)
				(*modulo)->neg = 1;
		}
	}
	if(neg1 != neg2){
		bx.num = 0;
		if(equal(res, &nx) != 0)
			res->neg = 1;
	}
	num1->neg = neg1;
	num2->neg = neg2;
	if(!sp || !sp[DIVISEUR])
		destroy_nbr(diviseur);
	if(!sp || !sp[DIVIDENDE])
		destroy_nbr(dividende);
	ADJUST_0(res, bs, bt);
	if(modulo){
		ADJUST_0((*modulo), bs, bt);
	}else
		if(!sp || !sp[MODULO]){
			destroy_nbr(reste);
		}
	return res;
}
void *nrdivision(struct nbr *num1, struct nbr *num2, unsigned long int bscale, int scale, struct nbr **sp){
	/*
		Method Newton-Raphson
		X/Y == X * 1/Y
			AND:
		Xn+1 = Xn * (2 - Y * Xn)
		Xn 1ere itération: 0.1, 0.01, 0.001, ...
	*/
	struct bin bun = INIT_BIN(1, 1, NULL, NULL),
			bzero = INIT_BIN(0, 1, NULL, NULL),
			btwo = INIT_BIN(2, 1, NULL, NULL),
			*r1, *r2;
	struct nbr un = INIT_NBR(0, 0, 1, 0, 0, NULL, "1"),
			zero = INIT_NBR(0, 0, 1, 0, 0, NULL, "0"),
			two = INIT_NBR(0, 0, 1, 0, 0, NULL, "2"),
			*nill, *result[3], *sub;
	unsigned long int bdot,
	#if __WORDSIZE == 32
		brmd = bscale+4;
	#else
		brmd = bscale+2;
	#endif
	int check = 0, neg1, neg2, i = 0, r, dot, approx, rmd = scale, mul[C_BLK] = COEFS;
	return NULL;
	un.num = &bun;
	zero.num = &bzero;
	two.num = &btwo;
	if(equal(num2, &zero) == 0){
		fprintf(stderr, "Division by 0\n");
		return NULL;
	}
	if(equal(num2, &un) == 0){
		if(!sp)
			result[0] = dup_nbr(num2);
		else
			num_cpy(result[0], num2);
		return result[0];
	}
	if(equal(num1, num2) < 0 && bscale == 0 && scale == 0){
		if(!sp)
			result[0] = dup_nbr(&zero);
		else
			num_cpy(result[0], &zero);
		return result[0];
	}
	/*
		bscale +1 for approximation
	*/
	/*
		approximation
		temporaire
		result[2]
	*/
	rmd+=4;
	if(rmd >= BLK){
		brmd++;
		rmd -= BLK;
	}
	neg1 = num1->neg;
	neg2 = num2->neg;
	num1->neg = num2->neg = 0;
	if(!sp || !sp[3]){
		if((nill = calloc(1, sizeof(struct nbr))) == NULL){
			perror("calloc()");
			exit(EXIT_FAILURE);
		}
		nill->num = new_num(1, bscale + (scale > 0));
	}else
		nill = sp[3];
	if(!sp){
		for(i = 0; i < 3; i++){
			if((result[i] = calloc(1, sizeof(struct nbr))) == NULL){
				perror("calloc()");
				exit(EXIT_FAILURE);
			}
			result[i]->num = new_num(num1->bval + (num1->val > 0) + num2->bdot + (num2->dot > 0) +1,
							num1->bdot + (num1->dot > 0) + 4*brmd + (rmd > 0) +1);
		}
	}else
		for(i = 0; i < 3; i++){
			result[i] = sp[i];
		}
	(void)num_cpy(result[0], &un);
	(void)bymin10(result[0], result[0], num2->bval, num2->val);
	(void)num_cpy(result[1], &zero);
	i = 0;
	while(equal(result[0], result[1]) != 0){
		/*
			Xn+1 = result[i] * (2 - diviseur * result[i])
		*/
		reset_num(result[!i]);
		(void)multiplication(result[i], num2, result[!i]);
		sub = soustraction(&two, result[!i], result[2]);
		reset_num(result[!i]);
		(void)multiplication(result[i], sub, result[!i]);
		reset_num(result[2]);
		i = !i;
		for(	r = 1, bdot = result[i]->bdot, dot = result[i]->dot,r1 = result[i]->num;
			bdot > brmd || (bdot == brmd && dot > rmd);
			r++
		){
			if(r1 == NULL)
				exit(0);
			r1->num -= r1->num%mul[r];
			if(--dot <= 0){
				r1 = r1->next;
				dot = BLK;
				bdot--;
				r = 0;
			}
		}
		DOT(result[i], r1, r2);
	}
	reset_num(result[!i]);
	(void)multiplication(result[i], num1, result[!i]);
	i = !i;
	/*PRINT_NBR(result[i])*/
	/*if(scale || bscale)*/
		for(	r = 1, bdot = result[i]->bdot, dot = result[i]->dot,r1 = result[i]->num;
			bdot > bscale || (bdot == bscale && dot > scale);
			r++
		){
			if(check == 2*BLK)
			{
				approx = (int)(r1->num%mul[r]);
				reset_num(nill);
				if(approx >= 5){
					nill->num->num = 1;
					nill->val = nill->num->nmemb = 1;
					bymin10(nill, nill, bdot, dot);
				}else{
					nill->val = nill->num->nmemb = 1;
				}
				addition(nill, result[i], result[2]);
				num_cpy(result[i], result[2]);
				check++;
			}
			if(check < 2*BLK)
				check++;
			r1->num -= r1->num%mul[r];
			if(dot-- <= 0){
				r1 = r1->next;
				dot = BLK;
				bdot--;
				r = 0;
			}
	}/*else{
		for(r1 = result[i]->num, bdot = result[i]->bdot, dot = result[i]->dot;bdot > 0 || dot > 0; r1 = r1->next){
			if(check == 2 && r1->num/mul[r1->nmemb-1] >= 5)
			addition(&un, result[i], result[i]);
			check += (check < BLK);
			r1->num = 0;
			if(dot > 0)
				dot = 0;
			else
				bdot--;
		}
	}*/
	DOT(result[i], r1, r2);
	if(neg1 != neg2)
		result[i]->neg = '-';
	num1->neg = neg1;
	num2->neg = neg2;
	if(!sp){
		destroy_nbr(result[2]);
		destroy_nbr(result[!i]);
	}
	if(!sp || !sp[3])
		destroy_nbr(nill);
	return result[i];
}
#undef M_BLK
#define M_BLK 1
void *kdivision(struct nbr *num1, struct nbr *num2, struct nbr **modulo,
		unsigned long int bscale, int scale, int approximation, struct nbr **sp
){
	/*
		Kuhn Algorithm ou ce que j'en ai compris... :/)]
	*/
	struct retbcpy *bcpy;
	struct bin bnx = INIT_BIN(5, 1, NULL, NULL), bzero = ZERO_BIN,
			*s1, *s2, *r1, *r2, *b1, *b2, *lread, *breste, *n1, *nr/*, *bnr, *bn1*/;
	struct nbr *diviseur, *dividende[2], *quotient, *reste, *mod, *tmod = NULL, *temp,
			n = INIT_NBR(0, 0, 1, 0, 0, NULL, NULL),
			zero = INIT_NBR(0, 0, 1, 0, 0, NULL, NULL);
	unsigned long int bidx, cmp_n1, cmp_n2, bj, bdot = 0, sbdot = bscale;
	int i, j, a = -1, idx, nrmemb
		#if M_BLK != 1
		, cread
		#endif
		, dot = 0, sdot = scale, blk, mul[C_BLK] = COEFS, norm = 0, start = 0, ret, ret_;
	/*nx.num = &bnx;*/
	zero.num = &bzero;
	if(equal(num2, &zero) == 0){
		fprintf(stderr, "Division by 0\n");
		return NULL;
	}
	/*sp1 = &psp1;
	sp2 = &psp2;*/
	/*if((diviseur = calloc(1, sizeof(struct nbr))) == NULL){
		perror("calloc()");
		exit(EXIT_FAILURE);
	}*/
	/*if((dividende[0] = calloc(1, sizeof(struct nbr))) == NULL){
		perror("calloc()");
		exit(EXIT_FAILURE);
	}*/
	if((dividende[1] = calloc(1, sizeof(struct nbr))) == NULL){
		perror("calloc()");
		exit(EXIT_FAILURE);
	}
	/*diviseur->num = new_num(num2->bval + (num2->val > 0) + num2->bdot + (num2->dot > 0), 0);*/
	/*dividende[0]->num = new_num(num2->bval + (num2->val > 0) + num1->bdot + (num1->dot > 0) +1, bscale+ (scale > 0)+1);*/
	dividende[1]->num = new_num(num2->bval + (num2->val > 0) + num1->bdot + (num1->dot > 0) +1, bscale+ (scale > 0)+1);
	if(num2->dot || num2->bdot){
		if((diviseur = calloc(1, sizeof(struct nbr))) == NULL){
			perror("calloc()");
			exit(EXIT_FAILURE);
		}
		diviseur->num = new_num(num2->bval + (num2->val > 0), num2->bdot + (num2->dot > 0));
		bdot = num2->bdot;
		dot = num2->dot;
		/*(void)num_cpy(diviseur, num2);*/
		
		(void)mv_dot(num2, diviseur, bdot, dot);
		/*DOT(diviseur, r1, r2);*/
	}else{
		/*(void)num_cpy(diviseur, num2);*/
		diviseur = num2;
	}
	sdot += dot;
	sbdot += bdot;
	if(sdot > BLK){
		sdot -= BLK;
		sbdot++;
	}
	if(sdot || sbdot){
		if((dividende[0] = calloc(1, sizeof(struct nbr))) == NULL){
			perror("calloc()");
			exit(EXIT_FAILURE);
		}
		dividende[0]->num = new_num(num2->bval + (num2->val > 0) + num1->bdot + (num1->dot > 0) +2, bscale+ (scale > 0)+2);
		/*(void)num_cpy(dividende[0], num1);*/
		(void)mv_dot(num1, dividende[0], sbdot, sdot);
		/*DOT(dividende[0], r1, r2);*/
	}else{
		/*(void)num_cpy(dividende[0], num1);*/
		dividende[0] = num1;
	}
	if(modulo && (dividende[0]->bdot > 0 || dividende[0]->dot > 0)){
		/*tmod = dup_nbr(dividende[0]);
		for(r1 = tmod->num->prev;tmod->val || tmod->bval; r1 = r1->prev){
			r1->full = r1->nmemb = r1->num = 0;
			if(tmod->val)
				tmod->val = 0;
			else
				tmod->bval--;
		}
		tmod->val = r1->prev->nmemb = 1;*/
		if((tmod = calloc(1, sizeof(struct nbr))) == NULL){
			perror("calloc()");
			exit(EXIT_FAILURE);
		}
		tmod->num = new_num(dividende[0]->bval + (dividende[0]->val > 0) +1, dividende[0]->bdot + (dividende[0]->dot > 0));
		for(r1 = dividende[0]->num, r2 = tmod->num;tmod->dot != dividende[0]->dot || tmod->bdot != dividende[0]->bdot; r1 = r1->next, r2=r2->next){
			r2->full = r1->full;
			r2->nmemb = r1->nmemb;
			r2->num = r1->num;
			/*if(!r2->full)*/
				tmod->dot += r1->nmemb;
				if(tmod->dot >= BLK){
					tmod->dot -= BLK;
					tmod->bdot++;
				}
			/*else
				tmod->bdot++;*/
		}
		tmod->val = r2->nmemb = 1;
	}
	for(r1 = dividende[0]->num; dividende[0]->bdot > 0 || dividende[0]->dot > 0; r1 = r1->next){
		if(dividende[0]->dot){
			dividende[0]->dot -= dividende[0]->num->nmemb;
		}else{
			dividende[0]->bdot--;
		}
		a = r1->num/mul[r1->nmemb-1];
	}
	for(r2 = dividende[0]->num, r1 = r1; r1; r1 = r1->next, r2 = r2->next){
		r2->num = r1->num;
		r2->nmemb = r1->nmemb;
		r2->full = r1->full;
	}
	for(r2 = r2;r2;r2 = r2->next){
		r2->num = r2->nmemb = r2->full = 0;
	}
	if(equal(dividende[0], diviseur) < 0){
		if(diviseur != num2)
			destroy_nbr(diviseur);
		if(dividende[0] != num1)
			destroy_nbr(dividende[0]);
		destroy_nbr(dividende[1]);
		quotient = dup_nbr(&zero);
		if(modulo)
			*modulo = dup_nbr(num1);
		/**modulo = NULL;*/
		/*printf("\tCalcule Reste\nReste: ");
		PRINT_NBR(mod);
		printf("\tFin Reste\n");
		destroy_nbr(mod);
		printf("Resultat: ");
		PRINT_NBR(quotient);*/
		return quotient;
	}
	/*printf("dividende:");
	PRINT_NBR(num1);
	printf("dividende:");
	PRINT_NBR(dividende[0]);
	printf("diviseur:");
	PRINT_NBR(num2);
	printf("diviseur:");
	PRINT_NBR(diviseur);*/
	#if M_BLK == 1
	if(dividende[0]->num->prev)
		for(lread = dividende[0]->num->prev;lread->nmemb == 0; lread = lread->prev);
	else
		lread = dividende[0]->num;
	/*
		Normalisation
	*/
	if(lread->num/mul[lread->nmemb-1]<5){
		for(	i = mul[1]-1;
			cmp_n1 = (lread->prev && lread->prev->next) ? lread->prev->num * i : 0,
			cmp_n1 /= mul[BLK],
			cmp_n1 += lread->num*i,
			(int)((unsigned int)cmp_n1/mul[lread->nmemb-1]) >= mul[1];
			i--
		);
		bnx.num = norm = i;
		/*printf("norm = %i\n", i);*/
		if(diviseur->val == 0 && diviseur == num2){
			if((diviseur = calloc(1, sizeof(struct nbr))) == NULL){
				perror("calloc()");
				exit(EXIT_FAILURE);
			}
			diviseur->num = new_num(num2->bval +1, 0);
			num_cpy(diviseur, num2);
			for(n1 = (diviseur->num->prev) ? diviseur->num->prev : diviseur->num;n1->nmemb == 0;n1 = n1->prev);
			cmp_n1 = n1->num*bnx.num + (diviseur->bval && diviseur->val) ? n1->prev->num * bnx.num / mul[BLK] : 0;
			if(/*n1->num*bnx.num/mul[BLK]*/ cmp_n1 / mul[BLK]> 0){
				if((diviseur = calloc(1, sizeof(struct nbr))) == NULL){
					perror("calloc()");
					exit(EXIT_FAILURE);
				}
				diviseur->num = new_num(num2->bval + 1, 0);
				(void)num_cpy(diviseur, num2);
			}
		}
		/*printf("%lu :: %i, %lu :: %i\n", diviseur->bval, diviseur->val, diviseur->bdot, diviseur->dot);
		PRINT_NBR(diviseur);
		for(nr = diviseur->num;nr;nr = nr->next)
			printf("%lu :: %i :: %i\n", nr->num, nr->nmemb, nr->full);*/
		SMALL_MUL(ret, diviseur, diviseur, bnx.num, n1, nr);
		/*for(nr = diviseur->num;nr;nr = nr->next)
			printf("%lu :: %i :: %i\n", nr->num, nr->nmemb, nr->full);
		exit(0);*/
		/*printf("= = = = = = = = =\n");
		PRINT_NBR(dividende[0]);*/
		SMALL_MUL(ret, dividende[0], dividende[0], bnx.num, n1, nr);
		/*PRINT_NBR(dividende[0]);
		printf("= = = = = = = = =\n");*/
		/*for(nr = diviseur->num;nr;nr = nr->next)
			printf("%lu :: %i :: %i\n", nr->num, nr->nmemb, nr->full);
		PRINT_NBR(diviseur);
		exit(0);*/
	}
	#endif
	num_cpy(dividende[1], dividende[0]);
	/************************************/
	if((quotient = calloc(1, sizeof(struct nbr))) == NULL){
		perror("calloc()");
		exit(EXIT_FAILURE);
	}
	quotient->num = new_num(num1->bval + (num1->val > 0),
					(bscale + (scale > 0) > num1->bdot + (num1->dot > 0))
						? bscale + (scale > 0) +1
						: num1->bdot + (num1->dot > 0) +1
				);
	num_cpy(quotient, &zero);
	if((reste = calloc(1, sizeof(struct nbr))) == NULL){
		perror("calloc()");
		exit(EXIT_FAILURE);
	}
	reste->num = breste = new_num(diviseur->bval + (diviseur->val > 0) +1, bscale + num2->bval + (num2->val > 0) + (scale + (approximation != 0) > 0) +2);
	for(blk = diviseur->bval + (diviseur->val > 0); blk > 1; blk--, breste = breste->next);
	if(dividende[0]->num->prev)
		for(r1 = dividende[0]->num->prev;r1->nmemb == 0; r1 = r1->prev);
	else
		r1 = dividende[0]->num;
	#if M_BLK == 1
	bcpy = nbytescpy(&breste, &r1, &start, diviseur->bval, diviseur->val);
	#else
	if(dividende[0]->val > M_BLK || dividende[0]->bval)
		cread = M_BLK;
	else
		cread = dividende[0]->val;
	if(cread > M_BLK)
		cread = M_BLK;
	bcpy = nbytescpy(&breste, &r1, &start, diviseur->bval, diviseur->val + cread -1);
	#endif
	reste->bval = bcpy->rblk;
	reste->val = bcpy->rbytes;
	lread = breste;
	r2 = &bnx;
	if(diviseur->num->prev)
		for(b2 = diviseur->num->prev ;b2->nmemb == 0; b2 = b2->prev);
	else
		b2 = diviseur->num;
	/*printf("%i => %lu\n", b2->nmemb, b2->num);
	exit(0);*/
	j = dividende[0]->val;
	bj = dividende[0]->bval;
	if(j < reste->val){
		j = BLK +j - reste->val;
		bj -= reste->bval+1;
	}else{
		j -= reste->val;
		bj -= reste->bval;
	}
	/*printf("%i\n", norm);
	printf("dividende:");
	PRINT_NBR(num1);
	printf("dividende:");
	PRINT_NBR(dividende[0]);
	printf("diviseur:");
	PRINT_NBR(num2);
	printf("diviseur:");
	PRINT_NBR(diviseur);
	printf("reste:");
	PRINT_NBR(reste);*/
	/*for(r1 = reste->num;r1; r1 = r1->next)
		printf("%lu :: %i\n", r1->num, r1->nmemb);*/
	/*if(tmod){
		printf("tmod:");
		PRINT_NBR(tmod);
	}*/
	/*PRINT_NBR(reste);
	printf("%lu :: %i\n", reste->bval, reste->val);*/
	#if M_BLK != 1
	for(j+=cread;;)
	#else
	#define cread 1
	for(j++;;){
	#endif
		#if M_BLK != 1
		if(j < M_BLK)
			cread = j;
		else
			cread = M_BLK;
		#endif
		bnx.num = 0;
		/*print_nbr(reste);
		putchar(':');
		print_nbr(diviseur);
		printf(">>> %lu :: %i, %lu :: %i == %i\n", reste->bval, reste->val, diviseur->bval, diviseur->val, equal(reste, diviseur));*/
		/*while(equal(reste, diviseur) >= 0){
			bnx.num++;
			for(n1 = diviseur->num,nr = reste->num, reste->bval = reste->val = 0, cmp_n2 = ret = ret_ = 0;
				n1 && n1->nmemb > 0;
				n1 = n1->next,
				nr = nr->next
			){
				cmp_n1 = n1->num * 1 + ret;
				ret = cmp_n1/mul[BLK];
				cmp_n1 %= mul[BLK];
				if(nr->num >= cmp_n1 +ret_){
					cmp_n2 = nr->num - cmp_n1 - ret_;
					ret_ = 0;
				}else{
					nrmemb = cmp_n1/mul[BLK];
					if(nrmemb == 0)
						nrmemb = 1;
					if(nrmemb * mul[BLK] < (int)nr->num)
						nrmemb++;
					cmp_n2 = nrmemb*mul[BLK] + (nr->num - ret_)/ - cmp_n1;
					ret_ = 1;
				}
				nr->num = cmp_n2;
				nr->nmemb = BLK;
				nr->full = 1;
				if(n1->next && n1->next->nmemb == 0){
					if(nr->next)
						nr->next->num = nr->next->nmemb = nr->next->full = 0;
					if(nr->num/mul[nr->nmemb-1] == 0){
						nr->nmemb--;
					}
					if(nr->nmemb == BLK){
						nr->full = 1;
						reste->bval++;
					}else{
						reste->val = nr->nmemb;
						nr->full = 0;
					}
					break;
				}else
					reste->bval++;
			}
			ADJUST_0(reste, b1, b2);
		}
		goto next;*/
		if(equal(reste, diviseur) >= 0){
			/*
			bin psp1, psp2 *sp1, *sp2
			int sp1_s1, sp2_s2;
			sp1 = &psp1;
			sp2 = &psp2;*/
					
			if(reste->num->prev){
				for(b1 = reste->num->prev;b1->nmemb == 0; b1 = b1->prev);
			}else
				b1 = reste->num;
			if(b2->prev && b2->prev->next){
			#if M_BLK == 1
				
				if(b1->nmemb == b2->nmemb)
					cmp_n1 = b1->num * mul[BLK-b1->nmemb] + b1->prev->num/mul[b1->nmemb];
				else
					cmp_n1 = b1->num * mul[BLK-b1->nmemb+1] + b1->prev->num/mul[b1->nmemb-1];
				cmp_n2 = b2->num * mul[BLK-b2->nmemb] + b2->prev->num/mul[b2->nmemb];
				/*for(bnx.num = mul[1] -1;cmp_n2 * bnx.num > cmp_n1;bnx.num--);*/
				/*printf("%lu :: %lu, %lu :: %lu => ", b1->num, b1->prev->num, b2->num, b2->prev->num);*/
				bnx.num = cmp_n1/cmp_n2;
				/*printf("%lu\n", bnx.num);*/
				if((int)bnx.num > mul[1]-1)
					bnx.num--;
				/*for(ret = b1->num - b2->num * bnx.num, s1 = b1->prev, s2 = b2->prev; s1->next; s1 = s1->prev, s2 = s2->prev){
					printf("%i :: %i\n", ret, (int)s1->num - s2->num * bnx.num);
					if(ret < (ret = s1->num - s2->num * bnx.num)){
						bnx.num--;
						break;
					}
				}*/
				/*reading = cmp_n2 - bnx.num * cmp_n1;
				while(b1->prev && b1->prev->next){
					b1 = b1->prev;
					b2 = b2->prev;

				}*/
				
			}else{
				if(b1->prev && b1->prev->next /*diviseur->bval + (diviseur->val > 0) > 1*/)
					cmp_n2 = b1->num*mul[b2->nmemb] + b1->prev->num/mul[BLK-b2->nmemb];
				else
					cmp_n2 = b1->num;
				cmp_n1 = b2->num;
				/*for(	bnx.num = mul[M_BLK]-1;
					cmp_n1 = b2->num*bnx.num,
					cmp_n1 > cmp_n2 && bnx.num > 0;
					bnx.num--
				);*/
				/*printf("%lu :: %lu\n", cmp_n1, cmp_n2);*/
				bnx.num = cmp_n2/cmp_n1;
			}
			/*if(bnx.num == 0)
				printf("ERROR:%lu, %lu\n", b1->num, b2->num);*/
			#else
				for(	bnx.num = mul[cread]-1;
					cmp_n2 = b2->prev->num * bnx.num,
					cmp_n1 = cmp_n2 / mul[BLK],
					cmp_n2 /= mul[BLK],
					cmp_n1 += b2->num*bnx.num,
					cmp_n1 = cmp_n1 * mul[BLK] + cmp_n2,
					cmp_n2 = b1->num * mul[BLK] + b1->prev->num,
					cmp_n1 > cmp_n2 /*(cmp_n1 > b1->num || cmp_n2 > b1->prev->num)*/ && bnx.num > 0;
					bnx.num--
				);
			else
				for(	bnx.num = mul[cread]-1;
					cmp_n1 = b2->num*bnx.num,
					cmp_n1 > b1->num && bnx.num > 0;
					bnx.num--
				);
			#endif
			/*printf("reste [%lu] : ", bnx.num);
			print_nbr(reste);
			putchar('-');
			print_nbr(diviseur);
			printf("*%lu = \n", bnx.num);*/
			/*ADJUST_0(reste, b1, b2);*/
			for(n1 = diviseur->num,nr = reste->num, reste->bval = reste->val = 0/*, nrmemb = nr->nmemb*/, cmp_n2 = ret = ret_ = 0;
				/*n1 && n1->nmemb > 0*/;
				n1 = n1->next,
				nr = nr->next
			){
				/*printf("%lu * %lu + %i = %lu\n", n1->num, bnx.num, ret,n1->num * bnx.num + ret);*/
				cmp_n1 = n1->num * bnx.num + ret;
				ret = cmp_n1/mul[BLK];
				cmp_n1 %= mul[BLK];
				if(nr->num >= cmp_n1 +ret_){
					cmp_n2 = nr->num - cmp_n1 - ret_;
					ret_ = 0;
				}else{
					nrmemb = cmp_n1/mul[BLK];
					if(nrmemb == 0)
						nrmemb = 1;
					if(nrmemb * mul[BLK] < (int)nr->num)
						nrmemb++;
					cmp_n2 = nrmemb*mul[BLK] + (nr->num - ret_) - cmp_n1;
					/*cmp_n2 = cmp_n1 - (nr->num - ret_);*/
					ret_ = 1;
				}
				nr->num = cmp_n2;
				nr->nmemb = BLK;
				nr->full = 1;
				if(!n1->next || n1->next->nmemb == 0){
					if(nr->next && nr->next->num)
						ret_ = 0;
					if(nr->next)
						nr->next->num = nr->next->nmemb = nr->next->full = 0;
					if(nr->num/mul[nr->nmemb-1] == 0){
						nr->nmemb--;
					}
					if(nr->nmemb == BLK){
						nr->full = 1;
						reste->bval++;
					}else{
						reste->val = nr->nmemb;
						nr->full = 0;
					}
					break;
				}else
					reste->bval++;
			}
			/*printf("%lu => ", bnx.num);*/
			if(ret_){
				bnx.num--;
				for(s1 = nr = reste->num, s2 = diviseur->num, ret = ret_ = 0;s1 && s1->nmemb;s1 = s1->next, s2 = s2->next, nr = nr->next){
					printf("%lu :: %lu (%lu):: %i\n", s2->num, s1->num+1, mul[s1->nmemb], ret);
					printf("%i\n", (int)(mul[s1->nmemb] + s2->num - (s1->num + ret)));
					nr->num = (int)(mul[s1->nmemb] + s2->num - (s1->num + ret));
					ret = 1;
					/*nr->nmemb = s1->nmemb;
					nr->full = s1->full;
					if(nr->full)
						reste->bval++;
					else
						reste->val = s1->nmemb;*/
					/*nr->num = mul[BLK] - s1->num - ret;
					if((unsigned int)s2->num >= (unsigned int)(nr->num -= ret_)){
						nr->num = s2->num - nr->num;
						ret_ = 0;
					}else{
						nr->num = mul[BLK] - s2->num - nr->num;
						ret_ = 1;
					}
					ret = 1;*/
				}
				/*reste->neg = 1;*/
				/*if(diviseur->num->prev)
					for(b2 = diviseur->num->prev ;b2->nmemb == 0; b2 = b2->prev);
				else
					b2 = diviseur->num;*/
			}
			/*PRINT_NBR(reste);*/
			ADJUST_0(reste, b1, b2);
			/*for(b1 = reste->num;b1;b1 = b1->next)
				printf("%lu :: %i\n", b1->num, b1->nmemb);
			PRINT_NBR(reste);*/
		}else
			bnx.num = 0;
		if(quotient->bval == 0 && quotient->num->num == 0){
			quotient->val = quotient->num->nmemb = 1;
			quotient->num->num = bnx.num;
		}else{
			/*printf("mv dot\n");*/
			(void)mv_dot(quotient, quotient, 0, cread);
			quotient->num->num += bnx.num;
		}
		/*printf(">>%lu(%i)\n", bnx.num, cread);
		for(nr = quotient->num; nr; nr = nr->next)
			printf("%lu :: %i\n", nr->num, nr->nmemb);
		PRINT_NBR(quotient);*/
		/*exit(0);*/
		#if M_BLK == 1
		if(--j == 0){
			if(bj > 0){
				bj--;
				j = BLK;
			}else
				break;
		}
		#else
		if((j -= cread) <= 0){
			if(bj > 0){
				bj--;
				j = BLK;
				cread = M_BLK;
			}else{
				break;
			}
		}
		#endif
		bnx.num = 0;
		bnx.nmemb = 0;
		if(reste->num->nmemb > 0){
			(void)mv_dot(reste, reste, 0, cread);
		}else
			reste->val = reste->num->nmemb = cread;
		bcpy = nbytescpy(&r2, &r1, &start, 0, cread);
		reste->num->num += bnx.num;
		/*printf("%lu, ", bnx.num);
		PRINT_NBR(reste);
		for(b1 = reste->num;b1;b1 = b1->next)
			printf("%lu :: %i :: %i\n", b1->num, b1->nmemb, b1->full);*/
	}
	/*for(nr = (reste->num->prev) ? reste->num->prev : reste->num; nr->nmemb == 0; nr = nr->prev);*/
	/*for(reste->bval = reste->val = 0,nr = reste->num;
		nr && nr->nmemb == BLK;
		nr = nr->next, reste->bval++
	);
	if(nr)
		reste->val = nr->nmemb;*/
	/*for(;nr->num/mul[nr->nmemb-1] == 0;nr->nmemb--){
		nr->full = 0;
		if(reste->val == 0){
			reste->val = BLK-1;
			reste->bval--;
		}else
			reste->val--;
	}*/
	/*PRINT_NBR(quotient);
	PRINT_NBR(reste);*/
	/*printf("%lu :: %i\n", reste->bval, reste->val);*/
	/*ADJUST_0(quotient, r1,r2);
	ADJUST_0(reste, r1,r2);*/
	if(approximation){
		if(reste->bval > 0 || reste->num->num > 0){
			temp = mv_dot(reste, NULL, 0, 1);
		}
		if(a > 0){
			temp->num->num = a;
		}
		if(equal(temp, diviseur) >= 0){
			for(b1 = reste->num->prev;b1->nmemb == 0; b1 = b1->prev);
			for(	bnx.num = 9;
				cmp_n1 = (b2->prev) ? b2->prev->num * bnx.num : 0,
				cmp_n1 /= mul[BLK],
				cmp_n1 += b2->num*bnx.num,
				cmp_n1 > b1->num && bnx.num > 0;
				bnx.num--
			);
		}else
			bnx.num = 0;
		destroy_nbr(temp);
		if(bnx.num >= 5){
			n.num = &bnx;
			bnx.num = 1;
			(void)addition(&n, quotient, quotient);
		}
	}
	if(bscale || scale){
		/*printf("RESULT = ");
		PRINT_NBR(quotient);*/
		(void)bymin10(quotient, quotient, bscale, scale);
		/*printf("Resultat:\t");
		PRINT_NBR(quotient);*/
	}
	if(modulo){
		if(equal(reste, &zero) != 0){
			idx = scale;
			bidx = bscale;
			if(idx >= BLK){
				idx -= BLK;
				bidx++;
			}
			/*printf("%lu :: %i, %lu :: %i\n", tmod->bval, tmod->val, tmod->bdot, tmod->dot);*/
			/*idx = bidx = 0;*/
			/*if(tmod){
				idx = tmod->dot;
				bidx = tmod->bdot;
				if(idx >= BLK){
					idx -= BLK;
					bidx++;
				}
			}
			if(idx < num1->dot){
				idx += BLK;
				bidx--;
			}
			idx -= num1->dot;*/
			idx += num2->dot;
			if(idx >= BLK){
				idx -= BLK;
				bidx++;
			}
			bidx += num2->bdot;
			/*printf("MODULO (%i), %lu %i:\n", norm, bidx, idx);
			PRINT_NBR(reste);*/
			/*printf("%lu :: %i\n", reste->bval, reste->val);*/
			if(norm){
				printf("NORM = %i (%lu::%i)\n", norm, bidx, idx);
				bnx.num = norm;
				n.num = &bnx;
				n.val = bnx.nmemb = 1;
				n.bval = bnx.full = 0;
				PRINT_NBR(reste);
				mod = kdivision(reste, &n, NULL, 0, 0, 0, NULL);
				PRINT_NBR(mod);
				/*reset_num(reste);*/
				if(tmod){
					(void)addition(tmod, mod, tmod);
					/*PRINT_NBR(tmod);
					exit(0);*/
					/*for(nr = tmod->num; nr; nr = nr->next)
						printf("%lu :: %i\n", nr->num, nr->nmemb);*/
					/*printf("%lu :: %i, %lu :: %i\n", tmod->bval, tmod->val, tmod->bdot, tmod->dot);*/
					/*printf("* * * * * *\n");
					for(r1 = tmod->num;r1;r1=r1->next)
						printf("%lu :: %i\n", r1->num, r1->nmemb);*/
					(void)bymin10(tmod, reste, bidx, idx);
					/*for(r1 = reste->num;r1;r1=r1->next)
						printf("%lu :: %i\n", r1->num, r1->nmemb);
					PRINT_NBR(reste);*/
					destroy_nbr(mod);
				}else{
					(void)bymin10(mod, reste, bidx, idx);
					/*PRINT_NBR(reste);
					exit(0);*/
					destroy_nbr(mod);
				}
				/*printf("kdiv:");
				PRINT_NBR(reste);*/
			}else{
				/*PRINT_NBR(reste);
				printf("%lu :: %i\n", reste->bval, reste->val);
				for(r1 = reste->num;r1;r1 = r1->next)
					printf("%lu :: %i :: %i\n", r1->num, r1->nmemb, r1->full);
				PRINT_NBR(tmod);
				printf("%lu :: %i\n", tmod->bval, tmod->val);
				for(r1 = tmod->num;r1;r1 = r1->next)
					printf("%lu :: %i :: %i\n", r1->num, r1->nmemb, r1->full);*/
				if(tmod){
					/*printf("**********\n");
					printf("%lu :: %i\n", bidx, idx);
					PRINT_NBR(tmod);
					printf("%lu :: %i, %lu :: %i\n", tmod->bval, tmod->val, tmod->bdot, tmod->dot);*/
					(void)addition(tmod, reste, tmod);
					/*PRINT_NBR(tmod);*/
					/*printf("%lu :: %i, %lu :: %i => %lu :: %i\n",
							tmod->bval, tmod->val, tmod->bdot, tmod->dot, bidx, idx);
					for(r1 = tmod->num;r1;r1=r1->next)
						printf("%lu :: %i\n", r1->num, r1->nmemb);
					printf("%lu :: %i\n", bidx, idx);*/
					(void)bymin10(tmod, reste, bidx, idx);
					/*print_nbr(reste);
					printf(">>%lu :: %i, %lu :: %i\n", reste->bval, reste->val, reste->bdot, reste->dot);
					exit(0);*/
				}else{
					/*printf("* * * * *(%lu :: %i)\n", bidx, idx);*/
					(void)bymin10(reste, reste, bidx, idx);
				}
			}
			if(tmod)
				destroy_nbr(tmod);
		}
		/*for(r1 = reste->num;r1;r1=r1->next)
			printf("%lu :: %i\n", r1->num, r1->nmemb);
		printf(">>");
		PRINT_NBR(reste);*/
		ADJUST_0(reste, r1,r2);
		*modulo = reste;
		/*PRINT_NBR((*modulo));*/
	}else
		destroy_nbr(reste);
	if(diviseur != num2)
		destroy_nbr(diviseur);
	if(dividende[0] != num1)
		destroy_nbr(dividende[0]);
	destroy_nbr(dividende[1]);
	/*destroy_nbr(reste);*/
	return quotient;
}
