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
	/*for(pbuf = &buffer[strlen(buffer)+1], ii = strlen(buffer);pbuf != buffer; pbuf--, ii--){ \
		*pbuf = buffer[ii]; \
	}*/ \
	memcpy(&buffer[1], buffer, strlen(buffer)); \
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
#define A_BLK 18
#define A_BLK_ A_BLK-1

#define ADDITION\
	switch(r){ \
		case 18: \
			if(result >= 1000000000000000000){\
				retenue = 1;\
				result -= 1000000000000000000;\
			}else retenue = 0; \
			break; \
		case 17: \
			if(result >= 100000000000000000){\
				retenue = 1;\
				result -= 100000000000000000;\
			}else retenue = 0; \
			break; \
		case 16: \
			if(result >= 10000000000000000){\
				retenue = 1;\
				result -= 10000000000000000;\
			}else retenue = 0; \
			break; \
		case 15: \
			if(result >= 1000000000000000){\
				retenue = 1;\
				result -= 1000000000000000;\
			}else retenue = 0; \
			break; \
		case 14: \
			if(result >= 100000000000000){\
				retenue = 1;\
				result -= 100000000000000;\
			}else retenue = 0; \
			break; \
		case 13: \
			if(result >= 10000000000000){\
				retenue = 1;\
				result -= 10000000000000;\
			}else retenue = 0; \
			break; \
		case 12: \
			if(result >= 1000000000000){\
				retenue = 1;\
				result -= 1000000000000;\
			}else retenue = 0; \
			break; \
		case 11: \
			if(result >= 100000000000){\
				retenue = 1;\
				result -= 100000000000;\
			}else retenue = 0; \
			break; \
		case 10: \
			if(result >= 10000000000){\
				retenue = 1;\
				result -= 10000000000;\
			}else retenue = 0; \
			break; \
		case 9: \
			if(result >= 1000000000){\
				retenue = 1;\
				result -= 1000000000;\
			}else retenue = 0; \
			break; \
		case 8: \
			if(result >= 100000000){\
				retenue = 1;\
				result -= 100000000;\
			}else retenue = 0; \
			break; \
		case 7: \
			if(result >= 10000000){\
				retenue = 1;\
				result -= 10000000;\
			}else retenue = 0; \
			break; \
		case 6: \
			if(result >= 1000000){\
				retenue = 1;\
				result -= 1000000;\
			}else retenue = 0; \
			break; \
		case 5: \
			if(result >= 100000){ \
				retenue = 1; \
				result -= 100000; \
			}else \
				retenue = 0; \
			break; \
		case 4: \
			if(result >= 10000){ \
				retenue = 1; \
				result -= 10000; \
			}else \
				retenue = 0; \
			break; \
		case 3: \
			if(result >= 1000){ \
				retenue = 1; \
				result -= 1000; \
			}else \
				retenue = 0; \
			break; \
		case 2: \
			if(result >= 100){ \
				retenue = 1; \
				result -= 100; \
			}else \
				retenue = 0; \
			break; \
		case 1: \
			if(result >= 10){ \
				retenue = 1; \
				result -= 10; \
			}else \
				retenue = 0; \
			break; \
		default: \
			retenue = 0; \
	}

