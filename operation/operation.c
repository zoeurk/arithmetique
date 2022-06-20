#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "operation.h"

#define DOT_INIT \
	dot1 = strchr(n1, '.'); \
	dot2 = strchr(n2, '.'); \
	dot1_len = (dot1) ? strlen(dot1) -1: 0;\
	dot2_len = (dot2) ? strlen(dot2) -1: 0;

#define DOT_SEARCH(dot1,dot2, dot1_len, dot2_len, val1, val2)\
	DOT_INIT; \
	val1 = (dot1) ? dot1 - 1: n1 + strlen(n1)-1;\
	val2 = (dot2) ? dot2 - 1: n2 + strlen(n2)-1;\
	if(dot1)dot1++;\
	if(dot2)dot2++;

#define NEG \
	for(n1 = n1; *n1 == '-' || *n1 == '+'; n1++) \
		if(*n1 == '-')neg1 = !neg1; \
	for(n2 = n2; *n2 == '-' || *n2 == '+'; n2++) \
		if(*n2 == '-')neg2 = !neg2;

#define NEG_TEST \
	if(neg1 != neg2)\
		neg = 1;

#define ZERO \
while(*n1 == '0' && *(n1+1) != '.' && *(n1+1) != 0) \
	n1++;\
while(*n2 == '0' && *(n2+1) != '.' && *(n2+1) != 0) \
	n2++;

#define VALEUR_NEGATIVE(buffer, pbuf, ii) \
	if(strlen(buffer) +1 == (unsigned long int)~0 || strlen(buffer)+2 == (unsigned long int)~0){\
		fprintf(stderr,"Taille de la chaine trop longue pour le systeme.\n");\
		free(buffer);\
		exit(EXIT_FAILURE);\
	}\
	if((buffer = realloc(buffer,strlen(buffer)+2)) == NULL){ \
		perror("realloc()"); \
		exit(EXIT_FAILURE); \
	} \
	for(pbuf = &buffer[strlen(buffer)+1], ii = strlen(buffer);pbuf != buffer; pbuf--, ii--){ \
		*pbuf = buffer[ii]; \
	} \
	*buffer = '-';

int strtype(void *str){
	unsigned char *_str_ = (unsigned char *)str, dot = 0;
	unsigned short int i;
	if(_str_[0] == 0)
		return 1;
	if(_str_[0] == '.')
		dot = 2;
	if(*_str_ == '-' || *_str_ == '+')
		_str_++;
	for(_str_ = _str_;((*_str_ > 47 && *_str_ < 58) || (*_str_ == '.' && dot < 2)) && *_str_ != 0; _str_++){
		if(*_str_ == '.' && dot == 0)
			dot = 1;
		else
			if(dot == 1 && *_str_ == '.')
				dot = 2;
	}
	if(*_str_ == 0 && dot < 2)
		return 0;
	for(_str_ = _str_;i = (unsigned short int) *_str_, ((i > 31 && i< 127) || (i > 128 && i < 256))  && *_str_ != 0; _str_++);;
	if(*_str_ == 0)
		return 1;
	return 2;
}

