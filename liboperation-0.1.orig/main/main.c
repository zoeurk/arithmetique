#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <stdio.h>
#include "../operation/operation.h"
#include "../arithmetique/arithmetique.h"
int main(int argc, char **argv){
	/*struct retbcpy *cpy, rd = I_RBCPY;*/
	struct bin *dot/*, bdix = { 10, 2, 0, NULL, NULL }*/;
	struct nbr *nbr1, *nbr2, *res, *reste = NULL/*, dix = INIT_NBR( 0, 0, 2, 0, 0, NULL, "10" )*/;
	unsigned long int virgule, bvirg;
	int comp = 0, approx = 0, virg;
	char c, *pn1, *pn2, *end;
	/*dix.num = &bdix;*/
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
	virg = virgule%BLK;
	bvirg = virgule/BLK;
	/*res = spuissance(&dix, bvirg, virg);
	print_nbr(res);
	putchar('\n');
	for(dot = res->num; dot; dot = dot->next)
		printf("%lu :: %i :: %i\n", dot->num, dot->nmemb, dot->full);
	destroy_nbr(res);
	exit(0);*/
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
	putchar('\n');
	destroy_nbr(res);
	res = soustraction(nbr1, nbr2);
	DOT(res, dot);
	printf("%s - %s = ", argv[1], argv[2]);
	print_nbr(res);
	putchar('\n');
	destroy_nbr(res);
	/*exit(0);*/
	res = multiplication(nbr1, nbr2);
	DOT(res, dot);
	printf("%s * %s = ", argv[1], argv[2]);
	print_nbr(res);
	putchar('\n');
	destroy_nbr(res);
	/*res = spuissance(nbr1, bvirg, virg);
	print_nbr(res);
	putchar('\n');
	destroy_nbr(res);*/
	if((res = division(nbr1, nbr2, &reste, bvirg, virg, approx)) != NULL){
		DOT(res, dot);
		DOT(reste, dot);
		printf("%s / %s = ", argv[1], argv[2]);
		print_nbr(res);
		putchar('\n');
		printf("%s %% %s = ", argv[1], argv[2]);
		print_nbr(reste);
		putchar('\n');
		destroy_nbr(res);
		destroy_nbr(reste);
	}else
		printf("NULL\n");
	/*destroy_nbr(nbr1);
	destroy_nbr(nbr2);*/
	if(!nbr1->neg){
		res = r_square(nbr1, bvirg, virg);
		printf("Square Root of %s = ", argv[1]);
		print_nbr(res);
		putchar('\n');
		destroy_nbr(res);
	}else
		printf("Square Root Not Computable: %s < 0", argv[1]);
	if(!nbr2->neg){
		res = r_square(nbr2, bvirg, virg);
		printf("Square Root of %s = ", argv[2]);
		print_nbr(res);
		putchar('\n');
		destroy_nbr(res);
	}else
		printf("Square Root Not Computable: %s < 0", argv[2]);
	destroy_nbr(nbr1);
	destroy_nbr(nbr2);
	exit(0);
	if(nbr2->dot == 0 && nbr2->bdot == 0){
		reste = NULL;
		res = puissance(nbr1, nbr2, &reste, bvirg, virg, approx);
		DOT(res, dot);
		printf("%s ^ %s = ", argv[1], argv[2]);
		print_nbr(res);
		putchar('\n');
		if(reste)
			destroy_nbr(reste);
		destroy_nbr(res);
	}else
		printf("Puissance non scalaire.\n");
	destroy_nbr(nbr1);
	destroy_nbr(nbr2);
	return 0;
}
