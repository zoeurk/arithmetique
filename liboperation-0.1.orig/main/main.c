#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <stdio.h>
#include "../operation/operation.h"
#include "../arithmetique/arithmetique.h"
int main(int argc, char **argv){
	struct bin *s;
	struct bin *dot;
	struct nbr *nbr1, *nbr2, *res, *reste = NULL, *result, *ediv_spaces[5] = { NULL, NULL, NULL, NULL, NULL };
	enum ediv_values i;
	unsigned long int virgule, bvirg, bv = 0, bl;
	int comp = 0, approx = 0, virg, v = 0, l;
	char c, *pn1, *pn2, *end;
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
	bv = nbr1->bdot + nbr2->bdot;
	v = nbr1->dot + nbr2->dot;
	if(v >= BLK){
		v -= BLK;
		bv++;
	}
	if(bv < bvirg || (bv == bvirg && v < virg)){
		v = virg;
		bv = bvirg;
	}
	bl = nbr1->bval + nbr2->bval;
	l = nbr1->val + nbr2->val;
	if(l >= BLK){
		l -= BLK;
		bl++;
	}
	if((result = calloc(1, sizeof(struct nbr))) == NULL){
		perror("calloc()");
		exit(EXIT_FAILURE);
	}
	result->num = new_num(bl + (l > 0), bv + (v > 0));
	comp = equal(nbr1, nbr2);
	c = (comp == 0) ? '=' : (comp == -1) ? '<' : '>';
	printf("%s %c %s\n", argv[1], c, argv[2]);
	res = addition(nbr1, nbr2, result);
	DOT(res, dot, s);
	printf("%s + %s = ", argv[1], argv[2]);
	print_nbr(result);
	putchar('\n');
	result = reset_num(result);
	res = soustraction(nbr1, nbr2, result);
	DOT(res, dot, s);
	printf("%s - %s = ", argv[1], argv[2]);
	print_nbr(res);
	putchar('\n');
	result = reset_num(result);
	res = multiplication(nbr1, nbr2, result);
	DOT(res, dot, s);
	printf("%s * %s = ", argv[1], argv[2]);
	print_nbr(res);
	putchar('\n');
	/*for(i = DIVISEUR; i <= MODULO; i++){
		if((ediv_spaces[i] = calloc(1, sizeof(struct nbr))) == NULL){
			perror("calloc()");
			exit(EXIT_FAILURE);
		}
		switch(i){
			case DIVISEUR:
				ediv_spaces[i]->num = new_num(nbr2->bval + (nbr2->val > 0),
								nbr2->bdot + (nbr2->dot > 0));
				break;
			case DIVIDENDE: case MODULO:
				ediv_spaces[i]->num = new_num(nbr1->bval + (nbr1->val > 0),
							nbr1->bdot + (nbr1->dot > 0)
							+ nbr2->bdot + (nbr2->dot > 0)
							+ bvirg + (virg > 0) + 3);
				break;
			case QUOTIENT:
				ediv_spaces[i]->num = new_num(nbr1->bval + (nbr1->val > 0),
								nbr1->bdot + (nbr1->dot > 0) + bvirg + (virg > 0) +1);
				break;
			case RESTE:
				ediv_spaces[i]->num = new_num(nbr1->bval + (nbr1->val > 0),
							nbr1->bdot + (nbr1->dot > 0)
							+ bvirg + (virg > 0) + 1);
				break;
		}
	}
	fprintf(stderr, "Method of Euclide\n");
	if((res = edivision(nbr1, nbr2, &reste, bvirg, virg, approx, ediv_spaces)) != NULL){
		DOT(res, dot, s);
		DOT(reste, dot, s);
		printf("%s / %s = ", argv[1], argv[2]);
		print_nbr(res);
		putchar('\n');
		printf("%s %% %s = ", argv[1], argv[2]);
		print_nbr(reste);
		putchar('\n');
	}else
		printf("NULL\n");
	for(i = DIVISEUR; i <= MODULO;i++)
		destroy_nbr(ediv_spaces[i]);*/
	/*fprintf(stderr, "Method of Newton-Raphston (Not Really Tested)\n");
	if((res = nrdivision(nbr1, nbr2, bvirg, virg, NULL)) != NULL){
		printf("%s / %s = ", argv[1], argv[2]);
		print_nbr(res);
		putchar('\n');
		destroy_nbr(res);
	}else
		printf("NULL\n");*/
	destroy_nbr(result);
	/*fprintf(stderr, "Method of Kuhn\n");*/
	if((res = kdivision(nbr1, nbr2, &reste, bvirg, virg, approx, NULL)) != NULL){
		DOT(res, dot, s);
		/*printf("%lu :: %i => %lu\n", reste->bdot, reste->dot, reste->num->num);
		for(s = reste->num;s; s = s->next)
			printf("%lu :: %i :: %i\n", s->num, s->nmemb, s->full);*/
		DOT(reste, dot, s);
		printf("%s / %s = ", argv[1], argv[2]);
		print_nbr(res);
		putchar('\n');
		printf("%s %% %s = ", argv[1], argv[2]);
		print_nbr(reste);
		putchar('\n');
	}else
		printf("NULL\n");
	if(res){
		destroy_nbr(res);
	}
	if(reste)
		destroy_nbr(reste);
	destroy_nbr(nbr1);
	destroy_nbr(nbr2);
	exit(0);
	if(!nbr1->neg){
		res = r_square(nbr1, bvirg, virg);
		printf("Square Root of %s = ", argv[1]);
		print_nbr(res);
		putchar('\n');
		destroy_nbr(res);
	}else
		printf("Square Root Not Computable: %s < 0", argv[1]);
	/*destroy_nbr(result);
	destroy_nbr(nbr1);
	destroy_nbr(nbr2);
	exit(0);*/
	if(!nbr2->neg){
		res = r_square(nbr2, bvirg, virg);
		printf("Square Root of %s = ", argv[2]);
		print_nbr(res);
		putchar('\n');
		destroy_nbr(res);
	}else
		printf("Square Root Not Computable: %s < 0", argv[2]);
	destroy_nbr(result);
	destroy_nbr(nbr1);
	destroy_nbr(nbr2);
	exit(0);
	if(nbr2->dot == 0 && nbr2->bdot == 0){
		reste = NULL;
		res = puissance(nbr1, nbr2, &reste, bvirg, virg, approx);
		DOT(res, dot, s);
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
