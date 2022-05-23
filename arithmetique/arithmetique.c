#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "../operation/operation.h"

const unsigned long int BUFFER = 56;

#define INTERNAL_BUFLEN \
	if(buffer[internal_buflen-1] != 0){\
		fprintf(stderr, "Tampon interne trop petit\n");\
		exit(0);\
	}
	
#define TRIGO(fn) \
	char *arg_ = NULL, *t_arg, pi[512], npi[512], *pi_ = NULL, *temp, *t = NULL, *mul, buffer[internal_buflen], *pbuf, *form;\
	long double val, format_;\
	form = format+2;\
	format_ = strtold(form,NULL);\
		/*fprintf(stderr, "Echec :%.0Lf < %lu (internal_buflen)\n", format_, internal_buflen);*/\
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
		/*free(arg_);*/\
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
		fprintf(stderr, "Tampon interne trop petit\n");\
		exit(0);\
	}\
	free(t);\
	t = multiplication(buffer,"1");\
	if(o_deg){\
		arg_ = multiplication(t, "180");\
		free(t);\
		t = division(arg_, pi, virgule, approximation);\
		free(arg_);\
	}/*else free(t);*/\
	/*pbuf = multiplication(buffer,"1");*/\
	pbuf = t;\
	//free(t);
	//return pbuf;

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
char *cosinus(char *arg, char *format,unsigned long internal_buflen, int i_deg, int o_deg, unsigned long int virgule, int approximation){
	TRIGO(cosl);
	return pbuf;
}

char *sinus(char *arg, char *format,unsigned long internal_buflen,int i_deg, int o_deg, unsigned long int virgule, int approximation){
	TRIGO(sinl);
	return pbuf;
}
char *tangente(char *arg, char *format,unsigned long internal_buflen,int i_deg, int o_deg, unsigned long int virgule, int approximation){
	TRIGO(tanl);
	return pbuf;
}
#define POWER(fn) \
	if(equal(num1, "0") < 0){\
		free(n1);\
		free(n2);\
		if((n1 = calloc(5,sizeof(char))) == NULL){\
			perror("calloc()");\
			exit(EXIT_FAILURE);\
		}\
		strcpy(n1, "-nan");\
		return n1;\
	}\
	if((v_ = calloc(strlen(n2)+2,sizeof(char))) == NULL){\
		perror("calloc()");\
		exit(EXIT_FAILURE);\
	}\
	strcpy(v_+1, v);\
	*v_ = '0';\
	*v = 0;\
	pseudo = buffer;\
	do{\
		pseudo_ = strtold(n1, NULL);\
		sprintf(buffer, format, pseudo_);\
		if((eq = equal(n1, pseudo)) > 0){\
			n1_ = racine_carree(n1, virgule,approximation);\
			free(n1);\
			n1 = n1_;\
			i_ = addition(i, "1");\
			free(i);\
			i = i_;\
		}\
	}while(eq > 0);\
	pdot_ = strchr(format,'.');\
	if((dot_ = calloc(strlen(format), sizeof(char))) == NULL){\
		perror("calloc()");\
		exit(EXIT_FAILURE);\
	}\
	strcpy(dot_, pdot_+1);\
	pdot_ = strchr(dot_, 'L');\
	/**dot_ = '0';*/\
	*pdot_ = 0;\
	if((p = strchr(v_,'.')) != NULL && strlen(p+1) > (unsigned long int)atol(dot_)){\
		fprintf(stderr,"Warning `%s`:\n\tTrop de chiffre apres la virgule.\n\tUtilisation de la valeur: ", v_);\
		fprintf(stderr, format, strtold(v_, NULL)); \
		fprintf(stderr,"\n");\
	}\
	free(dot_);\
	pseudo_ = powl(strtold(n1_, NULL), strtold(v_, NULL));\
	sprintf(buffer, format, pseudo_);\
	if(buffer[internal_buflen-1] != 0){\
		fprintf(stderr, "buffer interne trop court\n");\
		exit(EXIT_FAILURE);\
	}\
	if(equal(i,"0") != 0){\
		while(equal(i,"0") != 0){\
			if(set == 0){ \
				pseudo = multiplication(buffer, buffer);\
				set = 1;\
			}else{\
				p = pseudo;\
				pseudo = multiplication(p, buffer);\
				free(p);\
			}\
			i_ = soustraction(i, "1");\
			free(i);\
			i = i_;\
		}\
	}else pseudo = multiplication(buffer,"1");\
	free(n1_);\
	while(equal(n2, "0") != 0){\
		n1_ = multiplication(pseudo, num1);\
		free(pseudo);\
		pseudo = n1_;\
		n1_ = NULL;\
		n2_ = fn(n2, "1");\
		free(n2);\
		n2 = n2_;\
	}\
	n1_ = pseudo;\
	pseudo = NULL;\
	free(v_);\
	free(i);\
	i = NULL;\
	if(pseudo)\
		free(pseudo);


#define POW(fn)\
	n1_ = multiplication(n1,num1);\
	free(n1);\
	n1 = n1_;\
	n2_ = fn(n2, "1");\
	free(n2);\
	n2 = n2_;

void *puissance(void *num1, void *num2, unsigned long int internal_buflen, char *format, unsigned long int virgule, int approximation){
	char *n1 = multiplication(num1,"1"), *n2 = multiplication(num2,"1"),
		buffer[internal_buflen], *v, 
		*n1_ = n1, *n2_ = n2,
		*i = multiplication("1","0");
	char *i_, *v_, *pseudo = NULL, *p, *dot_, *pdot_;
	long double pseudo_;
	int eq, set = 0;
	memset(buffer, 0, internal_buflen);
	//unsigned long int i_ = 0;
	if(equal(num2, "0") == 0){
		free(n1);
		free(n2);
		n1 = multiplication("1","1");
		return n1;
	}
	if((v = strchr(n2, '.')) != NULL){
		if(*((char *)num2) != '-'){
				POWER(soustraction);
		}else{
				POWER(addition);
				free(n2_);
				n2_ = n1_;
				n1_ = division("1", n2_, virgule, approximation);
		}
	}else{
		if(equal(n2, "0") > 0){
			while(equal(n2,"1") != 0){
				POW(soustraction);
			}
		}else{
			while(equal(n2,"-1") != 0){
				POW(addition);
			}
			n1_ = division("1", n1, virgule, approximation);
			free(n1);
		}
	}
	if(i != NULL)
		free(i);
	if(n2_)
		free(n2_);
	return n1_;
}
#define LOG(fn)\
	char *n = multiplication(num, "1"), *n_, buffer[internal_buflen];\
	unsigned long long int i;\
	long double result;\
	memset(buffer, 0, internal_buflen);\
	for(n = n, i = 1;equal(n, "1000") > 0;n_ = racine_carree(n, virgule, approximation), free(n), n = n_, i *= 2);;\
	result = i*fn(strtold(n, NULL));\
	snprintf(buffer, internal_buflen, format, result);\
	if(buffer[internal_buflen-1] != 0){\
		fprintf(stderr,"Tampon (internal_buflen) trop petit\n");\
		exit(EXIT_FAILURE);\
	}\
	n_ = multiplication(buffer, "1");\
	free(n);\
	return n_;

void *log_n(void *num, unsigned long int internal_buflen, char *format, unsigned long int virgule,int approximation){
	LOG(logl);
}
void *log_10(void *num, unsigned long int internal_buflen, char *format, unsigned long int virgule,int approximation){
	LOG(log10l);
}
