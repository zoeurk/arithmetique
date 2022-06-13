#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../operation/operation.h"
#include "../arithmetique/arithmetique.h"
/*BUFFER > 1*/
//const unsigned long int BUFFER = 56;

const unsigned long int BUFFER = 56;

int main(int argc, char **argv){
	int ret, i, v;
	unsigned long int sz = 56;
	char *r, *format, *check;
	if(argc < 4){
		fprintf(stderr, "usage:\n\t%s num1 num2 virgule(>= 0) [internal_buflen(default = 56)]\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	v = atoi(argv[3]);
	if(argc > 4)
		sz = atoi(argv[4]);
	if(v < 0){
		fprintf(stderr, "usage:\n\t%s num1 num2 virgule(>= 0) [internal_buflen(default = 56)]\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	if(v > 0){
		if((format = malloc(strlen(argv[3])+7)) == NULL){
			perror("malloc()");
			exit(0);
		}
		strcpy(format,"%.");
		strcat(format,argv[3]);
		strcat(format,"Lf");
	}else{
		if((format = malloc(4)) == NULL){
			perror("malloc()");
			exit(EXIT_FAILURE);
		}
		strcpy(format,"%Lf");
	}
	for(i = 1; i < 3; i++)
		if((ret = strtype(argv[i])) != 0)
			switch(ret){
				case 1: case 2:
					fprintf(stderr, "Donnee invalide\n");
					exit(EXIT_FAILURE);
			}
	ret = equal(argv[1],argv[2]);
	switch(ret){
		case 0:
			printf("%s == %s\n", argv[1], argv[2]);
			break;
		case 1:
			printf("%s > %s\n", argv[1], argv[2]);
			break;
		case -1:
			printf("%s < %s\n", argv[1], argv[2]);
			break;
	}
	r = addition(argv[1], argv[2]);
	if(r){
		printf("addition:%s\n", (char *)r);
		free(r);
	}
	r = soustraction(argv[1], argv[2]);
	if(r){
		printf("soustraction:%s\n", (char *)r);
		free(r);
	}
	r = multiplication(argv[1], argv[2]);
	if(r){
		printf("multiplication:%s\n", (char *)r);
		free(r);
	}
	r = division(argv[1], argv[2], atoi(argv[3]),1);
	if(r){
		printf("division:%s\n", (char *)r);
		free(r);
	}
	r = modulo(argv[1], argv[2], 0);
	if(r){
		printf("modulo:%s\n", (char *)r);
		free(r);
	}
	r = modulo(argv[1], argv[2], atoi(argv[3]));
	if(r){
		printf("modulo etendu:%s\n",  (char *)r);
		free(r);
	}
	r = racine_carree(argv[1], atoi(argv[3]),1);
	if(r){
		check = multiplication(r, r);
		printf("racine carree aproximatif de '%s': %s\n", argv[1], r);
		printf("Verification:%s\n", check);
		free(r);
		free(check);
	}
	r = racine_carree(argv[1], atoi(argv[3]),0);
	if(r){
		check = multiplication(r, r);
		printf("racine carree de '%s': %s\n", argv[1], r);
		printf("Verification:%s\n", check);
		free(r);
		free(check);
	}	
	r = racine_carree(argv[2], atoi(argv[3]),1);
	if(r){
		check = multiplication(r, r);
		printf("racine carree aproximatif de '%s': %s\n", argv[2], r);
		printf("Verification:%s\n", check);
		free(r);
		free(check);
	}
	r = racine_carree(argv[2], atoi(argv[3]),0);
	if(r){
		check = multiplication(r, r);
		printf("racine carree de '%s': %s\n", argv[2], r);
		printf("Verification:%s\n", check);
		free(r);
		free(check);
	}
	printf("\t\tTrigonometrie:\n");
	r = cosinus(argv[1], format, sz,0, 0, atoi(argv[3]), 0);
	if(r){
		printf("le cosinus de \'%s\':%s\n", argv[1],r);
		free(r);
	}
	/*r = acosinus(argv[1], format, sz);
	if(r){
		printf("l'arc de cosinus de \'%s\':%s\n", argv[1],r);
		free(r);
	}*/
	r = cosinus(argv[2], format, sz, 0, 0, atoi(argv[3]), 0);
	if(r){
		printf("le cosinus de \'%s\':%s\n", argv[2], r);
		free(r);
	}
	/*r = acosinus(argv[2], format, sz);
	if(r){
		printf("l'arc de cosinus de \'%s\':%s\n", argv[2], r);
		free(r);
	}*/
	r = sinus(argv[1], format, sz, 0, 0, atoi(argv[3]), 0);
	if(r){
		printf("le sinus de \'%s\':%s\n", argv[1],r);
		free(r);
	}
	/*r = asinus(argv[1], format, sz);
	if(r){
		printf("l'arc de sinus de \'%s\':%s\n", argv[2],r);
		free(r);
	}*/
	r = sinus(argv[2], format, sz, 0, 0, atoi(argv[3]), 0);
	if(r){
		printf("le sinus de \'%s\':%s\n", argv[2], r);
		free(r);
	}
	/*r = asinus(argv[2], format, sz);
	if(r){
		printf("l'arc du sinus de \'%s\':%s\n", argv[2], r);
		free(r);
	}*/
	r = tangente(argv[1], format, sz, 0, 0, atoi(argv[3]), 0);
	if(r){
		printf("la tangente de \'%s\':%s\n", argv[1],r);
		free(r);
	}
	/*r = atangente(argv[1], format, sz);
	if(r){
		printf("l'arc de la tangente de \'%s\':%s\n", argv[2],r);
		free(r);
	}*/
	r = tangente(argv[2], format, sz, 0, 0, atoi(argv[3]), 0);
	if(r){
		printf("la tangente de \'%s\':%s\n", argv[2], r);
		free(r);
	}
	/*r = atangente(argv[2], format, sz);
	if(r){
		printf("l'arc de la tangente de \'%s\':%s\n", argv[2], r);
		free(r);
	}*/
	printf("\t\t\t===\n");
	r = puissance(argv[1],argv[2], sz, format,atoi(argv[3]), 1);
	if(r){
		printf("%s^%s  = %s\n", argv[1], argv[2], r);
		free(r);
	}
	r = log_n(argv[1], sz, format,atoi(argv[3]), 1);
	if(r){
		printf("Logarithme Neperien de '%s': %s\n", argv[1], r);
		free(r);
	}
	r = log_n(argv[2], sz, format,atoi(argv[3]), 1);
	if(r){
		printf("Logarithme Neperien de '%s': %s\n", argv[2], r);
		free(r);
	}
	r = log_10(argv[1], sz, format,atoi(argv[3]), 1);
	if(r){
		printf("Logarithme 10 de '%s': %s\n", argv[1], r);
		free(r);
	}
	r = log_10(argv[2], sz, format,atoi(argv[3]), 1);
	if(r){
		printf("Logarithme 10 de '%s': %s\n", argv[2], r);
		free(r);
	}
	r = exponentiel(argv[1], sz, format, atoi(argv[3]), 1);
	if(r){
		printf("Exponentiel '%s': %s\n", argv[1], r);
		free(r);
	}
	r = exponentiel(argv[2], sz, format, atoi(argv[3]), 1);
	if(r){
		printf("Exponentiel '%s': %s\n", argv[2], r);
		free(r);
	}
	free(format);
	return 0;
}