#define ZERO_SET\
	switch(r){ \
		case 18: \
			if(result < 10) \
				sprintf(temp, "00000000000000000%lu", result); \
			else \
				if(result < 100) \
					sprintf(temp, "0000000000000000%lu", result); \
				else \
					if(result < 1000) \
						sprintf(temp, "000000000000000%lu", result); \
					else \
						if(result < 10000) \
							sprintf(temp, "00000000000000%lu", result); \
						else \
							if(result < 100000) \
								sprintf(temp, "0000000000000%lu", result); \
							else \
								if(result < 1000000) \
									sprintf(temp, "000000000000%lu", result); \
								else \
									if(result < 10000000) \
										sprintf(temp, "00000000000%lu", result); \
									else \
										if(result < 100000000) \
											sprintf(temp,"0000000000%lu", result); \
										else \
											if(result < 1000000000) \
												sprintf(temp, "000000000%lu", result); \
											else \
												if(result < 10000000000) \
													sprintf(temp, "00000000%lu", result); \
												else \
													if(result < 100000000000) \
														sprintf(temp, "0000000%lu", result); \
													else \
														if(result < 1000000000000) \
															sprintf(temp, "000000%lu", result); \
														else \
															if(result < 10000000000000) \
																sprintf(temp, "00000%lu", result); \
															else \
																if(result < 100000000000000) \
																	sprintf(temp, "0000%lu", result); \
																else \
																	if(result < 1000000000000000) \
																		sprintf(temp, "000%lu", result); \
																	else \
																		if(result < 10000000000000000) \
																			sprintf(temp, "00%lu", result); \
																		else \
																			if(result < 100000000000000000) \
																				sprintf(temp, "0%lu", result); \
																			else \
																				sprintf(temp, "%lu", result); \
			break; \
		case 17: \
			if(result < 10) \
				sprintf(temp, "0000000000000000%lu", result); \
			else \
				if(result < 100) \
					sprintf(temp, "000000000000000%lu", result); \
				else \
					if(result < 1000) \
						sprintf(temp, "00000000000000%lu", result); \
					else \
						if(result < 10000) \
							sprintf(temp, "0000000000000%lu", result); \
						else \
							if(result < 100000) \
								sprintf(temp, "000000000000%lu", result); \
							else \
								if(result < 1000000) \
									sprintf(temp, "00000000000%lu", result); \
								else \
									if(result < 10000000) \
										sprintf(temp, "0000000000%lu", result); \
									else \
										if(result < 100000000) \
											sprintf(temp,"000000000%lu", result); \
										else \
											if(result < 1000000000) \
												sprintf(temp, "00000000%lu", result); \
											else \
												if(result < 10000000000) \
													sprintf(temp, "0000000%lu", result); \
												else \
													if(result < 100000000000) \
														sprintf(temp, "000000%lu", result); \
													else \
														if(result < 1000000000000) \
															sprintf(temp, "00000%lu", result); \
														else \
															if(result < 10000000000000) \
																sprintf(temp, "0000%lu", result); \
															else \
																if(result < 100000000000000) \
																	sprintf(temp, "000%lu", result); \
																else \
																	if(result < 1000000000000000) \
																		sprintf(temp, "00%lu", result); \
																	else \
																		if(result < 10000000000000000) \
																			sprintf(temp, "0%lu", result); \
																		else \
																			sprintf(temp, "%lu", result); \
			break; \
		case 16: \
			if(result < 10) \
				sprintf(temp, "000000000000000%lu", result); \
			else \
				if(result < 100) \
					sprintf(temp, "00000000000000%lu", result); \
				else \
					if(result < 1000) \
						sprintf(temp, "0000000000000%lu", result); \
					else \
						if(result < 10000) \
							sprintf(temp, "000000000000%lu", result); \
						else \
							if(result < 100000) \
								sprintf(temp, "00000000000%lu", result); \
							else \
								if(result < 1000000) \
									sprintf(temp, "0000000000%lu", result); \
								else \
									if(result < 10000000) \
										sprintf(temp, "000000000%lu", result); \
									else \
										if(result < 100000000) \
											sprintf(temp,"00000000%lu", result); \
										else \
											if(result < 1000000000) \
												sprintf(temp, "0000000%lu", result); \
											else \
												if(result < 10000000000) \
													sprintf(temp, "000000%lu", result); \
												else \
													if(result < 100000000000) \
														sprintf(temp, "00000%lu", result); \
													else \
														if(result < 1000000000000) \
															sprintf(temp, "0000%lu", result); \
														else \
															if(result < 10000000000000) \
																sprintf(temp, "000%lu", result); \
															else \
																if(result < 100000000000000) \
																	sprintf(temp, "00%lu", result); \
																else \
																	if(result < 1000000000000000) \
																		sprintf(temp, "0%lu", result); \
																	else \
																		sprintf(temp, "%lu", result); \
			break; \
		case 15: \
			if(result < 10) \
				sprintf(temp, "00000000000000%lu", result); \
			else \
				if(result < 100) \
					sprintf(temp, "0000000000000%lu", result); \
				else \
					if(result < 1000) \
						sprintf(temp, "000000000000%lu", result); \
					else \
						if(result < 10000) \
							sprintf(temp, "00000000000%lu", result); \
						else \
							if(result < 100000) \
								sprintf(temp, "0000000000%lu", result); \
							else \
								if(result < 1000000) \
									sprintf(temp, "000000000%lu", result); \
								else \
									if(result < 10000000) \
										sprintf(temp, "00000000%lu", result); \
									else \
										if(result < 100000000) \
											sprintf(temp,"0000000%lu", result); \
										else \
											if(result < 1000000000) \
												sprintf(temp, "000000%lu", result); \
											else \
												if(result < 10000000000) \
													sprintf(temp, "00000%lu", result); \
												else \
													if(result < 100000000000) \
														sprintf(temp, "0000%lu", result); \
													else \
														if(result < 1000000000000) \
															sprintf(temp, "000%lu", result); \
														else \
															if(result < 10000000000000) \
																sprintf(temp, "00%lu", result); \
															else \
																if(result < 100000000000000) \
																	sprintf(temp, "0%lu", result); \
																else \
																	sprintf(temp, "%lu", result); \
			break; \
		case 14: \
			if(result < 10) \
				sprintf(temp, "0000000000000%lu", result); \
			else \
				if(result < 100) \
					sprintf(temp, "000000000000%lu", result); \
				else \
					if(result < 1000) \
						sprintf(temp, "00000000000%lu", result); \
					else \
						if(result < 10000) \
							sprintf(temp, "0000000000%lu", result); \
						else \
							if(result < 100000) \
								sprintf(temp, "000000000%lu", result); \
							else \
								if(result < 1000000) \
									sprintf(temp, "00000000%lu", result); \
								else \
									if(result < 10000000) \
										sprintf(temp, "0000000%lu", result); \
									else \
										if(result < 100000000) \
											sprintf(temp,"000000%lu", result); \
										else \
											if(result < 1000000000) \
												sprintf(temp, "00000%lu", result); \
											else \
												if(result < 10000000000) \
													sprintf(temp, "0000%lu", result); \
												else \
													if(result < 100000000000) \
														sprintf(temp, "000%lu", result); \
													else \
														if(result < 1000000000000) \
															sprintf(temp, "00%lu", result); \
														else \
															if(result < 10000000000000) \
																sprintf(temp, "0%lu", result); \
															else \
																sprintf(temp, "%lu", result); \
			break; \
		case 13: \
			if(result < 10) \
				sprintf(temp, "000000000000%lu", result); \
			else \
				if(result < 100) \
					sprintf(temp, "00000000000%lu", result); \
				else \
					if(result < 1000) \
						sprintf(temp, "0000000000%lu", result); \
					else \
						if(result < 10000) \
							sprintf(temp, "000000000%lu", result); \
						else \
							if(result < 100000) \
								sprintf(temp, "00000000%lu", result); \
							else \
								if(result < 1000000) \
									sprintf(temp, "0000000%lu", result); \
								else \
									if(result < 10000000) \
										sprintf(temp, "000000%lu", result); \
									else \
										if(result < 100000000) \
											sprintf(temp,"00000%lu", result); \
										else \
											if(result < 1000000000) \
												sprintf(temp, "0000%lu", result); \
											else \
												if(result < 10000000000) \
													sprintf(temp, "000%lu", result); \
												else \
													if(result < 100000000000) \
														sprintf(temp, "00%lu", result); \
													else \
														if(result < 1000000000000) \
															sprintf(temp, "0%lu", result); \
														else \
															sprintf(temp, "%lu", result); \
			break; \
		case 12: \
			if(result < 10) \
				sprintf(temp, "00000000000%lu", result); \
			else \
				if(result < 100) \
					sprintf(temp, "0000000000%lu", result); \
				else \
					if(result < 1000) \
						sprintf(temp, "000000000%lu", result); \
					else \
						if(result < 10000) \
							sprintf(temp, "00000000%lu", result); \
						else \
							if(result < 100000) \
								sprintf(temp, "0000000%lu", result); \
							else \
								if(result < 1000000) \
									sprintf(temp, "000000%lu", result); \
								else \
									if(result < 10000000) \
										sprintf(temp, "00000%lu", result); \
									else \
										if(result < 100000000) \
											sprintf(temp,"0000%lu", result); \
										else \
											if(result < 1000000000) \
												sprintf(temp, "000%lu", result); \
											else \
												if(result < 10000000000) \
													sprintf(temp, "00%lu", result); \
												else \
													if(result < 100000000000) \
														sprintf(temp, "0%lu", result); \
													else \
														sprintf(temp, "%lu", result); \
			break; \
		case 11: \
			if(result < 10) \
				sprintf(temp, "0000000000%lu", result); \
			else \
				if(result < 100) \
					sprintf(temp, "000000000%lu", result); \
				else \
					if(result < 1000) \
						sprintf(temp, "00000000%lu", result); \
					else \
						if(result < 10000) \
							sprintf(temp, "0000000%lu", result); \
						else \
							if(result < 100000) \
								sprintf(temp, "000000%lu", result); \
							else \
								if(result < 1000000) \
									sprintf(temp, "00000%lu", result); \
								else \
									if(result < 10000000) \
										sprintf(temp, "0000%lu", result); \
									else \
										if(result < 100000000) \
											sprintf(temp,"000%lu", result); \
										else \
											if(result < 1000000000) \
												sprintf(temp, "00%lu", result); \
											else \
												if(result < 10000000000) \
													sprintf(temp, "0%lu", result); \
												else \
													sprintf(temp, "%lu", result); \
			break; \
		case 10: \
			if(result < 10) \
				sprintf(temp, "000000000%lu", result); \
			else \
				if(result < 100) \
					sprintf(temp, "00000000%lu", result); \
				else \
					if(result < 1000) \
						sprintf(temp, "0000000%lu", result); \
					else \
						if(result < 10000) \
							sprintf(temp, "000000%lu", result); \
						else \
							if(result < 100000) \
								sprintf(temp, "00000%lu", result); \
							else \
								if(result < 1000000) \
									sprintf(temp, "0000%lu", result); \
								else \
									if(result < 10000000) \
										sprintf(temp, "000%lu", result); \
									else \
										if(result < 100000000) \
											sprintf(temp,"00%lu", result); \
										else \
											if(result < 1000000000) \
												sprintf(temp, "0%lu", result); \
											else \
												sprintf(temp, "%lu", result); \
			break; \
		case 9: \
			if(result < 10) \
				sprintf(temp, "00000000%lu", result); \
			else \
				if(result < 100) \
					sprintf(temp, "0000000%lu", result); \
				else \
					if(result < 1000) \
						sprintf(temp, "000000%lu", result); \
					else \
						if(result < 10000) \
							sprintf(temp, "00000%lu", result); \
						else \
							if(result < 100000) \
								sprintf(temp, "0000%lu", result); \
							else \
								if(result < 1000000) \
									sprintf(temp, "000%lu", result); \
								else \
									if(result < 10000000) \
										sprintf(temp, "00%lu", result); \
									else \
										if(result < 100000000) \
											sprintf(temp,"0%lu", result); \
										else \
											sprintf(temp, "%lu", result); \
			break; \
		case 8: \
			if(result < 10) \
				sprintf(temp, "0000000%lu", result); \
			else \
				if(result < 100) \
					sprintf(temp, "000000%lu", result); \
				else \
					if(result < 1000) \
						sprintf(temp, "00000%lu", result); \
					else \
						if(result < 10000) \
							sprintf(temp, "0000%lu", result); \
						else \
							if(result < 100000) \
								sprintf(temp, "000%lu", result); \
							else \
								if(result < 1000000) \
									sprintf(temp, "00%lu", result); \
								else \
									if(result < 10000000) \
										sprintf(temp, "0%lu", result); \
									else \
										sprintf(temp,"%lu", result); \
			break; \
		case 7: \
			if(result < 10) \
				sprintf(temp, "000000%lu", result); \
			else \
				if(result < 100) \
					sprintf(temp, "00000%lu", result); \
				else \
					if(result < 1000) \
						sprintf(temp, "0000%lu", result); \
					else \
						if(result < 10000) \
							sprintf(temp, "000%lu", result); \
						else \
							if(result < 100000) \
								sprintf(temp, "00%lu", result); \
							else \
								if(result < 1000000) \
									sprintf(temp, "0%lu", result); \
								else \
									sprintf(temp, "%lu", result); \
			break; \
		case 6: \
			if(result < 10) \
				sprintf(temp, "00000%lu", result); \
			else \
				if(result < 100) \
					sprintf(temp, "0000%lu", result); \
				else \
					if(result < 1000) \
						sprintf(temp, "000%lu", result); \
					else \
						if(result < 10000) \
							sprintf(temp, "00%lu", result); \
						else \
							if(result < 100000) \
								sprintf(temp, "0%lu", result); \
							else \
								sprintf(temp, "%lu", result); \
			break; \
		case 5: \
			if(result < 10) \
				sprintf(temp, "0000%lu", result); \
			else \
				if(result < 100) \
					sprintf(temp, "000%lu", result); \
				else \
					if(result < 1000) \
						sprintf(temp, "00%lu", result); \
					else \
						if(result < 10000) \
							sprintf(temp, "0%lu", result); \
						else \
							sprintf(temp, "%lu", result); \
			break; \
		case 4: \
			if(result < 10) \
				sprintf(temp, "000%lu", result); \
			else \
				if(result < 100) \
					sprintf(temp, "00%lu", result); \
				else \
					if(result < 1000) \
						sprintf(temp, "0%lu", result); \
					else \
						sprintf(temp, "%lu", result); \
			break; \
		case 3: \
			if(result < 10) \
				sprintf(temp, "00%lu", result); \
			else \
				if(result < 100) \
					sprintf(temp, "0%lu", result); \
				else \
					sprintf(temp, "%lu", result); \
			break; \
		case 2:\
			if(result < 10) \
				sprintf(temp,"0%lu", result); \
			else \
				sprintf(temp, "%lu", result); \
				break; \
		case 1: \
			sprintf(temp, "%lu", result); \
	}