void *allocation(void **ptr, unsigned long int members, unsigned long int size){
	if(members == (unsigned long int)~0){
		fprintf(stderr,"Taille de la chaine trop longue pour le systeme.\n");
		exit(EXIT_FAILURE);
	}
	if((*ptr = calloc(members, size+1)) == NULL){
		perror("calloc()");
		exit(EXIT_FAILURE);
	}
	return *ptr;
}
void *reallocation(void **ptr, unsigned long int size){
	char *buf;
	unsigned long int i = 0, sz = 0, sz_;
	for(sz = strlen(*ptr)+1, sz_ = 0;sz != (unsigned long int)~0 && sz_ != size; sz++, sz_++);;
	if(sz == (unsigned long int)~0){
		fprintf(stderr,"Taille de la chaine trop longue pour le systeme.\n");
		free(*ptr);
		exit(EXIT_FAILURE);
	}
	if((*ptr = realloc(*ptr, size)) == NULL){
		perror("realloc()");
		exit(EXIT_FAILURE);
	}
	buf = (*ptr)+strlen(*ptr);
	sz = size - strlen(*ptr);
	for(i = 0; i < sz; i++){
		buf[i] = 0;
	}
	return (*ptr)+strlen(*ptr);
}
int equal(void *num1, void *num2){
	char *n1 = num1, *n2 = num2,
		*dot1, *dot2, /**pdot, dot[2] = {0, 0},*/
		*val1, *val2,
		v1[2] = { 0, 0 }, v2[2] = { 0, 0 },
		neg1 = 0, neg2 = 0;
	unsigned long int dot1_len = 0, dot2_len = 0,
				val1_len = 0, val2_len = 0, ii;
	/*printf("******\n");
	exit(0);*/
	for(val1 = n1; *val1 == '-' || *val1 == '+'; val1++)
		if(*val1 == '-')
			neg1 = !neg1;
	for(val2 = n2; *val2 == '-' || *val2 == '+'; val2++)
		if(*val2 == '-')
			neg2 = !neg2;
	DOT_INIT;
	if(dot1)dot1++;
	if(dot2)dot2++;
	for(n1 = n1;*(n1+1) != '.' && *(n1+1) != 0 && (*n1 == '0' || *n1 == '+' || *n1 == '-'); n1++);;
	for(n2 = n2;*(n2+1) != '.' && *(n2+1) != 0 && (*n2 == '0' || *n2 == '+' || *n2 == '-'); n2++);;
	val1 = n1;
	val2 = n2;
	val1_len = strlen(n1);
	val2_len = strlen(n2);
	val1_len = (dot1_len) ? val1_len - dot1_len -1: val1_len; 
	val2_len = (dot2_len) ? val2_len - dot2_len -1: val2_len;
	if(neg1 && !neg2){
		/*if((pdot = strchr(val1, '.')) != NULL)
			for(pdot = val1 +val1_len,
				dot[0] = 0;
				*pdot != 0 &&
				dot[0] == 0;
				pdot++
			)if(*pdot != '0')dot[0] = *pdot;
		if(strncmp(val1,val2, 1 ) == 0 && dot[0] == 0)
			return 0;*/
		return -1;
	}
	if(neg2 && !neg1){
		/*if((pdot = strchr(val2, '.')) != NULL)
			for(pdot = val2 +val2_len,
				dot[0] = 0;
				*pdot != 0 &&
				dot[0] == 0;
				pdot++
			){
				if(*pdot != '0')dot[0] = *pdot;
			}
		if(strncmp(val1,val2, 1 ) == 0 && dot[0] == 0){
			return 0;
		}*/
		return 1;
	}
	if(strcmp(val1,"0") == 0 && strcmp(val2,"0") == 0)
		return 0;
	if(strcmp(val1,val2)==0)
		return 0;
	while(*val1 == '0' && *(val1+1) != '.'){
		val1_len--;
		val1++;
	}
	while(*val2 == '0' && *(val2+1) != '.'){
		val2_len--;
		val2++;
	}
	if(val1[strlen(val1)-1] == '.')
		val1_len--;
	if(val2[strlen(val2)-1] == '.')
		val2_len--;
	if(*val1 == '0'){
		while(*val1 == '0' && *(val1+1) != 0){
			val1_len--;
			val1++;
		}
	}
	if(*val2 == '0'){
		while(*val2 == '0' && *(val2+1) != 0){
			val2_len--;
			val2++;
		}
	}
	if(val1_len > val2_len){
		if(neg1 && neg2){
			return -1;
		}
		return 1;
	}else	if(val1_len < val2_len){
			if(neg1 && neg2)
				return 1;
			else	return -1;
		}
	for(val1 = val1, val2 = val2, ii = 0;ii < val1_len; ii++, val1++, val2++){
		v1[0] = *val1;
		v2[0] = *val2;
		v1[0] = atoi(v1);
		v2[0] = atoi(v2);
		if(v1[0] > v2[0]){
			if(neg1 && neg2)
				return -1;
			return 1;
		}else	if(v1[0] < v2[0]){
				if(neg1 && neg2)
					return 1;
				return -1;
			}
	}
	if(dot1 && dot2){
		for(dot1 = dot1, dot2 = dot2;
			*dot1 != 0 && *dot2 !=0;
			dot1++, dot2++
		){
			if(*dot1 > *dot2){
				if(neg1 && neg2)
					return -1;
				return 1;
			}
			if(*dot1 < *dot2){
				if(neg1 && neg2)
					return 1;
				return -1;
			}
		}
	}
	if((!dot1 || *dot1 == 0 )&& dot2 && *dot2 != 0){
		for(dot2 = dot2; *dot2 != 0; dot2++)
			if(*dot2 > '0'){
				if(*n1 == '-' && *n2 == '-')
					return 1;
				return -1;
			}
	}
	if((dot1 && *dot1 != 0) && (!dot2 || *dot2 == 0)){
		for(dot1 = dot1; *dot1 != 0; dot1++)
			if(*dot1 > '0'){
				if(*n1 == '-' && *n2 == '-')
					return -1;
				return 1;
			}
	}
	return 0;
}
void *addition(void *num1, void *num2){
	char *n1 = num1, *n2 = num2,
		*dot1, *dot2,
		*val1, *val2,
		v1[2] = { 0, 0 }, v2[2] = { 0, 0 },
		*buffer, *pbuf, *ret, *pret,
		*ptr1 = NULL, *ptr2= NULL,
		result = 0, retenue = 0, neg = 0, neg1 = 0, neg2 = 0, set = 0;
	unsigned long int dot1_len = 0, dot2_len = 0,
				val1_len = 0, val2_len = 0,
				buflen = 0, z = 1;
	unsigned long int ii = 0, ij = 0;
	NEG;
	if(neg1 || neg2){
		if(neg1 && neg2)
			neg = 1;
		else
			if(neg1 && !neg2)
				return soustraction(n2,n1);
			else
				if(!neg1 && neg2)
					return soustraction(n1, n2);
	}
	ZERO;
	if(strlen(n1) == 0 && strlen(n2) == 0){
		ret = allocation((void **)&ret, 2, sizeof(char));
		*ret = '0';
		return ret;
	}
	DOT_SEARCH(dot1, dot2, dot1_len, dot2_len, val1, val2);
	val1_len = strlen(n1);
	val2_len = strlen(n2);
	val1_len = (dot1_len) ? val1_len - dot1_len -1: val1_len; 
	val2_len = (dot2_len) ? val2_len - dot2_len -1: val2_len;
	pbuf = allocation((void **)&buffer,BUFFER, sizeof(char));
	if(dot1 != NULL && dot1_len == 0)
		val1_len--;
	if(dot2 != NULL && dot2_len == 0)
		val2_len--;
	if(dot1 && dot2){
		if(dot1_len > dot2_len){
			ii = dot1_len;
			ij = dot2_len;
			ptr1 = &dot1[dot1_len-1];
			ptr2 = &dot2[dot2_len-1];
		}else{
			ii = dot2_len;
			ij = dot1_len;
			ptr1 = &dot2[dot2_len-1];
			ptr2 = &dot1[dot1_len-1];
		}
		for(ii = ii, ptr1 = ptr1; ii != ij && ii > 0; ii--, ptr1--){
			v1[0] = *ptr1;
			if(buflen + 1 >= BUFFER){
				z++;
				buflen = 0;
				pbuf = reallocation((void **)&buffer, BUFFER*z);
			}
			sprintf(pbuf,"%c", v1[0]);
			pbuf++;
			buflen++;
		}
		for(ii = ii, ptr1 = ptr1, ptr2 = ptr2; ii > 0; ii--, ptr1--, ptr2--){
			v1[0] = *ptr1;
			v2[0] = *ptr2;
			v1[0] = atoi(v1);
			v2[0] = atoi(v2);
			result = v1[0] + v2[0] + retenue;
			if(result >= 10){
				retenue = 1;
				result -= 10;
			}else	retenue = 0;
			if(buflen + 1 >= BUFFER){
				z++;
				buflen = 0;
				pbuf = reallocation((void **)&buffer, BUFFER*z);
			}
			sprintf(pbuf, "%i", result);
			pbuf++;
			buflen++;
		}
	}else{
		if(dot1 && !dot2){
			ii = dot1_len;
			ptr1 = &dot1[dot1_len-1];
		}else{
			if(!dot1 && dot2){
				ii = dot2_len;
				ptr1 = &dot2[dot2_len-1];
			}
		}
		for(ii = ii, ptr1 = ptr1; ii > 0;ii--, ptr1--){
			if(buflen + 1 >= BUFFER){
				z++;
				buflen = 0;
				pbuf = reallocation((void **)&buffer, BUFFER*z);
			}
			sprintf(pbuf,"%c", *ptr1);
			pbuf++;
			buflen++;
		}
	}
	if(dot1_len || dot2_len){
		if(buflen + 1 >= BUFFER){
			z++;
			buflen = 0;
			pbuf = reallocation((void **)&buffer, BUFFER*z);
		}
		sprintf(pbuf,".");
		pbuf++;
		buflen++;
	}
	for(ptr1 = val1, ptr2 = val2, ii = val1_len, ij = val2_len;
		ii > 0 && ij > 0; ii--, ij--, ptr1--, ptr2--){
		v1[0] = *ptr1;
		v2[0] = *ptr2;
		v1[0] = atoi(v1);
		v2[0] = atoi(v2);
		result = v1[0] + v2[0] + retenue;
		if(result >= 10){
			retenue = 1;
			result -= 10;
		}else	retenue = 0;
		if(buflen + 1 >= BUFFER){
			z++;
			buflen = 0;
			pbuf = reallocation((void **)&buffer, BUFFER*z);
		}
		sprintf(pbuf,"%i", result);
		pbuf++;
		buflen++;
	}
	if(ii || ij){
		if(ij){
			ii = ij;
			ptr1 = ptr2;
		}
		for(ii = ii, ptr1 = ptr1; ii > 0; ii--,ptr1--){
			v1[0] = *ptr1;
			v1[0] = atoi(v1);
			result = v1[0] + retenue;
			if(result >= 10){
				retenue = 1;
				result -= 10;
			}else	retenue = 0;
			if(buflen + 1 >= BUFFER){
				z++;
				buflen++;
				pbuf = reallocation((void **)&buffer, BUFFER*z);
			}
			sprintf(pbuf,"%i", result);
			pbuf++;
			buflen++;
		}
	}
	if(retenue){
		if(buflen + 1 >= BUFFER){
			z++;
			buflen = 0;
			pbuf = reallocation((void **)&buffer, BUFFER*z);
		}
		sprintf(pbuf,"%i", retenue);
		pbuf++;
		buflen++;
	}
	pbuf = buffer;
	ii = 0;
	while(*pbuf == '0'){
		if(*(pbuf+1) == 0)
			break;
		set = 1;
		pbuf++;
	}
	ij = strlen(buffer);
	pret = allocation((void **)&ret, ij, sizeof(char));
	for(ii = ij-1, pret = pret; ii != ~(unsigned long int)0; ii--, pret++)
		*pret = buffer[ii];
	free(buffer);
	buffer = ret;
	set = 0;
	if(dot1 || dot2){
		while(*buffer == '0'&& *(buffer+1) == '0'){
			*buffer = 0;
			buffer++;
			set = 1;
		}
		if(set){
			pbuf = allocation((void **)&pbuf, strlen(buffer), sizeof(char));
			strcpy(pbuf, buffer);
			memset(ret, 0, strlen(ret));
			strcpy(ret, pbuf);
			free(pbuf);
		}
	}
	pret = &ret[strlen(ret)-1];
	if(neg){
		if((ret = realloc(ret,strlen(ret)+2)) == NULL){
			perror("realloc()");
			exit(EXIT_FAILURE);
		}
		for(pret = &ret[strlen(ret)+1], ii = strlen(ret);pret != ret; pret--, ii--){
			*pret = ret[ii];
		}
		*ret = '-';
	}
	if((n1 = strchr(ret,'.')) != NULL)
		for(n2 = &ret[strlen(ret) - 1];(n2+1) != n1 && (*n2 == '0' || *n2 == '.'); n2--)
			*n2 = 0;
	return ret;
}
void *soustraction(void *num1, void *num2){
	char *n1 = num1, *n2 = num2,
		*dot1, *dot2,
		*val1, *val2,
		v1[2] = { 0, 0 }, v2[2] = { 0, 0 },
		*buffer, *pbuf, *ret, *pret,
		*ptr1 = NULL, *ptr2= NULL,
		result = 0, retenue = 0, neg = 0, neg1 = 0, neg2 = 0;
	unsigned long int dot1_len = 0, dot2_len = 0,
				val1_len = 0, val2_len = 0,
				buflen = 0, z = 1;
	unsigned long int ii_ = 0, ij_ =0;
	NEG;
	if(neg1 || neg2){
		if(neg1 && neg2){
			return soustraction(n2, n1);
		}else{
			if(neg1 && !neg2){
				buffer = addition(n2, n1);
				if((buffer = realloc(buffer, strlen(buffer)+2)) == NULL){
					perror("realloc()");
					exit(0);
				}
				for(pbuf = &buffer[strlen(buffer)+1], ii_ = strlen(buffer); buffer != pbuf; pbuf--, ii_--)
					*pbuf = buffer[ii_];
				*buffer = '-';
				return buffer;
			}else{
				if(!neg1 && neg2)
					return addition(n1, n2);
			}
		}
	}
	switch(equal(num1,num2)){
		case 0:
			pbuf = allocation((void **)&buffer, 2, sizeof(char));
			*pbuf = '0';
			return pbuf;
		case -1:
			neg = 1;
			n1 = num2;
			n2 = num1;
			goto next;
		case 1:
			n1 = num1;
			n2 = num2;
			next:
			ZERO;
			DOT_SEARCH(dot1, dot2, dot1_len, dot2_len, val1, val2);
			val1_len = strlen(n1);
			val2_len = strlen(n2);
			val1_len = (dot1_len) ? val1_len - dot1_len: val1_len; 
			val2_len = (dot2_len) ? val2_len - dot2_len: val2_len;
			break;
		default: printf("ERROR\n");
			exit(0);
	}
	pbuf = allocation((void **)&buffer,BUFFER, sizeof(char));
	if(dot1 != NULL && dot1_len == 0)
		val1_len--;
	if(dot2 != NULL && dot2_len == 0)
		val2_len--;
	if(dot1_len > dot2_len){
		for(ii_ = dot1_len; ii_ > 0 && ii_ != ~(unsigned long int)0 && ii_ != dot2_len; ii_--){
			if(buflen + 1 >= BUFFER){
				z++;
				buflen = 0;
				pbuf = reallocation((void **)&buffer,z*BUFFER);
			}
			sprintf(pbuf, "%c", dot1[ii_-1]);
			pbuf++;
			buflen++;
		}
		for(ii_ = ii_, ij_ = dot2_len; ii_ > 0 && ij_ > 0; ii_--, ij_--){
			v1[0] = dot1[ii_-1];
			v2[0] = dot2[ij_-1];
			v1[0] = atoi(v1);
			v2[0] = atoi(v2);
			if(v1[0] - retenue >= v2[0]){
				result = v1[0] - v2[0] - retenue;
				retenue = 0;
			}else{
				result = 10+v1[0] - v2[0] - retenue;
				retenue = 1;
			}
			if(buflen + 1 >= BUFFER){
				z++;
				buflen = 0;
				pbuf = reallocation((void **)&buffer,z*BUFFER);
			}
			sprintf(pbuf, "%i", result);
			pbuf++;
			buflen++;
		}
	}
	if(dot2_len > dot1_len){
		for(ii_ = dot2_len; ii_ > 0 && ii_ != dot1_len; ii_--){
			v1[0] = dot2[ii_-1];
			v1[0] = atoi(v1);
			if(v1[0] - retenue > 0){
				result = 10 - v1[0] - retenue;
				retenue = 1;
			}else{
				result = 10- (v1[0] + retenue);
				retenue = 1;
			}
			if(buflen + 1 >= BUFFER){
				z++;
				buflen = 0;
				pbuf = reallocation((void **)&buffer,z*BUFFER);
			}
			sprintf(pbuf, "%i", result);
			pbuf++;
			buflen++;
		}
		for(ii_ = ii_, ij_ = dot1_len; ii_ > 0 && ij_ > 0; ii_--, ij_--){
			v1[0] = dot1[ii_-1];
			v2[0] = dot2[ij_-1];
			v1[0] = atoi(v1);
			v2[0] = atoi(v2);
			if(v1[0] - retenue >= v2[0]){
				result = v1[0] - v2[0] - retenue;
				retenue = 0;
			}else{
				result = 10+v1[0] - v2[0] - retenue;
				retenue = 1;
			}
			if(buflen + 1 >= BUFFER){
				z++;
				buflen = 0;
				pbuf = reallocation((void **)&buffer,z*BUFFER);
			}
			sprintf(pbuf, "%i", result);
			pbuf++;
			buflen++;
		}
	}else{
		if(dot1_len == dot2_len){
			for(ii_ = dot1_len; ii_ > 0 && ii_ != dot2_len; ii_--){
				v1[0] = dot1[ii_-1];
				v1[0] = atoi(v1);
				if(v1[0] - retenue >= 0){
					result = 10 - v1[0] - retenue;
				}else{
					retenue = 0;
				}
				if(buflen + 1 >= BUFFER){
					z++;
					buflen = 0;
					pbuf = reallocation((void **)&buffer,z*BUFFER);
				}
				sprintf(pbuf, "%i", result);
				pbuf++;
				buflen++;
			}
			for(ii_ = ii_ , ij_ = dot2_len; ii_ > 0 && ij_ > 0; ii_--, ij_--){
				v1[0] = dot1[ii_-1];
				v2[0] = dot2[ij_-1];
				v1[0] = atoi(v1);
				v2[0] = atoi(v2);
				if(v1[0] - retenue >= v2[0]){
					result = v1[0] - v2[0] - retenue;
					retenue = 0;
				}else{
					result = 10+v1[0] - v2[0] - retenue;
					retenue = 1;
				}
				if(buflen + 1 >= BUFFER){
					z++;
					buflen = 0;
					pbuf = reallocation((void **)&buffer,z*BUFFER);
				}
				sprintf(pbuf, "%i", result);
				pbuf++;
				buflen++;
			}
		}
	}
	if(dot1_len || dot2_len){
		if(buflen + 1 >= BUFFER){
			z++;
			buflen = 0;
			pbuf = reallocation((void **)&buffer,z*BUFFER);
		}
		sprintf(pbuf, ".");
		pbuf++;
		buflen++;
	}
	for(ptr1 = val1, ptr2 = val2, ii_ = val1_len - (neg1 == 1), ij_ = val2_len - (dot2_len > 0);
		ii_ > 0 && ij_ > 0;
		ii_--, ij_--, ptr1--, ptr2--
	){
		v1[0] = *ptr1;
		v2[0] = *ptr2 ;
		v1[0] = atoi(v1);
		v2[0] = atoi(v2);
		if(v1[0] - retenue >= v2[0]){
			result = v1[0] - v2[0] - retenue;
			retenue = 0;
		}else{
			result = 10+v1[0] - v2[0] - retenue;
			retenue = 1;
		}
		if(buflen + 1 >= BUFFER){
			z++;
			buflen = 0;
			pbuf = reallocation((void **)&buffer,z*BUFFER);
		}
		sprintf(pbuf, "%i", result);
		pbuf++;
		buflen++;
	}
	for(ii_ = ii_; ii_ > 0; ii_--, ptr1--){
		if((ptr1 +1) == n1)break;
		v1[0] = *ptr1;
		v1[0] = atoi(v1);
		if(v1[0] - retenue >= 0){
			result = v1[0] - retenue;
			retenue = 0;
		}else{
			result = 10 - (v1[0] + retenue);
			if(result == 10)
				result = 0;
			retenue = 1;
		}
		if(buflen + 1 >= BUFFER){
			z++;
			buflen = 0;
			pbuf = reallocation((void **)&buffer,z*BUFFER);
		}
		sprintf(pbuf, "%i", result);
		pbuf++;
		buflen++;
	}
	for(ptr2 = ptr2, ij_ = ij_-1; ij_ > 0 && ij_ != ~(unsigned long int)0; ij_--, ptr2++){
		v2[0] = *ptr2;
		v2[0] = atoi(v2);
		if(v2[0] - retenue < 0){
			result = v2[0] - retenue;
			retenue = 0;
		}else{
			result = 10 - v2[0] - retenue;
			retenue = 1;
		}
		if(buflen + 1 >= BUFFER){
			z++;
			buflen = 0;
			pbuf = reallocation((void **)&buffer,z*BUFFER);
		}
		sprintf(pbuf, "%i", result);
		pbuf++;
		buflen++;
	}
	pbuf = (*(pbuf) == '.') ? pbuf +1: pbuf;
	while(strlen(buffer) > 1 && (buffer[strlen(buffer)-1] == '0'&& buffer[strlen(buffer)-2] != '.')){
		buffer[strlen(buffer)-1] = 0;
		pbuf = &buffer[strlen(buffer)-2];
	}
	if(neg){
		if(buflen + 1 >= BUFFER){
			z++;
			pbuf = reallocation((void **)&buffer,z*BUFFER);
		}else
			pbuf = buffer+strlen(buffer);
		sprintf(pbuf,"-");
		pbuf++;
		buflen++;
	}
	pbuf = buffer;
	ij_ = strlen(pbuf);
	pret = allocation((void **)&ret, ij_, sizeof(char));
	int stop = 0;
	for(ii_ = ij_-1, pret = ret; stop == 0; ii_--, pret++){
		*pret = pbuf[ii_];
		stop = (ii_ == 0) ? 1 :0;
	}
	if(equal("0",ret) == 0){
		strcpy(ret,"0");
	}
	if((n1 = strchr(ret,'.')) != NULL)
		for(n2 = &ret[strlen(ret) - 1];(n2+1) != n1 && (*n2 == '0' || *n2 == '.'); n2--)
			*n2 = 0;
	free(pbuf);
	return ret;
}
void *multiplication(void *num1, void *num2){
	char *n1 = num1, *n2 = num2, *n1_ = NULL, *n2_ = NULL,
		*dot1 = NULL, *dot2 = NULL, *pdot,
		v1[2] = { 0, 0 }, v2[2] = { 0, 0 }, temp[3] = { 0, 0, 0},
		*buffer = NULL,
		*resultat = NULL, 
		*result = NULL, *presult, *total = NULL,*pbuf,
		retenue = 0, neg = 0, neg1 = 0, neg2 = 0, z = 0;
	unsigned long int dot1_len = 0, dot2_len = 1, dot_len = 1,
				buflen = 0, iz = 1, zero = 0;
	unsigned long int ii = 0, ij = 0,ii_ = 0,x = 0;
	NEG;
	NEG_TEST;
	if(equal("0", n1) == 0 || equal("0", n2) == 0){
		resultat = allocation((void **)&resultat, 2, sizeof(char));
		*resultat = '0';
		return resultat;
	}
	ZERO;
	n1_ = allocation((void **)&n1_, strlen(n1), sizeof(char));
	strcpy(n1_, n1);
	n2_ = allocation((void **)&n2_, strlen(n2), sizeof(char));
	strcpy(n2_, n2);
	n1 = n1_;
	n2 = n2_;
	dot1 = strchr(n1, '.');
	dot2 = strchr(n2, '.');
	dot1_len = (dot1) ? strlen(dot1) -1: 0;
	dot2_len = (dot2) ? strlen(dot2) -1: 0;
	dot_len = dot1_len + dot2_len;
	pbuf = allocation((void **)&resultat, BUFFER, sizeof(char));
	if(dot1){
		pdot = dot1;
		while(1){
			*pdot = *(pdot +1);
			if(*pdot == 0)
				break;
			pdot++;
		}
	}
	if(dot2){
		pdot = dot2;
		while(1){
			//fprintf(stderr, "%c::%c\n", *pdot, *(pdot+1));
			strncpy(&pdot[0], &pdot[1], 1);
			if(*pdot == 0)
				break;
			pdot++;
		}
	}
	for(n1 = n1,
		ii = strlen(n1);
		ii > 0; ii--,
		zero += (ii > 0 && n1[ii-1] != '.')
	){
		if(n1[ii-1] == '.')continue;
		v1[0] = n1[ii-1];
		v1[0] = atoi(v1);
		if(zero){
			for(x = 0; (unsigned long int)x < zero; x++){
				if(buflen+1 >= BUFFER){
					iz++;
					buflen = 0;
					pbuf = reallocation((void **)&resultat, BUFFER*iz);
				}
				sprintf(pbuf,"0");
				pbuf++;
				buflen++;
			}
		}
		for(n2 = n2, ij = strlen(n2); ij > 0;ij--){
			if(n2[ij-1] == '.')continue;
			v2[0] = n2[ij-1];
			v2[0] = atoi(v2);
			z = retenue/10;
			retenue = 0;
			if(v1[0]*v2[0] + z >= 10 && ii-1 > 0 && ij-1 > 0){
				do{
					retenue += 10;
				}while(v1[0]*v2[0]+ z - retenue >= 10);
				//printf("%i\n",v1[0]*v2[0]+ z - retenue);
				if(buflen+1 >= BUFFER){
					iz++;
					buflen = 0;
					pbuf = reallocation((void **)&resultat, BUFFER*iz);
				}
				sprintf(pbuf,"%i", v2[0]*v1[0]+z-retenue);
				pbuf++;
				buflen++;
			}else{
				if(ij-1 == 0 && v1[0]*v2[0]+ z - retenue >= 10){
					if(buflen + 2 >= BUFFER){
						iz++;
						buflen = 0;
						pbuf = reallocation((void **)&resultat, BUFFER*iz);
					}
					sprintf(pbuf,"%i", v2[0]*v1[0]+z-retenue);
					temp[0] = *(pbuf+1);
					temp[1] = *pbuf;
					*pbuf = temp[0];
					*(pbuf+1) = temp[1];
					result = allocation((void **)&result, strlen(resultat), sizeof(char));
					for(presult = result,ii_ = strlen(resultat); ii_ > 0; ii_--,presult++)
						*presult = resultat[ii_-1];
					memset(resultat, 0, strlen(resultat));
					pbuf = reallocation((void **)&resultat,BUFFER);
					pbuf = resultat;
					if(total == NULL){
						total = result;
					}else{
						presult = addition(total,result);
						free(total);
						free(result);
						total = presult;
					}
					iz = 1;
					buflen = 0;
				}else{
					if(v1[0]*v2[0] + z >= 10){
						do{ 
							retenue += 10;
						}while(v1[0]*v2[0]+ z - retenue >= 10);
						if(buflen+1 >= BUFFER){
							iz++;
							buflen = 0;
							pbuf = reallocation((void **)&resultat, BUFFER*iz);
						}
						sprintf(pbuf,"%i", v2[0]*v1[0]+z-retenue);
						pbuf++;
						buflen++;
					}else{
						if(ij - 1 > 0){
							if(buflen+1 >= BUFFER){
								iz++;
								buflen = 0;
								pbuf = reallocation((void **)&resultat, BUFFER*iz);
							}
							sprintf(pbuf,"%i", v2[0]*v1[0]+z-retenue);
							pbuf++;
							buflen++;
						}else{
							if(buflen+1 >= BUFFER){
								iz++;
								buflen = 0;
								pbuf = reallocation((void **)&resultat, BUFFER*iz);
							}
							sprintf(pbuf,"%i", v2[0]*v1[0]+z-retenue);
							pbuf++;
							buflen++;
							result = allocation((void **)&result, strlen(resultat), sizeof(char));
							for(presult = result,ii_ = strlen(resultat); ii_ > 0; ii_--,presult++){
								*presult = resultat[ii_-1];
							}
							if(total == NULL){
								total = result;
							}else{
								presult = addition(total, result);
								free(result);
								free(total);
								total = presult;
							}
							pbuf = resultat;
							memset(resultat,0,BUFFER*iz);
							iz = 1;
							pbuf = reallocation((void **)&resultat, BUFFER);
						}
					}
				}
			}
		}
	}
	free(n1_);
	free(n2_);
	//printf("%s\n", total);
	//exit(0);
	if(total && dot_len > 0){
		if(strlen(total) < dot_len){
			pbuf = allocation((void **)&buffer, dot_len +1, sizeof(char));
			for(result = &buffer[dot_len +1], ii = strlen(total)-1;ii != ~(unsigned long int)0;result--,ii--)
				*result = total[ii];
			for(result = result, ii = 0;(unsigned long int)ii < dot_len-strlen(total);ii++, result--)
				*result = '0';
			*result = '.';
			result--;
			*result = '0';
			free(total);
			free(resultat);
			if(neg){
				VALEUR_NEGATIVE(result, pbuf, ii);
			}
			//printf("%s\n", result);
			//exit(0);
			return result;
		}
		pbuf = allocation((void **)&buffer, strlen(total)+1, sizeof(char));
		strcpy(pbuf, total);
		for(pbuf = &buffer[strlen(buffer)-1], ii = 0; (unsigned long int)ii != dot_len;pbuf--, ii++){
			*(pbuf+1) = *(pbuf);
		}
		*(pbuf+1) = '.';
		free(total);
		free(resultat);
		for(pbuf = &buffer[strlen(buffer)-1];*pbuf == '0'; pbuf--)
			*pbuf = 0;
		pbuf = strchr(buffer, '.');
		if(pbuf && *(pbuf + 1) == 0)
			*pbuf = 0;
		if(*buffer == '.'){
			total = allocation((void **)&total,strlen(buffer)+1, sizeof(char));
			*total = '0';
			strcpy(&total[1], buffer);
			free(buffer);
			buffer = total;
		}
		if(neg){
			VALEUR_NEGATIVE(buffer, pbuf, ii);
		}
		return buffer;
	}
	if(neg){
		VALEUR_NEGATIVE(total, pbuf, ii);
	}
	free(resultat);
	//printf("%s\n", total);
	//exit(0);
	return total;
}
void *division(void *num1, void *num2, unsigned long int virgule, int approximation){
	char *n1 = num1, *n2 = num2,
		*quotient = NULL, *dividende = NULL, *diviseur = NULL, *reste = NULL, *preste,
		*temp = NULL, *temp_ = NULL, t[2] = {0, 0}, *result = NULL, *pr, point = 0, *arrondi = NULL,
		neg = 0, neg1 = 0, neg2 = 0;
	unsigned long int buflen = 0, qbuf = 1, len = 0, virgule_ = 0, zero = 0, nreste = 0, qreste = 1;
	unsigned long int ii = 0;
	int x;
	NEG;
	NEG_TEST;
	if(equal(n2,"0") == 0 || equal(n2,"-0") == 0){
		fprintf(stderr, "Erreur: Division par 0\n");
		return NULL;
	}
	ZERO;
	if(equal(n1,"0") == 0){
		 quotient = allocation((void **)&quotient,1,sizeof(char));
		*quotient = '0';
		return quotient;
	}
	diviseur = allocation((void **)&diviseur, strlen(n2), sizeof(char));
	dividende = allocation((void **)&dividende, strlen(n1), sizeof(char));
	memcpy(diviseur, n2, strlen(n2));
	memcpy(dividende, n1, strlen(n1));
	do{
		if((n2 = strchr(diviseur,'.')) == NULL  && (n1 = strchr(dividende, '.')) == NULL)
				break;
		temp = multiplication(diviseur, "10");
		free(diviseur);
		diviseur = temp;
		temp = multiplication(dividende, "10");
		free(dividende);
		dividende = temp;
	}while(1);
	preste = allocation((void **)&reste, BUFFER, sizeof(char));
	pr = allocation((void **)&result, BUFFER, sizeof(char));
	len = strlen(dividende)-1;
	do{
		if(nreste +1 >= BUFFER){
			qreste++;
			nreste = 0;
			preste = reallocation((void **)&reste, qreste*BUFFER);
		}
		*preste = dividende[ii];
		preste[1] = 0;
		preste++;
		nreste++;
		ii++;
	}while((unsigned long int)ii < strlen(dividende) && equal(reste, diviseur) < 0);
	while(equal(reste, diviseur) < 0 && virgule_ <= virgule){
		temp = multiplication(reste, "10");
		free(reste);
		reste = temp;
		if(point == 0){
			if(buflen + 1 >= BUFFER){
				buflen = 0;
				qbuf++;
				pr = reallocation((void **)&result, qbuf * BUFFER);
			}
			*pr = '0';
			pr++;
			buflen++;
			if(buflen + 1 >= BUFFER){
				buflen = 0;
				qbuf++;
				pr = reallocation((void **)&result, qbuf * BUFFER);
			}
			*pr = '.';
			pr++;
			buflen++;
		}else{
			if(buflen + 1 >= BUFFER){
				buflen = 0;
				qbuf++;
				pr = reallocation((void **)&result, qbuf * BUFFER);
			}
			*pr = '0';
			pr++;
			buflen++;
			virgule_++;
		}
		point = 1;
		zero++;
	}
	for(x = 9; x >= 0;x--){
		sprintf(t,"%i", x);
		temp = multiplication(t, diviseur);
		temp_ = soustraction(reste, temp);
		if(equal(temp_,"0") >= 0){
			free(reste);
			free(temp);
			reste = temp_;
			break;
		}
		free(temp);
		free(temp_);
	}
	if(buflen + 1 >= BUFFER){
		buflen = 0;
		qbuf++;
		pr = reallocation((void **)&result, qbuf * BUFFER);
	}
	*pr = *t;
	pr++;
	buflen++;
	while(ii <= len || equal(reste,"0") != 0){
		temp = multiplication(reste, "10");
		free(reste);
		reste = temp;
		if((unsigned long int)ii <= len){
			t[0] = dividende[ii];
			t[1] = 0;
			temp = addition(reste, t);
			free(reste);
			reste = temp;
		}else{
			if(point == 0){
				point = 1;
				if(buflen + 1 >= BUFFER){
					buflen = 0;
					qbuf++;
					pr = reallocation((void **)&result, qbuf * BUFFER);
				}
				*pr = '.';
				pr++;
				buflen++;
			}else
				virgule_++;
			if(virgule_ >= virgule+1)break;
		}
		for(x = 9; x >= 0; x--){
			sprintf(t, "%i", x);
			temp = multiplication(t, diviseur);
			temp_ = soustraction(reste, temp);
			if(equal(temp_,"0") >= 0){
				free(reste);
				free(temp);
				reste = temp_;
				break;
			}else{
				free(temp);
				free(temp_);
			}
		}
		if(buflen + 1 >= BUFFER){
			buflen = 0;
			qbuf++;
			pr = reallocation((void **)&result, qbuf * BUFFER);
		}
		*pr = *t;
		pr++;
		buflen++;
		ii++;
	}
	if(neg){
		VALEUR_NEGATIVE(result, pr, ii);
	}
	if(approximation && virgule){
		if((temp = strchr(result, '.')) != NULL){
			temp_ = &result[strlen(result)-1];
				if(virgule == 0){
					if(equal(temp_, "5")>=0){
						if(!neg)
							temp_ = addition(result ,"1");
						else	temp_ = soustraction(result, "1");
						free(result);
						result = temp_;
						temp = strchr(result,'.');
						if(temp)
							*temp = 0;
					}else{
						temp = strchr(result,'.');
						*temp = 0;
					}
				}else{
						if(strlen(strchr(result,'.') +1) >= virgule){
							temp = allocation((void **)&temp, strlen(result), sizeof(char));
							if(equal(temp_, "5") >= 0){
								for(len = 0; len < virgule_; len++){
									if(len == 0){
										arrondi = allocation((void **)&arrondi, 2, sizeof(char));
										strcpy(arrondi, "0.");
									}else{	
										if((arrondi = realloc(arrondi,strlen(arrondi)+2)) == NULL){
											perror("ralloc()");
											exit(EXIT_FAILURE);
										}
										if(len+1 == virgule_){
											strcat(arrondi, "1");
										}else{	strcat(arrondi, "0");
										}
									}
								}
								free(reste);
								if(len == 0){
									arrondi = allocation((void **)&arrondi, 1,sizeof(char));
									strcpy(arrondi, "0");
								}
								if(!neg)
									reste = addition(result ,arrondi);
								else	reste = soustraction(result, arrondi);
								free(arrondi);
								free(result);
								result = reste;
								reste = temp;
								if(strlen(result) > 0)
									result[strlen(result)-1] = 0;
							}else{	free(temp);
								result[strlen(result)-1] = 0;

							}
						}
				}
		}
	}else{
		temp = strchr(result,'.');
		if(temp && strlen(temp+1) > virgule)
			*(temp+strlen(temp +1)) = 0;
	}
	if((n1 = strchr(result,'.')) != NULL)
		for(n2 = &result[strlen(result) - 1]; n2 != (n1-1) && (*n2 == '.' || *n2 == '0') ; *n2 = 0, n2--);;
	free(reste);
	free(dividende);
	free(diviseur);
	if(equal(result, "0") == 0){
			*result = '0';
			result[1] = 0;
	}
	return result;
}
void *modulo(void *num1, void *num2, unsigned long int virgule){
	char *n1 = num1, *n2 = num2,
		*quotient = NULL, *dividende = NULL, *diviseur = NULL, *reste = NULL, *preste, *zero_ = NULL, *pzero_,
		*temp = NULL, *temp_ = NULL, t[2] = {0, 0}, point = 0, *pvirgule,
		neg1 = 0, *dix, *pdix;
	unsigned long int len = 0, virgule_ = 0, zero = 0, nreste = 0, qreste = 1;
	unsigned long int ii = 0;
	int x;
	for(n1 = n1; *n1 == '-' || *n1 == '+'; n1++) 
		if(*n1 == '-')
			neg1 = !neg1;
	for(n2 = n2; *n2 == '-' || *n2 == '+'; n2++);;
	if(equal(n2,"0") == 0 || equal(n2,"-0") == 0){
		fprintf(stderr, "Erreur: Division par 0\n");
		return NULL;
	}
	ZERO;
	if(equal(n1, n2) == 0 || equal(n2,"1") == 0){
		temp = allocation((void **)&temp, 1, sizeof(char));
		*temp = '0';
		return temp;
	}
	if(equal(n1, n2) < 0){
		//temp = allocation((void **)&temp, strlen(n1)+1, sizeof(char));
		//strcpy(temp, n1);
		reste = multiplication(n1,"1");
		/*ICI*/
		if(virgule)
			dix = multiplication("1","1");
		if(virgule){
			diviseur = n2;
			do{
				pdix = multiplication(dix,"10");
				free(dix);
				dix = pdix;
				temp = multiplication(reste,"10");
				free(reste);
				reste = temp;
				for(x = 9; x >= 0; x--){
					sprintf(t, "%i", x);
					temp = multiplication(t, diviseur);
					temp_ = soustraction(reste, temp);
					if(equal(temp_,"0") >= 0){
						free(reste);
						free(temp);
						reste = temp_;
						break;
					}else{
						free(temp);
						free(temp_);
					}
				}
				virgule_++;
			}while(virgule_ < virgule && equal(reste,"0") != 0);
			temp = division(reste, dix, virgule, 0);
			free(reste);
			free(dix);
			reste = temp;
		}
		if(neg1){
			if(*reste != '0'){
				temp = allocation((void **)&temp,strlen(reste)+1, sizeof(char));
				*temp = '-';
				strcpy(&temp[1], reste);
				free(reste);
				reste = temp;
			}
		}
		return reste;
	}
	if(equal(n1,"0") == 0){
		 quotient = allocation((void **)&quotient,1,sizeof(char));
		*quotient = '0';
		return quotient;
	}
	//printf("==>%s::%s\n", n1, n2);
	/*diviseur = allocation((void **)&diviseur, strlen(n2), sizeof(char));
	dividende = allocation((void **)&dividende, strlen(n1), sizeof(char));
	memcpy(diviseur, n2, strlen(n2));
	memcpy(dividende, n1, strlen(n1));*/
	diviseur = multiplication(n2, "1");
	dividende = multiplication(n1, "1");
	do{
		if((n2 = strchr(diviseur,'.')) == NULL  && (n1 = strchr(dividende, '.')) == NULL)
				break;
		temp = multiplication(diviseur, "10");
		free(diviseur);
		diviseur = temp;
		temp = multiplication(dividende, "10");
		free(dividende);
		dividende = temp;
		if(zero_ == NULL)
			zero_ = multiplication("1", "10");
		else{
			pzero_ = multiplication(zero_,"10");
			free(zero_);
			zero_ = pzero_;
		}
	}while(1);
	preste = allocation((void **)&reste, BUFFER, sizeof(char));
	len = strlen(dividende)-1;
	do{
		if(nreste +1 >= BUFFER){
			qreste++;
			nreste = 0;
			preste = reallocation((void **)&reste, qreste*BUFFER);
		}
		*preste = dividende[ii];
		preste[1] = 0;
		preste++;
		nreste++;
		ii++;
	}while(/*(unsigned long int)ii < strlen(dividende) || */equal(reste, diviseur) < 0);
	while(equal(reste, diviseur) < 0){
		temp = multiplication(reste, "10");
		free(reste);
		reste = temp;
		point = 1;
		zero++;
	}
	for(x = 9; x >= 0;x--){
		sprintf(t,"%i", x);
		temp = multiplication(t, diviseur);
		temp_ = soustraction(reste, temp);
		if(equal(temp_,"0") >= 0){
			free(reste);
			free(temp);
			reste = temp_;
			break;
		}
		free(temp);
		free(temp_);
	}
	while(ii <= len){
		temp = multiplication(reste, "10");
		free(reste);
		reste = temp;
		if(ii <= len){
			t[0] = dividende[ii];
			t[1] = 0;
			temp = addition(reste, t);
			free(reste);
			reste = temp;
		}else{
			if(point == 0)
				point = 1;
			else
				virgule_++;
		}
		for(x = 9; x >= 0; x--){
			//fprintf(stderr,"%==>i", x);
			sprintf(t, "%i", x);
			temp = multiplication(t, diviseur);
			temp_ = soustraction(reste, temp);
			if(equal(temp_,"0") >= 0){
				free(reste);
				free(temp);
				reste = temp_;
				break;
			}else{
				free(temp);
				free(temp_);
			}
		}
		ii++;
	}
	if(zero_){
		virgule_ = strlen(zero_)+1;
		pvirgule = division(reste, zero_, virgule_, 0);
		free(reste);
		reste = pvirgule;
	}
	/*	if(virgule)
			dix = multiplication("1","1");
		if(virgule){
			do{
				pdix = multiplication(dix,"10");
				free(dix);
				dix = pdix;
				temp = multiplication(reste,"10");
				free(reste);
				reste = temp;
				for(x = 9; x >= 0; x--){
					sprintf(t, "%i", x);
					temp = multiplication(t, diviseur);
					temp_ = soustraction(reste, temp);
					if(equal(temp_,"0") >= 0){
						free(reste);
						free(temp);
						reste = temp_;
						break;
					}else{
						free(temp);
						free(temp_);
					}
				}
				virgule_++;
			}while(virgule_ < virgule && equal(reste,"0") != 0);
			temp = division(reste, dix, virgule, 0);
			free(reste);
			free(dix);
			reste = temp;
		}
	}*/
	if(zero_)
		free(zero_);
	if(neg1){
		if(equal(reste, "0") != 0){
			temp = allocation((void **)&temp,strlen(reste)+1, sizeof(char));
			*temp = '-';
			strcpy(&temp[1], reste);
			free(reste);
			reste = temp;
		}
	}
	free(dividende);
	free(diviseur);
	return reste;
}
void *racine_carree(void *num1, unsigned long int virgule, int approximation){
	unsigned long int len, v = virgule+1;
	/*last pour eviter une boucle plus bas*/
	char *num1_ = NULL, *pnum1_,*dix = NULL, *pdix, buffer[32], *buf, *pbuf, *result, *presult, *check = NULL, *test/*, *last = NULL*/;
	if(equal(num1, "0") < 0){
		fprintf(stderr, "racine_carre: Erreur: %s < 0\n", (char *)num1);
		return NULL;
	}
	if(equal(num1, "0") == 0){
		num1_ = multiplication("0","0");
		return num1_;
	}
	num1_ = multiplication(num1, "1");
	while(strchr(num1_,'.') != NULL){
		pnum1_ = multiplication(num1_,"100");
		free(num1_);
		num1_ = pnum1_;
		if(dix == NULL)
			dix = multiplication("1", "10");
		else{
			pdix = multiplication(dix, "10");
			free(dix);
			dix = pdix;
		}
	}
	len = strlen(num1_);
	if((test = strchr(num1_,'.')) != NULL){
		test++;
		if(virgule < strlen(test)){
			v = strlen(test)+1;
		}
	}
	sprintf(buffer,"%lu", len);
	buf = multiplication(buffer,"100");
	pbuf = division(num1_, buf,virgule, 0);
	result = addition(buf, pbuf);
	free(buf);
	presult = multiplication(result,"0.5");;
	//printf("%s\n", result);
	//exit(0);
	do{
		free(pbuf);
		free(result);
		test = strchr(presult,'.');
		pbuf = division(num1_, presult, v, 0);
		result = addition(presult, pbuf);
		free(presult);
		presult = multiplication(result,"0.5");
		if(check)
			free(check);
		if((test = strchr(presult,'.')) != NULL && *presult != '0'){
			if(strlen(test+1)>=v)
				*(test+1+v) = 0; 
		}
		check = multiplication(presult, presult);
		/*Possible boucle infinie*/
		/*if(last == NULL){
			last = allocation((void **)&last,strlen(check)+1, sizeof(char));
			strcpy(last, check);
		}else{
			if(equal(last, check) == 0){
				free(last);
				last = NULL;
				break;
			}
			free(last);
			last = allocation((void **)&last,strlen(check)+1, sizeof(char));
			strcpy(last, check);
		}*/
	}while(equal(num1_, check) < 0);
	free(num1_);
	/*if(last)
		free(last);*/
	free(pbuf);
	free(result);
	free(check);
	if((result = strchr(presult, '.'))){
		if(!approximation)
			result[virgule+1] = 0;
		else{
			if(presult[strlen(presult)-1] >= '5'){
				test = allocation((void **)&test, 3, sizeof(char));
				strcpy(test,"0.");
				if(v == 1){
					check = reallocation((void **)&test,4);
					strcat(test, "1");
					check = addition(presult, test);
					free(presult);
					presult = check;
				}else{
					while(strlen(test) +1 <= v){
						check = reallocation((void **)&test,strlen(test)+2);
						strcat(test, "0");
						check = addition(presult, test);
						free(presult);
						presult = check;
					}
					check = reallocation((void **)&test,strlen(test)+2);
					test[strlen(test)] = '1';
					check = addition(presult, test);
					free(presult);
					presult = check;
					if(strlen(presult) > virgule+1)
						presult[strlen(presult) - 1] = 0;
					else
						presult[strlen(presult)-1] = 0;
				}
				free(test);
			}else presult[strlen(presult)-1] = 0;
			for(result = &presult[strlen(presult)-1]; *result == '0'; *result = 0, result--);;
		}
	}
	if(dix){
		result = division(presult, dix,virgule, approximation);
		free(presult);
		presult = result;
		free(dix);
	}
	if((result = strchr(presult,'.')) && strlen(result) > virgule){
		result[virgule+1] = 0;
		for(result = &presult[strlen(presult)-1]; *result == '0'; *result = 0, result--);;
	}
	if(presult[strlen(presult)-1] == '.')
		presult[strlen(presult)-1] = 0;
	return presult;
}
#ifndef _MATH_H
void *puissance(void *num1, void *num2, unsigned long int internal_buflen, char *format, unsigned long int virgule, int approximation){
	struct elements *el, *pel, *pnext;
	char *n1 = multiplication(num1,"1"),
		*n2 = multiplication(num2,"1"),
		buffer[internal_buflen], *v, 
		*n1_ = n1, *n2_ = n2,
		*i = multiplication("1","0"), *mod, *len, *plen, *pplen, *val = NULL;
	char *i_, *v_, *pseudo = NULL, *p, *dot_, *pdot_, *rebut =  NULL, *prebut;
	long double pseudo_, ld = 1;
	int eq, set = 0, neg = 0, z = 0;
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
		fprintf(stderr, "Puissance non scalaire rejete dans ce mode (librairie compilee sans la librairie math.h\n");
		exit(EXIT_FAILURE);
		//if(equal(n1, "0") < 0){
			//neg = 1;
			//n1_ = multiplication(n1, "-1");
			//free(n1);
			//n1 = n1_;
		//}
		/*if(equal(n1, "0") < 0 && strchr(num2,'.')){
			free(n1);
			free(n2);
			free(i);
			if((n1 = calloc(5,sizeof(char))) == NULL){
				perror("calloc()");
				exit(EXIT_FAILURE);
			}
			strcpy(n1, "-nan");
			return n1;
		}*/
		/*if((v_ = calloc(strlen(n2)+2,sizeof(char))) == NULL){
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
		}while(eq > 0);*/
		/*if(equal(num2, "0") < 0){
			printf("ok\n");
			exit(0);
		}*/
		/*pdot_ = strchr(format,'.');
		if((dot_ = calloc(strlen(format), sizeof(char))) == NULL){
			perror("calloc()");
			exit(EXIT_FAILURE);
		}*/
		/*strcpy(dot_, pdot_+1);
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
		}else pseudo = multiplication(buffer,"1");
		free(n1_);*/
		ELEMENTS("0");
		//printf("%s\n", len);
		//exit(0);
		rebut = pseudo;
		do{
			mod = modulo(len,"2", 0);
			plen = soustraction(len, mod);
			free(len);
			len = plen;
			plen = division(len,"2", 0, 0);
			free(len);
			pplen = multiplication(plen, "1");
			len = plen;
			if(equal(buffer,el->value) != 0)
			/*if(ld * ld == INFINITY)*/{
				val = multiplication(el->value,el->value);
			}else{
				ld *= ld;
				snprintf(buffer,internal_buflen, format, ld);
				val = multiplication(buffer, "1");
			}
			/*if(el)
				ld = strtold(el->value, NULL);
			if(ld * ld == INFINITY){
				val = multiplication(el->value,el->value);
			}else{
				ld *= ld;
				snprintf(buffer,internal_buflen, format, ld);
				val = multiplication(buffer, "1");
			}*/
			//printf("****************\n");
			if(el){
				//val = multiplication(el->value,el->value);
				ld = strtold(el->value, NULL);
				if(ld * ld == INFINITY){
					val = multiplication(el->value,el->value);
				}else{
					ld *= ld;
					sprintf(buffer, format, ld);
					if(buffer[internal_buflen-1] != 0){
						fprintf(stderr,"Tampon interne trop court (internal_buflen)\n");
					}
					val = multiplication(buffer, "1");
				}
			}
			for(pel = el, len = len, plen = NULL;equal(len, "0") != 0;plen = soustraction(len,"1"), free(len), len = plen){
				free(pel->value);
				pel->value = multiplication(val, "1");
				pel = pel->next;
			}
			free(len);
			free(val);
			if(equal(mod,"1") == 0){
				if(rebut == NULL){
					rebut = multiplication(pel->value, "1");
				}else{
					prebut = multiplication(rebut, pel->value);
					free(rebut);
					rebut = prebut;
				}
			}
			if(pel && pel->prev)
				pel->prev->next = NULL;
			while(pel){
				pnext = pel->next;
				free(pel->value);
				free(pel);
				pel = pnext;
			}
			len = pplen;
			free(mod);
		}while(equal(pplen,"0") != 0);
		free(pplen);
		free(n2);
		pseudo = NULL;
		free(v_);
		free(i);
		i = NULL;
		if(pseudo)
			free(pseudo);
		if(neg == 1){
			//printf("******\n");
			//printf("*********************************************\n");
			n1_ = division("1", rebut, virgule, approximation);
			//printf("==>%s::%s::%i\n", n1_, rebut, neg);
			free(rebut);
			return n1_;
		}else{ 
				return rebut;
		}
	}else{
		//printf("====+++====\n");
		if(equal(n2,"0") < 0){
			n2_ = multiplication(n2,"-1");
			free(n2);
			n2 = n2_;
		}
		while(equal(n2,"1") != 0){
			ELEMENTS("1");
			int z = 0;
			do{
			//printf("====>%s\n", n2);
				mod = modulo(len,"2", 0);
				plen = soustraction(len, mod);
				free(len);
				len = plen;
				plen = division(len,"2", 0, 0);
				free(len);
				pplen = multiplication(plen, "1");
				len = plen;
				ld = strtold(el->value, NULL);
				snprintf(buffer,internal_buflen,format,ld);
				if(buffer[internal_buflen-1] != 0){
					fprintf(stderr, "Tampon trop petit (internal_buflen)\n");
					exit(EXIT_FAILURE);
				}
				z++;
				if(equal(buffer,el->value) != 0)
				/*if(ld * ld == INFINITY)*/{
					val = multiplication(el->value,el->value);
				}else{
					ld *= ld;
					snprintf(buffer,internal_buflen, format, ld);
					val = multiplication(buffer, "1");
				}
				for(pel = el, len = len, plen = NULL;equal(len, "0") != 0;plen = soustraction(len,"1"), free(len), len = plen){
					free(pel->value);
					pel->value = multiplication(val, "1");
					pel = pel->next;
				}
				free(len);
				free(val);
				if(equal(mod,"1") == 0){
					if(rebut == NULL){
						rebut = multiplication(pel->value, "1");
					}else{
						prebut = multiplication(rebut, pel->value);
						free(rebut);
						rebut = prebut;
					}
				}
				if(pel && pel->prev)
					pel->prev->next = NULL;
				while(pel){
					pnext = pel->next;
					free(pel->value);
					free(pel);
					pel = pnext;
				}
				len = pplen;
				free(mod);
			}while(equal(pplen,"0") != 0);
			free(n1);
			free(n2);
			free(i);
			free(pplen);
			if(neg){
				n1_ = division("1", rebut, virgule, approximation);
				free(rebut);
				return n1_;
			}else return rebut;
		}
	}
	if(i != NULL)
		free(i);
	if(n2_)
		free(n2_);
	if(neg){
		rebut = division("1", n1_, virgule, approximation);
		free(n1_);
		n1_ = rebut;
	}
	return n1_;
}
/*void *puissance(void *num1, void *num2, unsigned long int internal_buflen, char *format, unsigned long int virgule, int approximation){
	struct elements *el, *pel, *pnext;
	char *n1 = multiplication(num1,"1"),
		*n2 = multiplication(num2,"1"),
		buffer[internal_buflen], *v, 
		*n1_ = n1, *n2_ = n2,
		*i = multiplication("1","0"), *mod, *len, *plen, *pplen, *val = NULL;
	char *rebut =  NULL, *prebut;
	int neg = 0;
	long double ld = 1;
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
		//if(equal(n1, "0") < 0){
			//neg = 1;
			//n1_ = multiplication(n1, "-1");
			//free(n1);
			//n1 = n1_;
		//}
		fprintf(stderr,"WARNING: Puissance non scalaire impossible.\n");
		free(n1);
		free(n2);
		return NULL;
	}else{
		//printf("====+++====\n");
		if(equal(n2,"0") < 0){
			n2_ = multiplication(n2,"-1");
			free(n2);
			n2 = n2_;
		}
		while(equal(n2,"1") != 0){
			ELEMENTS("1");
			do{
				mod = modulo(len,"2", 0);
				plen = soustraction(len, mod);
				free(len);
				len = plen;
				plen = division(len,"2", 0, 0);
				free(len);
				pplen = multiplication(plen, "1");
				len = plen;
				ld = strtold(el->value, NULL);
				if(ld * ld == INFINITY){
					val = multiplication(el->value,el->value);
				}else{
					ld *= ld;
					snprintf(buffer,internal_buflen, format, ld);
					val = multiplication(buffer, "1");
				}
				for(pel = el, len = len, plen = NULL;equal(len, "0") != 0;plen = soustraction(len,"1"), free(len), len = plen){
					free(pel->value);
					pel->value = multiplication(val, "1");
					pel = pel->next;
				}
				free(len);
				free(val);
				if(equal(mod,"1") == 0){
					if(rebut == NULL){
						rebut = multiplication(pel->value, "1");
					}else{
						prebut = multiplication(rebut, pel->value);
						free(rebut);
						rebut = prebut;
					}
				}
				if(pel && pel->prev)
					pel->prev->next = NULL;
				while(pel){
					pnext = pel->next;
					free(pel->value);
					free(pel);
					pel = pnext;
				}
				len = pplen;
				free(mod);
			}while(equal(pplen,"0") != 0);
			free(n1);
			free(n2);
			free(i);
			free(pplen);
			if(neg){
				//n1_ = division("1", rebut, virgule, approximation);
				//free(rebut);
				return rebut;
			}else return rebut;
		}
	}
	if(i != NULL)
		free(i);
	if(n2_)
		free(n2_);
	if(neg){
		rebut = division("1", n1_, virgule, approximation);
		free(n1_);
		n1_ = rebut;
	}
	return n1_;
}*/
#endif
