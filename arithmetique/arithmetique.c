#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>
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
	error_set(SET, 0);\
	form = format+2;\
	format_ = strtold(form,NULL);\
	if(format_ > internal_buflen){\
		/*fprintf(stderr, "Echec :%.0Lf > %lu (internal_buflen)\n", format_, internal_buflen);*/\
		error_set(SET, 1);\
		return NULL;\
	}\
	arg_ = multiplication(arg,"1");\
	memset(buffer, 0, internal_buflen);\
	sprintf(pi,"%.54Lf", 2*atanl(1));\
	sprintf(npi,"-%.54Lf", 2*atanl(1));\
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
		/*fprintf(stderr, "Tampon interne trop petit (internal_buflen)\n");*/\
		error_set(SET, 1);\
		free(t);\
		return NULL;\
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

void *puissance(void *num1, void *num2, unsigned long int internal_buflen, char *format, unsigned long int virgule){
	char *n1 = multiplication(num1,"1"),
		*n2 = multiplication(num2,"1"),
		buffer[internal_buflen], *v, 
		*n1_ = n1, *n_,
		*i = multiplication("1",num2),*j = NULL, *j_, *j__, *mod = NULL;
	char *i_, *v_ = NULL, *pseudo = NULL, *pseudo__, *p, *dot_, *pdot_, *tst_, tst__[68], *_format_, f[20];
	unsigned long int l;
	long double pseudo_, pseudo_tst, tst;
	int eq, neg = 0;
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
			/*if((n1 = calloc(5,sizeof(char))) == NULL){
				perror("calloc()");
				exit(EXIT_FAILURE);
			}
			strcpy(n1, "-nan");*/
			error_set(SET, 2);
			return NULL;
		}
		if((v_ = calloc(strlen(n2)+2,sizeof(char))) == NULL){
			perror("calloc()");
			exit(EXIT_FAILURE);
		}
		strcpy(v_+1, v);
		*v_ = '0';
		*v = 0;
		pseudo = buffer;
		pseudo = n1;
		//printf("%s\n", n1);
		//exit(0);
		do{
			pseudo_tst = strtold(n1, NULL) * strtold(n1, NULL);
			pseudo_ = strtold(n1, NULL);
			memset(buffer, 0, internal_buflen);
			snprintf(buffer, internal_buflen, format, pseudo_tst);
			if(buffer[internal_buflen-1] != 0){
				error_set(1, 1);
				return NULL;
			}
			//fprintf(stderr, "===>%s\n", n1);
			//exit(0);
			if((eq = equal(n1, pseudo)) > 0){
				n1_ = racine_carree(n1, virgule);
				//if(n1_ == NULL)
					//exit(0);
				free(n1);
				n1 = n1_;
				i_ = addition(i, "1");
				free(i);
				i = i_;
				if(j == NULL){
					j = addition("0", "1");
				}else{
					j_ = addition(j,"1");
					free(j);
					j = j_;
				}
			}
		}while(eq > 0);
		if(virgule > 0){
			pdot_ = strchr(format,'.');
			if((dot_ = calloc(strlen(format), sizeof(char))) == NULL){
				perror("calloc()");
				exit(EXIT_FAILURE);
			}
			strcpy(dot_, pdot_+1);
			pdot_ = strchr(dot_, 'L');
			*pdot_ = 0;
			if((p = strchr(v_, '.')) != NULL){
				p++;
				if((tst_ = calloc(strlen(p)+3, sizeof(char))) == NULL){
					perror("calloc");
					exit(EXIT_FAILURE);
				}
				strcpy(tst_+2,p);
				l = strlen(p)+2;
				if((_format_ = calloc(l+7, sizeof(char))) == NULL){
					perror("calloc()");
					exit(EXIT_FAILURE);
				}
				memset(f , 0, 20);
				sprintf(f,"%lu", l);
				strcpy(_format_, "%.");
				strcat(_format_, f);
				strcat(_format_, "Lf");
				memcpy(tst_, "0.", 2);
				tst = strtold(tst_, NULL);
				memset(tst__, 0, 68);
				snprintf(tst__,68, _format_, tst);
				if(tst__[67] != 0){
					tst__[67] = 0;
					fprintf(stderr, "Warning %s: Nombre tronque (%s)\n", tst_, tst__);
				}
				//sprintf(tst__,_format_, tst);
				*pdot_ = '.';
				if(equal(tst__, tst_) != 0){
					fprintf(stderr,"Warning `%s`:\n\tTrop de chiffre apres la virgule.\n\tUtilisation de la valeur: ", v_);
					fprintf(stderr, format, strtold(v_, NULL));
					fprintf(stderr,"\n");
				}
				free(_format_);
				free(tst_);
				free(dot_);
			}
			/*if(strcmp((n__ = modulo(n2, "0.5", 0)), "0") == 0){
				free(n__);
				for(n__ = multiplication(n1, "1"),
					count = count;
					strchr(n__, '.');
					n___ = multiplication(n__, "100"),
					free(n__),
					n__ = n___,
					count++
				);;
				if(equal(n1, n__) == 0){
					free(n__);
					n__ = n1_;
				}
				n__ = n1_;
				//free(n1_);
			}else{
				free(n__);
				n__ = n1_;
			}*/
			//printf("%s:%s\n",n1_, v_);
			pseudo_ = powl(strtold(n1_, NULL), strtold(v_, NULL));
			sprintf(buffer,format, pseudo_);
			if(buffer[internal_buflen-1] != 0){
				/*fprintf(stderr, "Tampon interne trop petit (internal_buflen)\n");*/
				error_set(SET, 1);
				//free(n__);
				//free(n1);
				free(n2);
				free(i);
				free(v_);
				return NULL;
			//exit(EXIT_FAILURE);
			}
			/*if(count > 0){
				free(n__);
				for(count = count, n__ = multiplication(buffer, "1"); count > 0; n___ = division(n__, "10", virgule+1, approximation), free(n__), n__ = n___, count--);;
				memcpy(buffer, n__, internal_buflen-1);
				free(n__);
			}*/
			//free(n__);
			free(v_);
			v_ = NULL;
		}
		if(v_)
			free(v_);
	}
	if(j){
		for(j_ = addition("0","1"); equal(j, j_) != 0;j__ = addition(j_, "1"), free(j_), j_ = j__){
			n_ = multiplication(n1, n1);
			free(n1);
			n1 = n_;
		}
		free(j_);
	}
	if(i)
		free(i);
	i = multiplication(n2, "1");
	if(equal(n2,"0") == 0){
		if(j == NULL)
			pseudo = multiplication("1", buffer);
		else{
			pseudo = multiplication(n1, "1");
			free(j);
		}
		if(neg){
			free(n1);
			n1 = division("1", pseudo, virgule, 0);
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
	free(j);
	pseudo = addition("0", "1");
	while(equal(i,"1") != 0){
		mod = modulo(i, "2", 0);
		if(equal(mod, "0") != 0){
			pseudo__ = multiplication(pseudo, n1);
			i_ = soustraction(i, "1");
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
	free(n1);
	n1 = n1_;
	if(pseudo && strlen(pseudo) > 0 && strlen(buffer) > 0){
		n1_ = multiplication(buffer, n1);
		free(n1);
		n1 = n1_;
	}
	if(neg == 1){
		n1_ = division("1", n1, virgule, 0);
		free(n1);
		n1 = n1_;
	}
	if(i)
		free(i);
	if(pseudo)
		free(pseudo);
	free(n2);
	return n1;
}
#define LOG(fn, msg)\
	char *n = multiplication(num, "1"), *n_, buffer[internal_buflen], *i = multiplication("1", "1"),\
		*pi = i, *result, *dot_ = NULL, *pdot_, *p, *tst_, tst__[68], *_format_, f[20], comp[21];\
	unsigned long int l;\
	long double tst;\
	error_set(SET, 0);\
	memset(buffer,0, internal_buflen);\
	sprintf(comp,"%lu", (unsigned long int)~0);\
	if(equal(n, "0") <= 0){\
		if(equal(n, "0") < 0){\
			free(n);\
			free(i);\
			/*fprintf(stderr, "Erreur: %s: %s < 0 [-nan]\n", msg, (char *)num);*/\
			error_set(SET, 2);\
			return NULL;\
		}else{\
			free(n);\
			free(i);\
			/*if((n_ = malloc(5*sizeof(char))) == NULL){\
				perror("malloc()");\
				exit(EXIT_FAILURE);\
			}\
			strcpy(n_, "-inf");*/\
			error_set(SET, 3);\
			return NULL;\
		}\
	}\
	if(virgule > 0){\
		pdot_ = strchr(format,'.');\
		if((dot_ = calloc(strlen(format), sizeof(char))) == NULL){\
			perror("calloc()");\
			exit(EXIT_FAILURE);\
		}\
		strcpy(dot_, pdot_+1);\
		pdot_ = strchr(dot_, 'L');\
		*pdot_ = 0;\
		if((p = strchr(n, '.')) != NULL){\
			p++;\
			if((tst_ = calloc(strlen(p)+3, sizeof(char))) == NULL){\
				perror("calloc");\
				exit(EXIT_FAILURE);\
			}\
			strcpy(tst_+2,p);\
			l = strlen(p)+2;\
			if((_format_ = calloc(l+7, sizeof(char))) == NULL){\
				perror("calloc()");\
				exit(EXIT_FAILURE);\
			}\
			memset(f , 0, 20);\
			sprintf(f,"%lu", l);\
			strcpy(_format_, "%.");\
			strcat(_format_, f);\
			strcat(_format_, "Lf");\
			memcpy(tst_, "0.", 2);\
			tst = strtold(tst_, NULL);\
			memset(tst__, 0, 68);\
			snprintf(tst__,68, _format_, tst);\
			if(tst__[67] != 0){\
				tst__[67] = 0;\
				fprintf(stderr, "Warning %s: Nombre tronque (%s)\n", tst_, tst__);\
			}\
			*pdot_ = '.';\
			if(equal(tst__, tst_) != 0){\
				fprintf(stderr,"Warning `%s`:\n\tTrop de chiffre apres la virgule.\n\tUtilisation de la valeur: ", tst_);\
				fprintf(stderr, format, strtold(tst_, NULL));\
				fprintf(stderr,"\n");\
				p = strchr(n, '.');\
				*p = 0;\
				sprintf(f,format,strtold(tst_, NULL));\
				p = addition(n, f);\
				free(n);\
				n = p;\
			}\
			free(_format_);\
			free(tst_);\
			free(dot_);\
			dot_ = NULL;\
		}\
	}\
	memset(buffer, 0, internal_buflen);\
	if(equal(n,comp) > 0){\
		for(n = n, i = i;n && equal(n, comp) > 0;n_ = racine_carree(n, virgule+1), free(n), n = n_, pi = multiplication(i, "2"),free(i), i = pi);;\
		if(n != NULL)\
			snprintf(buffer, internal_buflen,format, fn(strtold(n, NULL)));\
	}else{\
		snprintf(buffer,internal_buflen,format, fn(strtold(n,NULL)));\
	}\
	if(n == NULL || buffer[internal_buflen -1] != 0){\
		/*fprintf(stderr,"Tampon interne trop petit (internal_buflen)\n");*/\
		error_set(SET, 1);\
		if(n)\
			free(n);\
		free(i);\
		if(dot_)\
			free(dot_);\
		return NULL;\
	}\
	result = multiplication(i, buffer);\
	free(i);\
	free(n);\
	if(dot_)\
		free(dot_);\
	n = division(result, "1", virgule+1, 0);\
	free(result);\
	return n;

void *log_n(void *num, unsigned long int internal_buflen, char *format, unsigned long int virgule){
	LOG(logl, "le logarithme neperien");
	return result;
}
void *log_10(void *num, unsigned long int internal_buflen, char *format, unsigned long int virgule){
	LOG(log10l, "le logarithme en base 10");
}
void *exponentiel(void *num,unsigned long int internal_buflen, char *format, unsigned long int virgule){
	char exp[66];
	memset(exp, 0, 66);
	sprintf(exp, "%.62Lf\n", expl(1));
	return puissance(exp, num, internal_buflen, format, virgule);
}

void *racine_carree(void *num1, unsigned long int virgule){
	char *n = multiplication(num1, "1"), *n_, trad[21], *trad_, *r, *r_, *r__ ,*tst, *rep = NULL;
	unsigned long int s, count = 0;
	error_set(SET, 0);
	if(equal(num1, "0") < 0){
		free(n);
		error_set(SET, 4);
		return NULL;
	}
	while(strchr(n, '.')){
		n_ = multiplication(n, "100");
		free(n);
		n = n_;
		count++;
	}
	s = strlen(n);
	sprintf(trad, "%lu", s);
	trad_ = multiplication(trad, "100");
	r = division(n, trad_, virgule+1, 1);
	r_ = addition(r, trad_);
	free(r);
	r = r_;
	free(trad_);
	r_ = multiplication(r, "0.5");
	free(r);
	r = r_;
	tst = multiplication(r, r);
	while(equal(tst, num1) > 0){
		r_ = division(n, r, virgule+1, 0);
		r__ = addition(r, r_);
		free(r);
		r = r__;
		free(r_);
		r_ = multiplication(r, "0.5");
		free(tst);
		free(r);
		r = r_;
		r_ = division(r, "1", virgule, 0);
		free(r);
		r = r_;
		tst = multiplication(r,r);
		if(rep == NULL){
			rep = multiplication(r, "1");
		}else{
			if(equal(rep, r) == 0){
				for(count = count; count > 0; count--){
					r_ = division(r, "10", virgule, 0);
					free(r);
					r = r_;
				}
				break;
			}else{
				free(rep);
				rep = multiplication(r, "1");
			}
		}
	}
	free(n);
	free(tst);
	if(rep)
		free(rep);
	return r;
}
int error_set(int op, int val){
	static int err = 0;
	if(op == SET)
		err = val;
	else return err;
	return 0;
}
int m_errno_(void){
	return error_set(GET,0);
}
#endif
