#include <string.h>
#include <stdlib.h>
#include <stdio.h>
unsigned long int BUFFER = 64;
struct number{
	char *neg;
	char *n;
	char *revert;
	char *nodot;
	size_t len;
	size_t dot;
	size_t val;
};
#define boolean int
#define NEG(num1, num2) \
	if(num1->neg) \
		for(n1 = num1->neg; *n1 == '-' || *n1 == '+'; n1--) \
			if(*n1 == '-')neg1 = !neg1; \
	if(num2->neg) \
		for(n2 = num2->neg; *n2 == '-' || *n2 == '+'; n2--) \
			if(*n2 == '-')neg2 = !neg2;
#define A_BLK 18
#define ADDITION\
	switch(r){ \
		case 18: \
			if(result >= 1000000000000000000){\
				retenue = 1;\
				result -= 1000000000000000000;\
			}else retenue = 0; \
			break; \
		case 17: \
			if(result >= 100000000000000000){\
				retenue = 1;\
				result -= 100000000000000000;\
			}else retenue = 0; \
			break; \
		case 16: \
			if(result >= 10000000000000000){\
				retenue = 1;\
				result -= 10000000000000000;\
			}else retenue = 0; \
			break; \
		case 15: \
			if(result >= 1000000000000000){\
				retenue = 1;\
				result -= 1000000000000000;\
			}else retenue = 0; \
			break; \
		case 14: \
			if(result >= 100000000000000){\
				retenue = 1;\
				result -= 100000000000000;\
			}else retenue = 0; \
			break; \
		case 13: \
			if(result >= 10000000000000){\
				retenue = 1;\
				result -= 10000000000000;\
			}else retenue = 0; \
			break; \
		case 12: \
			if(result >= 1000000000000){\
				retenue = 1;\
				result -= 1000000000000;\
			}else retenue = 0; \
			break; \
		case 11: \
			if(result >= 100000000000){\
				retenue = 1;\
				result -= 100000000000;\
			}else retenue = 0; \
			break; \
		case 10: \
			if(result >= 10000000000){\
				retenue = 1;\
				result -= 10000000000;\
			}else retenue = 0; \
			break; \
		case 9: \
			if(result >= 1000000000){\
				retenue = 1;\
				result -= 1000000000;\
			}else retenue = 0; \
			break; \
		case 8: \
			if(result >= 100000000){\
				retenue = 1;\
				result -= 100000000;\
			}else retenue = 0; \
			break; \
		case 7: \
			if(result >= 10000000){\
				retenue = 1;\
				result -= 10000000;\
			}else retenue = 0; \
			break; \
		case 6: \
			if(result >= 1000000){\
				retenue = 1;\
				result -= 1000000;\
			}else retenue = 0; \
			break; \
		case 5: \
			if(result >= 100000){ \
				retenue = 1; \
				result -= 100000; \
			}else \
				retenue = 0; \
			break; \
		case 4: \
			if(result >= 10000){ \
				retenue = 1; \
				result -= 10000; \
			}else \
				retenue = 0; \
			break; \
		case 3: \
			if(result >= 1000){ \
				retenue = 1; \
				result -= 1000; \
			}else \
				retenue = 0; \
			break; \
		case 2: \
			if(result >= 100){ \
				retenue = 1; \
				result -= 100; \
			}else \
				retenue = 0; \
			break; \
		case 1: \
			if(result >= 10){ \
				retenue = 1; \
				result -= 10; \
			}else \
				retenue = 0; \
			break; \
		default: \
			retenue = 0; \
	}
#define S_BLK 13
#define D_SOUSTRACTION_(i) \
		switch(i){ \
			case 13:\
				result = 10000000000000 - i1 - retenue; \
				break;\
			case 12:\
				result = 1000000000000 - i1 - retenue; \
				break;\
			case 11:\
				result = 100000000000 - i1 - retenue; \
				break;\
			case 10:\
				result = 10000000000 - i1 - retenue; \
				break;\
			case 9: \
				result = 1000000000 - i1 - retenue; \
				break; \
			case 8: \
				result = 100000000 - i1 - retenue; \
				break; \
			case 7: \
				result = 10000000 - i1 - retenue; \
				break; \
			case 6: \
				result = 1000000 - i1 - retenue; \
				break; \
			case 5: \
				result = 100000 - i1 - retenue; \
				break; \
			case 4: \
				result = 10000 - i1 - retenue; \
				break; \
			case 3: \
				result = 1000 - i1 - retenue; \
				break; \
			case 2: \
				result = 100 - i1 - retenue; \
				break; \
			case 1: \
				result = 10 - i1 - retenue; \
				break;\
		} \
	retenue = 1;
#define SOUSTRACTION(r) \
	switch(r){ \
		case 13: \
			result = 10000000000000 + i1 - i2 - retenue; \
			break; \
		case 12: \
			result = 1000000000000 + i1 - i2 - retenue; \
			break; \
		case 11: \
			result = 100000000000 + i1 - i2 - retenue; \
			break; \
		case 10: \
			result = 10000000000 + i1 - i2 - retenue; \
			break; \
		case 9: \
			result = 1000000000 + i1 - i2 - retenue; \
			break; \
		case 8: \
			result = 100000000 + i1 - i2 - retenue; \
			break; \
		case 7: \
			result = 10000000 + i1 - i2 - retenue; \
			break; \
		case 6: \
			result = 1000000 + i1 - i2 - retenue; \
			break; \
		case 5: \
			result = 100000 + i1 - i2 - retenue; \
			break; \
		case 4: \
			result = 10000 + i1 - i2 - retenue; \
			break; \
		case 3: \
			result = 1000 + i1 - i2 - retenue; \
			break; \
		case 2: \
			result = 100 + i1 - i2 - retenue; \
			break; \
		case 1: \
			result = 10 + i1 - i2 - retenue; \
			break; \
	} \
	retenue = 1;
#define NEG_TEST \
	if(neg1 != neg2)\
		neg = 1;
#define M_BLK 9

