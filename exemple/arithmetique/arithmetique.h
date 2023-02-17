#ifndef ARITHMETIQUE_H
#define ARITHMETIQUE_H
enum error_op{
	GET = 0,
	SET = 1
};
const char *error_msg[6] = {	"succe",
								"Tampon interne trop petit (internal_buflen)",
								"-nan detecte",
								"-inf detected",
								"Racine carree d'un nombre negatif",
								"Arrondit systeme: trop de chiffre apres la virgule"
							};
#define m_errno m_errno_()
int m_errno_(void);
void *cosinus(char *arg, char *format,unsigned long internal_buflen,int i_deg,int o_deg, unsigned long int virgule, int approximation);
void *sinus(char *arg, char *format,unsigned long internal_buflen,int i_deg,int o_deg, unsigned long int virgule, int approximation);
void *tangente(char *arg, char *format,unsigned long internal_buflen,int i_deg,int o_deg, unsigned long int virgule, int approximation);
void *puissance(void *num1, void *num2, unsigned long int internal_buflen, char *format, unsigned long int virgule, int approximation);
void *log_n(void *num, unsigned long int internal_buflen, char *format, unsigned long int virgule, int approximation);
void *log_10(void *num, unsigned long int internal_buflen, char *format, unsigned long int virgule, int approximation);
void *racine_carree(void *num1, unsigned long int virgule, int approximation);
void *exponentiel(void *num,unsigned long int internal_buflen, char *format, unsigned long int virgule, int approximation);
int error_set(int op, int val);
#endif
