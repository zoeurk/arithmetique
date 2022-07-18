#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "../operation/operation.h"
#include "arithmetique.h"

#ifdef _MATH_H
#define INTERNAL_BUFLEN \
	if(buffer[internal_buflen-1] != 0){\
		fprintf(stderr, "Tampon (internal_buflen) interne trop petit\n");\
		exit(EXIT_FAILURE);\
	}
	
#define TRIGO(fn) \
	char *arg_ = NULL, *t_arg, pi[58], npi[58], *pi_ = NULL, *temp, *t = NULL, *mul, buffer[internal_buflen], *pbuf, *form;\
	long double val, format_;\
	form = format+2;\
	format_ = strtold(form,NULL);\
	if(format_ > internal_buflen){\
		fprintf(stderr, "Echec :%.0Lf > %lu (internal_buflen)\n", format_, internal_buflen);\
		exit(EXIT_FAILURE);\
	}\
	arg_ = multiplication(arg,"1");\
	memset(buffer, 0, internal_buflen);\
	sprintf(pi,"%.54Lf", 8*atanl(1));\
	sprintf(npi,"-%.54Lf", 8*atanl(1));\
	t = multiplication(arg_,"1");\
	if(i_deg){\
		t_arg = multiplication(t, pi);\
		free(t);\
		t = division(t_arg, "180", virgule, approximation);\
		free(t_arg);\
	}\
	if(equal(arg_,"0") > 0){\
		mul = division(arg_, pi, 0,0);\
		pi_ = multiplication(pi, mul);\
		temp = soustraction(t, pi_);\
		free(mul);\
		free(pi_);\
		free(t);\
		t = temp;\
	}else{\
		if(equal(arg_, "0") < 0){\
			mul = division(arg_, pi, 0, 0);\
			pi_ = multiplication(npi, mul);\
			temp = addition(t, pi_);\
			free(mul);\
			free(pi_);\
			free(t);\
			t = temp;\
		}\
	}\
	free(arg_);\
	val = fn(strtold(t,NULL));\
	snprintf(buffer,  internal_buflen,format, val);\
	if(buffer[internal_buflen-1] != 0){\
		fprintf(stderr, "Tampon interne trop petit (internal_buflen)\n");\
		exit(EXIT_FAILURE);\
	}\
	free(t);\
	t = multiplication(buffer,"1");\
	if(o_deg){\
		arg_ = multiplication(t, "180");\
		free(t);\
		t = division(arg_, pi, virgule, approximation);\
		free(arg_);\
	}\
	pbuf = t;

#define TEST(msg,arg)\
	if(equal(arg,"-1") < 0){\
		fprintf(stderr, msg, arg);\
		return NULL;\
	}else{\
		if(equal("1", arg) < 0){\
			fprintf(stderr, msg, arg);\
			return NULL;\
		}\
	}
void *cosinus(char *arg, char *format,unsigned long internal_buflen, int i_deg, int o_deg, unsigned long int virgule, int approximation){
	TRIGO(cosl);
	return pbuf;
}

void *sinus(char *arg, char *format,unsigned long internal_buflen,int i_deg, int o_deg, unsigned long int virgule, int approximation){
	TRIGO(sinl);
	return pbuf;
}
void *tangente(char *arg, char *format,unsigned long internal_buflen,int i_deg, int o_deg, unsigned long int virgule, int approximation){
	TRIGO(tanl);
	return pbuf;
}