void *addition(void *num1, void *num2){
	char *n1 = num1, *n2 = num2,
		*dot1, *dot2,
		*val1, *val2,
		v1[21], v2[21], temp[21],
		*buffer, *pbuf, *ret,
		*ptr1 = NULL, *ptr2= NULL,
		retenue = 0, neg = 0, neg1 = 0, neg2 = 0;
	unsigned long int dot1_len = 0, dot2_len = 0,
				val1_len = 0, val2_len = 0;
	unsigned long int ii = 0, ij = 0, r = 0;
	unsigned long int i1, i2, result;
	//printf("%s::%s\n", n1, n2);
	if(equal(n1, "0") == 0 || equal(n2, "0") == 0){
		if(equal(n1, "0") == 0){
			n1 = allocation((void **)&n1, strlen(num2), sizeof(char));
			strcpy(n1, num2);
		}else{
			n1 = allocation((void **)&n1, strlen(num1), sizeof(char));
			strcpy(n1, num1);
		}
		return n1;
	}
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
	if(((val1_len > val2_len) ? val1_len : val2_len) +((dot1_len > dot2_len) ? dot1_len : dot2_len) + 2 < val1_len +dot1_len ||
		((val1_len > val2_len) ? val1_len : val2_len) +((dot1_len > dot2_len) ? dot1_len : dot2_len) + 2 < val2_len +dot2_len){
			fprintf(stderr,"Calcule de nombre trop grand:\n%s + %s\n", (char *)num1, (char *)num2);
			return NULL;
	}
	pbuf = allocation((void **)&buffer, ((val1_len > val2_len) ? val1_len : val2_len) +((dot1_len > dot2_len) ? dot1_len : dot2_len) + 2, sizeof(char));
	pbuf += ((val1_len > val2_len) ? val1_len : val2_len) +((dot1_len > dot2_len) ? dot1_len : dot2_len) + 2 +1;
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
		if(ii != ij){
			pbuf -= (ii-ij);
			ptr1 -= (ii-ij);
			ii = ij;
			memcpy(pbuf, ptr1+1, strlen(ptr1+1));
		}
		for(ii = ii,
			ptr1 = (ii > A_BLK_) ? ptr1-(A_BLK_): ptr1 -ii+1,
			ptr2 = (ii > A_BLK_) ? ptr2-(A_BLK_): ptr2 -ii+1;
			ii > 0;
			ii -= (ii > A_BLK_) ? A_BLK : ii,
			ptr1 -= (ii > A_BLK_) ? A_BLK : ii,
			ptr2 -= (ii > A_BLK_) ? A_BLK : ii
		){
			memset(v1, 0, 21);
			memset(v2, 0, 21);
			memset(temp, 0, 21);
			memcpy(v1, ptr1, (ii > A_BLK_)? A_BLK : ii);
			memcpy(v2, ptr2, (ii > A_BLK_)? A_BLK : ii);
			r = (strlen(v1) > strlen(v2)) ? strlen(v1) : strlen(v2);
			i1 = (unsigned long int)atol(v1);
			i2 = (unsigned long int)atol(v2);
			result = i1 + i2 + retenue;
			ADDITION;
			pbuf -= r;
			ZERO_SET;
			memcpy(pbuf, temp, r);
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
		if(ii > 0){
			ptr1 -= ii;
			pbuf -= ii;
			strcpy(pbuf, ptr1+1);
		}
	}
	if(dot1_len || dot2_len){
		pbuf--;
		*pbuf = '.';
	}
	for(ii = val1_len, ij = val2_len,
		ptr1 = (ii > A_BLK_) ? val1 -(A_BLK_): val1 -ii+1,
		ptr2 = (ij > A_BLK_) ? val2 -(A_BLK_): val2 -ij+1;
		ii > 0 || ij > 0 || retenue;
		ii-=(ii > A_BLK_) ? A_BLK: ii,
		ij-=(ij > A_BLK_) ? A_BLK : ij,
		ptr1-=(ii > A_BLK_)? A_BLK : ii,
		ptr2-=(ij > A_BLK_) ? A_BLK : ij
	){
		memset(v1, 0, 21);
		memset(v2, 0, 21);
		if(ii != 0)
			memcpy(v1, ptr1, (ii > A_BLK_) ? A_BLK : ii);
		else
			strcpy(v1, "0");
		if(ij != 0)
			memcpy(v2, ptr2, (ij > A_BLK_) ? A_BLK : ij);
		else
			strcpy(v2, "0");
		r = (strlen(v1) > strlen(v2)) ? strlen(v1) : strlen(v2);
		i1 = (unsigned long int)atol(v1);
		i2 = (unsigned long int)atol(v2);
		result = i1 + i2 + retenue;
		ADDITION;
		ZERO_SET;
		pbuf -= r;
		memcpy(pbuf, temp, strlen(temp));
	}
	if(dot1 || dot2){
		while(*pbuf == '0'&& *(pbuf+1) == '0'){
			*pbuf = 0;
			pbuf++;
		}
	}
	if(neg){
		pbuf--;
		*pbuf = '-';
	}
	if((n1 = strchr(pbuf,'.')) != NULL){
		for(n2 = &pbuf[strlen(pbuf) - 1];(n2+1) != n1 && *n2 == '0'; n2--)
			*n2 = 0;
		if(*n2 == '.')*n2 = 0;
	}
	if(pbuf != buffer){
		for(
			ii = ij = strlen(pbuf),
			ret = buffer;
			ij > 0;
			ret += (ij > 20) ? 21 : ij,
			pbuf += (ij > 20) ? 21 : ij,
			ij -= (ij > 20) ? 21 : ij
		){
			memset(temp, 0, 21);
			memcpy(temp, pbuf, (ij > 20) ? 21 : ij);
			memcpy(ret, temp, (ij > 20) ? 21 : ij);
		}
		*ret = 0;
	}
	return buffer;
}

