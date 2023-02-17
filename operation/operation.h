#ifndef OPERATION_H
#define OPERATION_H
enum err_op{
	E_GET = 0,
	E_SET = 1
};
char *o_err_msg[3] = { "Succe", "Nombre trop long", "Division par zero" };
#define o_errno errno_()
extern const unsigned long int BUFFER;
int strtype(void *str);
void *allocation(void **ptr, unsigned long int members, unsigned long int size);
void *reallocation(void **ptr, unsigned long int size);
int equal(void *num1, void *num2);
void *addition(void *num1, void *num2);
void *soustraction(void *num1, void *num2);
void *multiplication(void *num1, void *num2);
void *division(void *num1, void *num2, unsigned long int virgule, int approximation);
void *modulo(void *num1, void *num2, unsigned long int scale);
int err_set(int op, int val);
int errno_(void);
#endif
