#include "operation.h"
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
	if(dot)
		for(;*(pn-1) == '0'; pn--, len--);
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
	int dot = (n->dot > 0 || n->bdot > 0), i, k, num, len = n->val;
	if(n->neg)
		putchar('-');
	for(b = (n->num->prev) ? n->num->prev : n->num; b != NULL;b = (b == n->num) ? NULL : b->prev){
		if(b->full){
			j = jj;
		}else
			for(j = 1, i = 0, k = b->nmemb-1; i < k; i++)
				j *= 10;
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
	struct bin *b, *pb;
	for(b = n->num; b != NULL;){
		pb = b->next;
		free(b);
		b = pb;
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
	len = (len1 < len2)? len1 : len2;
	blen = (blen1 < blen2) ? blen1 : blen2;
	sdot = num1->val;
	sbdot = num1->bval;
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
		if(sbdot && b1->full)
			sbdot--;
		b1 = b1->prev;
		b2 = b2->prev;
		if(!sbdot && !sdot)
			break;
	}while(len != 0 || blen != 0);
	if(b1 && b2)
		do{
			if(b1->nmemb != b2->nmemb){
				if(b1->nmemb > b2->nmemb){
					for(dmul = 1, i = b1->nmemb - b2->nmemb; i > 0; i--)
						dmul *= 10;
					/*dmul = 10^(b1->nmemb - b2->nmemb);*/
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
			if(len)
				len -= (len1 < len2) ? b1->nmemb : b2->nmemb;
			else
				blen--;
			b1 = b1->prev;
			b2 = b2->prev;
		}while(len != 0 || blen != 0);
	if((b1 && b1 != num1->num->prev) && (b2 && b2 != num2->num->prev))
		return 0;
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
	struct bin *new = NULL, *pnew;
	unsigned long int lval = val, ldot = dot;
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
		if(lval > 0)
			lval--;
		else
			ldot--;
	}
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
	for(b1 = num->num, b2 = res->num; b1; b1 = b1->next, b2 = b2->next){
		b2->num = b1->num;
		b2->nmemb = b1->nmemb;
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
	r = (num->num->prev) ? num->num->prev : num->num;
	if(r->nmemb+faclen > BLK){
		if((nw = calloc(1, sizeof(struct bin))) == NULL){
			perror("calloc()");
			exit(EXIT_FAILURE);
		}
		r->next = nw;
		r->next->prev = r;
		nw->next = NULL;
		num->num->prev = nw;
		nw->nmemb = r->nmemb + faclen - BLK;
	}else
		r->nmemb += faclen;
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
			/*if((*b2)->nmemb == BLK)
				(*b2)->full = 1;*/
			/*(*b2)->full = 1;*/
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
	nadd = multiplication(dres, pui);
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
void *addition(struct nbr *num1, struct nbr *num2){
	struct bin *pb1, *pb2, *pbr;
	struct nbr *res, *n1, *n2;
	unsigned long int bdot, bval, cbdot = 0;
	unsigned long int b1n, b2n;
	int i, j, nmemb1, nmemb2, add = 0, retenue = 0,
		cf, full = 0, full1 = 0, full2 = 0, dot, val, cdot = 0, mul[C_BLK] = COEFS;
	if(num1->neg && !num2->neg){
		num1->neg = 0;
		res = soustraction(num2, num1);
		num1->neg = 1;
		return res;
	}else{
		if(!num1->neg && num2->neg){
			num2->neg = 0;
			res = soustraction(num1, num2);
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
		b2n = pb2->num;
		pbr;
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
				if(add != i)
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
	if(res->num->prev && res->num->prev->nmemb == 0 && res->num->prev->num == 0){
		pb1 = res->num->prev;
		res->num->prev = pb1->prev;
		pb1->prev->next = NULL;
		free(pb1);
	}
	res->neg = (num1->neg && num2->neg);
	/*if(dres)
		destroy_nbr(dres);*/
	/*if(nadd)
		destroy_nbr(nadd);*/
	return res;
}
void *soustraction(struct nbr *num1, struct nbr *num2){
	struct bin *pb1, *pb2, *pbr;
	struct nbr *res, *pn1, *pn2;
	unsigned long int n1, n2, cbdot = 0;
	int i, nmemb1, nmemb2, neg = 0, cf, mul[C_BLK] = COEFS, retenue = 0, cdot = 0;
	if(num1->neg && !num2->neg){
		num1->neg = 0;
		res = addition(num2, num1);
		num1->neg = res->neg = 1;
		return res;
	}else{
		if(!num1->neg && num2->neg){
			num2->neg = 0;
			res = addition(num2, num1);
			num2->neg = 1;
			return res;
		}else{
			if(num1->neg && num2->neg){
				num2->neg = 0;
				num1->neg = 0;
				res = soustraction(num2, num1);
				num2->neg = 1;
				num1->neg = 1;
				return res;
			}
		}
	}
	if((res = calloc(1, sizeof(struct nbr))) == NULL){
		perror("calloc()");
		exit(EXIT_FAILURE);
	}
	if(equal(num1, num2) == 0){
		if((res->num = calloc(1, sizeof(struct bin))) == NULL){
			perror("calloc()");
			exit(EXIT_FAILURE);
		}
		res->num->num = 0;
		res->num->nmemb = 1;
		res->val = 1;
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
	pbr = res->num = new_num(res->bval + (res->val > 0), res->bdot + (res->dot > 0));
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
		if(res->num->prev)
			res->num->prev->num += retenue*mul[res->num->prev->nmemb-1];
		else
			res->num->num += retenue*mul[res->num->nmemb-1];
	}
	/*print_nbr(res);
	putchar('\n');*/
	ADJUST_0(res, pb1, pb2);
	res->neg = neg;
	/*if(nadd)
		destroy_nbr(nadd);*/
	return res;
}
void *multiplication(struct nbr *num1, struct nbr *num2){
	struct bin *bn1, *bn2, *pbr1, *pbr2, *pbr3, z = ZERO_BIN;
	struct nbr *res, zero = ZERO( NULL );
	unsigned long int n1, n2, n3, dot, bdot, dot_, val, bval, retenue;
	int ret, nmemb1, rnmemb1, nmemb2, rnmemb2, nmemb3, rnmemb3, mul[C_BLK] = COEFS;
	zero.num = &z;
	if((res = calloc(1, sizeof(struct nbr))) == NULL){
		perror("calloc()");
		exit(EXIT_FAILURE);
	}
	if(equal(num1, &zero) == 0 || equal(num2, &zero) == 0){
		if((res->num = calloc(1, sizeof(struct bin))) == NULL){
			perror("calloc()");
			exit(EXIT_FAILURE);
		}
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
	pbr1 = res->num = new_num(bval + (val > 0), res->bdot + (res->dot > 0));
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
		}
		if(nmemb1 == BLK){
			nmemb1 = 0;
		}
		ret = 0;
		retenue = 0;
		pbr1 = pbr1->next;
	}
	if(dot){
		pbr1 = res->num->prev;
		pbr1->num *= mul[pbr1->prev->nmemb - dot];
		pbr1->num += pbr1->prev->num/mul[dot];
		pbr1->prev->num -= (pbr1->prev->num/mul[dot])*mul[dot];
		pbr1->nmemb += pbr1->prev->nmemb - dot;
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
	ADJUST_0(res, pbr1, pbr2);
	if(num1->neg != num2->neg)
		res->neg = 1;
	return res;
}
void *ispuissance(struct nbr *num, int pui){
	struct bin _un_ = { 1, 1, 0, NULL, NULL };
	struct nbr *n = num, *res = NULL, un = INIT_NBR( 0, 0, 1, 0, 0, NULL, NULL ), *pseudo = &un, *p;
	int comp = pui;
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
		return res;
	}
	while(comp > 1){
		if(comp%2){
			comp--;
			p = multiplication(n, pseudo);
			if(pseudo != &un)
				destroy_nbr(pseudo);
			pseudo = p;
		}else{
			comp /= 2;
			res = multiplication(n, n);
			if(n != num)
				destroy_nbr(n);
			n = res;
		}
	}
	if(pseudo != &un){
		res = multiplication(pseudo, n);
		destroy_nbr(pseudo);
		if(n != num)
			destroy_nbr(n);
	}else{
		if(n == num)
			res = dup_nbr(n);
	}
	return res;
}
void *spuissance(struct nbr *num, unsigned long int bpui, int pui){
	struct bin _un_ = { 1, 1, 0, NULL, NULL };
	struct nbr *n = num, *tn, *in, *res = NULL, un = INIT_NBR( 0, 0, 1, 0, 0, NULL, NULL ), *pseudo = &un, *p;
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
		return res;
	}
	in = ispuissance(num, pui);
	if(!bpui)
		return in;
	tn = n = ispuissance(num, BLK);
	while(bcomp > 1){
		if(bcomp%2){
			bcomp--;
			p = multiplication(n, pseudo);
			if(pseudo != &un)
				destroy_nbr(pseudo);
			pseudo = p;
		}else{
			bcomp /= 2;
			res = multiplication(n, n);
			if(n != num)
				destroy_nbr(n);
			n = res;
		}
	}
	if(pseudo != &un){
		res = multiplication(pseudo, n);
		destroy_nbr(pseudo);
		if(n != num)
			destroy_nbr(n);
	}else{
		if(n == num)
			res = dup_nbr(n);
	}
	if(n != tn){
		n = res;
	}
	res = multiplication(n, in);
	destroy_nbr(n);
	destroy_nbr(in);
	return res;
}
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
		/*printf("division_1\n");*/
		res->num = new_num(1, bscale + (scale > 0));
		/*print_nbr(num);
		putchar('\n');*/
		for(	bt = (num->num->prev) ? num->num->prev : num->num,
			bs = res->num->prev->prev,
			/*bval = bscale - num->bval,*/
			bval = bscale - num->bval,
			(scale >= num->val && (val = scale - num->val) >= 0) || (bval--, val = BLK + scale - num->val),
			len = num->val,
			blen = num->bval;
			;
		){
			/*exit(0);*/
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
		/*printf(" => %lu\n", res->num->prev->prev->num);*/
		/*exit(0);*/
	}else{	/*printf("division_2\n");*/
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
				bs = bs->next;
			}
			for(blen = bscale; bs;blen--, bs = bs->next){
				bs->num = bt->num/mul[x];
				bs->nmemb = bt->nmemb-x;
				if(!(bt = bt->next))
					break;
				bs->num += (bt->num%mul[x])*mul[BLK-x];
				bs->nmemb = BLK;
				bs->full = 1;
				bs->num -= (bs->num/mul[BLK]) * mul[BLK];
			}
			/*ICI*/
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
			/*printf("%lu :: %i\n", res->num->prev->num, res->val);*/
			if(res->num->prev->nmemb == 0)
				res->num->prev->nmemb = res->val = 1;
		}else{
			res = num;
		}
	}
	return res;
}
void *division(struct nbr *num1, struct nbr *num2, struct nbr **modulo, unsigned long int bscale, int scale, int approximation){
	struct retbcpy *bcpy;
	struct bin *bdividende, *breste,
		bdix = { 10, 2, 0, NULL, NULL }, bsingle = { 0, 1, 0, NULL, NULL }, *bs, *bt, bx = { 0, 1, 0, NULL, NULL };
	struct nbr *res = NULL, *diviseur, *dividende, *quotient, *mod, *reste = NULL,
		dix = INIT_NBR( 0, 0, 2, 0, 0, NULL, "10" ), *fac, *fac_, *temp, *temp_, *temp__, nx = INIT_NBR( 0, 0, 1, 0, 0, NULL, NULL );
	unsigned long int bval, blen, bdot_0 = 0, cbscale, bfac;
	int neg1, neg2, x, start = 0, val, len, dot_0 = 0, cscale, cfac;
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
		if((*modulo = calloc(1, sizeof(struct nbr))) == NULL){
			perror("calloc()");
			exit(EXIT_FAILURE);
		}
		res->num = new_num(1, 0);
		(*modulo)->num = new_num(1, 0);
		res->val = res->num->nmemb = (*modulo)->val = (*modulo)->num->nmemb = 1;
		return res;
	}
	if((neg1 = num1->neg)){
		num1->neg = 0;
	}
	if((neg2 = num2->neg)){
		num2->neg = 0;
	}
	if(num2->dot || num2->bdot){
		fac = &dix;
		bdot_0 = num2->bdot;
		dot_0 = num2->dot;
		/*do{*/
			/*for(;bdot_0 > 0 && dot_0 + BLK > dot_0;bdot_0--);*/
			fac_ = spuissance(fac, bdot_0, dot_0);
			if(fac != &dix)
				destroy_nbr(fac);
			fac = fac_;
		/*}while(bdot_0 || dot_0);*/
		diviseur = multiplication(num2, fac);
		dividende = multiplication(num1, fac);
		destroy_nbr(fac);
		DOT(diviseur, bs);
	}else{
		diviseur = num2;
		dividende = num1;
	}
	if(approximation){
		fac = spuissance(&dix, 0, 1);
		temp = multiplication(dividende, fac);
		if(dividende != num1)
			destroy_nbr(dividende);
		destroy_nbr(fac);
		dividende = temp;
	}
	if(scale || bscale){
		fac = spuissance(&dix, bscale, scale);
		temp = multiplication(dividende, fac);
		if(dividende != num1)
			destroy_nbr(dividende);
		destroy_nbr(fac);
		dividende = temp;
	}
	if(dividende == num1){
		dividende = dup_nbr(num1);
	}
	while(dividende->bdot > 0 || dividende->dot > 0){
		if(dividende->dot){
			dividende->dot -= dividende->num->nmemb;
		}else{
			dividende->bdot--;
		}
		bt = dividende->num;
		dividende->num = dividende->num->next;
		dividende->num->prev = (bt->prev != dividende->num) ? bt->prev : NULL;
		free(bt);
	}
	DOT(dividende, bs);
	/*print_nbr(diviseur);
	putchar('\n');
	print_nbr(dividende);
	putchar('\n');
	exit(0);*/
	if(diviseur == num2){
		diviseur = dup_nbr(num2);
	}
	bdividende = (dividende->num->prev) ? dividende->num->prev: dividende->num;
	bval = dividende->bval;
	val = dividende->val;
	blen = diviseur->bval;
	len = diviseur->val;
	if(blen < bval || (blen == bval && len <= val)){
		if((reste = calloc(1, sizeof(struct nbr))) == NULL){
			perror("calloc()");
			exit(EXIT_FAILURE);
		}
		reste->num = new_num(blen + (len > 0), 0);
		breste = (reste->num->prev) ? reste->num->prev : reste->num;
		/*reste->len = nbytescpy(&breste, &bdividende, &start, len);*/
		bcpy = nbytescpy(&breste, &bdividende, &start, blen, len);
		/*val -= reste->val;*/
		reste->val = bcpy->rbytes;
		reste->bval = bcpy->rblk;
		if(val < reste->val && bval > 0){
			val += BLK;
			bval--;
		}
		val -= reste->val;
		bval -= reste->bval;
		if(equal(reste, diviseur) < 0 && (val > 0 || bval > 0)){
			bymul10(reste, 10, 1);
			bs = &bsingle;
			nbytescpy(&bs, &bdividende, &start, 0, 1);
			reste->num->num += bsingle.num;
		}
		if((quotient = calloc(1, sizeof(struct nbr))) == NULL){
			perror("calloc()");
			exit(EXIT_FAILURE);
		}
		if((quotient->num = calloc(1, sizeof(struct bin))) == NULL){
			perror("calloc()");
			exit(EXIT_FAILURE);
		}
		bval = dividende->bval - reste->bval;
		if(dividende->val >= reste->val)
			val = dividende->val - reste->val + (approximation == 0);
		else{
			val = BLK + dividende->val - reste->val + (approximation == 0);
			bval--;
		}
		if(val > BLK){
			val -= BLK;
			bval++;
		}
		for(;;){
			if(equal(reste, diviseur) >= 0){
				for(x = 9; x > 0; x--){
					bx.num = x;
					temp = multiplication(diviseur, &nx);
					if(equal(reste, temp) >= 0){
						temp_ = soustraction(reste, temp);
						destroy_nbr(reste);
						destroy_nbr(temp);
						reste = temp_;
						break;
					}else{
						destroy_nbr(temp);
					}
				}
				quotient = bymul10(quotient, 10, 1);
				quotient->num->num += x;
			}else{
				quotient = bymul10(quotient, 10, 1);
				x = 0;
			}
			if(val || bval){
				if(val)
					val--;
				else{
					val = BLK-1;
					bval--;
				}
				if(!val && !bval){
					*modulo = dup_nbr(reste);
					if(approximation == 0)
						break;
				}
			}else
				break;
			bs = &bsingle;
			bsingle.num = bsingle.nmemb = 0;
			nbytescpy(&bs, &bdividende, &start, 0, 1);
			bymul10(reste, 10, 1);
			reste->num->num += bs->num;
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
		quotient->val = quotient->num->nmemb = 1;
		*modulo = dup_nbr(dividende);
	}
	if(reste && !*modulo){
		*modulo = dup_nbr(reste);
	}
	if(approximation && reste){
		if(x >= 5){
			fac = addition(&dix, quotient);
			destroy_nbr(quotient);
			quotient = fac;
		}
		for(bt = (quotient->num->prev) ? quotient->num->prev : quotient->num, bt->nmemb--; ;){
			x = bt->num%10;
			bt->num/=10;
			if(!(bt = bt->prev) || bt == quotient->num->prev){
				break;
			}
			bt->num += (unsigned long int)(x)*D_MAX_LIMIT;
		}
		if(quotient->val > 0)
			quotient->val--;
		else{
			quotient->val = BLK-1;
			quotient->bval--;
		}
	}
	if(bscale || scale){
		res = bymin10(quotient, bscale, scale);
	}
	/*printf("%i, %i\n",(num1->bdot > bdot_0 && bscale <= num1->bdot - bdot_0), (
			num1->bdot == bdot_0 && bscale == num1->bdot - bdot_0
			&& num1->dot > dot_0 && scale < num1->dot - dot_0
		)
	);*/
	/*printf("%lu::%i, %lu::%i, %lu::%i\n", num1->bdot, num1->dot, bdot_0, dot_0, bscale, scale);*/
	if(	(num1->bdot > bscale || (num1->bdot == bscale && num1->dot > scale))
		|| (
			num1->bdot == bdot_0 && bscale == num1->bdot - bdot_0
			&& num1->dot >= dot_0 && scale < num1->dot - dot_0
		)
	){
		/*printf("++++++++++\n");*/
		/*printf("******\n");*/
		cbscale = bdot_0 + bscale;
		cscale = dot_0 + scale;
		if(cscale >= BLK){
			cbscale++;
			cscale -= BLK;
		}
		bfac = num1->bdot - cbscale;
		if(cscale < num1->dot)
			cfac = num1->dot - cscale;
		else{
			bfac--;
			cfac = BLK + num1->dot - cscale;
		}
		if((num1->bdot > cbscale || (num1->bdot == cbscale && num1->dot > cscale))){
			/*printf("%lu :: %lu , %i :: %i\n", num1->bdot, bdot_0 + bscale, BLK + num1->dot, dot_0 + scale);*/
			fac = spuissance(&dix, /*num1->bdot - bdot_0 - bscale*/bfac, /*num1->dot - dot_0 - scale*/cfac);
			dot_0 += num1->dot - dot_0 - scale;
			bdot_0 += num1->bdot - bdot_0 - bscale;
		}else{
			if(	(num1->bdot > cbscale/* && BLK + num1->dot > cscale*/)){
				/*printf("ok:%i\n",BLK + num1->dot > dot_0 + scale);
				exit(0);*/
				fac = spuissance(&dix, /*num1->bdot - bdot_0 - bscale - 1*/bfac, /*BLK + num1->dot - dot_0 - scale*/cfac);
				dot_0 += BLK + num1->dot - dot_0 - scale;
				bdot_0 += num1->bdot - bdot_0 - bscale - 1;
			}else{
				fac = &nx;
				nx.num->num = 1;
			}
		}
		if(dot_0 >= BLK){
			bdot_0++;
			dot_0 -= BLK;
		}
		if(fac != &nx){
			/*print_nbr(*modulo);
			putchar('\n');*/
			temp = multiplication(*modulo, fac);
			temp__ = multiplication(dividende, fac);
			destroy_nbr(fac);
			fac = spuissance(&dix, num1->bdot, num1->dot);
			temp_ = multiplication(num1, fac);
			destroy_nbr(fac);
			DOT(temp_, bs);
			/*print_nbr(temp_);
			putchar('-');
			print_nbr(temp__);
			putchar('\n');*/
			mod = soustraction(temp_, temp__);
			destroy_nbr(temp_);
			destroy_nbr(temp__);
			destroy_nbr(*modulo);
			temp_ = addition(temp, mod);
			destroy_nbr(temp);
			destroy_nbr(mod);
			*modulo = temp_;
		}
	}
	bx.num = 0;
	if(scale || bscale || dot_0 || bdot_0){
		cscale = scale + dot_0;
		cbscale = bscale + bdot_0;
		if(cscale >= BLK){
			cscale -= BLK;
			cbscale++;
		}
		/*print_nbr(*modulo);
		putchar('\n');*/
		mod = bymin10(*modulo, cbscale, cscale);
		if(mod != *modulo){
			destroy_nbr(*modulo);
			*modulo = mod;
		}
	}
	/*if(equal(*modulo, &nx) != 0 && (scale || bscale || dot_0 || bdot_0)){
		if(((*modulo)->bval < bscale + bdot_0) || ((*modulo)->bval == bscale + bdot_0 && (*modulo)->val <= scale + dot_0)){
			if((mod = calloc(1,sizeof(struct nbr))) == NULL){
				perror("calloc()");
				exit(EXIT_FAILURE);
			}
			mod->num = new_num(1, bscale + bdot_0 + ((scale + dot_0) > 0));
			bval = bscale + bdot_0 - (*modulo)->bval;
			if(scale + dot_0 >= (*modulo)->val)
				val = scale + dot_0 - (*modulo)->val;
			else{
				bval--;
				val = BLK + scale + dot_0 - (*modulo)->val;
			}
			for(	bt = ((*modulo)->num->prev) ? (*modulo)->num->prev : (*modulo)->num,
				bs = mod->num->prev->prev;
				;
			){	
				if(val || bval){
					if(bval){
						bs->nmemb = BLK;
						bs->full = 1;
						bval--;
						mod->bdot++;
					}else{
						bs->nmemb = val;
						mod->dot = val;
						val = 0;
					}
				}else{
					mod->dot += bt->nmemb;
					if(mod->dot >= BLK){
						mod->bdot++;
						mod->dot -= BLK;
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
					if(bs->nmemb == BLK){
						bs->full = 1;
					}
					if(bt == (*modulo)->num)
						break;
					bt = bt->prev;
				}
				if(bs->nmemb >= BLK)
					bs = bs->prev;
			}
			mod->val = mod->num->prev->nmemb = 1;
			destroy_nbr(*modulo);
			*modulo = mod;
		}else{
			if(scale || dot_0 || bscale || bdot_0){
				if((mod = calloc(1,sizeof(struct nbr))) == NULL){
					perror("calloc()");
					exit(EXIT_FAILURE);
				}
				cbscale = blen = bscale + bdot_0;
				cscale = x = scale+dot_0;
				if(x >= BLK){
					cbscale = ++blen;
					cscale = x -= BLK;
				}
				if((*modulo)->val >= cscale){
					mod->num = new_num((*modulo)->bval - bscale - bdot_0 + (((*modulo)->val - cscale) > 0),
								blen + (x > 0));
				}else{
					mod->num = new_num((*modulo)->bval - cbscale + ((BLK + (*modulo)->val - cscale) > 0) - !((*modulo)->bval < cbscale || ((*modulo)->bval == cbscale && (*modulo)->val < cscale)),
								blen + (x > 0));
				}
				bt = (*modulo)->num;
				bs = mod->num;
				if(x){
					bs->num = bt->num%mul[x];
					bs->nmemb = x;
					bs = bs->next;
				}
				for(blen = bscale; bs;blen--, bs = bs->next){
					bs->num = bt->num/mul[x];
					bs->nmemb = BLK-x;
					bs->full = 0;
					if(!(bt = bt->next))
						break;
					bs->num += (bt->num%mul[x])*mul[BLK-x];
					bs->nmemb = BLK;
					bs->full = 1;
					bs->num -= (bs->num/mul[BLK]) * mul[BLK];
				}
				mod->bval = (*modulo)->bval - cbscale;
				mod->bdot = cbscale;
				mod->dot = cscale;
				if(mod->dot >= BLK){
					mod->dot -= BLK;
					mod->bdot++;
				}
				if((*modulo)->val > cscale){
					mod->num->prev->nmemb = mod->val = (*modulo)->val - cscale;
				}else{
					if(mod->bval > 0){
						mod->num->prev->nmemb = mod->val = BLK + (*modulo)->val - cscale;
						mod->bval--;
					}else
						mod->num->prev->nmemb = mod->val = 1;

				}
				mod->num->prev->full = (mod->num->prev->nmemb == BLK);
				destroy_nbr(*modulo);
				*modulo = mod;
			}
		}
	}*/
	/*(*modulo)->num->prev->nmemb = 4;
	(*modulo)->num->prev->full = 0;*/
	/*ADJUST_0((*modulo), bs, bt);*/
	/*printf("%lu :: %i, %lu :: %i ==> %i\n", (*modulo)->bval, (*modulo)->val, (*modulo)->bdot, (*modulo)->dot, (*modulo)->num->prev->full);*/
	if(neg1 != neg2){
		if(equal(res, &nx) != 0)	
			res->neg = 1;
	}
	if(neg1){
		if(equal(*modulo, &nx) != 0)
			(*modulo)->neg = 1;
	}
	num1->neg = neg1;
	num2->neg = neg2;
	if(reste)
		destroy_nbr(reste);
	if(res != quotient && res != NULL){
		destroy_nbr(quotient);
	}else
		res = quotient;
	destroy_nbr(diviseur);
	destroy_nbr(dividende);
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
			p = multiplication(n, pseudo);
			if(pseudo != &un)
				destroy_nbr(pseudo);
			pseudo = p;
		}
		if(equal(d, &zero) != 0){
			res = multiplication(n, n);
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
		res = multiplication(pseudo, n);
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