#define S_BLK 13

#define S_BLK_ S_BLK-1

#define D_SOUSTRACTION_(i) \
		switch(i){ \
			case 13:\
				result = 10000000000000 - i1 - retenue; \
				break;\
			case 12:\
				result = 1000000000000 - i1 - retenue; \
				break;\
			case 11:\
				result = 100000000000 - i1 - retenue; \
				break;\
			case 10:\
				result = 10000000000 - i1 - retenue; \
				break;\
			case 9: \
				result = 1000000000 - i1 - retenue; \
				break; \
			case 8: \
				result = 100000000 - i1 - retenue; \
				break; \
			case 7: \
				result = 10000000 - i1 - retenue; \
				break; \
			case 6: \
				result = 1000000 - i1 - retenue; \
				break; \
			case 5: \
				result = 100000 - i1 - retenue; \
				break; \
			case 4: \
				result = 10000 - i1 - retenue; \
				break; \
			case 3: \
				result = 1000 - i1 - retenue; \
				break; \
			case 2: \
				result = 100 - i1 - retenue; \
				break; \
			case 1: \
				result = 10 - i1 - retenue; \
				break;\
		} \
	retenue = 1;

#define SOUSTRACTION(r) \
	switch(r){ \
		case 13: \
			result = 10000000000000 +i1 - i2 - retenue; \
			break; \
		case 12: \
			result = 1000000000000 +i1 - i2 - retenue; \
			break; \
		case 11: \
			result = 100000000000 +i1 - i2 - retenue; \
			break; \
		case 10: \
			result = 10000000000 +i1 - i2 - retenue; \
			break; \
		case 9: \
			result = 1000000000 +i1 - i2 - retenue; \
			break; \
		case 8: \
			result = 100000000 +i1 - i2 - retenue; \
			break; \
		case 7: \
			result = 10000000 +i1 - i2 - retenue; \
			break; \
		case 6: \
			result = 1000000 +i1 - i2 - retenue; \
			break; \
		case 5: \
			result = 100000 +i1 - i2 - retenue; \
			break; \
		case 4: \
			result = 10000 + i1 - i2 - retenue; \
			break; \
		case 3: \
			result = 1000 + i1 -i2 - retenue; \
			break; \
		case 2: \
			result = 100 + i1 - i2 - retenue; \
			break; \
		case 1: \
			result = 10 + i1 - i2 - retenue; \
			break; \
	} \
	retenue = 1;

