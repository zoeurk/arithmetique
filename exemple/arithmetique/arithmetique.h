#ifndef ARITHMETIQUE_H
#define ARITHMETIQUE_H
void *cosinus(char *arg, char *format,unsigned long internal_buflen,int i_deg,int o_deg, unsigned long int virgule, int approximation);
void *sinus(char *arg, char *format,unsigned long internal_buflen,int i_deg,int o_deg, unsigned long int virgule, int approximation);
void *tangente(char *arg, char *format,unsigned long internal_buflen,int i_deg,int o_deg, unsigned long int virgule, int approximation);
void *puissance(void *num1, void *num2, unsigned long int internal_buflen, char *format, unsigned long int virgule, int approximation);
void *log_n(void *num, unsigned long int internal_buflen, char *format, unsigned long int virgule,int approximation);
void *log_10(void *num, unsigned long int internal_buflen, char *format, unsigned long int virgule,int approximation);
void *exponentiel(void *num,unsigned long int internal_buflen, char *format, unsigned long int virgule, int approximation);
#endif
