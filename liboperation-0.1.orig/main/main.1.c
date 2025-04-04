#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <stdio.h>
#include "../operation/operation.1.h"
int main(int argc, char **argv){
	struct bin *dot;
	struct nbr *nbr1, *nbr2, *res, *reste = NULL;
	unsigned long int virgule, n;
	int comp = 0, approx = 0, start = 0;
	char c, *pn1, *pn2, *end;
	int coef[C_BLK] = COEFS;
	if(argc < 4 || argc > 5){
		fprintf(stderr, "usage: %s num1 num2 virgule [approximation (yes|no)]\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	if(argc == 5){
		if(((comp = ((strcasecmp(argv[4], "yes") == 0))) != 1) && ((comp = (2*(strcasecmp(argv[4], "no") == 0))) != 2)){
			fprintf(stderr, "Invalid value for approximation:\n\t%s != yes|no\n", argv[4]);
			exit(EXIT_FAILURE);
		}
		approx = (comp == 1);
	}
	#if __WORDSIZE == 64 || __WORDSIZE == 32
		printf("ARCH: %i BITS\n", __WORDSIZE);
	#else
		fprintf(stderr, "ARCH UNKNOW: %i BITS\n", __WORDSIZE);
		exit(EXIT_FAILURE);
	#endif
	virgule = strtoul(argv[3], &end, 10);
	if(*end != 0){
		fprintf(stderr, "Invalid number for dot:\n\tendptr == %s\n\tinput == %s\n", end, argv[3]);
		exit(EXIT_FAILURE);
	}
	printf("Parsing num1: %s\n", argv[1]);
	if((pn1 = parse_nbr(argv[1])) == NULL)
		exit(EXIT_FAILURE);
	printf("Parsing num2: %s\n", argv[2]);
	if((pn2 = parse_nbr(argv[2])) == NULL)
		exit(EXIT_FAILURE);
	nbr1 = encode_nbr(pn1);
	nbr2 = encode_nbr(pn2);
	nbr1->n = pn1;
	nbr2->n = pn2;
	comp = equal(nbr1, nbr2);
	c = (comp == 0) ? '=' : (comp == -1) ? '<' : '>';
	printf("%s %c %s\n", argv[1], c, argv[2]);
	res = addition(nbr1, nbr2);
	DOT(res, dot);
	printf("%s + %s = ", argv[1], argv[2]);
	print_nbr(res);
	destroy_nbr(res);
	res = soustraction(nbr1, nbr2);
	DOT(res, dot);
	printf("%s - %s = ", argv[1], argv[2]);
	print_nbr(res);
	destroy_nbr(res);
	res = multiplication(nbr1, nbr2);
	DOT(res, dot);
	printf("%s * %s = ", argv[1], argv[2]);
	print_nbr(res);
	destroy_nbr(res);
	if((res = division(nbr1, nbr2, &reste, virgule, approx)) != NULL){
		DOT(res, dot);
		DOT(reste, dot);
		printf("%s / %s = ", argv[1], argv[2]);
		print_nbr(res);
		printf("%s %% %s = ", argv[1], argv[2]);
		print_nbr(reste);
		destroy_nbr(res);
		destroy_nbr(reste);
	}
	if(nbr2->dot == 0){
		reste = NULL;
		res = puissance(nbr1, nbr2, &reste, virgule, approx);
		DOT(res, dot);
		printf("%s ^ %s = ", argv[1], argv[2]);
		print_nbr(res);
		if(reste)
			destroy_nbr(reste);
		destroy_nbr(res);
	}
	destroy_nbr(nbr1);
	destroy_nbr(nbr2);
	return 0;
}
