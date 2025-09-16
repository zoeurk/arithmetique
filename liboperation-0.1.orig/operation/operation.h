#ifndef OPERATION_H
#define OPERATION_H
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
struct bin{
	unsigned long int num;
	int nmemb;
	int full;
	struct bin *next;
	struct bin *prev;
};
#define INIT_BIN(num, nmemb, next, prev) \
	{ num, nmemb, (nmemb == BLK), next, prev }
#define ZERO_BIN { 0, 1, 0, NULL, NULL }
struct nbr{
	unsigned long int bval;
	unsigned long int bdot; /*128*/
	int val;
	int dot;
	int neg;
	#if  __WORDSIZE != 32
		#define PADDING 0
		#define ZERO(zero_bin) { 0, 0, 1, 0, 0, PADDING, zero_bin, NULL }
		#define INIT_NBR(sig, bval, val, bdot, dot, num, n) \
			{ bval, bdot, val, dot, (sig != 0 && sig != '+'), PADDING, num, n }
		int ___; /*128*/
	#else
		#define ZERO(zero_bin) { 0, 0, 1, 0, 0, zero_bin, NULL }
		#define INIT_NBR(sig, bval, val, bdot, dot, num, n) \
			{ bval, bdot, val, dot, (sig != 0 && sig != '+'), num, n }
	#endif
	struct bin *num;
	char *n;
};
/*****************************************************************/
#if __WORDSIZE == 32
	#define BLK 4
	#define C_BLK BLK+1
	#define D_MAX 1000
	#define D_MAX_LIMIT 10000
	#define COEFS { 1, 10, 100, D_MAX, D_MAX_LIMIT }
#else
	#define BLK 9
	#define C_BLK BLK+1
	#define D_MAX 100000000
	#define D_MAX_LIMIT 1000000000
	#define COEFS { 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, D_MAX, D_MAX_LIMIT }
#endif

#define DOT(res_nbr, bin_ptr) \
	bin_ptr = res_nbr->num ;\
	while((res_nbr->dot || res_nbr->bdot) && bin_ptr->num%10 == 0){ \
		if(res_nbr->dot){ \
			res_nbr->dot--; \
		}else{ \
			res_nbr->dot = BLK-1;\
			res_nbr->bdot--; \
		} \
		bin_ptr->num /= 10; \
		bin_ptr->full = 0; \
		if(--bin_ptr->nmemb == 0){ \
			bin_ptr = bin_ptr->next; \
			/*bin_ptr = res_nbr->num; \
			res_nbr->num->next->prev = res_nbr->num->prev; \
			res_nbr->num = res_nbr->num->next; \
			res_nbr->num->prev->next = NULL; \
			free(bin_ptr);*/ \
		} \
	}
/*
#define DOT(res_nbr, bin_ptr) \
	while((res_nbr->dot || res_nbr->bdot) && res_nbr->num->num%10 == 0){ \
		if(res_nbr->dot){ \
			res_nbr->dot--; \
		}else{ \
			res_nbr->dot = BLK-1;\
			res_nbr->bdot--; \
		} \
		res_nbr->num->num /= 10; \
		res_nbr->num->full = 0; \
		if(--res_nbr->num->nmemb == 0){ \
			bin_ptr = res_nbr->num; \
			res_nbr->num->next->prev = res_nbr->num->prev; \
			res_nbr->num = res_nbr->num->next; \
			res_nbr->num->prev->next = NULL; \
			free(bin_ptr); \
		} \
	}
*/
#define ADJUST_0(res_nbr, bin_ptr1, bin_ptr2) \
	/*if(res_nbr->num->prev && res_nbr->num->prev->nmemb == 0){ \
		bin_ptr1 = res_nbr->num->prev; \
		bin_ptr2 = res_nbr->num; \
		bin_ptr1->prev->next = NULL; \
		bin_ptr2->prev = (bin_ptr1->prev == res_nbr->num) ? NULL : bin_ptr1->prev; \
		free(bin_ptr1); \
	}*/ \
	for(	bin_ptr1 = (res_nbr->num->prev) ? res_nbr->num->prev : res_nbr->num; \
		(res_nbr->val > 1 || res_nbr->bval > 0) && bin_ptr1->num/mul[bin_ptr1->nmemb -1] == 0; \
		/*res_nbr->val--*/ \
	){ \
		bin_ptr1->full = 0;\
		if(--bin_ptr1->nmemb == 0){ \
			bin_ptr1 = bin_ptr1->prev; \
			/*bin_ptr2 = res_nbr->num; \
			bin_ptr1->prev->next = NULL; \
			bin_ptr2->prev = (bin_ptr1->prev == res_nbr->num) ? NULL : bin_ptr1->prev;*/ \
			res_nbr->val -= bin_ptr1->nmemb; \
			/*free(bin_ptr1); \
			bin_ptr1 = (res_nbr->num->prev) ? res_nbr->num->prev : res_nbr->num;*/ \
		} \
		if(res_nbr->val > 0){ \
			res_nbr->val--; \
		}else{ \
			res_nbr->val = BLK-1; \
			res_nbr->bval--; \
		} \
	}
char *parse_nbr(char *n);
struct nbr *encode_nbr(char *n);
void print_nbr(struct nbr *n);
void *destroy_nbr(struct nbr *n);
int equal(struct nbr *num1, struct nbr *num2);
struct bin *new_num(unsigned long int val, unsigned long int dot);
void *dup_nbr(struct nbr *num);
void *bymul10(struct nbr *num, int fac, int faclen);
void *align_dot(struct nbr *_num, unsigned long int blk, int bytes);
struct retbcpy{
	unsigned long int rblk;
	int rbytes;
	#if __WORDSIZE != 32
		#define I_RBCPY { 0, 0, PADDING }
		int ___;
	#else
		#define I_RBCPY { 0, 0 }
	#endif
};
struct retbcpy *nbytescpy(struct bin **b2, struct bin **b1, int *bstart, unsigned long int lbytes, unsigned long int bytes);
void *addition(struct nbr *num1, struct nbr *num2, struct nbr *result);
void *soustraction(struct nbr *num1, struct nbr *num2, struct nbr *result);
void *multiplication(struct nbr *num1, struct nbr *num2, struct nbr *result);
void *ispuissance(struct nbr *num, int pui);
void *spuissance(struct nbr *num, unsigned long int bpui, int pui);
void *bymin10(struct nbr *num, unsigned long int bscale, int scale);
void *division(struct nbr *num1, struct nbr *num2, struct nbr **modulo, unsigned long int bscale, int scale, int approximation);
void *puissance(struct nbr *num1, struct nbr *num2, struct nbr **modulo, unsigned long int bscale, int scale, int approximation);
#endif
