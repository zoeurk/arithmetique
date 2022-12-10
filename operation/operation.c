#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <math.h>
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
	unsigned long int i = 0, sz = 0, sz_;
	for(sz = strlen(*ptr)+1, sz_ = 0;sz != (unsigned long int)~0 && sz_ != size; sz++, sz_++);;
	/*if(sz == (unsigned long int)~0){
		fprintf(stderr,"Taille de la chaine trop longue pour le systeme.\n");
		free(*ptr);
		exit(EXIT_FAILURE);
	}*/
	if(size == sz){
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
		*dot1, *dot2,
		*val1, *val2,
		v1[2] = { 0, 0 }, v2[2] = { 0, 0 },
		neg1 = 0, neg2 = 0;
	unsigned long int dot1_len = 0, dot2_len = 0,
				val1_len = 0, val2_len = 0, ii;
	//fprintf(stderr, "%s\n", (char *) num1);
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
		return -1;
	}
	if(neg2 && !neg1){
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
		v1[21], v2[21], temp[21],
		*buffer, *pbuf, *ret, *pret,
		*ptr1 = NULL, *ptr2= NULL,
		retenue = 0, neg = 0, neg1 = 0, neg2 = 0, set = 0;
	unsigned long int dot1_len = 0, dot2_len = 0,
				val1_len = 0, val2_len = 0;
	unsigned long int ii = 0, ij = 0, r = 0;
	unsigned long int i1, i2, result;
	//printf("%s + %s = ", (char *)num1, (char *)num2);
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
	if(dot1 != NULL && dot1_len == 0)
		val1_len--;
	if(dot2 != NULL && dot2_len == 0)
		val2_len--;
	pbuf = allocation((void **)&buffer, ((val1_len > val2_len) ? val1_len : val2_len) +((dot1_len > dot2_len) ? dot1_len : dot2_len) + 2, sizeof(char));
	pbuf += ((val1_len > val2_len) ? val1_len : val2_len) +((dot1_len > dot2_len) ? dot1_len : dot2_len) + 2;
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
		/*for(ii = ii, ptr1 = ptr1; ii != ij && ii > 0; ii--, ptr1--){
			v1[0] = *ptr1;
			sprintf(temp,"%c", v1[0]);
			memcpy(pbuf, temp, 1);
			pbuf--;
		}*/
		if(ii != ij){
			pbuf -= (ii-ij);
			ptr1 -= (ii-ij);
			ii = ij;
			memcpy(pbuf, ptr1+1, strlen(ptr1+1));
			//pbuf-=(ii > 1) ? 1 : 1;
		}
		//printf("%s\n", pbuf);
		for(ii = ii,
			ptr1 = (ii > 1) ? ptr1-1: ptr1,
			ptr2 = (ii > 1) ? ptr2-1: ptr2;
			ii > 0;
			ii -= (ii > 1) ? 2 : ii,
			ptr1 -= (ii > 1) ? 2 : ii,
			ptr2 -= (ii > 1) ? 2 : ii
		){
			memset(v1, 0, 21);
			memset(v2, 0, 21);
			memset(temp, 0, 21);
			memcpy(v1, ptr1, (ii > 1)? 2 : ii);
			memcpy(v2, ptr2, (ii > 1)? 2 : ii);
			r = (strlen(v1) > strlen(v2)) ? strlen(v1) : strlen(v2);
			i1 = (unsigned long int)atol(v1);
			i2 = (unsigned long int)atol(v2);
			result = i1 + i2 + retenue;
			if(result >= 100){
				retenue = 1;
				result -= 100;
			}else	retenue = 0;
			pbuf -= r;
			if(ii > 1 && result < 10)
				sprintf(temp, "0%lu", result);
			else
				sprintf(temp, "%lu", result);
			memcpy(pbuf, temp, strlen(temp));
			//pbuf -= strlen(temp);
		}
		pbuf--;
		//printf("%s:%lu\n", pbuf, r);
		//exit(0);
		//if((*pbuf+1) == 0)
			//pbuf++;
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
		if(ii > 0){
			ptr1 -= ii;
			pbuf -= ii;
			strcpy(pbuf, ptr1+1);
			pbuf--;
		}
	}
	if(dot1_len || dot2_len){
		*pbuf = '.';
	/*if(val1_len > 1 || val2_len > 1)
			pbuf-=2;
		else
			pbuf-=1;*/
	}
	for(ii = val1_len, ij = val2_len,
		ptr1 = (ii > 1) ? val1 -1: val1,
		ptr2 = (ij > 1) ? val2 -1: val2;
		ii > 0 || ij > 0 || retenue;
		ii-=(ii > 1) ? 2: ii,
		ij-=(ij > 1) ? 2 : ij,
		ptr1-=(ii > 1)? 2 : ii,
		ptr2-=(ij > 1) ? 2 : ij
	){
		memset(v1, 0, 21);
		memset(v2, 0, 21);
		if(ii != 0)
			memcpy(v1, ptr1, (ii > 1) ? 2 : 1);
		else
			strcpy(v1, "0");
		if(ij != 0)
			memcpy(v2, ptr2, (ij > 1) ? 2 : 1);
		else
			strcpy(v2, "0");
		r = (strlen(v1) > strlen(v2)) ? strlen(v1) : strlen(v2);
		//printf("%s :: %s :: %lu\n", v1, v2, r);
		i1 = (unsigned long int)atol(v1);
		i2 = (unsigned long int)atol(v2);
		result = i1 + i2 + retenue;
		if(result >= 100){
			retenue = 1;
			result -= 100;
		}else	retenue = 0;
		if(result < 10 && r == 2)
			sprintf(temp,"0%lu", result);
		else
			sprintf(temp, "%lu", result);
		if(r < 2 && (ii < 3 && ij < 3) && strlen(temp) == 1){
			pbuf-=1;
			/*if(retenue && r == 2){
				pbuf--;
				printf("***%i::%lu\n", retenue, r);
			}*/
		}else{
			//printf("TEMP=%s::%s\n", temp, pbuf+2);
			pbuf-=strlen(temp);
		}
		memcpy(pbuf, temp, strlen(temp));
		//printf("==>%s::%lu\n", pbuf, r);
	}
	//printf("%s::%lu\n", pbuf, strlen(temp));
	//if(*(pbuf+2) == 0)
		//pbuf++;
	/*if(ii || ij){
		if(ij){
			ii = ij;
			ptr1 = ptr2;
		}
		for(ii = ii, ptr1 = ptr1; ii > 0; ii--,ptr1--){
			memset(v1, 0, 21);
			memset(temp, 0, 21);
			memcpy(v1, ptr1, 1);
			i1 = (unsigned long int)atol(v1);
			result = i1 + retenue;
			if(result >= 10){
				retenue = 1;
				result -= 10;
			}else	retenue = 0;
			sprintf(temp,"%lu", result);
			memcpy(pbuf, temp, 1);
			pbuf--;
		}
	}
	if(retenue){
		sprintf(temp,"%i", retenue);
		memcpy(pbuf, temp, 1);
		pbuf--;
	}*/
	ij = strlen(pbuf);
	pret = allocation((void **)&ret, ij, sizeof(char));
	strcpy(pret, pbuf);
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
		memcpy(ret+1, ret, strlen(ret));
		*ret = '-';
	}
	if((n1 = strchr(ret,'.')) != NULL){
		for(n2 = &ret[strlen(ret) - 1];(n2+1) != n1 && *n2 == '0'; n2--)
			*n2 = 0;
		if(*n2 == '.')*n2 = 0;
	}
	//printf("%s\n", ret);
	return ret;
}
void *soustraction(void *num1, void *num2){
	char *n1 = num1, *n2 = num2,
		*dot1, *dot2,
		*val1, *val2,
		v1[21], v2[21],temp[21],
		*buffer, *pbuf, *ret,
		*ptr1 = NULL, *ptr2= NULL,
		retenue = 0, neg = 0, neg1 = 0, neg2 = 0;
	unsigned long int dot1_len = 0, dot2_len = 0,
				val1_len = 0, val2_len = 0;
	unsigned long int ii_ = 0, ij_ = 0;
	long int i1, i2, result;
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
	if(dot1 != NULL && dot1_len == 0)
		val1_len--;
	if(dot2 != NULL && dot2_len == 0)
		val2_len--;
	pbuf = allocation((void **)&buffer,((val1_len > val2_len) ? val1_len : val2_len) + ((dot1_len > dot2_len) ? dot1_len : dot2_len) +2, sizeof(char));
	pbuf += ((val1_len > val2_len) ? val1_len : val2_len) + ((dot1_len > dot2_len) ? dot1_len : dot2_len) +2;
	if(dot1_len > dot2_len){
		ii_ = dot1_len;
		ij_ = dot2_len;
		/*for(ii_ = dot1_len; ii_ > 0 && ii_ != ~(unsigned long int)0 && ii_ != dot2_len; ii_--){
			sprintf(temp, "%c", dot1[ii_ - 1]);
			memcpy(pbuf, temp, 1);
			pbuf--;
		}*/
		if(ii_ != ij_){
			pbuf -= (ii_ - ij_);
			dot1 += strlen(dot1) - (ii_ - ij_);
			ii_ = ij_;
			memcpy(pbuf, dot1, strlen(dot1));
			pbuf--;
		}
		for(ii_ = ii_; ii_ > 0; ii_--){
			memset(v1, 0, 21);
			memset(v2, 0, 21);
			memset(temp, 0, 21);
			memcpy(v1, &dot1[ii_], 1);
			memcpy(v2, &dot2[ii_], 1);
			i1 = atol(v1);
			i2 = atol(v2);
			if(i1 - retenue >= i2){
				result = i1 - i2 - retenue;
				retenue = 0;
			}else{
				result = 10 + i1 - i2 - retenue;
				retenue = 1;
			}
			sprintf(temp, "%li", result);
			memcpy(pbuf, temp, 1);
			pbuf--;
		}
	}
	if(dot2_len > dot1_len){
		for(ii_ = dot2_len; ii_ > 0 && ii_ != dot1_len; ii_--){
			memset(v1, 0, 21);
			memset(temp, 0, 21);
			memcpy(v1, &dot2[ii_-1], 1);
			i1 = atol(v1);
			if(i1 - retenue > 0){
				result = 10 - i1 - retenue;
				retenue = 1;
			}else{
				result = 10 - i1 - retenue;
				retenue = 1;
			}
			sprintf(temp, "%li", result);
			memcpy(pbuf, temp, 1);
			pbuf--;
		}
		for(ii_ = ii_; ii_ > 0; ii_--){
			memset(v1, 0, 21);
			memset(v2, 0, 21);
			memset(temp, 0, 21);
			memcpy(v1, &dot1[ii_-1], 1);
			memcpy(v2, &dot2[ii_-1], 1);
			i1 = atol(v1);
			i2 = atol(v2);
			if(i1 - retenue >= i2){
				result = i1 - i2 - retenue;
				retenue = 0;
			}else{
				result = 10 + i1 - i2 - retenue;
				retenue = 1;
			}
			sprintf(temp, "%li", result);
			memcpy(pbuf, temp, 1);
			pbuf--;
		}
	}else{
		if(dot1_len == dot2_len){
			for(ii_ = dot1_len; ii_ > 0; ii_--){
				memset(v1, 0, 21);
				memset(v2, 0, 21);
				memset(temp, 0, 21);
				memcpy(v1, &dot1[ii_-1], 1);
				memcpy(v2, &dot2[ii_-1], 1);
				i1 = atol(v1);
				i2 = atol(v2);
				if(i1 - retenue >= i2){
					result = i1 - i2 - retenue;
					retenue = 0;
				}else{
					result = 10 + i1 - i2 - retenue;
					retenue = 1;
				}
				sprintf(temp, "%li", result);
				memcpy(pbuf, temp, 1);
				pbuf--;
			}
		}
	}
	if(dot1_len || dot2_len){
		*pbuf = '.';
		pbuf--;
	}
	for(ptr1 = val1,
		ptr2 = val2,
		ii_ = val1_len - (neg1 == 1),
		ij_ = val2_len - (dot2_len > 0);
		ii_ > 0 || ij_ > 0 || retenue == 1;
		ii_ -= (ii_ > 1) ? 1 : ii_, ij_ -= (ij_ > 1) ? 1 : ij_, ptr1 -= (ii_ > 1) ? 1 : ii_, ptr2 -= (ij_ > 1) ? 1 : ij_
	){
		if((ptr1 +1) == n1)continue;
		memset(v1, 0, 21);
		memset(v2, 0, 21);
		memset(temp, 0, 21);
		if(ii_ > 0)
			memcpy(v1, ptr1, 1);
		else
			strcpy(v1, "0");
		if(ij_ > 0)
			memcpy(v2, ptr2, 1);
		else
			strcpy(v2, "0");
		i1 = atol(v1);
		i2 = atol(v2);
		//if(ii_ < 3 && ij_ < 3 && retenue == 0)
			//break;
			//printf("%i\n",retenue);
		if(i1 - retenue >= i2){
			result = i1 - i2 - retenue;
			retenue = 0;
		}else{
			result = 10 + i1 - i2 - retenue;
			retenue = 1;
		}
		sprintf(temp, "%li", result);
		memcpy(pbuf, temp, 1);
		pbuf--;
	}
	/*for(ii_ = ii_; ii_ > 0; ii_--, ptr1--){
		if((ptr1 +1) == n1)break;
		memset(v1, 0, 21);
		memset(temp, 0, 21);
		memcpy(v1, ptr1, 1);
		i1 = atol(v1);
		if(i1 - retenue >= 0){
			result = i1 - retenue;
			retenue = 0;
		}else{
			result = 10 - i1 - retenue;
			if(result == 10)
				result = 0;
			retenue = 1;
		}
		sprintf(temp, "%li", result);
		memcpy(pbuf, temp, 1);
		pbuf--;
	}
	for(ptr2 = ptr2, ij_ = ij_-1; ij_ > 0 && ij_ != ~(unsigned long int)0; ij_--, ptr2++){
		memset(v2, 0, 21);
		memset(temp, 0, 21);
		memcpy(v2, ptr2, 1);
		i2 = atol(v2);
		if(i2 - retenue < 0){
			result = i2 - retenue;
			retenue = 0;
		}else{
			result = 10 - i2 - retenue;
			retenue = 1;
		}
		sprintf(temp, "%li", result);
		memcpy(pbuf, temp, 1);
		pbuf--;
	}*/
	
	while(*(pbuf+1) == '0' && *(pbuf +2) != '.')
		pbuf++;
	/*if(*(pbuf) == '0'){
		while(*pbuf == '0' && *(pbuf + 1) != '.'){
			*pbuf = 0;
			pbuf++;
		}
		pbuf--;
	}*/
	if(neg){
		*pbuf = '-';
		pbuf--;
	}
	ij_ = strlen(pbuf+1);
	ret = allocation((void **)&ret, ij_, sizeof(char));
	strcpy(ret, pbuf+1);
	free(buffer);
	if(equal("0",ret) == 0){
		strcpy(ret,"0");
	}
	if((n1 = strchr(ret,'.')) != NULL){
		for(n2 = &ret[strlen(ret) - 1];(n2+1) != n1 && *n2 == '0'; n2--)
			*n2 = 0;
		if(*n2 == '.')
			*n2 = 0;
	}
	return ret;
}
void *multiplication(void *num1, void *num2){
	char *n1 = num1, *n2 = num2, *n1_ = NULL, *n2_ = NULL, *n_,
		*dot1 = NULL, *dot2 = NULL, *pdot,
		v1[2] = { 0, 0 }, v2[20], nombre[20], r[20],
		*buffer = NULL,
		*resultat = NULL, 
		*result = NULL, *presult = NULL, *total = NULL,*pbuf,
		neg = 0, neg1 = 0, neg2 = 0,
		___zero___[20];
	unsigned long int dot1_len = 0, dot2_len = 1, dot_len = 1,
				zero = 0, zero_ = 0, z_;
	unsigned long int ii = 0, ij = 0,ii_ = 0, x = 0;
	unsigned long int rn1 = 0, rn2 = 0;
	if(equal(num1, "0") == 0 || equal(num2, "0") == 0){
		result = allocation((void **)&result, 1, sizeof(char));
		*result = '0';
		return result;
	}
	memset(___zero___, '0', 20);
	___zero___[18] = 0;
	NEG;
	NEG_TEST;
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
			strncpy(&pdot[0], &pdot[1], 1);
			if(*pdot == 0)
				break;
			pdot++;
		}
	}
	if(strlen(n2) < strlen(n1)){
		n_= n2;
		n2 = n1;
		n1 = n_;
	}
	for(n1 = n1,
		ii = strlen(n1);
		ii > 0; ii--,
		zero += (ii > 0)
	){
		v1[0] = n1[ii-1];
		rn1 = (unsigned long int)atol(v1);
		for(n2 = n2, zero_ = 0,rn2 = 0, memset(v2, 0, 20), ii_ = 18, ij = (strlen(n2) > 18) ? strlen(n2)-18 : 0;
			ii_ != 0;
			ii_ = ij,
			ij-=(ij > 18) ? 18 : ij,
			zero_++
		){
			memset(nombre,0,20);
			if(ii_ > 18)
				strncpy(nombre, &n2[ij], 18);
			else strncpy(nombre, &n2[ij], ii_);
			rn2 = (unsigned long int)atol(nombre);
			snprintf(r,20,"%lu", rn2*rn1);
			if(resultat == NULL){
				resultat = allocation((void **)&resultat, strlen(r),sizeof(char));
				strcpy(resultat, r);
			}else{
				result = allocation((void **)&result, strlen(r)+zero_*18+zero, sizeof(char));
				strcpy(result, r);
			}
			for(z_ = 0; z_ != zero_; z_++)
				strcat(result, ___zero___);
			if(zero)
				for(x = 0; (unsigned long int)x < zero; x++)
					strcat(result,"0");
			if(result){
				presult = addition(resultat, result);
				free(resultat);
				free(result);
				resultat = presult;
				result = NULL;
			}
		}
	}
	total = resultat;
	free(n1_);
	free(n2_);
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
			if(neg){
				VALEUR_NEGATIVE(result, pbuf, ii);
			}
			return result;
		}
		pbuf = allocation((void **)&buffer, strlen(total)+1, sizeof(char));
		strcpy(pbuf, total);
		for(pbuf = &buffer[strlen(buffer)-1], ii = 0; (unsigned long int)ii != dot_len;pbuf--, ii++){
			*(pbuf+1) = *(pbuf);
		}
		*(pbuf+1) = '.';
		free(total);
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
		free(result);
		return buffer;
	}
	if(neg){
		VALEUR_NEGATIVE(total, pbuf, ii);
	}
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
				}else{
					free(temp);
					result[strlen(result)-1] = 0;
				}		
			}
		}
	}else{
		/*if(atoi(&result[strlen(result)-1]) >= 5){
			test = allocation((void **)&test, 3, sizeof(char));
			strcpy(test,"0.");
			while(strlen(test) <= virgule){
				reallocation((void **)&test,strlen(test)+2);
				strcat(test, "0");
			}
			reallocation((void **)&test,strlen(test)+2);
			test[strlen(test)] = '1';
			pr = addition(result, test);
			free(result);
			result = pr;*/
			if((temp = strchr(result, '.')) != NULL && strlen(temp+1) >= virgule)
				result[strlen(result) -1] = 0;
			//free(test);
		//}
	}
	if((n1 = strchr(result,'.')) != NULL)
		for(n2 = &result[strlen(result) - 1]; n2 != (n1-1) && (*n2 == '.' || *n2 == '0') ; *n2 = 0, n2--);;
	//printf("%s\n", reste);
	free(reste);
	free(dividende);
	free(diviseur);
	if(equal(result, "0") == 0){
			*result = '0';
			result[1] = 0;
	}
	if(result[strlen(result)-1] == '.')
		result[strlen(result)-1] = 0;
	return result;
}
void *modulo(void *num1, void *num2, unsigned long int scale){
	char *dividende = NULL, *reste = NULL,
		*result = NULL, *temp = NULL;
	result = division(num1, num2, scale, 0);
	if(result == NULL)
		return NULL;
	//printf("-->%s::%s\n", result, (char *)num2);
	reste = multiplication(result, num2);
	if(strlen(reste) == 0){
		free(reste);
		reste = multiplication("0", "0");
	}
	dividende = soustraction(num1, reste);
	free(result);
	if(equal(num1, "0") < 0){
		result = multiplication(dividende, "-1");
		free(dividende);
		dividende = result;
		if(*dividende == '-' && equal(dividende, "-0") == 0){
			result = dividende + 1;
			temp = allocation((void **)&temp, strlen(result), sizeof(char));
			strcpy(temp, dividende + 1);
			free(dividende);
			dividende = temp;
		}
	}
	free(reste);
	return dividende;
}
