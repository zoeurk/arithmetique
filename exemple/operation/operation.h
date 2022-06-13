#ifndef OPERATION_H
#define OPERATION_H

struct elements{
	char *value;
	struct elements *next;
	struct elements *prev;
};

#define ELEMENTS(init)\
	for(el = NULL, len = multiplication("1", init); equal(n2, len) > 0; plen = addition(len, "1"), free(len), len = plen){\
		if(el == NULL){\
			el = calloc(1, sizeof(struct elements));\
			el->value = multiplication(num1, "1");\
			el->next = NULL;\
			el->prev = NULL;\
			pel = el;\
		}else{\
			pel->next = calloc(1, sizeof(struct elements));\
			pel->next->prev = pel;\
			pel = pel->next;\
			pel->value = multiplication(num1, "1");\
			pel->next = NULL;\
		}\
	}


extern const unsigned long int BUFFER;
int strtype(void *str);
void *allocation(void **ptr, size_t members, unsigned long int size);
void *reallocation(void **ptr, unsigned long int size);
int equal(void *num1, void *num2);
void *addition(void *num1, void *num2);
void *soustraction(void *num1, void *num2);
void *multiplication(void *num1, void *num2);
void *division(void *num1, void *num2, unsigned long int virgule, int approximation);
void *modulo(void *num1, void *num2, unsigned long int virgule);
char *racine_carree(void *num1, unsigned long int virgule, int approximation);
#ifndef _MATH_H

# if __GNUC_PREREQ (3, 3)
#  define INFINITY (__builtin_inff ())
# else
#  define INFINITY HUGE_VALF
# endif
void *puissance(void *num1, void *num2, unsigned long int internal_buflen, char *format, unsigned long int virgule, int approximation);
#endif
#endif