void *soustraction(void *num1, void *num2){
	char *n1 = num1, *n2 = num2,
		*dot1, *dot2,
		*val1, *val2,
		v1[21], v2[21],temp[21],
		*buffer, *pbuf, *ret,
		*ptr1 = NULL, *ptr2= NULL,
		neg = 0, neg1 = 0, neg2 = 0;
	unsigned long int dot1_len = 0, dot2_len = 0,
				val1_len = 0, val2_len = 0,
				result = 0, retenue = 0, r, i1, i2;
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
	if(dot1 != NULL && dot1_len == 0)
		val1_len--;
	if(dot2 != NULL && dot2_len == 0)
		val2_len--;
	if((((val1_len > val2_len) ? val1_len : val2_len) + ((dot1_len > dot2_len) ? dot1_len : dot2_len)+2 < val1_len +dot1_len) ||(((val1_len > val2_len) ? val1_len : val2_len) + ((dot1_len > dot2_len) ? dot1_len : dot2_len)+2 < val2_len +dot2_len)){
		fprintf(stderr, "Calcule de nombre trop grands:\n%s - %s\n", (char *)num1, (char *)num2);
		return NULL;
	}
	pbuf = allocation((void **)&buffer,((val1_len > val2_len) ? val1_len : val2_len) + ((dot1_len > dot2_len) ? dot1_len : dot2_len)+2, sizeof(char));
	pbuf += ((val1_len > val2_len) ? val1_len : val2_len) + ((dot1_len > dot2_len) ? dot1_len : dot2_len)+2;
	memset(v1, 0, 21);
	memset(v2, 0, 21);
	memset(temp, 0, 21);
	if(dot1_len > dot2_len){
		ii_ = dot1_len;
		pbuf -= (dot1_len - dot2_len);
		ii_ = dot2_len;
		strcpy(pbuf, &dot1[dot2_len]);
		for(ii_ = ii_, ij_ = dot2_len,
			dot1 = (ii_ > S_BLK_) ? &dot1[ii_-S_BLK] : &dot1[S_BLK-ii_],
			dot2 = (ii_ > S_BLK_) ? &dot2[ii_-S_BLK] : &dot2[S_BLK-ii_];
			ii_ > 0;
			ii_-= (ii_ > S_BLK_) ? S_BLK : ii_,
			dot1 -= (ii_ > S_BLK_) ? S_BLK : ii_,
			dot2 -= (ii_ > S_BLK_) ? S_BLK : ii_
		){
			memset(v1, 0, 21);
			memset(v2, 0, 21);
			memset(temp, 0, 21);
			memcpy(v1, dot1, (ii_ > S_BLK_) ? S_BLK : ii_);
			memcpy(v2, dot2, (ii_ > S_BLK_) ? S_BLK : ii_);
			r = (strlen(v1) > strlen(v2)) ? strlen(v1) : strlen(v2);
			pbuf -= r;
			i1 = atol(v1);
			i2 = atol(v2);
			if(i1 >= i2 + retenue){
				result = i1 - i2 - retenue;
				retenue = 0;
			}else{
				SOUSTRACTION(r);
			}
			ZERO_SET;
			memcpy(pbuf, temp, strlen(temp));
		}
	}
	if(dot2_len > dot1_len){
		for(ij_ = dot2_len - dot1_len, 
			ii_ = dot2_len,
			dot2 = (ij_ > S_BLK_) ? &dot2[ii_- S_BLK_+1] : &dot2[ii_ - ij_];
			ij_ > 0;
			ij_ -= (ij_ > S_BLK_) ? S_BLK : ij_,
			ii_-= (ij_ > S_BLK_) ? S_BLK : ij_,
			dot2 -= (ij_ > S_BLK_) ? S_BLK : ij_
		){
			memset(v1, 0, 21);
			memset(temp, 0, 21);
			memcpy(v1, dot2, (ij_ > S_BLK_) ? S_BLK : ij_);
			i1 = atol(v1);
			r = strlen(v1);
			pbuf-=r;
			D_SOUSTRACTION_(r);
			ZERO_SET;
			memcpy(pbuf, temp, strlen(temp));
		}
		for(ii_ = ij_ = dot1_len,
			dot2 -= (ii_ > S_BLK_) ? S_BLK : dot1_len ,
			dot1 = (ii_ > S_BLK_) ? &dot1[ii_- S_BLK] : &dot1[S_BLK-ii_ -1];
			ii_ > 0;
			ii_-= (ii_ > S_BLK_) ? S_BLK : ii_,
			dot1 -= (ii_ > S_BLK_) ? S_BLK : ii_,
			dot2 -= (ii_ > S_BLK_) ? S_BLK : ii_
		){
			memset(v1, 0, 21);
			memset(v2, 0, 21);
			memset(temp, 0, 21);
			memcpy(v1, dot1, (ii_ > S_BLK_) ? S_BLK : ii_);
			memcpy(v2, dot2, (ii_ > S_BLK_) ? S_BLK : ii_);
			r = (strlen(v1) > strlen(v2)) ? strlen(v1) : strlen(v2);
			pbuf -= r;
			i1 = atol(v1);
			i2 = atol(v2);
			if(i1 >= i2 + retenue){
				result = i1 - i2 - retenue;
				retenue = 0;
			}else{
				SOUSTRACTION(r);
			}
			ZERO_SET;
			memcpy(pbuf, temp, strlen(temp));
		}
	}else{
		if(dot1_len == dot2_len){
			for(ii_ = dot1_len, ij_ = dot1_len,
				dot1 = (ii_ > S_BLK_) ? &dot1[ii_- S_BLK] : &dot1[S_BLK-ii_],
				dot2 = (ii_ > S_BLK_) ? &dot2[ii_- S_BLK] : &dot2[S_BLK-ii_];
				ii_ > 0;
				ii_-= (ii_ > S_BLK_) ? S_BLK : ii_,
				dot1 -= (ii_ > S_BLK_) ? S_BLK : ii_,
				dot2 -= (ii_ > S_BLK_) ? S_BLK : ii_
			){
				memset(v1, 0, 21);
				memset(v2, 0, 21);
				memset(temp, 0, 21);
				memcpy(v1, dot1, (ii_ > S_BLK_) ? S_BLK : ii_);
				memcpy(v2, dot2, (ii_ > S_BLK_) ? S_BLK : ii_);
				r = (strlen(v1) > strlen(v2)) ? strlen(v1) : strlen(v2);
				pbuf -= r;
				i1 = atol(v1);
				i2 = atol(v2);
				if(i1 >= i2 + retenue){
					result = i1 - i2 - retenue;
					retenue = 0;
				}else{
					SOUSTRACTION(r);
				}
				ZERO_SET;
				memcpy(pbuf, temp, strlen(temp));
			}
		}
	}
	if(dot1_len || dot2_len){
		pbuf--;
		*pbuf = '.';
	}
	for(ptr1 = val1,
		ptr2 = val2,
		ii_ = val1_len - (dot1_len > 0),
		ij_ = val2_len - (dot2_len > 0),
		ptr1 -= (ii_ > S_BLK_) ? S_BLK_ : ii_-1,
		ptr2 -= (ij_ > S_BLK_) ? S_BLK_ : ij_-1;
		ii_ > 0 || ij_ > 0 || retenue;
		ii_-= (ii_ > S_BLK_) ? S_BLK : ii_,
		ij_ -= (ij_ > S_BLK_) ? S_BLK : ij_,
		ptr1 -= (ii_ > S_BLK_) ? S_BLK : ii_,
		ptr2 -= (ij_ > S_BLK_) ? S_BLK : ij_
	){
		memset(v1, 0, 21);
		memset(v2, 0, 21);
		memset(temp, 0, 21);
		if(ii_ > 0)
			memcpy(v1, ptr1, (ii_ > S_BLK_) ? S_BLK : ii_);
		else
			v1[0] = '0';
		if(ij_ > 0)
			memcpy(v2, ptr2, (ij_ > S_BLK_) ? S_BLK : ij_);
		else
			v2[0] = '0';
		r = (strlen(v1) > strlen(v2)) ? strlen(v1) : strlen(v2);
		i1 = atol(v1);
		i2 = atol(v2);
		if(i1 >= i2 + retenue){
			result = i1 - i2 - retenue;
			retenue = 0;
		}else{
			SOUSTRACTION(r);
		}
		ZERO_SET;
		pbuf-=strlen(temp);
		memcpy(pbuf, temp, strlen(temp));
	}
	if(*(pbuf) == '0'){
		while(*(pbuf) == '0' && *(pbuf +1) != '.')
			pbuf++;
		if(*pbuf == '0' && *(pbuf + 1) != '.'){
			*pbuf = 0;
			pbuf++;
		}
	}
	if(neg){
		pbuf--;
		*pbuf = '-';
	}
	if(pbuf != buffer){
		for(
			ii_ = ij_ = strlen(pbuf),
			ret = buffer;
			ij_ > 0;
			ret += (ij_ > 20) ? 21 : ij_,
			pbuf += (ij_ > 20) ? 21 : ij_,
			ij_ -= (ij_ > 20) ? 21 : ij_
		){
			memset(temp, 0, 21);
			memcpy(temp, pbuf, (ij_ > 20) ? 21 : ij_ );
			memcpy(ret, temp, (ij_ > 20) ? 21 : ij_);
		}
		*ret = 0;
	}
	ret = buffer;
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
	char *n1 = num1, *n2 = num2, *n1_ = NULL, *n2_ = NULL, *n_, *n__,
		*dot1 = NULL, *dot2 = NULL,
		nombre1[21], nombre2[21], temp[21], r__[21], rlast[21],
		*last = NULL, *plast, *plast_,
		*pr = NULL, *pbuf,
		neg = 0, neg1 = 0, neg2 = 0;
	unsigned long int dot1_len = 0, dot2_len = 0, dot_len = 0;
	unsigned long int ii = 0, ij = 0, r;
	unsigned long int rn1 = 0, rn2 = 0, result;
	if(equal(num1, "0") == 0 || equal(num2, "0") == 0){
		n1 = allocation((void **)&last, 1, sizeof(char));
		*n1 = '0';
		return n1;
	}
	NEG;
	NEG_TEST;
	ZERO;
	n1_ = allocation((void **)&n1_, strlen(n1), sizeof(char));
	strcpy(n1_, n1);
	n2_ = allocation((void **)&n2_, strlen(n2), sizeof(char));
	strcpy(n2_, n2);
	n1 = n1_;
	n2 = n2_;
	if(strlen(n1) + strlen(n2)+neg < strlen(n1) || strlen(n1) + strlen(n2)+neg < strlen(n2)){
		fprintf(stderr,"Calcule trop de nombre trop grands:\n%s * %s\n", (char *)num1, (char *)num2);
		free(n1);
		free(n2);
		return NULL;
	}
	last = allocation((void **)&last, strlen(n1) + strlen(n2)+neg, sizeof(char));
	memset(last, '0', strlen(n1) + strlen(n2));
	plast = last + strlen(n1) +strlen(n2);
	dot1 = strchr(n1, '.');
	dot2 = strchr(n2, '.');
	dot1_len = (dot1) ? strlen(dot1) -1: 0;
	dot2_len = (dot2) ? strlen(dot2) -1: 0;
	dot_len = dot1_len + dot2_len;
	if(dot1)
		memcpy(dot1, dot1+1, dot1_len+1);
	if(dot2)
		memcpy(dot2, dot2+1, dot2_len+1);
	while(*n1 == '0')
		n1++;
	while(*n2 == '0')
		n2++;
	for(n__ = (strlen(n1 )> 8) ? n1 + strlen(n1) - 9 : n1,
		plast_ = plast,
		ii = strlen(n1);
		ii > 0;
		ii -=(ii > 8) ? 9 : ii,
		n__ -= (ii > 8) ? 9 : ii,
		plast_ -= 9
	){
		memset(nombre1, 0, 21);
		memset(r__, 0, 21);
		memcpy(nombre1, n__, (ii > 8) ? 9 : ii);
		rn1 = atol(nombre1);
		for(plast = plast_,
			n_ = (strlen(n2) > 8) ? n2 +strlen(n2)-9 : n2,
			ij = strlen(n2);
			ij != 0 || strlen(r__);
			ij-=(ij > 8) ? 9: ij,
			n_ -= (ij > 8) ? 9 : ij,
			plast -= (ij > 8) ? 9 : ij
		){
			memset(nombre2, 0,21);
			memset(temp, 0, 21);
			memset(rlast, 0, 21);
			if(ij == 0){
				plast -= strlen(r__);
				memcpy(plast, r__, strlen(r__));
				*r__ = 0;
				break;
			}
			strncpy(nombre2, n_, (ij > 8) ? 9 : ij);
			rn2 = atol(nombre2);
			memcpy(rlast, plast, (ij > 8) ? 9 : ij);
			result = rn2*rn1+atoi(r__)+atoi(rlast);
			r = strlen(nombre2);
			ZERO_SET;
			memset(r__, 0, 21);
			if(strlen(temp) > 1){
				memcpy(r__, temp, strlen(temp)-r);
				memcpy(temp, temp + strlen(temp) - r, r);
			}
			memcpy(plast, temp, r);
		}
		*r__ = 0;
	}
	if(dot_len){
		pr = last+strlen(last)-dot_len-1;
		for(*r__ = '.', pr = last + strlen(last) - dot_len - 1;pr != last;pr--){
			*rlast = *pr;
			*pr = *r__;
			*r__ = *rlast;
		}
	}
	pbuf = last;
	if(*last == '0'){
		r = 0;
		ij = strlen(last);
		pbuf = last; 
		while(*(pbuf+neg) == '0' && *(pbuf+1+neg) != '.'){
			pbuf++;
			r++;
		}
		memcpy(last + neg,last+r+neg, ij-r);
		last[ij-r+neg] = 0;
	}
	if(dot_len){
		for(plast = &last[strlen(last)-1]; *plast == '0'; *plast = 0, plast--);;
		if(last[strlen(last)-1] == '.')
			last[strlen(last)-1] = 0;
	}
	if(neg)
		*last = '-';
	free(n1_);
	free(n2_);
	return last;
}
void *division(void *num1, void *num2, unsigned long int virgule, int approximation){
	char *n1 = num1, *n2 = num2,
		*quotient = NULL, *dividende = NULL, *diviseur = NULL, *reste = NULL, *preste,
		*temp = NULL, *temp_ = NULL, t[2] = {0, 0}, *result = NULL, *pr, point = 0, *arrondi = NULL,
		neg = 0, neg1 = 0, neg2 = 0;
	unsigned long int len = 0, virgule_ = 0, zero = 0, nreste = 0, qreste = 1;
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
		//virgule_++;
		//printf("***%s::%s\n", diviseur, dividende);
	}while(1);
	//printf("***\n");
	if(strlen(num1) + virgule + 3 < strlen(num1)){
		fprintf(stderr, "Calcule de nombre trop grand:\n%s / %s\n", (char *)num1, (char *)num2);
		free(dividende);
		free(diviseur);
		return NULL;
	}
	preste = allocation((void **)&reste, BUFFER, sizeof(char));
	pr = allocation((void **)&result, strlen(num1) + virgule+3, sizeof(char));
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
			/*if(buflen + 1 >= BUFFER){
				buflen = 0;
				qbuf++;
				pr = reallocation((void **)&result, qbuf * BUFFER);
			}*/
			*pr = '0';
			pr++;
			/*buflen++;
			if(buflen + 1 >= BUFFER){
				buflen = 0;
				qbuf++;
				pr = reallocation((void **)&result, qbuf * BUFFER);
			}*/
			*pr = '.';
			pr++;
			//buflen++;
		}else{
			/*if(buflen + 1 >= BUFFER){
				buflen = 0;
				qbuf++;
				pr = reallocation((void **)&result, qbuf * BUFFER);
			}*/
			*pr = '0';
			pr++;
			//buflen++;
			virgule_++;
		}
		point = 1;
		zero++;
	}
	for(x = 9; x >= 0;x--){
		sprintf(t,"%i", x);
		temp = multiplication(t, diviseur);
		//fprintf(stderr,"%s::%s\n", temp, reste);
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
	/*if(buflen + 1 >= BUFFER){
		buflen = 0;
		qbuf++;
		pr = reallocation((void **)&result, qbuf * BUFFER);
	}*/
	*pr = *t;
	pr++;
	//buflen++;
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
				/*if(buflen + 1 >= BUFFER){
					buflen = 0;
					qbuf++;
					pr = reallocation((void **)&result, qbuf * BUFFER);
				}*/
				*pr = '.';
				pr++;
				//buflen++;
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
		/*if(buflen + 1 >= BUFFER){
			buflen = 0;
			qbuf++;
			pr = reallocation((void **)&result, qbuf * BUFFER);
		}*/
		*pr = *t;
		pr++;
		//buflen++;
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
	//printf("-->%s == %s/%s\n", result, (char *)num1, (char *)num2);
	reste = multiplication(result, num2);
	//printf("%s == %s * %s\n", reste, result, (char *)num2);
	//exit(0);
	if(strlen(reste) == 0){
		free(reste);
		reste = multiplication("0", "0");
	}
	//fprintf(stderr, "%s - %s\n", (char *)num1, reste);
	dividende = soustraction(num1, reste);
	//printf("%s == %s - %s\n", dividende, (char *)num1, (char *)reste);
	//exit(0);
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
	//printf("%s\n", dividende);
	//exit(0);
	free(reste);
	return dividende;
}