/*enum err_op{
	E_GET = 0,
	E_SET = 1
};
int err_set(int op, int val){
	static int err = 0;
	if(op == E_SET)
		err = val;
	else return err;
	return 0;
}*/
void *allocation(void **ptr, unsigned long int members, unsigned long int size){
	if(members+1 <= members){
		fprintf(stderr,"Taille de la chaine trop longue pour le systeme.\n");
		exit(EXIT_FAILURE);
	}
	/*if(members == (unsigned long int)~0){
		fprintf(stderr,"Taille de la chaine trop longue pour le systeme.\n");
		exit(EXIT_FAILURE);
	}*/
	if((*ptr = calloc(members, size+1)) == NULL){
		perror("calloc()");
		exit(EXIT_FAILURE);
	}
	return *ptr;
}
void *reallocation(void **ptr, unsigned long int size){
	char *buf;
	unsigned long int s = 0, sz = 0;
	s = strlen(*ptr);
	sz = s +1;
	if(sz >= size){
		fprintf(stderr,"Taille de la chaine trop longue pour le systeme.\n");
		free(*ptr);
		exit(EXIT_FAILURE);
	}
	//for(sz = strlen(*ptr)+1, sz_ = 0;sz != (unsigned long int)~0 && sz_ != size; sz++, sz_++);;
	/*if(sz == (unsigned long int)~0){
		fprintf(stderr,"Taille de la chaine trop longue pour le systeme.\n");
		free(*ptr);
		exit(EXIT_FAILURE);
	}*/
	/*if(size == sz){
		fprintf(stderr,"Taille de la chaine trop longue pour le systeme.\n");
		free(*ptr);
		exit(EXIT_FAILURE);
	}*/
	if((*ptr = realloc(*ptr, size)) == NULL){
		perror("realloc()");
		exit(EXIT_FAILURE);
	}
	buf = (*ptr)+s;
	sz = size - s;
	memset(buf, 0, sz);
	/*for(i = 0; i < sz; i++){
		buf[i] = 0;
	}*/
	return (*ptr)+s;
}
char *parse_nbr(char *n);
void destroy_number(struct number **n, int el_only);
static void init(char *num, struct number *n);
void reorder(struct number *n);
static char *inc(char *buf, size_t size);
static char *tochar(size_t num, int *size);
static int equal(struct number *num1, struct number *num2);
static void *addition(struct number *num1, struct number *num2);
static void *soustraction(struct number *num1, struct number *num2);
static void *multiplication(struct number *num1, struct number *num2);
static void *division(struct number *num1, struct number *num2, unsigned long int virgule, boolean approximation, boolean *mod);
static void *modulo(struct number *num1, struct number *num2, unsigned long int scale);
int main(int argc, char **argv){
	struct number number1 = { NULL, NULL, NULL, NULL, 0, 0, 0 }, number2 = { NULL, NULL, NULL, 0, 0, 0 }, *n1, *r, 
		un = { NULL, "1", "1", "1", 1, 0, 1 }, *pseudo = &un, *p = NULL;
	//char *result, *n1 = argv[1], *n2 = argv[2], *pseudo = "1", *p = NULL;
	/*result = division(argv[1], argv[2], atol(argv[3]), 1);
	printf("%s / %s = %s\n", argv[1], argv[2], result);
	free(result);
	return 0;*/
	char c, *n;
	int comp, mod;
	n = parse_nbr(argv[1]);
	init(n, &number1);
	n = parse_nbr(argv[2]);
	init(n, &number2);
	printf("%s: %s: %s\n", argv[1], number1.n, number1.revert);
	printf("%s: %s: %s\n", argv[2], number1.n, number2.revert);
	comp = equal(&number1, &number2);
	c = (comp == 0) ? '=' : (comp == -1) ? '<' : '>';
	printf("%s %c %s\n", number1.n, c, number2.n);
	r = addition(&number1, &number2);
	if(r){
		reorder(r);
		printf("%s + %s = %s (%s)\n", number1.n, number2.n, r->n, r->revert);
		destroy_number(&r, 0);
	}
	r = soustraction(&number1, &number2);
	if(r){
		reorder(r);
		printf("%s - %s = %s (%s)\n", number1.n, number2.n, r->n, r->revert);
		destroy_number(&r, 0);
	}
	r = multiplication(&number1, &number2);
	if(r){
		reorder(r);
		printf("%s * %s = %s (%s) (%lu,%lu,%lu)\n", number1.n, number2.n, r->n, r->revert, r->len, r->val, r->dot);
		destroy_number(&r, 0);
	}
	r = division(&number1, &number2, atoi(argv[3]), 1, &mod);
	if(r){
		if(r->n == NULL)
			reorder(r);
		printf("%s / %s = %s(%s)\n\tmod exist: %s\n", number1.n, number2.n, r->n, r->revert, (mod == 1) ? "Yes" : "No");
		destroy_number(&r, 0);
	}
	if(mod){
		r = modulo(&number1, &number2, atoi(argv[3]));
		if(r){
			reorder(r);
			printf("%s %% %s = %s (%s)\n", number1.n, number2.n, r->n, r->revert);
			destroy_number(&r, 0);
		}
	}else
			printf("So modulo is not computed.\n\t%s %% %s = 0\n", number1.n, number2.n);
	/*comp = atoi(argv[2]);
	n1 = &number1;
	r = NULL;
	if(comp > 0){
		while(comp > 1){
			if(comp%2){
				comp--;
				p = multiplication(n1, pseudo);
				if(pseudo != &un)
					destroy_number(&pseudo, 0);
				pseudo = p;
			}else{
				comp /= 2;
				r = multiplication(n1, n1);
				if(n1 != &number1)
					destroy_number(&n1, 0);
				n1 = r;
			}
		}
		r = multiplication(pseudo, n1);
		reorder(r);
		printf("%s ^ %s = %s\n", number1.n, number2.n, r->n);
		if(p)
			destroy_number(&p, 0);
		if(n1 != &number1)
			destroy_number(&n1, 0);
	}else
		printf("%s ^ %s = 1\n", number1.n, number2.n);*/
	destroy_number(&r, 0);
	r = &number1;
	destroy_number(&r, 1);
	r = &number2;
	destroy_number(&r, 1);
	return 0;
	return 0;
}
void destroy_number(struct number **n, int el_only){
	if((*n)->n)
		free((*n)->n);
	if((*n)->revert)
		free((*n)->revert);
	if((*n)->nodot)
		free((*n)->nodot);
	if(!el_only){
		free(*n);
		*n = NULL;
	}else
		memset(*n, 0, sizeof(struct number));
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
			if(*pn == '0' && zero == 1)
				continue;
			else{
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
void reorder(struct number *n){
	char *num, *pn;
	size_t i;
	i = strlen(n->revert);
	if((n->n = calloc(i+1, sizeof(char))) == NULL){
		perror("malloc()");
		exit(EXIT_FAILURE);
	}
	for(	num = n->n,
		pn = n->revert + i-1;
		i > 0;
		i--, num++, pn--
	){
		*num = *pn;
	}
}
void init(char *num, struct number *n){
	char *pn, *ppn;
	size_t l;
	size_t *values[2] = { &n->dot, &n->val }, **pval;
	pval = values;
	n->n = num;
	l = n->len = strlen(num);
	if((pn = n->revert = malloc((n->len+1)*sizeof(char))) == NULL){
		perror("malloc()");
		exit(EXIT_FAILURE);
	}
	if(!strchr(num, '.'))
		pval++;
	ppn = inc(num, n->len-1);
	do{
		*pn = *ppn;
		if(*ppn == '.'){
			pval++;
			ppn--;
			pn++;
			continue;
		}
		(**pval)++;
		ppn--;
		pn++;
	}while(--l);
	n->neg = (*(pn-1) == '+' || *(pn-1) == '-')  ? pn-1 : NULL;
	*pn = 0;
	for(pn--;*pn == '-' || *pn == '+';pn--,n->len--, n->val--);
	/*if(n->neg)
		printf("%s\n", n->neg);*/
}
char *inc(char *buf, size_t size){
	size_t sz = size%sizeof(size_t), lsz = size/sizeof(size_t), *pbuf;
	for(;sz > 0; sz--, buf++);
	for(pbuf = (size_t *)buf;lsz > 0; pbuf++, lsz--);
	return (char *)pbuf;
	
}
void setto_0(char *pbuf){
	for(;*pbuf;*pbuf = 0, pbuf++);
}
size_t settobin(char **buf, int sz){
	size_t n = 0, mul = 1, size = 0;
	while(**buf && size < sz){
		n += (**buf-(3<<4))*mul;
		(*buf)++;
		size++;
		mul *= 10;
	}
	return n;
}
char *tochar(size_t num, int *___size___){
	static char temp[21];
	size_t n = num, sz = 0;
	int *size = ___size___;
	char *ptemp;
	memset(temp, 0,21);
	ptemp = temp;
	if(num == 0){
		*ptemp = '0';
		*size = 1;
	}else{
		for(;;){
			*ptemp = n%10 | (3<<4);
			n /= 10;
			sz++;
			if(!n)break;
			ptemp++;
		}
	}
	*___size___ = sz;
	return temp;
}
int equal(struct number *num1, struct number *num2){
	char *val1 = num1->revert + num1->len, *val2 = num2->revert + num2->len,
		neg1 = 0, neg2 = 0, *ptr, *pval, *vc1;
	int ret = 0;
	if(num1->neg)
		for(val1 = num1->neg; *val1 == '-' || *val1 == '+'; val1--)
			if(*val1 == '-')
				neg1 = !neg1;
	if(num2->neg)
		for(val2 = num2->neg; *val2 == '-' || *val2 == '+'; val2--)
			if(*val2 == '-')
				neg2 = !neg2;
	if(neg1 && !neg2)
		return -1;
	if(!neg1 && neg2)
		return 1;
	if(num1->val > num2->val)
		return 1;
	if(num1->val < num2->val)
		return -1;
	for(val1 = val1, val2 = val2,ptr = (num1->dot) ? num1->revert +num1->dot+1: (num2->dot) ? num2->revert+num2->dot+1 :num1->revert; val1 != ptr; val1--, val2--)
		if(*val1 > *val2)
			return (neg1 && neg2) ? -1 : 1;
		else
			if(*val1 < *val2)
				return (neg1 && neg2) ? 1 : -1;
	if(*val1 > *val2){
		return (neg1 && neg2) ? -1 : 1;
	}else{
		if(*val1 < *val2)
			return (neg1 && neg2) ? 1 : -1;
		else
			if(num1->dot == 0 && num2->dot == 0)
				return 0;
	}
	if(num1->dot)
		val1--;
	if(num2->dot)
		val2--;
	if(num1->dot > num2->dot){
		pval = val2;
		ptr = num2->revert-1;
		vc1 = num1->revert-1;
		ret = 1;
	}else{
		if(num1->dot < num2->dot){
			pval = val1;
			ptr = num1->revert-1;
			vc1 = num2->revert-1;
			val1 = val2;
			ret = -1;
		}else{
			ret = 1;
			pval = val2;
			ptr = num2->revert - 1;
			vc1 = NULL;
		}
	}
	for(;ptr != pval && *pval == *val1;pval--, val1--);
	if(pval == ptr)
		return 0;
	if(*val1 > *pval)
		return ret*((neg1 && neg2) ? -1 : 1);
	else
		if(*val1 < *pval)
			return ret*((neg1 && neg2) ? 1 : -1);
	if(vc1 == NULL)
		return 0;
	for(;val1 != vc1; val1--)
		if(*val1 != 0)
			return ret*((neg1 && neg2) ? 1 : -1);
	return 0;
}
void *addition(struct number *num1, struct number *num2){
	struct number *res, zero = {NULL, "0", "0", NULL, 1, 0, 1};
	char *n1 = num1->n, *n2 = num2->n,
		*dot, *ng,
		*ptemp, *buffer, *pbuf,
		*ptr1 = NULL, *ptr2= NULL,
		retenue = 0, neg = 0, neg1 = 0, neg2 = 0;
	unsigned long int dot1_len = 0, dot2_len = 0,
				val1_len = 0, val2_len = 0;
	unsigned long int ii = 0, ij = 0, ki = 0, kj = 0, r = 0;
	unsigned long int i1, i2, r1, r2, result;
	int i;
	switch((equal(num1, &zero) == 0) | (equal(num2, &zero) == 0)*2){
		case 1:
			NEG(num1, num2);
			if((res = malloc(sizeof(struct number))) == NULL){
				perror("malloc()");
				exit(EXIT_FAILURE);
			}
			if((res->revert = malloc((strlen(num2->revert)+1)*sizeof(char))) == NULL){
				perror("malloc()");
				exit(EXIT_FAILURE);
			}
			if(neg1){
				*(res->revert + strlen(res->revert)) = '-';
				res->neg = res->revert + strlen(res->revert);
			}else
				res->neg = NULL;
			res->n = NULL;
			res->len = num2->len;
			res->val = num2->val;
			res->dot = num2->dot;
			res->nodot = NULL;
			strcpy(res->revert, num2->revert);
			return res;
		case 2:
		case 3:
			NEG(num1, num2);
			if((res = malloc(sizeof(struct number))) == NULL){
				perror("malloc()");
				exit(EXIT_FAILURE);
			}
			if((res->revert = malloc((strlen(num1->revert)+1)*sizeof(char))) == NULL){
				perror("malloc()");
				exit(EXIT_FAILURE);
			}
			if(neg2){
				*(res->revert + strlen(res->revert)) = '-';
				res->neg = res->revert + strlen(res->revert);
			}else
				res->neg = NULL;
			res->n = NULL;
			res->len = num1->len;
			res->val = num1->val;
			res->dot = num1->dot;
			res->nodot = NULL;
			strcpy(res->revert, num1->revert);
			return res;
		default:
			break;
	}
	//err_set(E_SET, 0);
	NEG(num1, num2);
	if(neg1 || neg2){
		if(neg1 && neg2)
			neg = 1;
		else
			if(neg1 && !neg2){
				n2 = num1->n;
				num1->n = n1;
				ng = num1->neg;
				num1->neg = NULL;
				res = soustraction(num2,num1);
				num1->n = n2;
				num1->neg = ng;
				return res;
			}else{
				if(!neg1 && neg2){
					n1 = num2->n;
					num2->n = n2;
					ng = num2->neg;
					num2->neg = NULL;
					res = soustraction(num1,num2);
					num2->n = n1;
					num2->neg = ng;
					return res;
				}
			}
	}
	val1_len = num1->val;
	val2_len = num2->val;
	dot1_len = num1->dot;
	dot2_len = num2->dot;
	/*if(((val1_len > val2_len) ? val1_len : val2_len) +((dot1_len > dot2_len) ? dot1_len : dot2_len) + 2 < val1_len +dot1_len ||
		((val1_len > val2_len) ? val1_len : val2_len) +((dot1_len > dot2_len) ? dot1_len : dot2_len) + 2 < val2_len +dot2_len ||
			((char *)num1->n)[num1->len+neg] != 0 || ((char *)num2->n)[num2->len+neg] != 0){
			fprintf(stderr,"Calcule de nombre trop grand\n");
			//err_set(E_SET, 1);
			return NULL;
	}*/
	ptr1 = num1->revert;
	ptr2 = num2->revert;
	pbuf = allocation((void **)&buffer, ((val1_len > val2_len) ? val1_len : val2_len) +((dot1_len > dot2_len) ? dot1_len : dot2_len) + 1, sizeof(char));
	if(dot1_len && dot2_len){
		if(dot1_len > dot2_len){
			ii = dot1_len;
			ij = dot2_len;
			ki = val1_len;
			kj = val2_len;
			ptr1 = num1->revert;
			ptr2 = num2->revert;
		}else{
			ii = dot2_len;
			ij = dot1_len;
			ki = val2_len;
			kj = val1_len;
			ptr1 = num2->revert;
			ptr2 = num1->revert;
		}
		if(ii != ij){
			memcpy(pbuf, ptr1, ii - ij);
			pbuf = inc(pbuf, ii - ij);
			ptr1 = inc(ptr1, ii - ij);
			ii = ij;
		}
		while(ii >= A_BLK){
			i1 = settobin(&ptr1, A_BLK);
			i2 = settobin(&ptr2, A_BLK);
			r = A_BLK;
			result = i1 + i2 + retenue;
			ADDITION;
			ptemp = tochar(result, &i);
			if(i < r){
				memset(ptemp +i, '0', r - i);
				r-=i;
			}
			memcpy(pbuf, ptemp, A_BLK);
			pbuf = inc(pbuf, A_BLK);
			ii -= A_BLK;
		}
		if(ii){
			i1 = settobin(&ptr1, ii);
			i2 = settobin(&ptr2, ii);
			r = ii;
			result = i1 + i2 + retenue;
			ADDITION;
			ptemp = tochar(result, &i);
			if(i < r){
				memset(ptemp +i, '0', r - i);
				r -= i;
			}
			memcpy(pbuf, ptemp, r);
			pbuf = inc(pbuf, r);
			ii = 0;
		}
	}else{
		if(dot1_len && !dot2_len){
			ii = dot1_len;
			ptr1 = num1->revert;
			strncpy(pbuf, ptr1, ii);
			ptr1 = inc(ptr1, ii);
			pbuf = inc(pbuf, ii);
		}else{
			if(!dot1_len && dot2_len){
				ij = dot2_len;
				ptr2 = num2->revert;
				strncpy(pbuf, ptr2, ij);
				ptr2 = inc(ptr2, ij);
				pbuf = inc(pbuf, ij);
			}
		}
	}
	if(dot1_len || dot2_len){
		*pbuf = '.';
		dot = pbuf++;
		if(dot1_len)
			ptr1++;
		if(dot2_len)
			ptr2++;
	}else
		dot = NULL;
	ii = (ki) ? ki : val1_len;
	ij = (kj) ? kj : val2_len;
	while(ii > 0 || ij > 0 || retenue){
		if(ii >= A_BLK){
			i1 = settobin(&ptr1, A_BLK);
			ii -= A_BLK;
			r1 = A_BLK;
		}else{
			if(ii > 0){
				i1 = settobin(&ptr1, ii);
				r1 = ii;
				ii -= ii;
			}else{
				i1 = 0;
				r1 = 0;
			}
		}
		if(ij >= A_BLK){
			ij -= A_BLK;
			r2 = A_BLK;
			i2 = settobin(&ptr2, A_BLK);
		}else{
			if(ij > 0){
				i2 = settobin(&ptr2, ij);
				r2 = ij;
				ij -= ij;
			}else{
				i2 = 0;
				r2 = 0;
			}
		}
		r = (r1 > r2) ? r1 : r2;
		result = i1 + i2 + retenue;
		ADDITION;
		ptemp = tochar(result, &i);
		if(i < r){
			memset(ptemp +i, '0', r - i);
		}
		memcpy(pbuf, ptemp, r);
		pbuf += r;
	}
	if((res = malloc(sizeof(struct number))) == NULL){
		perror("malloc()");
		exit(EXIT_FAILURE);
	}
	r1 = strlen(buffer);
	res->nodot = NULL;
	res->n = NULL;
	res->len = r1;
	res->val = (dot) ? strlen(dot+1) : r1;
	res->dot = (dot) ? res->len-res->val-1: 0;
	res->revert = buffer;
	if(neg){
		strcpy(res->revert +r1, "-");
		res->neg = res->revert +r1;
	}else{
		res->neg = NULL;
	}
	return res;
}

void *soustraction(struct number *num1, struct number *num2){
	struct number *nbr1, *nbr2, *res;
	char *n1 = num1->n, *n2 = num2->n,
		*dot1, *dot2, *dot = NULL,
		*val1, *val2,*ptemp,
		*buffer, *pbuf,
		*ptr1 = NULL, *ptr2= NULL,
		neg = 0, neg1 = 0, neg2 = 0;
	unsigned long int dot1_len = 0, dot2_len = 0,
				val1_len = 0, val2_len = 0,
				result = 0, retenue = 0, r, r1, r2, i1, i2;
	unsigned long int ii_ = 0, ij_ =0, j;
	int i;
	NEG(num1, num2);
	if(neg1 || neg2){
		if(neg1 && neg2){
			ptr1 = num1->n;
			ptr2 = num2->n;
			num1->n = n1;
			num2->n = n2;
			dot1 = num1->neg;
			dot2 = num2->neg;
			num1->neg = NULL;
			num2->neg = NULL;
			res = soustraction(num2, num1);
			num1->n = ptr1;
			num2->n = ptr2;
			num1->neg = dot1;
			num2->neg = dot2;
			return res;
		}else{
			if(neg1 && !neg2){
				ptr1 = num1->n;
				num1->n = n1;
				dot1 = num1->neg;
				num1->neg = NULL;
				res = addition(num2, num1);
				num1->n = ptr1;
				num1->neg = dot1;
				if((res->revert = realloc(res->revert, res->len+2)) == NULL){
					perror("realloc()");
					exit(0);
				}
				*(res->revert + res->len) = '-';
				return res;
			}else{
				if(!neg1 && neg2){
					ptr2 = num2->n;
					num2->n = n2;
					dot2 = num2->neg;
					num2->neg = NULL;
					res = addition(num1, num2);
					num2->n = ptr2;
					num2->neg = dot2;
					return res;
				}
			}
		}
	}
	switch(equal(num1,num2)){
		case 0:
			if((res = malloc(sizeof(struct number))) == NULL){
				perror("malloc()");
				exit(EXIT_FAILURE);
			}
			if((res->revert = malloc(2*sizeof(char))) == NULL){
				perror("malloc()");
				exit(EXIT_FAILURE);
			}
			res->neg = NULL;
			res->nodot = NULL;
			res->n = NULL;
			res->len = 1;
			res->val = 1;
			res->dot = 0;
			strcpy(res->revert, "0");
			return res;
		case -1:
			neg = 1;
			nbr1 = num2;
			nbr2 = num1;
			n1 = num2->revert;
			n2 = num1->revert;
			goto next;
		case 1:
			nbr1 = num1;
			nbr2 = num2;
			n1 = num1->revert;
			n2 = num2->revert;
			next:
			if((dot1_len = nbr1->dot))
				dot1 = nbr1->revert;
			else
				dot1 = NULL;
			if((dot2_len = nbr2->dot))
				dot2 = nbr2->revert;
			else
				dot2 = NULL;
			val1 = n1 + nbr1->dot + (dot1 != NULL);
			val2 = n2 + nbr2->dot + (dot2 != NULL);
			val1_len = nbr1->val;
			val2_len = nbr2->val;
			break;
		default: printf("ERROR\n");
			exit(0);
	}
	//err_set(E_SET, 0);
	/*if((((val1_len > val2_len) ? val1_len : val2_len) + ((dot1_len > dot2_len) ? dot1_len : dot2_len)+2 < val1_len +dot1_len) ||(((val1_len > val2_len) ? val1_len : val2_len) + ((dot1_len > dot2_len) ? dot1_len : dot2_len)+2 < val2_len +dot2_len)||
			((char *)num1->n)[strlen(num1->n)] != 0 || ((char *)num2->n)[strlen(num2->n)] != 0){
		fprintf(stderr, "Calcule de nombre trop grand\n");
		//err_set(E_SET, 1);
		return NULL;
	}*/
	pbuf = allocation((void **)&buffer,((val1_len > val2_len) ? val1_len : val2_len) + ((dot1_len > dot2_len) ? dot1_len : dot2_len)+2, sizeof(char));
	if(dot1 || dot2){
		if(dot1_len > dot2_len){
			strncpy(pbuf , dot1, dot1_len - dot2_len);
			dot1 += (dot1_len - dot2_len);
			pbuf = inc(pbuf, dot1_len - dot2_len);
			ii_ = dot2_len;
			if(ii_ >= S_BLK){
				r = S_BLK;
				do{
					i1 = settobin(&dot1, S_BLK);
					i2 = settobin(&dot2, S_BLK);
					if(i1 >= i2 + retenue){
						result = i1 - i2 - retenue;
						retenue = 0;
					}else{
						result = 10000000000000 +i1 - i2 - retenue;
						retenue = 1;
					}
					ptemp = tochar(result, &i);
					if(i < r){
						memset(ptemp +i, '0', r - i);
					}
					memcpy(pbuf, ptemp, S_BLK);
					pbuf = inc(pbuf, S_BLK);
					ii_ -= S_BLK;
				}while(ii_ >= S_BLK);
			}
			if(ii_){
				r = ii_;
				i1 = settobin(&dot1, r);
				i2 = settobin(&dot2, r);
				if(i1 >= i2 + retenue){
					result = i1 - i2 - retenue;
					retenue = 0;
				}else{
					SOUSTRACTION(r);
				}
				ptemp = tochar(result, &i);
				if(i < r){
					memset(ptemp +i, '0', r - i);
				}
				memcpy(pbuf, ptemp, r);
				pbuf = inc(pbuf, r);
				ii_ = 0;
			}
		}else{
			if(dot2_len > dot1_len){
				ij_ = dot2_len - dot1_len;
				ii_ = dot2_len;
				if(ij_ >= S_BLK){
					r = S_BLK;
					do{
						i1 = settobin(&dot2, S_BLK);
						result = 10000000000000 - i1 - retenue;
						retenue = 1;
						ptemp = tochar(result, &i);
						if(i < r){
							memset(ptemp +i, '0', r - i);
						}
						memcpy(pbuf, ptemp, S_BLK);
						pbuf = inc(pbuf, S_BLK);
						ij_ -= S_BLK;
					}while(ij_ >= S_BLK);
				}
				if(ij_){
					r = ij_;
					i1 = settobin(&dot2, r);
					D_SOUSTRACTION_(r);
					ptemp = tochar(result, &i);
					if(i < r){
						memset(ptemp +i, '0', r - i);
					}
					memcpy(pbuf, ptemp, r);
					pbuf = inc(pbuf, r);
					ij_ -= r;
				}
				ii_ = dot1_len;
				while(ii_ >= S_BLK){
					r = S_BLK;
					i1 = settobin(&dot1, r);
					i2 = settobin(&dot2, r);
					if(i1 >= i2 + retenue){
						result = i1 - i2 - retenue;
						retenue = 0;
					}else{
						result = 10000000000000 + i1 - i2 - retenue;
						retenue = 1;
					}
					ptemp = tochar(result, &i);
					if(i < r){
						memset(ptemp +i, '0', r - i);
					}
					memcpy(pbuf, ptemp, S_BLK);
					ii_ -= S_BLK;
					pbuf = inc(pbuf, S_BLK);
				}
				if(ii_){
					r = ii_;
					i1 = settobin(&dot1, r);
					i2 = settobin(&dot2, r);
					if(i1 >= i2 + retenue){
						result = i1 - i2 - retenue;
						retenue = 0;
					}else{
						SOUSTRACTION(r);
					}
					ptemp = tochar(result, &i);
					if(i < r){
						memset(ptemp +i, '0', r - i);
					}
					memcpy(pbuf, ptemp, r);
					pbuf = inc(pbuf, r);
				}
			}else{
				if(dot1_len == dot2_len){
					ii_ = dot1_len;
					r = S_BLK;
					while(ii_ >= S_BLK){
						i1 = settobin(&dot1, S_BLK);
						i2 = settobin(&dot2, S_BLK);
						if(i1 >= i2 + retenue){
							result = i1 - i2 - retenue;
							retenue = 0;
						}else{
							result = 10000000000000 +i1 - i2 - retenue;
							retenue = 1;
						}
						ptemp = tochar(result, &i);
						if(i < r){
							memset(ptemp +i, '0', r - i);
						}
						memcpy(pbuf, ptemp, S_BLK);
						ii_ -= S_BLK;
						pbuf = inc(pbuf, S_BLK);
					}
					if(ii_){
						r = ii_;
						i1 = settobin(&dot1, ii_);
						i2 = settobin(&dot2, ii_);
						if(i1 >= i2 + retenue){
							result = i1 - i2 - retenue;
							retenue = 0;
						}else{
							SOUSTRACTION(r);
						}
						ptemp = tochar(result, &i);
						if(i < ii_){
							memset(ptemp +i, '0', ii_ - i);
						}
						memcpy(pbuf, ptemp, ii_);
						pbuf = inc(pbuf, ii_);
						ii_ -= ii_;
					}
				}
			}
		}
		*pbuf = '.';
		dot = pbuf++;
	}
	ii_ = val1_len;
	ij_ = val2_len;
	ptr1 = val1;
	ptr2 = val2;
	do{
		if(ii_ >= S_BLK){
			i1 = settobin(&ptr1, S_BLK);
			ii_ -= S_BLK;
			r1 = S_BLK;
		}else{
			if(ii_ != 0){
				i1 = settobin(&ptr1, ii_);
			}else{
				i1 = 0;
			}
			r1 = ii_;
			ii_ = 0;
		}
		if(ij_ >= S_BLK){
			i2 = settobin(&ptr2, S_BLK);
			ij_ -= S_BLK;
			r2 = S_BLK;
		}else{
			if(ij_ != 0){
				i2 = settobin(&ptr2, ij_);
			}else{
				i2 = 0;
			}
			r2 = ij_;
			ij_ = 0;
		}
		r = (r1 > r2) ? r1 : r2;
		if(i1 >= i2 + retenue){
			result = i1 - i2 - retenue;
			retenue = 0;
		}else{
			SOUSTRACTION(r);
		}
		ptemp = tochar(result, &i);
		if((j = strlen(ptemp)) < r){
			memset(ptemp +j, '0', r - j);
		}
		memcpy(pbuf, ptemp, r);
		pbuf = inc(pbuf, r);
	}while(ii_ > 0 || ij_ > 0);
	if(*(--pbuf) == '0'){
		while(pbuf != buffer && *(pbuf) == '0' && *(pbuf -1) != '.'){
			*pbuf = 0;
			pbuf--;
		}
	}
	if(neg){
		pbuf++;
		*pbuf = '-';
	}
	if((res = malloc(sizeof(struct number))) == NULL){
		perror("malloc()");
		exit(EXIT_FAILURE);
	}
	r = strlen(buffer);
	res->neg = (*pbuf == '-') ? pbuf : NULL;
	res->nodot = NULL;
	res->n = NULL;
	res->len = r;
	res->val = (dot) ? strlen(dot+1) -neg : r -neg;
	res->dot = (dot) ? res->len-res->val-1: 0;
	res->revert = buffer;
	return res;
}
void *multiplication(struct number *num1, struct number *num2){
	struct number *res, zero = {NULL, "0", "0", NULL, 1, 0, 1};
	char *n1, *n2, *n_, *n__,
		r__[21], rlast,
		*last = NULL, *plast, *plast_, *ppl, *ptemp,
		*pr = NULL,
		neg = 0, neg1 = 0, neg2 = 0;
	unsigned long int dot1_len = 0, dot2_len = 0, dot_len = 0;
	unsigned long int ii = 0, ij = 0, r1, r2, l1, l2;
	unsigned long int rn1 = 0, rn2 = 0, rn = 0, result;
	int i, i1;
	if(equal(num1, &zero) == 0 || equal(num2, &zero) == 0){
		if((res = malloc(sizeof(struct number))) == NULL){
			perror("malloc()");
			exit(EXIT_FAILURE);
		}
		if((res->revert = malloc(2*sizeof(char))) == NULL){
			perror("malloc()");
			exit(EXIT_FAILURE);
		}
		res->nodot = NULL;
		res->n = NULL;
		res->len = 1;
		res->val = 1;
		res->dot = 0;
		res->neg = NULL;
		strcpy(res->revert, "0");
		return res;
	}
	memset(r__, 0, 10);
	//err_set(E_SET, 0);
	NEG(num1, num2);
	NEG_TEST;
	l1 = num1->len;
	l2 = num2->len;
	/*if(l1 + l2+neg < l1 || l1 + l2+neg < l2||
			n1[l1] != 0 || n2[l2] != 0){
		fprintf(stderr,"Calcule de nombre trop grands %s(%c) %s(%c)\n", n1, n1[l1], n2, n2[l2]);
		//err_set(E_SET, 1);
		exit(0);
		return NULL;
	}*/
	n1 = num1->revert;
	n2 = num2->revert;
	dot1_len = num1->dot;
	dot2_len = num2->dot;
	dot_len = dot1_len + dot2_len;
	if((plast = last = calloc(l1 + l2 + neg + (dot_len > 0) +1, sizeof(char))) == NULL){
		perror("calloc()");
		exit(EXIT_FAILURE);
	}
	if(dot1_len){
		if(num1->nodot == NULL){
			if((num1->nodot = malloc(num1->len)) == NULL){
				perror("malloc()");
				exit(EXIT_FAILURE);
			}
			memcpy(num1->nodot, n1, num1->dot);
			memcpy(num1->nodot + num1->dot,  n1 + num1->dot+1, num1->val+1);
			n1 = num1->nodot;
			l1--;
		}else{
			n1 = num1->nodot;
			l1--;
		}
	}
	if(dot2_len){
		if(num2->nodot == NULL && dot2_len){
			if((num2->nodot = malloc(num2->len)) == NULL){
				perror("malloc()");
				exit(EXIT_FAILURE);
			}
			memcpy(num2->nodot, n2, num2->dot);
			memcpy(num2->nodot + num2->dot,  n2 + num2->dot+1, num2->val+1);
			n2 = num2->nodot;
			l2--;
		}else{
			n2 = num2->nodot;
			l2--;
		}
	}
	for(	n__ = n1,
		*r__ = 0,
		plast_ = plast,
		i = 0,
		ii = l1;
		ii > 0;
	){
		if(ii > 8){
			i1 = ii - 9;
			r1 = 9;
		}else{
			i1 = 0;
			r1 = ii;
		}
		rn1 = settobin(&n__, r1);
		for(	ppl = plast = plast_,
			n_ = n2,
			ij = l2;
			ij != 0;
			plast = ppl
		){
			r2 = (ij > 8) ? 9 : ij;
			rn2 = settobin(&n_, r2);
			rn = (plast != last) ? settobin(&plast, r2): 0;
			pr = r__;
			result = rn1*rn2+settobin(&pr, i)+rn;
			memset(r__, 0, i);
			ptemp = tochar(result, &i);
			while(i < r2 || i < r1){
				ptemp[i] = '0';
				i++;
			}
			if(i > r2){
				i -= r2;
				memcpy(r__, ptemp+r2, i);
			}else
				i = 0;
			memcpy(ppl, ptemp, r2);
			ppl = inc(ppl, r2);
			//ppl += r2;
			ij -= r2;
		}
		if(i){
			memcpy(plast, r__, i);
			*r__ = 0;
			i = 0;
		}
		ii = i1;
		plast_ = inc(plast_, M_BLK);
	}
	if((res = calloc(1,sizeof(struct number))) == NULL){
		perror("malloc()");
		exit(EXIT_FAILURE);
	}
	res->len = strlen(last)+1;
	res->dot = dot_len;
	res->val = res->len - dot_len;
	if(res->len-1 == res->dot){
		res->len++;
		*(last+res->dot) = '0';
	}
	for(plast = last + res->len-1; res->val > 1 && plast != last && (*plast == '0' || *plast == 0); plast--)
		if(*(plast-1) == '.'){
			break;
		}else{	
			*plast = 0;
			res->len--;
			res->val--;
		}
	if(dot_len){
		if((res->nodot = malloc(res->len+1)) == NULL){
			perror("malloc()");
			exit(0);
		}
		for(plast = last;*plast == '0' && res->dot > 0; plast++, res->dot--, res->len--);
		memcpy(res->nodot, plast, res->len+1);
		if(res->dot){
			for(pr = last + dot_len, rlast = *pr, *r__ = '.' ;rlast != 0 ;pr++){
				rlast = *pr;
				*pr = *r__;
				*r__ = rlast;
			}
			if(*plast == '.'){
				plast++;
				res->len--;
			}
			memcpy(last, plast, res->len+1);
			*(last + res->len +1) = 0;
		}else{
			strcpy(last, res->nodot);
			res->len--;
		}
		if(neg){
			*(last+res->len +1) = '-';
			res->neg = last+res->len +1;
		}else
			res->neg = NULL;
		res->len += (dot_len > 0);
	}else{
		if(neg){
			*(last+res->len) = '-';
			res->neg = last+res->len;
		}else
			res->neg = NULL;
		res->nodot = NULL;
	}
	res->n = NULL;
	res->revert = last;
	return res;
}
void *division(struct number *num1, struct number *num2, unsigned long int virgule, boolean approximation, boolean *mod){
	struct number *res, *dviseur, *dvidende, dix = { NULL, "10", "01", NULL, 2, 0, 2 }, *tp, *tp_, *rst, n = { NULL, NULL, NULL, NULL, 1, 0, 1 }, z = {NULL, "0", "0", NULL, 1, 0, 1}, tst = { NULL, NULL, NULL, NULL, 0, 0, 0 };
	char *n1 = num1->n, *n2 = num2->n,
		*reste = NULL, *preste, *dot = NULL, *mul,
		t[2] = {0, 0}, *result = NULL, *pr, point = 0, *pneg,
		neg = 0, neg1 = 0, neg2 = 0;
	unsigned long int len = 0, virgule_ = 0, zero = 0, nreste = 0, qreste = 1;
	unsigned long int ii = 0;
	int x;
	NEG(num1, num2);
	NEG_TEST;
	//err_set(E_SET, 0);
	if(equal(num2, &z) == 0){
		fprintf(stderr, "Erreur: Division par 0\n");
		//err_set(E_SET, 2);
		return NULL;
	}
	if(equal(num1, &z) == 0){
		if((res = malloc(sizeof(struct number))) == NULL){
			perror("malloc()");
			exit(EXIT_FAILURE);
		}
		if((res->revert = malloc(2*sizeof(char))) == NULL){
			perror("malloc()");
			exit(EXIT_FAILURE);
		}
		res->neg = NULL;
		res->nodot = NULL;
		res->n = NULL;
		res->len = 1;
		res->val = 1;
		res->dot = 0;
		strcpy(res->revert, "0");
		return res;
	}
	if((dviseur = calloc(1, sizeof(struct number))) == NULL){
		perror("calloc()");
		exit(EXIT_FAILURE);
	}
	if((dvidende = calloc(1, sizeof(struct number))) == NULL){
		perror("calloc()");
		exit(EXIT_FAILURE);
	}
	if((dviseur->n = malloc((strlen(num2->n) +1)*sizeof(char))) == NULL){
		perror("malloc()");
		exit(EXIT_FAILURE);
	}
	if((dvidende->n = malloc((strlen(num1->n) + 1)*sizeof(char))) == NULL){
		perror("malloc()");
		exit(EXIT_FAILURE);
	}
	if((dviseur->revert = malloc((strlen(num2->revert) +1)*sizeof(char))) == NULL){
		perror("malloc()");
		exit(EXIT_FAILURE);
	}
	if((dvidende->revert = malloc((strlen(num1->revert) + 1)*sizeof(char))) == NULL){
		perror("malloc()");
		exit(EXIT_FAILURE);
	}
	strcpy(dviseur->n, num2->n);
	strcpy(dvidende->n, num1->n);
	strcpy(dviseur->revert, num2->revert);
	strcpy(dvidende->revert, num1->revert);
	dviseur->len = num2->len;
	dviseur->val = num2->val;
	dviseur->dot = num2->dot;
	dvidende->len = num1->len;
	dvidende->val = num1->val;
	dvidende->dot = num1->dot;
	dviseur->neg = (num2->neg) ? dviseur->revert + dviseur->len : NULL;
	dvidende->neg = (num2->neg) ? dvidende->revert + dvidende->len : NULL;
	//pneg = dviseur->revert + strlen(dviseur->revert)-1;
	pneg = dviseur->neg;
	dviseur->neg = (pneg && (*pneg == '+' || *pneg == '-')) ? pneg : NULL;
	//pneg = dvidende->revert + strlen(dvidende->revert)-1;
	pneg = dvidende->neg;
	dvidende->neg = (pneg && (*pneg == '+' || *pneg == '-')) ? pneg : NULL;
	//pneg = dvidende->revert + strlen(dvidende->revert)-1;
	if(neg1){
		*dvidende->neg = 0;
	}
	if(neg2){
		*dviseur->neg = 0;
	}
	if(dviseur->dot){
		memcpy(dviseur->revert+dviseur->dot, dviseur->revert+dviseur->dot+1, dviseur->val+1);
		mul = malloc(dviseur->dot+2);
		*(mul+dviseur->dot+1) = 0;
		memset(mul+1, '0',dviseur->dot);
		*mul = '1';
		dviseur->dot = 0;
		init(mul, &tst);
		tp = multiplication(dvidende, &tst);
		destroy_number(&dvidende, 0);
		dvidende = tp;
		dviseur->val = --dviseur->len;
		tp = &tst;
		destroy_number(&tp, 1);
	}
	/*if(strlen(dvidende->n) + virgule + 3 < strlen(dviseur->n)||
			((char *)dviseur->n)[dviseur->len] != 0 || ((char *)dvidende->n)[dvidende->len] != 0){
		fprintf(stderr, "Calcule de nombre trop grand\n");
		free(dviseur->n);
		free(dviseur->revert);
		if(dviseur->result)
			free(dviseur->result);
		if(dviseur->nodot)
			free(dviseur->nodot);
		free(dviseur);
		free(dvidende->n);
		free(dvidende->revert);
		if(dvidende->result)
			free(dvidende->result);
		if(dvidende->nodot)
			free(dvidende->nodot);
		free(dvidende);
		return NULL;
	}*/
	preste = allocation((void **)&reste, BUFFER, sizeof(char));
	pr = allocation((void **)&result, dvidende->len + neg + virgule+3, sizeof(char));
	*preste = 0;
	if(neg){
		*pr = '-';
		pr++;
	}
	len = dvidende->len-1;
	do{
		if(tst.n){
			free(tst.revert);
			tst.revert = NULL;
			tst.val = tst.dot = tst.len = 0;
		}
		if(nreste +1 >= BUFFER){
			qreste++;
			nreste = 0;
			preste = reallocation((void **)&reste, qreste*BUFFER);
		}
		*preste = dvidende->revert[dvidende->len - ii -1];
		preste[1] = 0;
		preste++;
		nreste++;
		ii++;
		init(reste, &tst);
	}while((unsigned long int)ii < dvidende->len && equal(&tst, dviseur) < 0);
	if((rst = calloc(1, sizeof(struct number))) == NULL){
		perror("calloc()");
		exit(EXIT_FAILURE);
	}
	memcpy(rst, &tst, sizeof(struct number));
	while((*mod = equal(rst, dviseur)) < 0 && virgule_ < virgule+(approximation != 0)){
		tp = multiplication(rst, &dix);
		destroy_number(&rst, 0);
		rst = tp;
		if(point == 0){
			*pr = '0';
			pr++;
			*pr = '.';
			dot = ++pr;
		}else{
			*pr = '0';
			pr++;
			virgule_++;
		}
		point = 1;
		zero++;
	}
	for(x = 9; x >= 0;x--){
		*t = x | (3 << 4);
		n.n = n.revert = t;
		tp = multiplication(&n, dviseur);
		tp_ = soustraction(rst, tp);
		if(equal(tp_,&z) >= 0){
			destroy_number(&rst, 0);
			destroy_number(&tp, 0);
			rst = tp_;
			break;
		}
		destroy_number(&tp, 0);
		destroy_number(&tp_, 0);
	}
	*pr = *t;
	pr++;
	while(ii < len || (*mod = equal(rst,&z)) != 0){
		tp = multiplication(rst, &dix);
		destroy_number(&rst, 0);
		rst = tp;
		if(ii < len){
			t[0] = dvidende->revert[dvidende->len - ii-1];
			t[1] = 0;
			n.n = n.revert = t;
			tp = addition(rst, &n);
			destroy_number(&rst, 0);
			rst = tp;
		}else{
			if(virgule+(approximation != 0) == 0)
				break;
			if(point == 0){
				point = 1;
				*pr = '.';
				dot = ++pr;
			}else
				virgule_++;
			if(virgule_ >= virgule+(approximation != 0))break;
		}
		for(x = 9; x >= 0; x--){
			*t = x | (3 << 4);
			n.n = n.revert = t;
			tp = multiplication(&n, dviseur);
			tp_ = soustraction(rst, tp);
			if(equal(tp_,&z) >= 0){
				destroy_number(&rst, 0);
				destroy_number(&tp, 0);
				rst = tp_;
				break;
			}else{
				destroy_number(&tp, 0);
				destroy_number(&tp_, 0);
			}
		}
		*pr = *t;
		pr++;
		ii++;
	}
	if((res = calloc(1, sizeof(struct number))) == NULL){
		perror("malloc()");
		exit(EXIT_FAILURE);
	}
	destroy_number(&dviseur, 0);
	destroy_number(&dvidende, 0);
	destroy_number(&rst, 0);
	if(dot){
		if(approximation && virgule+(approximation != 0) && dot){
			if((*(dot + virgule) - (3 << 4)) < 5){
				*(dot + virgule) = 0;
				for(dot = result; *dot == '0'; dot++);
				if(*dot == '.'){
					dot++;
				}
				if(dot != result)
					strcpy(result, dot);
				dot = NULL;
			}else{
				*(dot + virgule) = 0;
				dot = malloc(virgule+3);
				*(dot + virgule+2) = 0;
				memset(dot+1, '0', virgule);
				*(dot +1+virgule) = '1';
				*(dot +1) = '.';
				*dot = '0';
			}
		}else{
			dot = NULL;
		}
	}
	init(result, res);
	if(!dot){
		if(res->dot){
			for(dot = res->revert; *dot == '0'; dot++, res->len--, res->dot--);
			if(dot != res->revert){
				if(*dot == '.'){
					dot++;
					res->len--;
					ii++;
				}
				memcpy(res->revert, dot, res->len+1+neg);
				free(res->n);
				res->n = NULL;
			}
		}
		return res;
	}
	if((tp = calloc(1, sizeof(struct number))) == NULL){
		perror("malloc()");
		exit(EXIT_FAILURE);
	}
	init(dot, tp);
	rst = addition(res, tp);
	for(dot = rst->revert; *dot == '0'; dot++, rst->dot--, rst->len--);
	if(*dot == '.'){
		rst->len--;
		dot++;
	}
	if(dot != rst->revert)
		memcpy(rst->revert, dot, rst->len +neg +1);
	destroy_number(&tp, 0);
	destroy_number(&res, 0);
	return rst;
}
void *modulo(struct number *num1, struct number *num2, unsigned long int scale){
	struct number *div, *mul, *res/*, zero = { "0", "0", NULL, NULL, 1, 0, 1}*/;
	int mod;
	div = division(num1, num2, scale, 0, &mod);
	if(div == NULL)
		return NULL;
	if(mod == 0){
		if((res = calloc(1, sizeof(struct number))) == NULL){
			perror("calloc()");
			exit(EXIT_FAILURE);
		}
		if((res->n = malloc(2*sizeof(char))) == NULL){
			perror("malloc()");
			exit(EXIT_FAILURE);
		}
		if((res->revert = malloc(2*sizeof(char))) == NULL){
			perror("malloc()");
			exit(EXIT_FAILURE);
		}
		res->len = res->val = 1;
		strcpy(res->n, "0");
		strcpy(res->revert, "0");
		return res;
	}
	mul = multiplication(div, num2);
	if(mul == NULL){
		destroy_number(&div, 0);
		return NULL;
	}
	/*if(strlen(reste) == 0){
		free(reste);
		reste = multiplication("0", "0");
		if(reste == NULL){
			free(result);
			return NULL;
		}
	}*/
	/*reorder(mul);
	printf("%s - %s = ", num1->n, mul->n);*/
	res = soustraction(num1, mul);
	/*reorder(res);
	printf("%s\n", res->n);*/
	//exit(0);
	/*if(dividende == NULL){
		free(result);
		free(reste);
		return NULL;
	}*/
	destroy_number(&mul, 0);
	destroy_number(&div, 0);
	return res;
}