void *puissance(void *num1, void *num2, unsigned long int internal_buflen, char *format, unsigned long int virgule, int approximation){
	char *n1 = multiplication(num1,"1"),
		*n2 = multiplication(num2,"1"),
		buffer[internal_buflen], *v, 
		*n1_ = n1,
		*i = multiplication("1","0"), *mod = NULL;
	char *i_, *v_ = NULL, *pseudo = NULL, *pseudo__, *p, *dot_, *pdot_;
	long double pseudo_;
	int eq, set = 0, neg = 0;
	memset(buffer, 0, internal_buflen);
	if(equal(num2, "0") == 0){
		free(n1);
		free(n2);
		free(i);
		n1 = multiplication("1","1");
		return n1;
	}
	if(equal(num2,"0") < 0){
		neg = 1;
		free(n2);
		n2 = multiplication(num2, "-1");
	}
	if((v = strchr(n2, '.')) != NULL){
		n1 = n1_;
		if(equal(n1, "0") < 0 && strchr(num2,'.')){
			free(n1);
			free(n2);
			free(i);
			if((n1 = calloc(5,sizeof(char))) == NULL){
				perror("calloc()");
				exit(EXIT_FAILURE);
			}
			strcpy(n1, "-nan");
			return n1;
		}
		if((v_ = calloc(strlen(n2)+2,sizeof(char))) == NULL){
			perror("calloc()");
			exit(EXIT_FAILURE);
		}
		strcpy(v_+1, v);
		*v_ = '0';
		*v = 0;
		pseudo = buffer;
		do{
			pseudo_ = strtold(n1, NULL);
			sprintf(buffer, format, pseudo_);
			if((eq = equal(n1, pseudo)) > 0){
				n1_ = racine_carree(n1, virgule,approximation);
				free(n1);
				n1 = n1_;
				i_ = addition(i, "1");
				free(i);
				i = i_;
			}
		}while(eq > 0);
		pdot_ = strchr(format,'.');
		if((dot_ = calloc(strlen(format), sizeof(char))) == NULL){
			perror("calloc()");
			exit(EXIT_FAILURE);
		}
		strcpy(dot_, pdot_+1);
		pdot_ = strchr(dot_, 'L');
		*pdot_ = 0;
		if((p = strchr(v_,'.')) != NULL && strlen(p+1) > strtoul(dot_, NULL,10)){
			fprintf(stderr,"Warning `%s`:\n\tTrop de chiffre apres la virgule.\n\tUtilisation de la valeur: ", v_);
			fprintf(stderr, format, strtold(v_, NULL));
			fprintf(stderr,"\n");
		}
		free(dot_);
		pseudo_ = powl(strtold(n1_, NULL), strtold(v_, NULL));
		snprintf(buffer, internal_buflen,format, pseudo_);
		free(v_);
		if(buffer[internal_buflen-1] != 0){
			fprintf(stderr, "buffer interne trop court (internal_buflen\n");
			exit(EXIT_FAILURE);
		}
		if(equal(i,"0") != 0){
			while(equal(i,"0") != 0){
				if(set == 0){ 
					pseudo = multiplication(buffer, buffer);
					set = 1;
				}else{
					p = pseudo;
					pseudo = multiplication(p, buffer);
					free(p);
				}
				i_ = soustraction(i, "1");
				free(i);
				i = i_;
			}
			free(i);
			i = NULL;
		}
	}
	if(i)
		free(i);
	i = multiplication(n2, "1");
	if(equal(n2,"0") == 0){
		pseudo = multiplication("1", buffer);
		if(neg){
			free(n1);
			n1 = division("1", pseudo, virgule, approximation);
			free(pseudo);
			free(n2);
			free(i);
			return n1;
		}
		free(i);
		free(n1);
		free(n2);
		return pseudo;
	}
	pseudo = addition("0", "1");
	while(equal(i,"1") != 0){
		mod = modulo(n2, "2", 0);
		if(equal(mod, "1") == 0){
			i_ = soustraction(i, "1");
			pseudo__ = multiplication(pseudo, n1);
			free(pseudo);
			pseudo = pseudo__;
			free(i);
			i = i_;
		}else{
			n1_ = multiplication(n1, n1);
			free(n1);
			n1 = n1_;
			i_ = division(i, "2",0, 0);
			free(i);
			i = i_;
		}
		free(mod);
	}
	n1_ = multiplication(n1, pseudo);
	free(pseudo);
	free(n1);
	n1 = n1_;
	if(strlen(buffer) > 0){
		n1_ = multiplication(buffer, n1);
		free(n1);
		n1 = n1_;
	}
	if(neg == 1){
		n1_ = division("1", n1, virgule, approximation);
		free(n1);
		n1 = n1_;
	}
	if(i)
		free(i);
	free(n2);
	return n1;
}
#define LOG(fn, msg)\
	char *n = multiplication(num, "1"), *n_, buffer[internal_buflen], *i = multiplication("1", "1"), *pi = i, *result;\
	memset(buffer,0, internal_buflen);\
	if(equal(n, "0") <= 0){\
			if(equal(n, "0") < 0){\
			free(n);\
			free(i);\
			fprintf(stderr, "Erreur: %s: %s < 0 [-nan]\n", msg, (char *)num);\
			return NULL;\
		}else{\
			free(n);\
			free(i);\
			n_ = malloc(5*sizeof(char));\
			strcpy(n_, "-inf");\
			return n_;\
		}\
	}\
	memset(buffer, 0, internal_buflen);\
	if(equal(n,"10") > 0){\
		for(n = n, i = i;equal(n, "10") > 0;n_ = racine_carree(n, virgule, approximation), free(n), n = n_, pi = multiplication(i, "2"),free(i), i = pi);;\
		snprintf(buffer, internal_buflen,format, fn(strtold(n, NULL)));\
	}else{\
		snprintf(buffer,internal_buflen,format, logl(strtold(n,NULL)));\
	}\
	if(buffer[internal_buflen -1] != 0){\
		fprintf(stderr,"Tampon interne (internal_buflen) trop court\n");\
		exit(EXIT_FAILURE);\
	}\
	result = multiplication(i, buffer);\
	free(i);\
	free(n);\
	return result;

void *log_n(void *num, unsigned long int internal_buflen, char *format, unsigned long int virgule,int approximation){
	LOG(logl, "le logarithme neperien");
}
void *log_10(void *num, unsigned long int internal_buflen, char *format, unsigned long int virgule,int approximation){
	LOG(log10l, "le logarithme en base 10");
}
void *exponentiel(void *num,unsigned long int internal_buflen, char *format, unsigned long int virgule, int approximation){
	char exp[66];
	memset(exp, 0, 66);
	sprintf(exp, "%.62Lf\n", expl(1));
	return puissance(exp, num, internal_buflen, format, virgule, approximation);
}
#endif
