#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
/*********************************************************************************/
#if LLONG_MAX == 214748364
#define BITS 32
#define CHAINES 10
#define LIMAX 1000000000
#else
#define BITS 64
#define CHAINES 20
#define LIMAX 10000000000000000000
#endif

#define ERREUR_INCONUE 1
#define BUFFER_TROP_PETIT 2
#define END_OF_NUM 4

#define OUI 1
#define NON 0

#define NBR { 0, 0, { NULL, NULL, 0, 0, 0 }, NULL }

struct nbr{
	void *value;
	unsigned long int *n;
	unsigned long int blk;
	int bit;
	int ___;
};
struct vnbr{
	struct nbr n;
	struct nbr mdix;
};
struct nombre{
	int signe;
	int info;
	struct nbr entier;
	struct vnbr *virgule;
};
struct bufnbr{
	unsigned long int *n;
	unsigned long int buffer;
	unsigned long int blk;
	int bit;
	int read;
	unsigned long int n_blk;
	short int n_bit;
	short int c_bit;
	int info;
	int write;
	int ___;
};
struct bufferin{
	unsigned long int buffer;
	int bit;
	int ___;
};
struct bufferout{
	unsigned long int *buffer;
	unsigned long int buf_blk;
	unsigned long int blk;
	short int bit;
	short int gauche;
	int info;
};
static int strtype(void *str);
static void destroy_nbr(void *num);
static unsigned long int read_in(void *b);
static unsigned long int iread_in(void *b);
static void write_in(void *output, void *input);
static int equal(void *num1, void *num2);
static void *addition(void *num1, void *num2, void *result, int blk);
static void *soustraction(void *num1, void *num2, void *result, int blk);
static void *multiplication(void *num1, void *num2, void *result, int blk);
static void *gauche(void *num, void *result, char bit, int blk);
static void *droite(void *num, void *result, char bit, int correct, int blk);
static void *duplic_nbr(void *num, void *result,int blk);
static void arrange (void *num);
static void *division(void *num1, void *num2, void *virgule, int arrondi, int blk, void *result, void *reste);
static void *modulo(void *num1, void *num2, void *virgule, int blk);
static void *vers_base10_len(void *num);
static unsigned long int vers_base(void *num, void *base, int blk);
static void destroy_nbrvalue(void *num);
static void cpyoverlap(char *string, int longueur);
static void decode_vers(void *num, void *base, int blk);
static void *depuis_base(void *num, void *base, void *md, int blk);
static void *encode_depuis_base(void *num, void *base, int zero, int blk);
static unsigned long int block_possible(unsigned long int eblk, int ebit, unsigned long int vblk, int vbit);
static void *vers_entier(void *n, int blk);
static void vers_entier_aligne(void *num1, void *num2, void **resultat1, void **resultat2, int blk);
static void *lavirgule(void *num, void *mdix, int blk);
static void *puissance_scalaire(void *num, void *p, void *virgule, int arrondi, int blk);
/*********************************************************************************/
/*********************************************************************************/
char *in_base(char *base, unsigned long int *len, const char c){
	if(*len == 0){
		if((base = calloc(1, sizeof(char))) == NULL){
			perror("calloc()");
			exit(EXIT_FAILURE);
		}
		*base = c;
		*len = 1;
	}else{
		(*len)++;
		if((base = realloc(base,*len*sizeof(char))) == NULL){
			perror("realloc()");
			exit(EXIT_FAILURE);
		}
		base[*len-1] = c;
	}
	return base;
}
int main(int argc, char **argv){
	struct nombre *n1, *n2, *n3[2], r, *re, *re_, *verif, virgule, entier, *vir, *vir_ = NULL, *e, b10 = NBR, zero = NBR;
	struct bufnbr buf;
	struct bufferin in;
	struct bufferout out;
	unsigned long int ecrit, len = 0, b10_ = 10, *v, j, k, z = 0;
	int i, blk = OUI;
	const char *base32hex = "0123456789ABCDEFGHIJKLMNOPQRSTUV";
	char *base = NULL, *pbase;
	if(argc < 4){
		fprintf(stderr, "usage:%s num1 num2 virgule\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	for(i = 1; i < 4; i++){
		switch(strtype(argv[i])){
			case 0:
				break;
			default:
				fprintf(stderr, "Caractere invalide detecte dans: \"%s\".\n", argv[i]);
				exit(EXIT_FAILURE);
		}
	}
	zero.entier.bit = 1;
	zero.entier.n = &z;
	/*b32.entier.n = &b32_;
	b32.entier.bit = 6;*/
	b10.entier.n = &b10_;
	b10.entier.bit = 4;
	vir = encode_depuis_base(argv[3], &b10, 0, blk);
	if(equal(vir, &zero) != 0)
		vir_ = puissance_scalaire(&b10, vir, NULL, 0, blk);
	n3[0] = n1 = encode_depuis_base(argv[1], &b10, 0, blk);
	decode_vers(n1, &b10, blk);
	printf("donnée encodée: %c%s.%s\n", n1->signe, (char *)n1->entier.value, (n1->virgule) ? (char *)n1->virgule->n.value : "0");
	printf("value:\n%c%s.%s\n%s\ne_blk=%lu; e_bit=%i\nv_blk=%lu; v_bit=%i\nvaleur %c 0\n", n1->signe,
			(char *)n1->entier.value, (n1->virgule) ? (char *)n1->virgule->n.value : "0", argv[1], n1->entier.blk, n1->entier.bit, (n1->virgule) ? n1->virgule->n.blk : 0, (n1->virgule) ? n1->virgule->n.bit : 0,
			(n1->signe == '-') ? '<' : (equal(n1, &zero) == 0) ? '=' : '>'
	);
	//destroy_nbr(n1);
	//exit(EXIT_FAILURE);
	n3[1] = n2 = encode_depuis_base(argv[2], &b10, 0, blk);
	decode_vers(n2, &b10, blk);
	printf("donnée encodée: %c%s.%s\n", n2->signe, (char *)n2->entier.value, (n2->virgule) ? (char *)n2->virgule->n.value : "0");
	printf("value:\n%c%s.%s\n%s\ne_blk=%lu; e_bit=%i\nv_blk=%lu; v_bit=%i\nvaleur %c 0\n", n2->signe,
			(char *)n2->entier.value, (n2->virgule) ? (char *)n2->virgule->n.value : "0", argv[2], n2->entier.blk, n2->entier.bit, (n2->virgule) ? n2->virgule->n.blk : 0, (n2->virgule) ? n2->virgule->n.bit : 0,
			(n2->signe == '-' ) ? '<' : (equal(n2, &zero) == 0) ? '=' : '>'
	);
	memcpy(n3[0], n1, sizeof(struct nombre));
	memcpy(n3[1], n2, sizeof(struct nombre));
	for(i = 0; i < 2; i++){
		ecrit = 0;
		memset(&r, 0, sizeof(struct nombre));
		if(n3[i]->virgule){
			memset(&buf, 0, sizeof(struct bufnbr));
			buf.n = n3[i]->virgule->n.n;
			buf.read = 5;
			buf.n_blk = n3[i]->virgule->n.blk;
			buf.n_bit = n3[i]->virgule->n.bit;
			memset(&out, 0, sizeof(struct bufferout));
			out.buf_blk = buf.n_blk+(buf.n_bit > 0);
			if((out.buffer = calloc(out.buf_blk,sizeof(unsigned long int))) == NULL){
				perror("calloc()");
				exit(EXIT_FAILURE);
			}
			while(buf.info == 0){
				read_in(&buf);
				in.buffer = buf.buffer;
				in.bit = buf.write;
				out.info |= (buf.info&END_OF_NUM);
				if((out.info&END_OF_NUM) != END_OF_NUM)
					base = in_base(base, &len, base32hex[buf.buffer]);
				else
					if(buf.buffer != 0)
						base = in_base(base, &len, base32hex[buf.buffer]);
				write_in(&out, &in);
				ecrit += buf.write;
			}
			for(v = n3[i]->virgule->n.n + n3[i]->virgule->n.blk,
				j = n3[i]->virgule->n.blk+1 ,
				k = n3[i]->virgule->n.bit;
				j > 0;
				v--, j--, k = BITS
			){
				for(k = k; k > 0;k--){
					if((*v&((unsigned long int)1<<(k-1))) != 0)
						break;
					base = in_base(base, &len, '0');
				}
			}
			base = in_base(base, &len, '.');
			if((r.virgule = malloc(sizeof(struct vnbr))) == NULL){
				perror("malloc()");
				exit(EXIT_FAILURE);
			}
			memset(r.virgule, 0, sizeof(struct vnbr));
			r.virgule->n.n = out.buffer;
			r.virgule->n.blk = out.blk;
			r.virgule->n.bit = out.bit;
		}
		memset(&buf, 0, sizeof(struct bufnbr));
		buf.n = n3[i]->entier.n;
		buf.read = 5;
		buf.n_blk = n3[i]->entier.blk;
		buf.n_bit = n3[i]->entier.bit;
		memset(&out, 0, sizeof(struct bufferout));
		out.buf_blk = buf.n_blk+(buf.n_bit > 0);
		if((out.buffer = calloc(out.buf_blk,sizeof(unsigned long int))) == NULL){
			perror("calloc()");
			exit(EXIT_FAILURE);
		}
		while(buf.info == 0){
			read_in(&buf);
			in.buffer = buf.buffer;
			in.bit = buf.write;
			out.info |= (buf.info&END_OF_NUM);
			if((out.info&END_OF_NUM) != END_OF_NUM)
				base = in_base(base, &len, base32hex[buf.buffer]);
			else
				if(buf.buffer != 0)
					base = in_base(base, &len, base32hex[buf.buffer]);
			if((out.info&END_OF_NUM) == END_OF_NUM && buf.buffer == 0 && buf.n_blk == 0)
				base = in_base(base, &len, base32hex[buf.buffer]);
			write_in(&out, &in);
			ecrit += buf.write;
		}
		r.entier.n = out.buffer;
		r.entier.blk = out.blk;
		r.entier.bit = out.bit;
		r.signe = n3[i]->signe;
		decode_vers(&r, &b10, blk);
		printf("ecrit: %lu\n%lu:%i::%lu:%i||%lu:%i::%lu:%i == %c%s.%s\nBase 32hex (conversion binaire): \n%c", ecrit,
				r.entier.blk, r.entier.bit, (r.virgule) ? r.virgule->n.blk : 0 , (r.virgule) ? r.virgule->n.bit : 0,n3[i]->entier.blk, n3[i]->entier.bit, (n3[i]->virgule) ? n3[i]->virgule->n.blk : 0, (n3[i]->virgule) ? n3[i]->virgule->n.bit : 0, r.signe, (char *)r.entier.value,(r.virgule)? (char *)r.virgule->n.value :"0", r.signe
			);
		for(pbase = base +len-1; len > 0 ;pbase--, len--)
			printf("%c", *pbase);
		printf("\nbit: %lu\n", ecrit);
		free(r.entier.value);
		free(r.entier.n);
		if(r.virgule){
			free(r.virgule->n.value);
			free(r.virgule->n.n);
			free(r.virgule->mdix.n);
			free(r.virgule);
		}
		free(base);
		base = NULL;
	}
	if((re = vers_entier(n1, blk)) == NULL){
		re = n1;
	}else
		decode_vers(re, &b10, blk);
	printf("valeur en entier de '%s': %c%s\n", argv[1], re->signe, (char *)re->entier.value);
	if(re != n1)
		destroy_nbr(re);
	if((re = vers_entier(n2, blk)) == NULL){
		re = n2;
	}else
		decode_vers(re, &b10, blk);
	printf("valeur en entier de '%s': %c%s\n", argv[2], re->signe, (char *)re->entier.value);
	if(re != n2)
		destroy_nbr(re);
	re = gauche(n1, NULL, BITS-1, blk);
	if(re){
		decode_vers(re, &b10, blk);
		printf("%s << %i = %c%s\n", argv[1], BITS-1, re->signe, (char *)re->entier.value);
		verif = droite(re, NULL, BITS-1, 0, blk);
		decode_vers(verif, &b10, blk);
		printf("Verification: %s == %c%s\n", argv[1], verif->signe, (char *)verif->entier.value);
		destroy_nbr(verif);
		destroy_nbr(re);
	}else
		printf("Impossible de deplasser des binaires flottants\n");
	re = droite(n1, NULL, BITS-1, 1, blk);
	if(re){
		decode_vers(re, &b10, blk);
		printf("%s >> %i = %c%s.%s\n", argv[1], BITS-1, re->signe, (char *)re->entier.value, (re->virgule) ? (char *)re->virgule->n.value : "0");
		memset(&entier, 0, sizeof(struct nombre));
		memset(&virgule, 0, sizeof(struct nombre));
		entier.signe = re->signe;
		entier.entier.n = re->entier.n;
		entier.entier.blk = re->entier.blk;
		entier.entier.bit = re->entier.bit;
		if(re->virgule){
			virgule.entier.n = re->virgule->n.n;
			virgule.entier.blk = re->virgule->n.blk;
			virgule.entier.bit = re->virgule->n.bit;
		}
		e = gauche(&entier, NULL, BITS-1, blk);
		if(virgule.entier.n){
			verif = addition(e, &virgule, NULL, blk);
		}else
			verif = duplic_nbr(e, NULL, blk);
		decode_vers(verif, &b10, blk);
		printf("Verification: %s == %c%s\n", argv[1], verif->signe, (char *)verif->entier.value);
		destroy_nbr(e);
		destroy_nbr(verif);
		destroy_nbr(re);
	}else
		printf("Impossible de deplasser des binaires flottants\n");
	re = gauche(n2, NULL, BITS-1, blk);
	if(re){
		decode_vers(re, &b10, blk);
		printf("%s << %i = %c%s\n", argv[2], BITS-1, re->signe, (char *)re->entier.value);
		verif = droite(re, NULL, BITS-1, 0, blk);
		decode_vers(verif, &b10, blk);
		printf("Verification: %s == %c%s\n", argv[2], verif->signe, (char *)verif->entier.value);
		destroy_nbr(verif);
		destroy_nbr(re);
	}else
		printf("Impossible de deplasser des binaires flottants\n");
	re = droite(n2, NULL, BITS-1, 1, blk);
	if(re){
		decode_vers(re, &b10, blk);
		printf("%s >> %i = %c%s.%s\n", argv[2], BITS-1, re->signe, (char *)re->entier.value, (re->virgule) ? (char *)re->virgule->n.value : "0");
		memset(&entier, 0, sizeof(struct nombre));
		memset(&virgule, 0, sizeof(struct nombre));
		entier.signe = re->signe;
		entier.entier.n = re->entier.n;
		entier.entier.blk = re->entier.blk;
		entier.entier.bit = re->entier.bit;
		if(re->virgule){
			virgule.entier.n = re->virgule->n.n;
			virgule.entier.blk = re->virgule->n.blk;
			virgule.entier.bit = re->virgule->n.bit;
		}
		e = gauche(&entier, NULL, BITS-1, blk);
		if(virgule.entier.n){
			verif = addition(e, &virgule, NULL, blk);
		}else
			verif = duplic_nbr(e, NULL, blk);
		decode_vers(verif, &b10, blk);
		printf("Verification: %s == %c%s\n", argv[2], verif->signe, (char *)verif->entier.value);
		destroy_nbr(e);
		destroy_nbr(verif);
		destroy_nbr(re);
	}else
		printf("Impossible de deplasser des binaires flottants\n");
	switch(equal(n1, n2)){
		case 1:
			printf("%c%s.%s > %c%s.%s\n", n1->signe, (char *)n1->entier.value,(n1->virgule) ? (char *)n1->virgule->n.value : "0", n2->signe, (char *)n2->entier.value, (n2->virgule) ? (char *)n2->virgule->n.value : "0");
			break;
		case -1:
			printf("%c%s.%s < %c%s.%s\n", n1->signe, (char *)n1->entier.value, (n1->virgule) ? (char *)n1->virgule->n.value : "0", n2->signe, (char *)n2->entier.value, (n2->virgule) ? (char *)n2->virgule->n.value : "0");
			break;
		case 0:
			printf("%c%s.%s == %c%s.%s\n", n1->signe, (char *)n1->entier.value, (n1->virgule) ? (char *)n1->virgule->n.value : "0", n2->signe, (char *)n2->entier.value, (n2->virgule) ? (char *)n2->virgule->n.value : "0");
			break;
	}
	re = duplic_nbr(n1, NULL, blk);
	decode_vers(re, &b10, blk);
	printf("%s == %c%s.%s (duplication)\n", argv[1], re->signe, (char *)re->entier.value,(re->virgule) ? (char *)re->virgule->n.value : "0");
	destroy_nbr(re);
	re = duplic_nbr(n2, NULL, blk);
	decode_vers(re, &b10, blk);
	printf("%s == %c%s.%s (duplication)\n", argv[2], re->signe,(char *)re->entier.value,(re->virgule) ? (char *)re->virgule->n.value : "0");
	destroy_nbr(re);
	//exit(0);
	re = addition(n1, n2, NULL, blk);
	//exit(0);
	decode_vers(re, &b10, blk);
	printf("%s + %s = %c%s.%s\n", argv[1], argv[2], re->signe,(char *)re->entier.value,(re->virgule) ? (char *)re->virgule->n.value : "0");
	destroy_nbr(re);
	//destroy_nbr(n1);
	//destroy_nbr(n2);
	//exit(EXIT_FAILURE);
	re = soustraction(n1, n2, NULL, blk);
	decode_vers(re, &b10, blk);
	printf("%s - %s = %c%s.%s\n", argv[1], argv[2], re->signe, (char *)re->entier.value,(re->virgule) ? (char *)re->virgule->n.value : "0");
	destroy_nbr(re);
	re = multiplication(n1, n2, NULL, blk);
	decode_vers(re, &b10, blk);
	printf("%s * %s = %c%s.%s\n", argv[1], argv[2], re->signe, (char *)re->entier.value,(re->virgule) ? (char *)re->virgule->n.value : "0");
	destroy_nbr(re);
	re = division(n1, n2, vir_ ,0, blk, NULL, NULL);
	decode_vers(re, &b10, blk);

	if(re){
		decode_vers(re, &b10, blk);
		printf("%s / %s = %c%s.%s\n", argv[1], argv[2], re->signe, (char *)re->entier.value,(re->virgule) ? (char *)re->virgule->n.value : "0");
		destroy_nbr(re);
	}else
		printf("Erreur: %s / %s = NULL\n", argv[1], argv[2]);
	re = modulo(n1, n2, vir_, blk);
	if(re){
		decode_vers(re, &b10, blk);
		printf("%s %c %s = %c%s.%s\n", argv[1], '%',argv[2], re->signe, (char *)re->entier.value,(re->virgule) ? (char *)re->virgule->n.value : "0");
		destroy_nbr(re);
	}else
		printf("Erreur: %s %c %s = NULL\n", argv[1], '%',argv[2]);
	re = puissance_scalaire(n1, n2, vir_, 0, blk);
	if(re != NULL){
		decode_vers(re, &b10, blk);
		printf("puissance scalaire: %s^%s = %c%s.%s\n", argv[1], argv[2], re->signe, (char *)re->entier.value,(re->virgule) ? (char *)re->virgule->n.value : "0");
		destroy_nbr(re);
	}else
		printf("%s^%s = puissance non scalaire\n", argv[1], argv[2]);
	/*decode_vers(n1, &b32, blk);
	if((n1->info&2) == 0)
		printf("%c%s.%s\n", n1->signe, (char *)n1->entier.value,(n1->virgule) ? (char *)n1->virgule->n.value : "0");
	else{
		printf("%s en base 32: %c", argv[1], n1->signe);
		for(cr = (char **)n1->entier.value; *cr != NULL; cr++){
			printf("%s", *cr);
			if(*(cr+1) == NULL){
				if(n1->virgule == NULL)
					printf("\n");
				else
					printf(" . ");
			}else
				if(**cr != '-')
					printf(" ");
		}
		if(n1->virgule)
			for(cr = (char **)n1->virgule->n.value; *cr != NULL; cr++){
				printf("%s", *cr);
				if(*(cr+1) == NULL)
					printf("\n");
				else
					printf(" ");
			}
	}
	decode_vers(n2, &b32, blk);
	if((n2->info&2) == 0)
		printf("%c%s.%s\n", n2->signe, (char *)n2->entier.value,(re->virgule) ? (char *)re->virgule->n.value : "0");
	else{
		printf("%s en base 32: %c", argv[2], n2->signe);
		for(cr = (char **)n2->entier.value; *cr != NULL; cr++){
			printf("%s", *cr);
			if(*(cr+1) == NULL){
				if(n2->virgule == NULL)
					printf("\n");
				else
					printf(" . ");
			}else
					printf(" ");
		}
		if(n2->virgule)
			for(cr = (char **)n2->virgule->n.value; *cr != NULL; cr++){
				printf("%s", *cr);
				if(*(cr+1) == NULL)
					printf("\n");
				else
					printf(" ");
			}
	}*/
	if(vir_)
		destroy_nbr(vir_);
	destroy_nbr(vir);
	destroy_nbr(n1);
	destroy_nbr(n2);
	return 0;
}
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
void destroy_nbrvalue(void *num){
	struct nombre *n = num;
	void **ptr, **p;
	if(n->entier.value){
		switch(n->info&6){
			case 2:
				n->info -= 2;
				p = ptr = n->entier.value;
				while(*ptr){
					free(*ptr);
					ptr++;
				}
				free(p);
				if(n->virgule){
					p = ptr = n->virgule->n.value;
					while(*ptr){
						free(*ptr);
						ptr++;
					}
					free(p);
				}
				break;
			case 4:
				n->info -= 4;
				free(n->entier.value);
				if(n->virgule)
					free(n->virgule->n.value);
		}
	}
	n->entier.value = NULL;
	if(n->virgule)
		n->virgule->n.value = NULL;
}
void destroy_nbr(void *num){
	struct nombre *n = num;
	if(n == NULL)
		return;
	if(n->virgule)
		free(n->virgule->mdix.n);
	destroy_nbrvalue(n);
	if(n->info&1){
		free(n);
	}else{
		if(n->virgule){
			if(n->virgule->n.n)
				free(n->virgule->n.n);
			if((n->info&8) == 0)
				free(n->virgule);
		}
		if(n->entier.n)
			free(n->entier.n);
		free(n);
	}
}
unsigned long int read_in(void *b){
	unsigned long int *ptr, mask, hbit, lbit;
	int bit;
	struct bufnbr *buf = b;
	buf->buffer = 0;
	if(buf->blk > buf->n_blk){
		buf->info |= ERREUR_INCONUE;
		return 0;
	}
	if(buf->info){
		buf->bit = 0;
		buf->blk = 0;
		return 0;
	}
	ptr = buf->n + buf->blk;
	mask = ((unsigned long int)~0) >> (BITS-buf->read);
	if(buf->read + buf->c_bit > BITS){
		bit = buf->read + buf->c_bit - BITS;
		lbit = (*ptr&(mask<<(buf->c_bit))) >> buf->c_bit;
		if(buf->blk < buf->n_blk)
			hbit = (*(ptr+1)&(mask>>(BITS-buf->c_bit))) << (buf->read - bit);
		else
			hbit = 0;
		buf->buffer = hbit | lbit;
		buf->bit+=buf->read;
	}else{
		hbit = 0;
		lbit = (*ptr&(mask<<(buf->c_bit))) >> buf->c_bit;
		buf->buffer = lbit;
		buf->bit += buf->read;
	}
	if(buf->bit >= BITS){
		buf->blk++;
		buf->bit-=BITS;
	}
	buf->write = buf->read;
	if((buf->bit >= buf->n_bit && buf->blk >= buf->n_blk) || buf->blk > buf->n_blk){
		if(buf->bit < buf->n_bit && buf->blk > buf->n_blk){
			buf->bit = BITS;
			buf->blk--;
		}
		if(buf->buffer != 0){
			for(buf->bit = BITS;(buf->buffer&((unsigned long int)1)<<(buf->bit-1)) == 0; buf->bit--);;
		}else
			buf->bit = 1;
		buf->write = buf->bit;
		buf->info = END_OF_NUM;
	}
	buf->c_bit = buf->bit;
	return buf->buffer;
}
unsigned long int iread_in(void *b){
	unsigned long int *ptr, mask;
	struct bufnbr *buf = b;
	buf->buffer = 0;
	if(buf->info)
		return 0;
	if(buf->read > BITS || buf->read < 1){
		buf->info |= ERREUR_INCONUE;
		return 0;
	}
	ptr = buf->n + buf->blk;
	buf->write = 0;
	mask = ((unsigned long int)~0);
	//if(buf->bit == 0)
		//exit(EXIT_FAILURE);
	if(buf->c_bit - buf->read < 0){
		if(buf->blk == 0){
			//printf("ICI 1\n");
			mask >>= (BITS-buf->c_bit);
			buf->buffer = (*ptr&(mask));
			buf->write = buf->c_bit;
			buf->c_bit = buf->bit = 0;
			buf->info |= END_OF_NUM;
		}else{
			//printf("ICI 2\n");
			buf->buffer = (*ptr&(~(mask<<(buf->read))))<<(buf->read-buf->c_bit);
			buf->blk--;
			//if(buf->blk == 0)
				//buf->info |= END_OF_NUM;
				//return buf->buffer;
			ptr--;
			buf->bit = (BITS-(buf->read-buf->c_bit));
			mask = (~(unsigned long int)0) << buf->bit;
			buf->buffer |= ((*ptr&(mask)) >> buf->bit);
			buf->c_bit = buf->bit;
			buf->write = buf->read;
			//printf("******************************\n");
		}
	}else{
		//printf("ICI 3\n");
		mask >>= (BITS-buf->read);
		mask <<= (buf->bit-buf->read);
		buf->buffer = (*ptr&mask) >> (buf->c_bit-buf->read);
		buf->bit -= buf->read;
		buf->c_bit = buf->bit;
		buf->write = buf->read;
	}
	if(buf->bit == 0)
		switch(buf->blk){
			case 0:
				buf->info |= END_OF_NUM;
				break;
			default:
				buf->c_bit = buf->bit = BITS;
				buf->blk--;
				break;
		}
	/*if(buf->bit > BITS)
		exit(EXIT_FAILURE);*/
	return buf->buffer;
}
void write_in(void *output, void *input){
	struct bufferout *out = output;
	struct bufferin *in = input;
	int bit;
	unsigned long int *ptr, mask = (unsigned long int)~0, lbit, hbit;
	if(out->buf_blk == 0 || (out->info&BUFFER_TROP_PETIT)){
		out->info |= BUFFER_TROP_PETIT;
		return;
	}
	ptr = out->buffer +out->blk;
	if(in->bit + out->bit < BITS){
			bit = BITS - in->bit;
			mask <<= bit;
			mask >>= bit;
			mask <<= out->bit;
			mask = ~mask;
			*ptr = ((*ptr&mask)|(in->buffer<<out->bit));
			out->bit += in->bit;
	}else{
		if(in->bit +out->bit == BITS){
			mask >>= (in->bit);
			*ptr = ((*ptr&mask)|(in->buffer<<out->bit));
			out->bit += in->bit;
			out->bit = 0;
			out->blk++;
		}else{
			hbit = BITS - out->bit;
			lbit = in->bit-hbit;
			mask = (~(unsigned long int)0)>>hbit;
			*ptr = ((*ptr&mask)|(in->buffer<<out->bit));
			out->blk++;
			if(out->blk == out->buf_blk){
				out->info |= BUFFER_TROP_PETIT;
				return;
			}
			mask = (~(unsigned long int)0)<<lbit;
			ptr++;
			*ptr = ((*(ptr)&mask)|(in->buffer>>hbit));
			out->bit += in->bit;
			out->bit -= BITS;
		}
	}
}
int equal(void *num1, void *num2){
	struct nombre *n1 = num1, *n2 = num2, *n[2] = { NULL, NULL };
	unsigned long int *n1_blk, *n2_blk;
	int pr=1, nr = -1, *pret = &pr, *nret = &nr, xset = 1, set;
	switch((((struct nombre *)num1)->signe == '-') | ((((struct nombre *)num2)->signe == '-') << 1)){
		case 3:
			pret = &nr;
			nret = &pr;
			break;
		case 2:
			return 1;
		case 1:
			return -1;
		default:
			break;
	}
	vers_entier_aligne(n1, n2, (void **)&n[0], (void **)&n[1], (n1->info&OUI)&(n2->info&OUI));
	if(n[0] == NULL || n[1] == NULL){
		n[0] = n1;
		n[1] = n2;
		xset = 0;
	}
	switch(((n[0]->entier.blk > n[1]->entier.blk) || (n[0]->entier.blk == n[1]->entier.blk && n[0]->entier.bit > n[1]->entier.bit)) |
			(((n[1]->entier.blk > n[0]->entier.blk) || (n[1]->entier.blk == n[0]->entier.blk && n[1]->entier.bit > n[0]->entier.bit)) << 1)
	){
		case 2:
			for(set = 0; set < (xset != 0)*2; set++)
				destroy_nbr(n[set]);
			return *nret;
		case 1:
			for(set = 0; set < (xset != 0)*2; set++)
				destroy_nbr(n[set]);
			return *pret;
	
	}
	n1_blk = n[0]->entier.n + n[0]->entier.blk,
	n2_blk = n[1]->entier.n + n[1]->entier.blk;
	do{
		switch((*n1_blk > *n2_blk) | ((*n1_blk < *n2_blk)<<1)){
			case 2:
				for(set = 0; set < (xset != 0)*2; set++)
					destroy_nbr(n[set]);
				return *nret;
			case 1:
				for(set = 0; set < (xset != 0)*2; set++)
					destroy_nbr(n[set]);
				return *pret;
		}
	}while(n1_blk != n[0]->entier.n &&
			(n1_blk--, n2_blk)
		);
	return 0;
}
void *addition(void *num1, void *num2, void *result, int blk){
	struct nombre *n1 = num1, *n2 = num2, *_n1 = NULL, *_n2 = NULL, m1 = NBR, m2 = NBR, *r = NULL, *_r = NULL, *n, zero = NBR;
	struct nbr *md;
	struct bufnbr bnbr1, bnbr2;
	struct bufferin in;
	struct bufferout out;
	unsigned long int ret, resultat, re_blk, z = 0, *eblk, *vblk = NULL, sz, *s, *d;
	void *ptr;
	int re_bit, retenue = 0;
	int *ebit, *vbit = NULL, set = 0;
	if(n1->signe == '-' && n2->signe != '-'){
		n1->signe = 0;
		r = soustraction(n2, n1, NULL, blk);
		n1->signe = '-';
		return r;
	}else{
		if(n1->signe != '-' && n2->signe == '-'){
			n2->signe = '+';
			r = soustraction(n1, n2, NULL, blk);
			n2->signe = '-';
			return r;
		}
	}
	zero.entier.n = &z;
	zero.entier.bit = 1;
	if(equal(n1,&zero) == 0 || equal(n2, &zero) == 0){
		if(equal(n1, &zero) == 0){
			eblk = &n2->entier.blk;
			ebit = &n2->entier.bit;
			if(n2->virgule){
				vbit = &n2->virgule->n.bit;
				vblk = &n2->virgule->n.blk;
				md = &n2->virgule->mdix;
			}
			n = n2;
		}else{
			eblk = &n1->entier.blk;
			ebit = &n1->entier.bit;
			if(n1->virgule){
				vbit = &n1->virgule->n.bit;
				vblk = &n1->virgule->n.blk;
				md = &n1->virgule->mdix;
			}
			n = n1;
		}
		if(result == NULL)
			switch(blk){
				case OUI:
					if((sz = block_possible(*eblk, *ebit, (vblk) ? *vblk : 0, (vbit) ? *vbit : 0)) != 0){
						sz = (*eblk + 1) * sizeof(unsigned long int) + sizeof(struct nombre) + (((vblk) ? *vblk : 0) + ((vbit) ? 1 : 0)) *sizeof(unsigned long int) + (vbit || vblk) * sizeof(struct vnbr);
						if((ptr = malloc(sz)) == NULL){
							perror("malloc()");
							exit(EXIT_FAILURE);
						}
						memset(ptr, 0, sz);
						r = ptr;
						r->info |= 1;
						r->entier.n = (unsigned long int *)(struct nombre *)(r+1);
						if(vbit || vblk){
							r->virgule = (struct vnbr *)(unsigned long int *)(r->entier.n + (n->entier.blk + (n->entier.bit > 0)));
							r->virgule->n.n = (unsigned long int *)(struct vnbr *)(r->virgule+1);
						}
						break;
					}
				default:
					if((r = calloc(1, sizeof(struct nombre))) == NULL){
						perror("calloc()");
						exit(EXIT_FAILURE);
					}
					if((r->entier.n = calloc(*eblk+1, sizeof(unsigned long int))) == NULL){
						perror("calloc()");
						exit(EXIT_FAILURE);
					}
					if(vblk || vbit){
						if((r->virgule = calloc(1, sizeof(struct vnbr))) == NULL){
							perror("calloc()");
							exit(EXIT_FAILURE);
						}
						if((r->virgule->n.n = calloc(*vblk +1, sizeof(unsigned long int))) == NULL){
							perror("calloc()");
							exit(EXIT_FAILURE);
						}
					}
					break;
			}
		else
			r = result;
		for(s = n->entier.n, d = r->entier.n, sz = n->entier.blk+1;sz > 0; sz--, *d = *s, d++,s++);;
		r->entier.bit = *ebit;
		r->entier.blk = *eblk;
		if(vblk || vbit){
			for(s = n->virgule->n.n, d = r->virgule->n.n, sz = n->virgule->n.blk+1;sz > 0; sz--, *d = *s, d++,s++);;
			r->virgule->n.bit = *vbit;
			r->virgule->n.blk = *vblk;
			memcpy(&r->virgule->mdix, md, sizeof(struct nbr));
			if((r->virgule->mdix.n = calloc(md->blk+1, sizeof(unsigned long int))) == NULL){
				perror("calloc()");
				exit(EXIT_FAILURE);
			}
			for(s = md->n, d = r->virgule->mdix.n, sz = md->blk+1;sz > 0; sz--, *d = *s, d++,s++);;
		}
		r->signe = n->signe;
		return r;
	}
	vers_entier_aligne(n1, n2, (void **)&_n1, (void **)&_n2, blk);
	if(!_n1 && !_n2){
		_n1 = n1;
		_n2 = n2;
	}else{
		if(result){
			fprintf(stderr, "addition: operation non permise (result != NULL)\n");
			exit(EXIT_FAILURE);
		}
		if(((struct nombre *)num1)->virgule && ((struct nombre *)num2)->virgule){
			memcpy(&m1.entier, &((struct nombre *)num1)->virgule->mdix, sizeof(struct nbr));
			memcpy(&m2.entier, &((struct nombre *)num2)->virgule->mdix, sizeof(struct nbr));
			set = (equal(&m1, &m2) < 0) ? 2 : 1;
		}else{
			set = (((struct nombre *)num1)->virgule && !((struct nombre *)num2)->virgule) ? 1 : 2;
			if(((struct nombre *)num1)->virgule && !((struct nombre *)num2)->virgule)
				memcpy(&m1.entier, &((struct nombre *)num1)->virgule->mdix, sizeof(struct nbr));
			else
				if(!((struct nombre *)num1)->virgule && ((struct nombre *)num2)->virgule)
					memcpy(&m2.entier, &((struct nombre *)num2)->virgule->mdix, sizeof(struct nbr));
		}
	}
	re_blk = (_n1->entier.blk > _n2->entier.blk) ? _n1->entier.blk : _n2->entier.blk;
	re_bit = (_n1->entier.bit > _n2->entier.bit) ? _n1->entier.bit : _n2->entier.bit;
	re_bit++;
	if(re_bit > BITS){
		re_bit -= BITS;
		re_blk++;
		if(re_blk == 0){
			fprintf(stderr, "Resultat possiblement trop long\n");
			exit(EXIT_FAILURE);
		}
	}
	if(result == NULL)
		switch(blk){
			case OUI:
				if((sz = block_possible(re_blk, 1, 0, 0)) != 0){
					if((ptr = malloc(sz)) == NULL){
						perror("malloc()");
						exit(EXIT_FAILURE);
					}
					memset(ptr, 0, sz);
					r = ptr;
					r->entier.n = (long unsigned int *)(struct nombre *)(r+1);
					r->info |= 1;
					break;
				}
			default:
				if((r = calloc(1, sizeof(struct nombre))) == NULL){
					perror("calloc()");
					exit(EXIT_FAILURE);
				}
				if(re_blk+1 == 0){
					fprintf(stderr, "Nombre pouvant entre trop long\n");
					exit(EXIT_FAILURE);
				}
				if((r->entier.n = calloc(re_blk+1, sizeof(unsigned long int))) == NULL){
					perror("calloc()");
					exit(EXIT_FAILURE);
				}
				break;
		}
	else{
		r = result;
		r->entier.blk = r->entier.bit = 0;
	}
	memset(&in, 0, sizeof(struct bufferin));
	memset(&out, 0, sizeof(struct bufferout));
	memset(&bnbr1, 0, sizeof(struct bufnbr));
	memset(&bnbr2, 0, sizeof(struct bufnbr));
	bnbr1.read = bnbr2.read = BITS-1;
	bnbr1.n = _n1->entier.n;
	bnbr1.n_blk = _n1->entier.blk;
	bnbr1.n_bit = _n1->entier.bit;
	bnbr2.n = _n2->entier.n;
	bnbr2.n_blk = _n2->entier.blk;
	bnbr2.n_bit = _n2->entier.bit;
	out.buffer = r->entier.n;
	out.buf_blk = re_blk + (re_bit > 0);
	while(bnbr1.info == 0 || bnbr2.info == 0){
		read_in(&bnbr1);
		read_in(&bnbr2);
		resultat = bnbr1.buffer + bnbr2.buffer + retenue;
		if(bnbr1.info != 0 && bnbr2.info != 0){
			retenue = (bnbr1.write > bnbr2.write) ? bnbr1.write : bnbr2.write;
			in.bit = ((resultat&((unsigned long int)1 << retenue)) != 0) +retenue;
			in.buffer = resultat;
			retenue = 0;
		}else{
			resultat -= ((ret = (resultat&(unsigned long int)1<<(BITS-1)))) ? (retenue = 1)*ret : (retenue = 0)&0;
			in.bit = BITS-1;
			in.buffer = resultat;
		}
		write_in(&out, &in);
	}
	if(_n1 != n1 && _n2 != n2){
		destroy_nbr(_n1);
		destroy_nbr(_n2);
	}
	r->entier.blk = re_blk;
	r->entier.bit = re_bit;
	r->signe = (equal(r, &zero) != 0 && n1->signe == '-' && n2->signe == '-') ? '-' : '+';
	arrange(r);
	if(set){
		_r = lavirgule(r, (set == 1)? &n1->virgule->mdix : &n2->virgule->mdix, blk);
		destroy_nbr(r);
		r = _r;
	}
	return r;
}
void *soustraction(void *num1, void *num2, void *result, int blk){
	struct nombre *n1 = num1, *n2 = num2, *_n1, *_n2,*r = NULL, *_r, *n, zero = NBR, m1 = NBR, m2 = NBR;
	struct nbr *md;
	struct bufnbr bnbr1, bnbr2;
	struct bufferin in;
	struct bufferout out;
	unsigned long int re_blk, z = 0, *eblk, *vblk = NULL, sz, *d, *s;
	void *ptr;
	int re_bit, retenue = 0, signe = '+';
	int *ebit, *vbit = NULL, set = 0, bit, _ret, ret;
	if(n1->signe == '-' && 
		n2->signe != '-')
	{
		n1->signe = 0;
		r = addition(n1, n2, NULL, blk);
		r->signe = n1->signe = '-';
		return r;
	}else{
		if(n1->signe != '-' && n2->signe == '-'){
			n2->signe = 0;
			r = addition(n1, n2, NULL, blk);
			n2->signe = '-';
			return r;
		}else{
			if(n1->signe == '-' && n2->signe == '-'){
				n1->signe = n2->signe = 0;
				r = soustraction(n2, n1, NULL, blk);
				n1->signe = n2->signe = '-';
				return r;
			}
		}
	}
	zero.entier.n = &z;
	zero.entier.bit = 1;
	if(equal(n1,&zero) == 0 || equal(n2, &zero) == 0){
		if(equal(n1, &zero) == 0){
			eblk = &n2->entier.blk;
			ebit = &n2->entier.bit;
			if(n2->virgule){
				vbit = &n2->virgule->n.bit;
				vblk = &n2->virgule->n.blk;
				md = &n2->virgule->mdix;
			}
			n = n2;
			signe = (n2->signe != '-') ? '-' : '+';
		}else{
			eblk = &n1->entier.blk;
			ebit = &n1->entier.bit;
			if(n1->virgule){
				vbit = &n1->virgule->n.bit;
				vblk = &n1->virgule->n.blk;
				md = &n1->virgule->mdix;
			}
			n = n1;
			signe = (n1->signe == '-') ? '-' : '+';
		}
		switch(blk){
			case OUI:
				if((sz = block_possible(*eblk, 1, (vblk) ? * vblk : 0, (vbit) ? *vbit : 0)) != 0){
					if((ptr = malloc(sz)) == NULL){
						perror("malloc()");
						exit(EXIT_FAILURE);
					}
					memset(ptr, 0, sz);
					r = ptr;
					r->entier.n = (unsigned long int *)(r+1);
					r->info |= 1;
					if(vbit || vblk){
						r->virgule = (struct vnbr *)(unsigned long int *)(r->entier.n + (n->entier.blk + (n->entier.bit > 0)));
						r->virgule->n.n = (unsigned long int *)(struct vnbr *)(r->virgule+1);
					}
					break;
				}
			default:
				if((r = calloc(1, sizeof(struct nombre))) == NULL){
					perror("calloc()");
					exit(EXIT_FAILURE);
				}
				if((r->entier.n = calloc(*eblk+1, sizeof(unsigned long int))) == NULL){
					perror("calloc()");
					exit(EXIT_FAILURE);
				}
				if(vblk || vbit){
					if((r->virgule = calloc(1, sizeof(struct vnbr))) == NULL){
						perror("calloc()");
						exit(EXIT_FAILURE);
					}
					if((r->virgule->n.n = calloc(*vblk +1, sizeof(unsigned long int))) == NULL){
						perror("calloc()");
						exit(EXIT_FAILURE);
					}
				}
				break;
		}
		for(s = n->entier.n, d = r->entier.n, sz = n->entier.blk+1;sz > 0; sz--, *d = *s, d++,s++);;
		r->entier.bit = *ebit;
		r->entier.blk = *eblk;
		if(vblk || vbit){
			for(s = n->virgule->n.n, d = r->virgule->n.n, sz = n->virgule->n.blk+1;sz > 0; sz--, *d = *s, d++,s++);;
			r->virgule->n.bit = *vbit;
			r->virgule->n.blk = *vblk;
			memcpy(&r->virgule->mdix, md, sizeof(struct nbr));
			if((r->virgule->mdix.n = calloc(md->blk+1, sizeof(unsigned long int))) == NULL){
				perror("calloc()");
				exit(EXIT_FAILURE);
			}
			for(s = md->n, d = r->virgule->mdix.n, sz = md->blk+1;sz > 0; sz--, *d = *s, d++,s++);;
		}
		r->signe = (equal(r, &zero)) ? signe : '+';
		return r;
	}
	n1 = n2 = NULL;
	vers_entier_aligne(num1, num2, (void **)&n1, (void **)&n2, blk);
	if(!n1 && !n2){
		n1 = num1;
		n2 = num2;
	}else{
		if(result){
			fprintf(stderr, "soustraction: operation non permise (result != NULL)\n");
			exit(EXIT_FAILURE);
		}
		if(((struct nombre *)num1)->virgule && ((struct nombre *)num2)->virgule){
			memcpy(&m1.entier, &((struct nombre *)num1)->virgule->mdix, sizeof(struct nbr));
			memcpy(&m2.entier, &((struct nombre *)num2)->virgule->mdix, sizeof(struct nbr));
			set = (equal(&m1, &m2) < 0) ? 2 : 1;
		}else{
			set = (((struct nombre *)num1)->virgule && !((struct nombre *)num2)->virgule) ? 1 : 2;
			if(((struct nombre *)num1)->virgule && !((struct nombre *)num2)->virgule)
				memcpy(&m1.entier, &((struct nombre *)num1)->virgule->mdix, sizeof(struct nbr));
			else
				if(!((struct nombre *)num1)->virgule && ((struct nombre *)num2)->virgule)
					memcpy(&m2.entier, &((struct nombre *)num2)->virgule->mdix, sizeof(struct nbr));
		}
	}
	if(equal(n1, n2) < 0){
		_n1 = n2;
		n2 = _n2 = n1;
		n1 = _n1;
		_n1 = num2;
		_n2 = num1;
		signe = '-';
	}else{
		_n1 = num1;
		_n2 = num2;
	}
	re_blk = (n1->entier.blk > n2->entier.blk) ? n1->entier.blk : n2->entier.blk;
	re_bit = (n1->entier.bit > n2->entier.bit) ? n1->entier.bit : n2->entier.bit;
	if(result == NULL)
		switch(blk){
			case OUI:
				if((sz = block_possible(re_blk, 1, 0, 0)) != 0){
					if((ptr = malloc(sz)) == NULL){
						perror("malloc()");
						exit(EXIT_FAILURE);
					}
					memset(ptr, 0, sz);
					r = ptr;
					r->entier.n = (long unsigned int *)(r+1);
					r->info |= 1;
					break;
				}
			default:
				if((r = calloc(1, sizeof(struct nombre))) == NULL){
					perror("calloc()");
					exit(EXIT_FAILURE);
				}
				if(re_blk + 1 == 0){
					fprintf(stderr, "Nombre pouvant etre trop grand\n");
					exit(EXIT_FAILURE);
				}
				if((r->entier.n = calloc(re_blk+1, sizeof(unsigned long int))) == NULL){
					perror("calloc()");
					exit(EXIT_FAILURE);
				}
				break;
		}
	else
		r = result;
	r->entier.blk = re_blk;
	r->entier.bit = re_bit;
	memset(&in, 0, sizeof(struct bufferin));
	memset(&out, 0, sizeof(struct bufferout));
	memset(&bnbr1, 0, sizeof(struct bufnbr));
	memset(&bnbr2, 0, sizeof(struct bufnbr));
	bnbr1.read = bnbr2.read = BITS;
	bnbr1.n = n1->entier.n;
	bnbr1.n_blk = n1->entier.blk;
	bnbr1.n_bit = n1->entier.bit;
	bnbr2.n = n2->entier.n;
	bnbr2.n_blk = n2->entier.blk;
	bnbr2.n_bit = n2->entier.bit;
	out.buffer = r->entier.n;
	out.buf_blk = re_blk + (re_bit > 0);
	_ret = ret = retenue = 0;
	while(bnbr1.info == 0 || bnbr2.info == 0){
		for(read_in(&bnbr1),
			read_in(&bnbr2),
			in.buffer = bit = 0,
			in.bit = (bnbr1.write > bnbr2.write) ? bnbr1.write : bnbr2.write;
			bit < in.bit;
			in.buffer |= ((ret = ((bnbr1.buffer&((unsigned long int)1<<bit)) == 0 && (bnbr2.buffer&((unsigned long int)1<<bit)) != 0))
							|| (_ret = ((bnbr1.buffer&((unsigned long int)1<<bit)) != 0 && (bnbr2.buffer&((unsigned long int)1<<bit)) == 0))
						) ? (unsigned long int)!retenue<<bit
						: (unsigned long int)retenue<<bit,
			retenue = (ret == 1 || _ret == 1) ? ret : retenue,
			bit++
		);;
		write_in(&out, &in);
	}
	r->signe = signe;
	arrange(r);
	if(set){
		if(r->signe == '-'){
			signe = '-';
			r->signe = '+';
		}else
			signe = '+';
		_r = lavirgule(r, (set == 1)? &m1.entier : &m2.entier, blk);
		destroy_nbr(n1);
		destroy_nbr(n2);
		destroy_nbr(r);
		r = _r;
		r->signe = signe;
	}else
		r->signe = signe;
	return r;
}
void *multiplication(void *num1, void *num2, void *result, int blk){
	struct nombre *n1 = num1, *n2 = num2, *_n1 = NULL, *_n2 = NULL, *r = NULL, *n, sp = NBR, sp_n, md1 = NBR, md2 = NBR, *md = NULL, *_r;
	struct bufnbr bnbr1, bnbr2, pr, pr_;
	struct bufferin in;
	struct bufferout out, outcpy;
	void *ptr;
	const unsigned long int resultat_mask = 0x7FFFFFFF;
	unsigned long int retenue, resultat, re_blk, sp_ = 0, sz, *_s, *_d;
	int re_bit, neg1 = 0, neg2 = 0, set = 0;
	sp.entier.n = &sp_;
	sp.entier.bit = 1;
	neg1 = n1->signe;
	n1->signe = 0;
	neg2 = n2->signe;
	n2->signe = 0;
	if(equal(n1, &sp) == 0 || equal(n2, &sp) == 0){
		if(result == NULL)
			switch(blk){
				case OUI:
					if((ptr = malloc(sizeof(struct nombre) + sizeof(unsigned long int))) == NULL){
						perror("malloc()");
						exit(EXIT_FAILURE);
					}
					memset(ptr, 0, sizeof(struct nombre) + sizeof(unsigned long int));
					r = ptr;
					r->info |= 1;
					r->entier.n = (long unsigned int *)(r+1);
					break;
				default:
					if((r = calloc(1, sizeof(struct nombre))) == NULL){
						perror("calloc()");
						exit(EXIT_FAILURE);
					}
					if((r->entier.n = calloc(1, sizeof(unsigned long int))) == NULL){
						perror("calloc()");
						exit(EXIT_FAILURE);
					}
					break;
			}
		else
			r = result;
		*r->entier.n = 0;
		r->entier.bit = 1;
		n2->signe = neg2;
		n1->signe = neg1;
		r->signe = '+';
		return r;
	}
	n1->signe = neg1;
	n2->signe = neg2;
	*sp.entier.n = 1;
	memcpy(&sp_n, &sp, sizeof(struct nombre));
	sp_n.signe = '-';
	if(equal(n1, &sp) == 0 || equal(n2, &sp) == 0 || equal(n1, &sp_n) == 0 || equal(n2, &sp_n) == 0){
		if(equal(n1, &sp) == 0 || equal(n1, &sp_n) == 0)
			n = n2;
		else
			n = n1;
		if(result == NULL)
			switch(blk){
				case OUI:
					if((sz = block_possible(n->entier.blk, n->entier.bit, (n->virgule) ? n->virgule->n.blk : 0, (n->virgule) ? n->virgule->n.bit : 0)) != 0){
						if((ptr = malloc(sz)) == NULL){
							perror("malloc()");
							exit(EXIT_FAILURE);
						}
						memset(ptr, 0, sz);
						r = ptr;
						r->entier.n = (long unsigned int *)(struct nombre *)(r+1);
						r->info |= 1;
						if(n->virgule){
							*r->entier.n = 0;
							r->virgule = (struct vnbr *)(unsigned long int *)(r->entier.n + (n->entier.blk + (n->entier.bit > 0)));
							r->virgule->n.n = (unsigned long int *)(struct vnbr *)(r->virgule+1);
						}
						break;
					}
				default:
					if((r = calloc(1, sizeof(struct nombre))) == NULL){
						perror("calloc()");
						exit(EXIT_FAILURE);
					}
					if((r->entier.n = calloc(n->entier.blk+1, sizeof(unsigned long int))) == NULL){
						perror("calloc()");
						exit(EXIT_FAILURE);
					}
					if(n->virgule){
						if((r->virgule = calloc(1, sizeof(struct vnbr))) == NULL){
							perror("calloc()");
							exit(EXIT_FAILURE);
						}
						if((r->virgule->n.n = calloc(n->virgule->n.blk +1, sizeof(unsigned long int))) == NULL){
							perror("calloc()");
							exit(EXIT_FAILURE);
						}
					}
					break;
			}
		else
			r = result;
		for(_s = n->entier.n, _d = r->entier.n, sz = n->entier.blk+1; sz > 0; sz--, *_d = *_s, _d++, _s++);
		r->entier.bit = n->entier.bit;
		r->entier.blk = n->entier.blk;
		if(n->virgule){
			for(_s = n->virgule->n.n, _d = r->virgule->n.n, sz = n->virgule->n.blk+1; sz > 0; sz--, *_d = *_s, _d++, _s++);
			r->virgule->n.bit = n->virgule->n.bit;
			r->virgule->n.blk = n->virgule->n.blk;
			memcpy(&r->virgule->mdix, &n->virgule->mdix, sizeof(struct nbr));
			if((r->virgule->mdix.n = calloc(n->virgule->mdix.blk +1, sizeof(unsigned long int))) == NULL){
				perror("calloc()");
				exit(EXIT_FAILURE);
			}
			for(_s = n->virgule->mdix.n, _d = r->virgule->mdix.n, sz = n->virgule->mdix.blk+1; sz > 0; sz--, *_d = *_s, _d++, _s++);
		}
		r->signe = ((n1->signe == '-' && n2->signe == '-')|| (n1->signe != '-' && n2->signe != '-')) ? '+' : '-';
		return r;
	}
	if(n1->virgule || n2->virgule){
		if(result){
			fprintf(stderr, "Multiplication: operation impossible (result != NULL)\n");
			exit(EXIT_FAILURE);
		}
		if(n1->virgule && n2->virgule){
			set = 1;
			memcpy(&md1.entier, &n1->virgule->mdix, sizeof(struct nbr));
			memcpy(&md2.entier, &n2->virgule->mdix, sizeof(struct nbr));
			_n1 = vers_entier(n1, n1->info&OUI);
			_n2 = vers_entier(n2, n2->info&OUI);
			md = multiplication(&md1, &md2, NULL, blk);
		}else{
			if(n1->virgule && !n2->virgule){
				_n1 = vers_entier(n1, n1->info&OUI);
				_n2 = n2;
				memcpy(&md1.entier, &n1->virgule->mdix, sizeof(struct nbr));
				md = &md1;
			}else{
				_n2 = vers_entier(n2, n2->info&OUI);
				_n1 = n1;
				memcpy(&md2.entier, &n2->virgule->mdix, sizeof(struct nbr));
				md = &md2;
			}
		}
	}else{
		_n1 = n1;
		_n2 = n2;
	}
	re_blk = _n1->entier.blk + _n2->entier.blk;
	re_bit = _n1->entier.bit + _n2->entier.bit;
	while(re_bit > BITS){
		re_bit -= BITS;
		re_blk++;
	}
	if(result == NULL)
		switch(blk){
			case OUI:
				if((sz = block_possible(re_blk, 1, 0, 0)) != 0){
					if((ptr = malloc(sz)) == NULL){
						perror("malloc()");
						exit(EXIT_FAILURE);
					}
					memset(ptr, 0, sz);
					r = ptr;
					r->entier.n = (long unsigned int *)(r+1);
					r->info |= 1;
					break;
				}
			default:
				if((r = calloc(1, sizeof(struct nombre))) == NULL){
					perror("calloc()");
					exit(EXIT_FAILURE);
				}
				if(re_blk + 1 == 0){
					fprintf(stderr, "Nombre pouvant etre trop grand\n");
					exit(EXIT_FAILURE);
				}
				if((r->entier.n = calloc(re_blk +1, sizeof(unsigned long int))) == NULL){
					perror("calloc()");
					exit(EXIT_FAILURE);
				}
				break;
		}
	else
		r = result;
	r->entier.bit = re_bit;
	r->entier.blk = re_blk;
	r->signe = (_n1->signe != _n2->signe && (_n1->signe == '-' || _n2->signe == '-')) ? '-' : '+';
	memset(&in, 0, sizeof(struct bufferin));
	memset(&out, 0, sizeof(struct bufferout));
	memset(&bnbr1, 0, sizeof(struct bufnbr));
	memset(&bnbr2, 0, sizeof(struct bufnbr));
	memset(&pr, 0, sizeof(struct bufnbr));
	bnbr1.read = bnbr2.read = (BITS/2 -1);
	bnbr1.n = _n1->entier.n;
	bnbr1.n_blk = _n1->entier.blk;
	bnbr1.n_bit = _n1->entier.bit;
	bnbr2.n = _n2->entier.n;
	bnbr2.n_blk = _n2->entier.blk;
	bnbr2.n_bit = _n2->entier.bit;
	pr.n = r->entier.n;
	pr.n_blk = re_blk;
	pr.n_bit = re_bit;
	pr.read = bnbr1.read;
	out.buffer = r->entier.n;
	out.buf_blk = re_blk + (re_bit > 0);
	re_bit = re_blk = 0;
	for(read_in(&pr);
		bnbr2.info == 0;
		read_in(&pr)
	){
		read_in(&bnbr2);
		bnbr1.info = bnbr1.bit = bnbr1.c_bit = bnbr1.blk = 0;
		for(retenue = 0,
			memcpy(&outcpy, &out, sizeof(struct bufferout)),
			memcpy(&pr_, &pr, sizeof(struct bufnbr));
			bnbr1.info == 0;
			read_in(&pr_),
			out.info = 0
		){
			read_in(&bnbr1);
			resultat = bnbr1.buffer * bnbr2.buffer + retenue + pr_.buffer;
			in.bit = bnbr1.write + bnbr2.write;
			if(bnbr2.info == 0 && bnbr1.info == 0){
				in.bit = 31;
				retenue = resultat >> (BITS/2 - 1);
				resultat &= resultat_mask;
			}else
				if((retenue = (bnbr1.info == 0)))
					if((retenue = (in.bit > BITS/2 - 1))){
						in.bit = (BITS/2 -1);
						retenue = resultat >> (BITS/2 - 1);
						resultat &= resultat_mask;
					}
			in.buffer = resultat;
			write_in(&out, &in);
		}
		memcpy(&out, &outcpy, sizeof(struct bufferout));
		out.bit += (BITS/2-1);
		if(out.bit > BITS-1){
			out.bit -= BITS;
			out.blk++;
		}
	}
	arrange(r);
	if(md){
		_r = lavirgule(r, &md->entier, r->info&OUI);
		destroy_nbr(r);
		r = _r;
		if(set)
			destroy_nbr(md);
		if(_n1 != n1)
			destroy_nbr(_n1);
		if(_n2 != n2)
			destroy_nbr(_n2);
	}
	return r;
}
void *gauche(void *num, void *result, char bit, int blk){
	struct nombre *n = num, *r, zero = NBR;
	struct bufnbr buf;
	struct bufferout out;
	int ibit;
	unsigned long int *oblk, blk_, iblk, h_bit, l_bit, z = 0, sz, *d, *s;
	void *ptr;
	if(bit > BITS-1 || bit < 0 || n->virgule)
		return NULL;
	iblk = n->entier.blk;
	ibit = n->entier.bit+bit;
	if(ibit > BITS){
		ibit-=BITS;
		iblk++;
	}
	zero.entier.n = &z;
	zero.entier.bit = 1;
	memset(&buf, 0, sizeof(struct bufnbr));
	memset(&out, 0, sizeof(struct bufferout));
	if(bit == 0 || equal(n, &zero) == 0){
		if(result == NULL){
			switch(blk){
				case OUI:
					if((sz = block_possible(iblk, 1, 0, 0)) != 0){
						if((ptr = malloc(sz)) == NULL){
							perror("malloc()");
							exit(EXIT_FAILURE);
						}
						memset(ptr, 0, sz);
						r = ptr;
						r->entier.n = (unsigned long int *)(r+1);
						r->info |= 1;
						break;
					}
				default:
					if((r = calloc(1, sizeof(struct nombre))) == NULL){
						perror("calloc()");
						exit(EXIT_FAILURE);
					}
					if((r->entier.n = calloc(iblk+1, sizeof(unsigned long int))) == NULL){
						perror("calloc()");
						exit(EXIT_FAILURE);
					}
					break;
			}
			buf.n_bit = r->entier.bit = ibit;
			buf.n_blk = r->entier.blk = iblk;
		}else{
			r = result;
			buf.n_bit = r->entier.bit;
			buf.n_blk = r->entier.blk;
		}
		for(s = n->entier.n, d = r->entier.n, sz = n->entier.blk+1; sz > 0; sz--, *d = *s, d++, s++);
		r->signe = n->signe;
		return r;
	}
	if(result == NULL){
		iblk++;
		switch(blk){
			case OUI:
				if((sz = block_possible(iblk, 1, 0, 0)) != 0){
					if((ptr = malloc(sz)) == NULL){
						perror("malloc()");
						exit(EXIT_FAILURE);
					}
					memset(ptr, 0, sz);
					r = ptr;
					r->info |= 1;
					r->entier.n = (unsigned long int *)(r+1);
					break;
				}
			default:
				if((r = calloc(1, sizeof(struct nombre))) == NULL){
					perror("calloc()");
					exit(EXIT_FAILURE);
				}
				if(iblk + 1 == 0){
					fprintf(stderr, "Nombre pouvant etre trop grand\n");
					exit(EXIT_FAILURE);
				}
				if((r->entier.n = calloc(iblk+1, sizeof(unsigned long int))) == NULL){
					perror("calloc()");
					exit(EXIT_FAILURE);
				}
				break;
		}
		r->entier.blk = iblk+1;
	}else
		r = result;
	for(s = n->entier.n, d = r->entier.n, sz = n->entier.blk + 1; sz > 0; sz--, *d = *s, s++, d++);;
	for(blk_ = n->entier.blk+1, oblk = r->entier.n +n->entier.blk;blk_ > 0;blk_--, oblk--){
		h_bit = *oblk >> (BITS-bit);
		l_bit = *oblk << bit;
		if(blk_ == n->entier.blk+1 && h_bit !=0)
			*(oblk+1) |= h_bit;
		else
			if(blk_ != n->entier.blk+1)
				*(oblk+1) |= h_bit;
		*oblk = l_bit;
	}
	r->entier.blk = iblk;
	r->entier.bit = ibit;
	r->signe = n->signe;
	if(r->entier.blk == 0 && r->entier.bit == 0)
		r->entier.bit = 1;
	r->signe = n->signe;
	arrange(r);
	return r;
}
void *droite(void *num, void *result, char bit, int correct, int blk){
	struct nombre *n = num, *r, zero = NBR, v = NBR, *_v;
	int ibit;
	unsigned long int *oblk, blk_, nblk, iblk, h_bit, l_bit, z = 0, sz, *_s, *_d;
	void *ptr, *s, *d;
	if(bit > BITS-1 || bit < 0 || n->virgule)
		return NULL;
	zero.entier.n = &z;
	zero.entier.bit = 1;
	iblk = n->entier.blk;
	ibit = n->entier.bit;
	zero.entier.n = &z;
	zero.entier.bit = 1;
	if(bit == 0 || equal(n, &zero) == 0){
		if(result == NULL){
			switch(blk){
				case OUI:
					if((sz = block_possible(iblk, 1, 0, 0)) != 0){
						if((ptr = malloc(sz)) == NULL){
							perror("malloc()");
							exit(EXIT_FAILURE);
						}
						memset(ptr, 0, sz);
						r = ptr;
						r->entier.n = (unsigned long int *)(r+1);
						r->info |= 1;
						break;
					}
				default:
					if((r = calloc(1, sizeof(struct nombre))) == NULL){
						perror("calloc()");
						exit(EXIT_FAILURE);
					}
					if((r->entier.n = calloc(iblk+1, sizeof(unsigned long int))) == NULL){
						perror("calloc()");
						exit(EXIT_FAILURE);
					}
					break;
			}
			r->entier.bit = ibit;
			r->entier.blk = iblk;
		}else
			r = result;
		for(_s = n->entier.n, _d = r->entier.n, sz = n->entier.blk+1; sz > 0; sz--, *_d = *_s, _d++, _s++);
		r->signe = n->signe;
		return r;
	}
	if(result == NULL){
		switch(blk){
			case OUI:
				if((sz = block_possible(iblk, 1, 0, 1)) != 0){
					if((ptr = malloc(sz)) == NULL){
						perror("malloc()");
						exit(EXIT_FAILURE);
					}
					memset(ptr, 0, sz);
					r = ptr;
					r->info |= 1;
					r->entier.n = (unsigned long int *)(r+1);
					r->virgule = (struct vnbr *)(unsigned long int *)(r->entier.n + (iblk + (ibit > 0)));
					r->virgule->n.n = (unsigned long int *)(struct vnbr *)(r->virgule + 1);
					break;
				}
			default:
				if((r = calloc(1, sizeof(struct nombre))) == NULL){
					perror("calloc()");
					exit(EXIT_FAILURE);
				}
				if(iblk + 1 == 0){
					fprintf(stderr, "Nombre pouvant etre trop grand\n");
					exit(EXIT_FAILURE);
				}
				if((r->entier.n = calloc(iblk+1, sizeof(unsigned long int))) == NULL){
					perror("calloc()");
					exit(EXIT_FAILURE);
				}
				if((r->virgule = calloc(1, sizeof(struct vnbr))) == NULL){
					perror("calloc()");
					exit(EXIT_FAILURE);
				}
				if((r->virgule->n.n = calloc(1, sizeof(unsigned long int))) == NULL){
					perror("calloc()");
					exit(EXIT_FAILURE);
				}
				break;
		}
		r->virgule->n.bit = BITS;
	}else
		r = result;
	if(n->entier.blk + 1 == 0){
		fprintf(stderr, "Nombre pouvant etre trop grand\n");
		exit(EXIT_FAILURE);
	}
	for(sz = n->entier.blk + 1, d = r->entier.n, s = n->entier.n; sz > 0; sz--, d++, s++)
		*((unsigned long int *)d) = *((unsigned long int *)s);
	memcpy(r->entier.n, n->entier.n, (n->entier.blk+1) *sizeof(unsigned long int));
	for(blk_ = n->entier.blk,
		oblk = r->entier.n +n->entier.blk,
		nblk = *oblk,
		*oblk = 0;
		blk_ > 0;
		blk_--,
		oblk--
	){
		h_bit = nblk >> bit;
		l_bit = nblk << (BITS-bit);
		nblk = *(oblk-1);
		*(oblk-1) = 0;
		*oblk |= h_bit;
		*(oblk-1) = l_bit;
	}
	h_bit = nblk >> bit;
	l_bit = nblk << (BITS-bit);
	*oblk |= h_bit;
	if(l_bit != 0)
		*r->virgule->n.n = (correct) ? l_bit >> (BITS-bit) : l_bit;
	r->entier.blk = iblk;
	r->entier.bit = ibit;
	r->signe = n->signe;
	if(r->entier.blk == 0 && r->entier.bit == 0)
		r->entier.bit = 1;
	r->signe = n->signe;
	arrange(r);
	if(r->virgule){
		if(*r->virgule->n.n == 0){
			switch(r->info&OUI){
				case OUI:
					r->virgule = NULL;
					break;
				default:
					free(r->virgule->n.n);
					free(r->virgule);
					r->virgule = NULL;
					break;
			}
		}else{
			memcpy(&v.entier, &r->virgule->n, sizeof(struct nbr));
			_v = vers_base10_len(&v);
			memcpy(&r->virgule->mdix, &_v->entier, sizeof(struct nbr));
			free(_v);
		}
	}
	return r;
}
void *duplic_nbr(void *num, void *result, int blk){
	struct nombre *n = num, *r;
	struct bufnbr buf;
	struct bufferin in;
	struct bufferout out;
	void *ptr;
	unsigned long int sz, *d, *s;
	memset(&buf, 0, sizeof(struct bufnbr));
	memset(&out, 0, sizeof(struct bufferout));
	if(result == NULL){
		switch(blk){
			case OUI:
				if((sz = block_possible(n->entier.blk, n->entier.bit, (n->virgule) ? n->virgule->n.blk : 0, (n->virgule) ? n->virgule->n.bit : 0)) != 0){
					if((ptr = malloc(sz)) == NULL){
						perror("malloc()");
						exit(EXIT_FAILURE);
					}
					memset(ptr, 0, sz);
					r = ptr;
					r->entier.n = (unsigned long int *)(struct nombre *)(r+1);
					if(n->virgule){
						r->virgule = (struct vnbr *)(unsigned long int *)(r->entier.n + n->entier.blk + 1);
						r->virgule->n.n = (unsigned long int *)((struct vnbr *)(r->virgule + 1));
					}
					r->info |= blk;
					break;
				}
			default:
				if((r = calloc(1, sizeof(struct nombre))) == NULL){
					perror("calloc()");
					exit(EXIT_FAILURE);
				}
				if(n->entier.blk + 1 == 0){
					fprintf(stderr, "Nombre pouvant etre trop grand\n");
					exit(EXIT_FAILURE);
				}
				if((r->entier.n = calloc((n->entier.blk + 1), sizeof(unsigned long int))) == NULL){
					perror("calloc()");
					exit(EXIT_FAILURE);
				}
				if(n->virgule){
					if((r->virgule = calloc(1, sizeof(struct vnbr))) == NULL){
						perror("calloc()");
						exit(EXIT_FAILURE);
					}
					if(n->virgule->n.blk + 1 == 0){
						fprintf(stderr, "Nombre pouvant etre trop grand\n");
						exit(EXIT_FAILURE);
					}
					if((r->virgule->n.n = calloc((n->virgule->n.blk + 1), sizeof(unsigned long int))) == NULL){
						perror("calloc()");
						exit(EXIT_FAILURE);
					}
				}
				break;
		}
		buf.n_bit = r->entier.bit = n->entier.bit;
		buf.n_blk = r->entier.blk = n->entier.blk;
	}else{
		r = result;
		buf.n_bit = r->entier.bit = n->entier.bit;
		buf.n_blk = r->entier.bit = n->entier.blk;
	}
	buf.read = BITS;
	buf.n = n->entier.n;
	out.buf_blk = buf.n_blk+(buf.n_bit > 0);
	out.buffer = r->entier.n;
	while(buf.info == 0){
		read_in(&buf);
		in.bit = buf.write;
		in.buffer = buf.buffer;
		write_in(&out, &in);
	}
	if(n->virgule){
		memset(&buf, 0, sizeof(struct bufnbr));
		memset(&out, 0, sizeof(struct bufferout));
		buf.n_bit = r->virgule->n.bit = n->virgule->n.bit;
		buf.n_blk = r->virgule->n.blk = n->virgule->n.blk;
		buf.read = BITS;
		buf.n = n->virgule->n.n;
		out.buf_blk = buf.n_blk+(buf.n_bit > 0);
		out.buffer = r->virgule->n.n;
		while(buf.info == 0){
			read_in(&buf);
			in.bit = buf.write;
			in.buffer = buf.buffer;
			write_in(&out, &in);
		}
		memcpy(&r->virgule->mdix, &n->virgule->mdix, sizeof(struct nbr));
		if((r->virgule->mdix.n = calloc(n->virgule->mdix.blk+1, sizeof(unsigned long int))) == NULL){
			perror("calloc()");
			exit(EXIT_FAILURE);
		}
		for(s = n->virgule->mdix.n, d = r->virgule->mdix.n, sz = n->virgule->mdix.blk+1;sz > 0;sz--, *d = *s, d++, s++);;
	}
	r->signe = n->signe;
	return r;
}
void arrange(void *num){
	struct nombre *n1 = num;
	for(;
		n1->entier.blk > 0 && n1->entier.n[n1->entier.blk] == 0;
		n1->entier.blk--
	);;
	for(n1->entier.bit = BITS;n1->entier.bit > 0; n1->entier.bit--)
		if((n1->entier.n[n1->entier.blk]&((unsigned long int)1<<(n1->entier.bit-1))) != 0)
			break;
	if(n1->entier.bit == 0 && n1->entier.blk == 0)
		n1->entier.bit++;
}
void *division(void *num1, void *num2, void *virgule, int arrondi, int blk, void *result, void *reste){
	struct nombre *n1 = num1, *n2 = num2, *_n1 = NULL, *_n2 = NULL, zero = NBR, _reste[2] = { NBR, NBR }, *__reste = _reste, *r, *r_, md1 = NBR/*, b10 = NBR*/;
	struct bufnbr bnbr;
	unsigned long int null = 0, sz, *_d, *_s, blk_;
	int neg1 = 0, neg2 = 0, set = 0, bit_, ret, vir = 0, vir_ = 0;
	void *ptr, *preste[2];
	zero.entier.n = &null;
	zero.entier.bit = 1;
	neg1 = n1->signe;
	n1->signe = 0;
	neg2 = n2->signe;
	n2->signe = 0;
	if(equal(n2, &zero) == 0){
		n1->signe = neg1;
		fprintf(stderr, "Division par 0\n");
		return NULL;
	}
	if(equal(n1,&zero) != 0)
		n1->signe = neg1;
	n2->signe = neg2;
	if(n2->virgule){
		_n2 = vers_entier(n2, blk);
		memcpy(&md1.entier, &n2->virgule->mdix, sizeof(struct nbr));
		_n1 = multiplication(n1, &md1, NULL, blk);
	}else{
		_n1 = n1;
		_n2 = n2;
	}
	if(virgule && equal(virgule, &zero) != 0){
		vir_ = 1;
		if(_n1 != n1)
			vir = 1;
		n1 = multiplication(_n1, virgule, NULL, blk);
		if(vir)
			destroy_nbr(_n1);
		_n1 = n1;
	}
	if(!reste){
		for(set = 2; set > 0; set--)
			if((preste[set-1] = _reste[set-1].entier.n = calloc((_n1->entier.blk + 1), sizeof(unsigned long int))) == NULL){
				perror("calloc()");
				exit(EXIT_FAILURE);
			}
	}else
		__reste = reste;
	if(!result){
		switch(blk){
			case OUI:
				if((sz = block_possible(_n1->entier.blk, 1, 0, 0)) != 0){
					if((ptr = malloc(sz)) == NULL){
						perror("malloc()");
						exit(EXIT_FAILURE);
					}
					memset(ptr, 0, sz);
					r = ptr;
					r->entier.n = (unsigned long int *)(r+1);
					r->info |= 1;
					break;
				}
			default:
				if((r = calloc(1, sizeof(struct nombre))) == NULL){
					perror("calloc()");
					exit(EXIT_FAILURE);
				}
				if((r->entier.n = calloc(_n1->entier.blk+1, sizeof(unsigned long int))) == NULL){
					perror("calloc()");
					exit(EXIT_FAILURE);
				}
				break;
		}
	}else{
		if((virgule && equal(virgule,&zero) != 0) || __reste == _reste){
			fprintf(stderr, "Division: Operation non permise (virgule != 0 ou reste == NULL)\n");
			exit(EXIT_FAILURE);
		}
		r = result;
	}
	_n2->signe = _n1->signe = '+';
	memset(&bnbr, 0, sizeof(struct bufnbr));
	bnbr.n = _n1->entier.n;
	bnbr.n_blk = bnbr.blk = _n1->entier.blk;
	bnbr.c_bit = bnbr.bit = bnbr.n_bit = _n1->entier.bit;
	blk_ = _n2->entier.blk;
	bit_ = _n2->entier.bit;
	while(bnbr.info == 0){
		while((ret = equal(&__reste[set], _n2)) < 0 && bnbr.info == 0){
			switch((bit_ > 0) | ((bnbr.blk > 0 && blk_ > 0) << 1)){
				case 3:
				case 2:
					bnbr.read = BITS-1;
					bit_++;
					if(bit_ == BITS)
						bit_ -= (BITS-1);
					else
						blk_--;
					break;
				case 1:
					bnbr.read = (bit_ == BITS) ? BITS-1 : bit_;
					bit_ = (bit_ == BITS) ? 0 : 0;
					break;
				default:
					bnbr.read = 1;
					break;
			}
			iread_in(&bnbr);
			gauche(&__reste[set], &__reste[set], bnbr.write, blk);
			__reste[set].entier.bit = (__reste[set].entier.blk == 0 && *__reste[set].entier.n == 0)
									? bnbr.write : __reste[set].entier.bit;
			*__reste[set].entier.n |= bnbr.buffer;
			gauche(r, r, bnbr.write, blk);
		}
		if(ret >= 0){
				if(_n2->virgule){
					printf("FATAL1\n");
					exit(EXIT_FAILURE);
				}
				if(__reste[set].virgule){
					printf("FATAL2\n");
					exit(EXIT_FAILURE);
				}
				soustraction(&__reste[set], _n2, &__reste[!set], blk);
				for(sz = __reste[set].entier.blk+1, _d = __reste[set].entier.n; sz > 0;sz--, *_d = 0, _d++);;
				__reste[set].entier.blk = __reste[set].entier.bit = 0;
				set = !set;
				*r->entier.n |= 1;
				bit_ = _n2->entier.bit - __reste[set].entier.bit;
				blk_ = _n2->entier.blk - __reste[set].entier.blk;
		}
	}
	if(reste == NULL)
		for(set = 0; set < 2; set++)
			free(preste[set]);
	else{
		if(set == 1){
			__reste[!set].entier.bit = __reste[set].entier.bit;
			__reste[!set].entier.blk = __reste[set].entier.blk;
			for(sz = __reste[set].entier.blk +1, _d = __reste[!set].entier.n, _s = __reste[set].entier.n; sz > 0; sz--, *_d = *_s,_d++, _s++);;
		}
	}
	n1->signe = neg1;
	n2->signe = neg2;
	r->entier.bit += (r->entier.bit == 0);
	r->signe = (neg1 != neg2 && (neg1 == '-' || neg2 == '-')) ? '-' : '+';
	arrange(r);
	if(vir_){
		r_ = lavirgule(r, &((struct nombre *)virgule)->entier, blk);
		destroy_nbr(r);
		r = r_;
		//destroy_nbr(_n1);
		//_n1 = NULL;
	}
	if(vir_ || _n1 != n1)
		destroy_nbr(_n1);
	if(_n2 != n2)
		destroy_nbr(_n2);
	return r;
}
void *modulo(void *num1, void *num2, void *virgule, int blk){
	struct nombre *n1 = NULL, *n2 = NULL, *d, *m, *s;
	n1 = num1;
	n2 = num2;
	d = division(n1, n2, virgule, NON, blk, NULL, NULL);
	if(d == NULL){
		if(n1){
			if(n1 != num1)
				destroy_nbr(n1);
			if(n2 != num2)
				destroy_nbr(n2);
		}
		return NULL;
	}
	m = multiplication(d, n2, NULL, blk);
	s = soustraction(n1, m, NULL, blk);
	arrange(s);
	destroy_nbr(d);
	destroy_nbr(m);
	return s;
}
void *vers_base10_len(void *num){
	struct nombre *n, *d, dix = NBR, zero = NBR, un = NBR, *_d = &un, _r[2] = { NBR, NBR }, cpy[2] = { NBR, NBR }, reste[2] = { NBR, NBR }, result[2] = { NBR, NBR };
	const unsigned long int _dix = 10, _zero = 0, _un = 1;
	unsigned long int iblk, *rlui[2] = { NULL, NULL }, *_reste[2] = { NULL, NULL },*_rlui, sz, _sz, __sz, nsz, *d_, *s_, lblk;
	int _ibit, ibit, lbit, set = 0, i;
	if(((struct nombre *)num)->virgule)
		return 0;
	dix.entier.n = (unsigned long int *)&_dix;
	dix.entier.bit = 4;
	zero.entier.n = (unsigned long int *)&_zero;
	zero.entier.bit = 1;
	ibit = ((struct nombre *)num)->entier.bit;
	iblk = ((struct nombre *)num)->entier.blk;
	un.entier.n = (unsigned long int *)&_un;
	un.entier.bit = 1;
	for(sz = iblk+1, i = 0; i < 2; i++){
		if((rlui[i] = calloc(sz, sizeof(unsigned long int))) == NULL){
			perror("calloc()");
			exit(EXIT_FAILURE);
		}
		if((_reste[i] = calloc(sz, sizeof(unsigned long int))) == NULL){
			perror("calloc()");
			exit(EXIT_FAILURE);
		}
		reste[i].entier.n = _reste[i];
		if((result[i].entier.n = calloc(sz, sizeof(unsigned long int))) == NULL){
			perror("calloc()");
			exit(EXIT_FAILURE);
		}
		cpy[i].entier.n = rlui[i];
		cpy[i].info = OUI;
		memcpy(&_r[i], &cpy[i], sizeof(struct nombre));
	}
	n = duplic_nbr(num, &result[0], OUI);
	n->entier.blk = ((struct nombre *)num)->entier.blk;
	n->entier.bit = ((struct nombre *)num)->entier.bit;
	arrange(n);
	if(n->entier.bit > ((struct nombre *)num)->entier.bit){
		iblk = (((struct nombre *)num)->entier.blk-n->entier.blk-1);
		ibit = BITS + ((struct nombre *)num)->entier.bit - n->entier.bit;
	}else{
		ibit = ((struct nombre *)num)->entier.bit - n->entier.bit;
		iblk = ((struct nombre *)num)->entier.blk - n->entier.blk;
	}
	_r[set].entier.bit = 1;
	*_r[set].entier.n = 1;
	while(equal(n, &zero) != 0){
		_d = &_r[set];
		set = !set;
		lbit = _d->entier.bit + dix.entier.bit;
		lblk = _d->entier.blk + dix.entier.blk;
		if(lblk < _d->entier.blk || lblk < dix.entier.blk){
			fprintf(stderr, "Nombre trop grand\n");
			exit(EXIT_FAILURE);
		}
		if(lbit > BITS){
			lblk++;
			if(lblk == 0 || lblk+1 == 0){
				fprintf(stderr, "Nombre trop grand\n");
				exit(EXIT_FAILURE);
			}
			lbit -= BITS;
		}
		if(lblk + 1 > sz){
			nsz = sz;
			while(nsz < lblk+1){
				if((nsz<<1)&((unsigned long int)1<<(BITS-1))){
					nsz = lblk+1;
					break;
				}
				nsz <<= 1;
			}
			if((_sz = block_possible(nsz, 0, 0, 0)) != 0){
				_sz -= sizeof(struct nombre);
				for(i = 0; i < 2; i++){
					if((_r[i].entier.n = cpy[i].entier.n = rlui[i] = realloc(rlui[i], _sz)) == NULL){
						perror("realloc()");
						exit(EXIT_FAILURE);
					}
					for(__sz = nsz - sz, d_ = rlui[i] + sz; __sz > 0; __sz--, d_++)
						*d_ = 0;
				}
			}else{
				for(i = 0; i < 2; i++){
					if((_rlui = calloc(nsz, sizeof(unsigned long int))) == NULL){
						perror("calloc()");
						exit(EXIT_FAILURE);
					}
					for(_sz = sz, d_ = _rlui, s_ = rlui[i]; _sz > 0; _sz--, d_++, s_++)
						*d_ = *s_;
					free(rlui[i]);
					rlui[i] = _rlui;
					cpy[i].entier.n = _rlui;
					_r[i].entier.n = _rlui;
				}
			}
			sz = nsz;
		}else
			for(_rlui = rlui[set], _sz = sz; _sz > 0; _sz--, _rlui++)
				*_rlui = 0;
		division(n, &dix, NULL, 0, OUI, &result[set], reste);
		for(_sz = n->entier.blk+1, nsz = reste[0].entier.blk+1, d_ = n->entier.n, s_ = reste[0].entier.n; nsz > 0 || _sz > 0; nsz-=(nsz > 0), _sz-=(_sz > 0), d_++, s_++){
			if(_sz > 0)
				*d_ = 0;
			if(nsz > 0)
				*s_ = 0;
		}
		n->entier.blk = n->entier.bit = 0;
		n = &result[set];
		multiplication(_d, &dix, &_r[set], OUI);
	}
	for(i = 0; i < 2; i++)
		free(_reste[i]);
	for(;iblk > 0; iblk--){
		for(_ibit = BITS; _ibit > 0; _ibit--){
			_d = &_r[set];
			set = !set;
			lbit = _d->entier.bit + dix.entier.bit;
			lblk = _d->entier.blk + dix.entier.blk;
			if(lblk < _d->entier.blk || lblk < dix.entier.blk){
				fprintf(stderr, "Nombre trop grand\n");
				exit(EXIT_FAILURE);
			}
			if(lbit > BITS){
				lblk++;
				if(lblk == 0 || lblk+1 == 0){
					fprintf(stderr, "Nombre trop grand\n");
					exit(EXIT_FAILURE);
				}
				lbit -= BITS;
			}
			if(lblk + 1 > sz){
				nsz = lblk+1;
				if((_sz = block_possible(nsz, 0, 0, 0)) != 0){
					_sz -= sizeof(struct nombre);
					for(i = 0; i < 2; i++){
						if((_r[i].entier.n = cpy[i].entier.n = rlui[i] = realloc(rlui[i], _sz)) == NULL){
							perror("realloc()");
							exit(EXIT_FAILURE);
						}
						for(__sz = nsz - sz, d_ = rlui[i] + sz; __sz > 0; __sz--, d_++)
							*d_ = 0;
					}
				}else{
					for(i = 0; i < 2; i++){
						if((_rlui = calloc(nsz, sizeof(unsigned long int))) == NULL){
							perror("calloc()");
							exit(EXIT_FAILURE);
						}
						for(_sz = sz, d_ = _rlui, s_ = rlui[i]; _sz > 0; _sz--, d_++, s_++)
							*d_ = *s_;
						free(rlui[i]);
						rlui[i] = _rlui;
						cpy[i].entier.n = _rlui;
						_r[i].entier.n = _rlui;
					}
				}
				sz = nsz;
			}
			for(_rlui = rlui[set], _sz = sz; _sz > 0; _sz--, _rlui++)
				*_rlui = 0;			
			multiplication(_d, &dix, &_r[set], NON);
		}
	}
	for(;ibit > 0; ibit--){
		_d = &_r[set];
		set = !set;
		lbit = _d->entier.bit + dix.entier.bit;
		lblk = _d->entier.blk + dix.entier.blk;
		if(lblk < _d->entier.blk || lblk < dix.entier.blk){
			fprintf(stderr, "Nombre trop grand\n");
			exit(EXIT_FAILURE);
		}
		if(lbit > BITS){
			lblk++;
			if(lblk == 0 || lblk+1 == 0){
				fprintf(stderr, "Nombre trop grand\n");
				exit(EXIT_FAILURE);
			}
			lbit -= BITS;
		}
		if(lblk + 1 > sz){
			nsz = lblk+1;
			if((_sz = block_possible(nsz, 0, 0, 0)) != 0){
				_sz -= sizeof(struct nombre);
				for(i = 0; i < 2; i++){
					if((_r[i].entier.n = cpy[i].entier.n = rlui[i] = realloc(rlui[i], _sz)) == NULL){
						perror("realloc()");
						exit(EXIT_FAILURE);
					}
					for(__sz = nsz - sz, d_ = rlui[i] + sz; __sz > 0; __sz--, d_++)
						*d_ = 0;
				}
			}else{
				for(i = 0; i < 2; i++){
					if((_rlui = calloc(nsz, sizeof(unsigned long int))) == NULL){
						perror("calloc()");
						exit(EXIT_FAILURE);
					}
					for(_sz = sz, d_ = _rlui, s_ = rlui[i]; _sz > 0; _sz--, d_++, s_++)
						*d_ = *s_;
					free(rlui[i]);
					rlui[i] = _rlui;
					cpy[i].entier.n = _rlui;
					_r[i].entier.n = _rlui;
				}
			}
			sz = nsz;
		}
		for(_rlui = rlui[set], _sz = sz; _sz > 0; _sz--, _rlui++)
			*_rlui = 0;		
		multiplication(_d, &dix, &_r[set], NON);
	}
	d = duplic_nbr(&_r[set], NULL, NON);
	for(i = 0; i < 2; i++){
		free(rlui[i]);
		free(result[i].entier.n);
	}
	return d;
}
void cpyoverlap(char *string, int longueur){
	char *o, *i;
	int c, ch = CHAINES-2;
	//printf("STRING == %s ", string);
	if(strlen(string) < ch)
		for(c = longueur,
			o = &string[CHAINES-3],
			i = string+longueur-1;
			ch > 0;
			*o = (c > 0) ? *i : '0',
			c--,
			ch--,
			i--,
			o--
		);;
	//string[CHAINES-] = 0;
	//printf("%s\n", string);
}
/*POINT*/
unsigned long int vers_base(void *num, void *base, int blk){
	struct nombre *n, *d, dix = NBR, b = NBR, base_ = NBR, zero = NBR,reste[2] = { NBR, NBR };
	const unsigned long int _dix = 10, _zero = 0, b_ = LIMAX/10, z = 0;
	unsigned long int _ibit, ibit, iblk, size = 0, tab = 0, *ret, _reste[4] = { 0, 0 };
	char *c = NULL, **tc = NULL, temp[CHAINES+1], eq;
	if(((struct nombre *)num)->virgule || ((struct nombre *)base)->virgule)
		return 0;
	if(((struct nombre *)num)->entier.value)
		destroy_nbrvalue(num);
	dix.entier.n = (unsigned long int *)&_dix;
	dix.entier.bit = 4;
	ret = &size;
	base_.entier.n = (unsigned long int *)&b_;
	for(base_.entier.bit = BITS; (*base_.entier.n&((unsigned long int)1<<(base_.entier.bit-1))) == 0; base_.entier.bit--);;
	zero.entier.n = (unsigned long int *)&z;
	zero.entier.bit = 1;
	if(equal(base, &dix) > 0){
		((struct nombre *)num)->info |= 2;
		tab = 1;
		ret = &tab;
	}else
		((struct nombre *)num)->info |= 4;
	n = duplic_nbr(num, NULL, (((struct nombre *)num)->info&OUI));
	arrange(n);
	iblk = n->entier.blk;
	ibit = n->entier.bit;
	eq = equal(n, &zero);
	while(eq != 0){
		memset(reste, 0, 2*sizeof(struct nombre));
		memset(_reste, 0, 2* sizeof(unsigned long int));
		reste[0].entier.n = &_reste[0];
		reste[1].entier.n = &_reste[1];
		d = division(n, &base_, NULL, 0, blk, NULL, reste);
		memset(temp, 0, CHAINES+1);
		sprintf(temp, "%lu", *reste[0].entier.n);
		if((eq = equal(d, &zero)) != 0)
			cpyoverlap(temp, strlen(temp));
		if(c == NULL && tc == NULL){
			if(tab == 0){
				if((c = malloc(((strlen(temp)+1)*sizeof(char)))) == NULL){
					perror("malloc()");
					exit(EXIT_FAILURE);
				}
				memset(c, 0, strlen(temp)+1);
				memcpy(c, temp, strlen(temp));
				size = strlen(temp);
			}else{
				if((tc = malloc(2*sizeof(char *))) == NULL){
					perror("malloc()");
					exit(EXIT_FAILURE);
				}
				tab = 2;
				tc[1] = NULL;
			}
		}else{
			if(tab == 0){
				if(size + strlen(temp) +1< size){
					fprintf(stderr, "Erreur: nombre trop grand !\n");
					exit(EXIT_FAILURE);
				}
				size += strlen(temp);
				if((c = realloc(c, (size+1) * sizeof(char))) == NULL){
					perror("realloc()");
					exit(EXIT_FAILURE);
				}
				memcpy(c+strlen(temp), c, size-strlen(temp)+1);
			}else{
				tab++;
				if(!tab){
					fprintf(stderr, "Erreur: nombre trop grand !\n");
					exit(EXIT_FAILURE);
				}
				if((tc = realloc(tc, tab * sizeof(char *))) == NULL){
					perror("realloc()");
					exit(EXIT_FAILURE);
				}
				memcpy(tc+1, tc, (tab-1)*sizeof(char *));
			}
		}
		if(tab == 0)
			memcpy(c, temp, strlen(temp));
			//*c = (3 << 4) | *reste[0].entier.n;
		else{
			b.entier.n = reste[0].entier.n;
			b.entier.bit = reste[0].entier.bit;
			b.entier.blk = reste[0].entier.blk;
			vers_base(&b, &dix, blk);
			*tc = b.entier.value;
			b.entier.value = NULL;
		}
		destroy_nbr(n);
		n = d;
	}
	if(ibit > ((struct nombre *)num)->entier.bit){
		iblk-=((struct nombre *)num)->entier.blk-1;
		ibit = BITS + ((struct nombre *)num)->entier.bit - ibit;
	}else{
		ibit = ((struct nombre *)num)->entier.bit - ibit;
		iblk = ((struct nombre *)num)->entier.blk-iblk;
	}
	for(;iblk > 0; iblk--){
		for(_ibit = BITS; _ibit > 0; _ibit--){
			if(c == NULL && tc == NULL){
				if(tab == 0){
					if(size+2 < size){
						fprintf(stderr, "Erreur: nombre trop grand !\n");
						exit(EXIT_FAILURE);
					}
					size++;
					if((c = malloc((size+1)*sizeof(char))) == NULL){
						perror("malloc()");
						exit(EXIT_FAILURE);
					}
					memset(c, 0, size+1);
					//size += 1;
				}else{
					if((tc = malloc(2*sizeof(char *))) == NULL){
						perror("malloc()");
						exit(EXIT_FAILURE);
					}
					tab = 2;
					tc[1] = NULL;
				}
			}else{
				if(tab == 0){
					if((size+2)<size){
						fprintf(stderr, "Erreur: nombre trop grand !\n");
						exit(EXIT_FAILURE);
					}
					size++;
					if((c = realloc(c, (size+1)* sizeof(char))) == NULL){
						perror("realloc()");
						exit(EXIT_FAILURE);
					}
					memcpy(c+1, c, size);
				}else{
					tab++;
					if(!tab){
						fprintf(stderr, "Erreur: nombre trop grand !\n");
						exit(EXIT_FAILURE);
					}
					if((tc = realloc(tc, tab * sizeof(char *))) == NULL){
						perror("realloc()");
						exit(EXIT_FAILURE);
					}
					memcpy(tc+1, tc, (tab-1)*sizeof(char *));
				}
			}
			if(tab == 0)
				*c = '0';
			else{
				b.entier.n = (unsigned long int *)&_zero;
				b.entier.bit = 1;
				b.entier.blk = 0;
				vers_base(&b, &dix, blk);
				*tc = b.entier.value;
				b.entier.value = NULL;
			}
		}
	}
	for(
		ibit = (c == NULL && tc == NULL && ibit == 0) ? 1 : ibit;
		ibit > 0 || (c == NULL && tc == NULL);
		ibit--
	){
		if(c == NULL && tc == NULL){
			if(tab == 0){
				if((size+2)<size){
					fprintf(stderr, "Erreur: nombre trop grand !\n");
					exit(EXIT_FAILURE);
				}
				size++;
				if((c = malloc((size+1)*sizeof(char))) == NULL){
					perror("malloc()");
					exit(EXIT_FAILURE);
				}
				memset(c, 0, size+1);
			}else{
				if((tc = malloc(2*sizeof(char *))) == NULL){
					perror("malloc()");
					exit(EXIT_FAILURE);
				}
				tab = 2;
				tc[1] = NULL;
			}
		}else{
			if(tab == 0){
				if(size+2<size){
					fprintf(stderr, "Erreur: nombre trop grand !\n");
					exit(EXIT_FAILURE);
				}
				size++;
				if((c = realloc(c, (size+1) * sizeof(char))) == NULL){
					perror("realloc()");
					exit(EXIT_FAILURE);
				}
				memcpy(c+1, c, size);
			}else{
				tab++;
				if(!tab){
					fprintf(stderr, "Erreur: nombre trop grand !\n");
					exit(EXIT_FAILURE);
				}
				if((tc = realloc(tc, tab * sizeof(char *))) == NULL){
					perror("realloc()");
					exit(EXIT_FAILURE);
				}
				memcpy(tc+1, tc, (tab-1)*sizeof(char *));
			}
		}
		if(tab == 0)
			*c = '0';
		else{
			b.entier.n = (unsigned long int *)&_zero;
			b.entier.bit = 1;
			b.entier.blk = 0;
			vers_base(&b, &dix, blk);
			*tc = b.entier.value;
			b.entier.value = NULL;
		}
	}
	destroy_nbr(n);
	if(tab)
		((struct nombre *)num)->entier.value = tc;
	else
		((struct nombre *)num)->entier.value = c;
	return *ret-1;
}
void decode_vers(void *num, void *base, int blk){
	struct nombre *n = num, e = NBR, v = NBR, zero = NBR;
	int neg = n->signe;
	unsigned long int _zero = 0;
	if(((struct nombre *)base)->virgule != NULL){
		fprintf(stderr, "Base invalid\n");
		exit(EXIT_FAILURE);
	}
	destroy_nbrvalue(n);
	e.entier.n = n->entier.n;
	e.entier.bit = n->entier.bit;
	e.entier.blk = n->entier.blk;
	vers_base(&e, base, blk);
	if(n->virgule){
		v.entier.n = n->virgule->n.n;
		v.entier.bit = n->virgule->n.bit;
		v.entier.blk = n->virgule->n.blk;
		arrange(&v);
		zero.entier.n = &_zero;
		zero.entier.bit = 1;
		if(equal(&v, &zero) != 0){
			v.entier.n = n->virgule->n.n;
			v.entier.bit = n->virgule->n.bit;
			v.entier.blk = n->virgule->n.blk;
			vers_base(&v, base, blk);
			n->virgule->n.value = v.entier.value;
		}
	}
	n->entier.value = e.entier.value;
	n->info |= e.info;
	n->signe = neg;
}
int read_base10(void *num, void *end, unsigned long int *buf, int *bits){
	char *n, *e, b[CHAINES];
	int i;
	if(end == num)
		return 0;
	memset(b, 0, CHAINES);
	for(i = 0,
		n = num,
		e = end;
		n != e && i < CHAINES-1;
		i++,
		n--
	);;
	memcpy(b, n, i);
	*buf = atol(b);
	for(*bits = BITS; *bits > 0 && (*buf&((unsigned long int)1<<((*bits)-1))) == 0; (*bits)--);;
	return i;
}
void *depuis_base(void *num, void *base, void *mdix, int blk){
	struct nombre *r = NULL, n = NBR, _dix = NBR, base_ = NBR,*dix, _zero_ = NBR, _r[5] = { NBR, NBR, NBR, NBR, NBR }, cpy[5] = { NBR, NBR, NBR, NBR, NBR };
	char *ppc, *pc, *c = num, *z = num, e_bit, neg = '+';
	unsigned long int b = LIMAX, i, _d = 10, *_zero = NULL, *__zero = NULL, e_blk,_z_ = 0, *d, *s, nsz, sz, _sz, __sz, sz_ = 12,*rlui[5] = { NULL, NULL, NULL, NULL, NULL }, *_rlui, lblk = 0;
	int set = 0, i_, lbit = 0, ret, count;
	base_.entier.n = &b;
	base_.entier.bit = BITS;
	if(*c == '-' || *c == '+'){
		neg = *c;
		pc = ++c;
	}else
		pc = c;
	for(c = pc;*c != 0 && *c != '.'; c++);;
	_dix.entier.n = &_d;
	_dix.entier.bit = 4;
	if(equal(base, &_dix) > 0){
		fprintf(stderr, "base invalide\n");
		exit(EXIT_FAILURE);
	}
	if(*c == 0 && c == pc)
		return NULL;
	for(z = pc; *z == '0'; z++);;
	if(z == pc)
		z = NULL;
	else
		z--;
	//c--;
	ppc = (z == NULL) ? pc: z;
	for(set = 0; set < 5; set++){
		if((rlui[set] = calloc(sz_, sizeof(unsigned long int))) == NULL){
			perror("calloc()");
			exit(EXIT_FAILURE);
		}
		cpy[set].info = OUI;
		cpy[set].entier.n = rlui[set];
		memcpy(&_r[set], &cpy[set], sizeof(struct nombre));
	}
	set = 0;
	r = &_r[0];
	*r->entier.n = 0;
	r->entier.bit = 1;
	r->info |= 1;
	dix = &_r[2];
	*dix->entier.n = 1;
	dix->entier.bit = 1;
	n.entier.n = &i;
	do{
		r = &_r[set];
		dix = &_r[set+2];
		lbit = r->entier.bit + dix->entier.bit;
		lblk = r->entier.blk + dix->entier.blk;
		if(lblk < r->entier.blk || lblk < dix->entier.blk){
			fprintf(stderr, "Nombre trop grand\n");
			exit(EXIT_FAILURE);
		}
		if(lbit > BITS){
			lblk++;
			if(lblk == 0 || lblk+1 == 0){
				fprintf(stderr, "Nombre trop grand\n");
				exit(EXIT_FAILURE);
			}
			lbit -= BITS;
		}
		if(lblk + 1 > sz_){
			nsz = sz_;
			while(nsz < lblk+1){
				if((nsz<<1)&((unsigned long int)1<<(BITS-1))){
					nsz = lblk+1;
					break;
				}
				nsz <<= 1;
			}
			if((sz = block_possible(nsz, 0, 0, 0)) != 0){
				sz -= sizeof(struct nombre);
				for(i = 0; i < 2; i++){
					if((_r[set+(i*2)].entier.n = cpy[set+(i*2)].entier.n = rlui[set+(i*2)] = realloc(rlui[set+(i*2)], sz)) == NULL){
						perror("realloc()");
						exit(EXIT_FAILURE);
					}
					for(d = rlui[set+(i*2)] + sz_, __sz = nsz - sz_; __sz > 0; __sz--, d++)
						*d = 0;
				}
				for(i = 0; i < 2; i++){
					if((_r[!set+(i*2)].entier.n = cpy[!set+(i*2)].entier.n = rlui[!set+(i*2)] = realloc(rlui[!set+(i*2)], sz)) == NULL){
						perror("realloc()");
						exit(EXIT_FAILURE);
					}
					for(d = rlui[!set+(i*2)] + sz_, __sz = nsz - sz_; __sz > 0; __sz--, d++)
						*d = 0;
					memcpy(&_r[!set+(i*2)], &cpy[!set+(i*2)], sizeof(struct nombre));
				}
				if((_r[4].entier.n = cpy[4].entier.n = rlui[4] = realloc(rlui[4], sz)) == NULL){
					perror("realloc()");
					exit(EXIT_FAILURE);
				}
				for(d = rlui[4] + sz_, __sz = (nsz -sz_); __sz > 0; __sz--, d++)
					*d = 0;
				memcpy(&_r[4], &cpy[4], sizeof(struct nombre));
			}else{
				for(i = 0; i < 2; i++){
					if((_rlui = calloc(nsz, sizeof(unsigned long int))) == NULL){
						perror("calloc()");
						exit(EXIT_FAILURE);
					}
					for(sz = sz_, d = _rlui, s = rlui[set+(i*2)]; sz > 0; sz--, d++, s++)
						*d = *s;
					free(rlui[set+(i*2)]);
					rlui[set+(i*2)] = _rlui;
					cpy[set+(i*2)].entier.n = _rlui;
					_r[set+(i*2)].entier.n = _rlui;
				}
				for(i = 0; i < 2; i++){
					free(rlui[!set+(i*2)]);
					if((rlui[!set+(i*2)] = calloc(nsz, sizeof(unsigned long int))) == NULL){
						perror("calloc()");
						exit(EXIT_FAILURE);
					}
					cpy[!set+(i*2)].entier.n = rlui[!set+(i*2)];
					_r[!set+(i*2)].entier.n = rlui[!set+(i*2)];
					memcpy(&_r[!set+(i*2)], &cpy[!set+(i*2)], sizeof(struct nombre));
				}
				if((_rlui = calloc(nsz, sizeof(unsigned long int))) == NULL){
					perror("calloc()");
					exit(EXIT_FAILURE);
				}
				for(sz = sz_, d = _rlui, s = rlui[4]; sz > 0; sz--, d++, s++)
					*d = *s;
				free(rlui[4]);
				rlui[4] = _rlui;
				cpy[4].entier.n = _rlui;
				_r[4].entier.n = _rlui;
				memcpy(&_r[4], &cpy[4], sizeof(struct nombre));
			}
			sz_ = nsz;
		}
		set = !set;
		for(_sz = 0, i_ = set; _sz < 3; _sz++, i_ = (i_ +2 < 4) ? i_+2 : 4){
			for(d = rlui[i_], sz = sz_; sz > 0; sz--, d++)
				*d = 0;
			memcpy(&_r[i_], &cpy[i_], sizeof(struct nombre));
		}
		ret = read_base10(c, ppc, n.entier.n, &n.entier.bit);
		for(count = ret;count != CHAINES-1; count++)*base_.entier.n/=10;
		for(base_.entier.bit = BITS;(*base_.entier.n&((unsigned long int)1<<(base_.entier.bit-1))) == 0 && base_.entier.bit > 0;base_.entier.bit--);
		multiplication(&n, dix, &_r[4], blk);
		addition(r, &_r[4], &_r[set], blk);
		multiplication(dix, &base_, &_r[set+2], blk);
	}while(c-=ret, c != ppc);
	_zero_.entier.n = &_z_;
	_zero_.entier.bit++;
	r = duplic_nbr(&_r[set], NULL, blk);
	if(mdix && z){
		if((_zero = calloc(r->entier.blk + (r->entier.bit > 0),sizeof(unsigned long int))) == NULL){
			perror("calloc()");
			exit(EXIT_FAILURE);
		}
		for(d = _zero, s = r->entier.n, sz = r->entier.blk + (r->entier.bit > 0); sz > 0;sz--, s++, d++)
			*d = *s;
		do{
			r->entier.bit++;
			dix = &_r[set+2];
			lbit = r->entier.bit + dix->entier.bit;
			lblk = r->entier.blk + dix->entier.blk;
			if(lblk < r->entier.blk || lblk < dix->entier.blk){
				fprintf(stderr, "Nombre trop grand\n");
				exit(EXIT_FAILURE);
			}
			if(lbit > BITS){
				lblk++;
				if(lblk == 0 || lblk+1 == 0){
					fprintf(stderr, "Nombre trop grand\n");
					exit(EXIT_FAILURE);
				}
				lbit -= BITS;
			}
			if(lblk + 1 > sz_){
				nsz = sz_;
				while(nsz < lblk+1){
					if((nsz<<1)&((unsigned long int)1<<(BITS-1))){
						nsz = lblk+1;
						break;
					}
					nsz <<= 1;
				}
				if((sz = block_possible(nsz, 0, 0, 0)) != 0){
					sz -= sizeof(struct nombre);
					for(i = 0; i < 2; i++){
						if((_r[set+(i*2)].entier.n = cpy[set+(i*2)].entier.n = rlui[set+(i*2)] = realloc(rlui[set+(i*2)], sz)) == NULL){
							perror("realloc()");
							exit(EXIT_FAILURE);
						}
						for(d = rlui[set+(i*2)] + sz_, __sz = nsz - sz_; __sz > 0; __sz--, d++)
							*d = 0;
					}
					for(i = 0; i < 2; i++){
						if((_r[!set+(i*2)].entier.n = cpy[!set+(i*2)].entier.n = rlui[!set+(i*2)] = realloc(rlui[!set+(i*2)], sz)) == NULL){
							perror("realloc()");
							exit(EXIT_FAILURE);
						}
						for(d = rlui[!set+(i*2)] + sz_, __sz = nsz - sz_; __sz > 0; __sz--, d++)
							*d = 0;
						memcpy(&_r[!set+(i*2)], &cpy[!set+(i*2)], sizeof(struct nombre));
					}
				}else{
					for(i = 0; i < 2; i++){
						if((_rlui = calloc(nsz, sizeof(unsigned long int))) == NULL){
							perror("calloc()");
							exit(EXIT_FAILURE);
						}
						for(sz = sz_, d = _rlui, s = rlui[set+(i*2)]; sz > 0; sz--, d++, s++)
							*d = *s;
						free(rlui[set+(i*2)]);
						rlui[set+(i*2)] = _rlui;
						cpy[set+(i*2)].entier.n = _rlui;
						_r[set+(i*2)].entier.n = _rlui;
					}
					for(i = 0; i < 2; i++){
						free(rlui[!set+(i*2)]);
						if((rlui[!set+(i*2)] = calloc(nsz, sizeof(unsigned long int))) == NULL){
							perror("calloc()");
							exit(EXIT_FAILURE);
						}
						cpy[!set+(i*2)].entier.n = rlui[!set+(i*2)];
						_r[!set+(i*2)].entier.n = rlui[!set+(i*2)];
						memcpy(&_r[!set+(i*2)], &cpy[!set+(i*2)], sizeof(struct nombre));
					}
				}
				sz_ = nsz;
			}
			set = !set;
			for(_sz = 0, i_ = set; _sz < 3; _sz++, i_ = (i_ +2 < 4) ? i_+2 : 4){
				for(d = rlui[i_], sz = sz_; sz > 0; sz--, d++)
					*d = 0;
				memcpy(&_r[i_], &cpy[i_], sizeof(struct nombre));
			}
			if(r->entier.bit > BITS){
				r->entier.bit -= BITS;
				r->entier.blk++;
				if(r->entier.blk == 0 || r->entier.blk + (r->entier.bit > 0) == 0 || r->entier.blk + (r->entier.bit > 0) +1 == 0){
					fprintf(stderr, "Nombre trop grand.\n");
					exit(EXIT_FAILURE);
				}
				if((sz = block_possible(r->entier.blk + (r->entier.bit > 0) +1, 0, 0, 0)) != 0){
					sz -= sizeof(struct nombre);
					if((_zero = realloc(_zero, sz)) == NULL){
						perror("realloc()");
						exit(EXIT_FAILURE);
					}
					*(_zero + r->entier.blk) = 0;
					*(_zero + r->entier.blk +1) = 0;
				}else{
					if((__zero = calloc(r->entier.blk + (r->entier.bit > 0)+1,sizeof(unsigned long int))) == NULL){
						perror("calloc()");
						exit(EXIT_FAILURE);
					}
					for(d = __zero, s = _zero, sz = r->entier.blk; sz > 0;sz--, s++, d++)
						*d = *s;
					free(_zero);
					_zero = __zero;
				}
			}
			//printf("%lu\n", *((struct nombre *)base)->entier.n);
			multiplication(dix, base, &_r[set+2], blk);
		}while(z != pc && z--);
		e_bit = r->entier.bit;
		e_blk = r->entier.blk;
		destroy_nbr(r);
		if((r = malloc(sizeof(struct nombre))) == NULL){
			perror("malloc()");
			exit(EXIT_FAILURE);
		}
		memset(r, 0, sizeof(struct nombre));
		r->entier.bit = e_bit;
		r->entier.blk = e_blk;
		r->entier.n = _zero;
	}else{
		if(r->entier.bit == 0 && r->entier.blk == 0)
			r->entier.bit = 1;
		set = !set;
	}
	if(mdix)
		memcpy(mdix, &_r[!set+2], sizeof(struct nombre));
	else
		free(rlui[!set+2]);
	for(i_ = 0; i_ < 5; i_++)
		if(!set+2 != i_)
			free(rlui[i_]);
	r->signe = (equal(r, &_zero_)) ? neg : '+';
	return r;
}
void *encode_depuis_base(void *num, void *base, int zero, int blk){
	struct nombre *r, *pr,*entier, *virgule, v = NBR,dix = NBR, *b, *pb,_zero_ = NBR, md = NBR;
	unsigned long int _dix = 10, _zero = 0;
	char *c = (char *)num, **pc = (char **)num, **ptr, **ppc;
	int tab = 0, neg = 0;
	dix.entier.n = &_dix;
	dix.entier.bit = 4;
	_zero_.entier.bit = 1;
	_zero_.entier.n = &_zero;
	if(equal(base, &dix) > 0)
		tab = 1;
	if(!tab){
		entier = depuis_base(c, base, NULL, NON);
		for(c = c; *(c+1) != 0 && *c != '.'; c++);;
		if(*(c) == '.'){
			c++;
			virgule = depuis_base((void *)c, base, &md, NON);
			memcpy(&v, virgule, sizeof(struct nombre));
			arrange(&v);
			switch(equal(&v, &_zero_)){
				case 0:
					destroy_nbr(virgule);
					free(md.entier.n);
					r = entier;
					break;
				default:
					if((r = malloc(sizeof(struct nombre) + sizeof(struct vnbr))) == NULL){
						perror("malloc()");
						exit(EXIT_FAILURE);
					}
					memcpy(r, entier, sizeof(struct nombre));
					r->virgule = (struct vnbr *)(struct nombre *)(r+1);
					memcpy(&r->virgule->n, &virgule->entier, sizeof(struct nbr));
					r->info-=(r->info&OUI);
					r->info |= 8;
					memcpy(&r->virgule->mdix, &md.entier, sizeof(struct nbr));
					free(virgule);
					free(entier);
					break;
			}
		}else
			r = entier;
	}else{
		ptr = pc = num;
		if(**pc == '-'){
			neg = '-';
			ptr = ++pc;
		}
		if(!zero){
			for(ppc = pc; *ppc != NULL && *(pc+1) != NULL; ppc++){
				r = depuis_base(*ppc, &dix, NULL, blk);
				if(equal(r, &_zero_) != 0){
					destroy_nbr(r);
					break;
				}
				destroy_nbr(r);
			}
			if(ppc != NULL)
				pc = ppc;
		}
		for(pc = pc; pc+=(pc+1)!=NULL; pc++);;
		if((b = malloc(sizeof(struct nombre) + sizeof(unsigned long int))) == NULL){
			perror("malloc()");
			exit(EXIT_FAILURE);
		}
		if((r = malloc(sizeof(struct nombre) + sizeof(unsigned long int))) == NULL){
			perror("malloc()");
			exit(EXIT_FAILURE);
		}
		b->entier.n = (unsigned long int *)(b+1);
		*b->entier.n = 1;
		r->entier.n = (unsigned long int *)(r+1);
		*r->entier.n = 0;
		do{
			c = *pc;
			entier = depuis_base(c, &dix, &md, blk);
			virgule = multiplication(entier, b, NULL, blk);
			pr = addition(r, virgule, NULL, blk);
			pb = multiplication(b, base, NULL, blk);
			destroy_nbr(b);
			destroy_nbr(entier);
			destroy_nbr(virgule);
			destroy_nbr(r);
			b = pb;
			r = pr;
		}while(pc != ptr && pc--);
		r->signe = neg;
		destroy_nbr(b);
	}
	return r;
}

unsigned long int block_possible(unsigned long int eblk, int ebit, unsigned long int vblk, int vbit){
	struct nombre s_nbr = NBR, s_vnbr = NBR, s_ulint = NBR, temp = NBR, *n,
					r[2] = { NBR, NBR }, rcpy[2];
	unsigned long int _s_ulint = sizeof(unsigned long int),
						_s_nbr = sizeof(struct nombre),
						_s_vnbr = sizeof(struct vnbr),
						blk[4] = {eblk, (unsigned long int)(ebit > 0), vblk, (unsigned long int)(vbit > 0)},
						_r[4] = { 0, 0, 0, 0 };
	int i, set = 0;
	s_ulint.entier.n = &_s_ulint;
	s_ulint.entier.bit = 4;
	s_nbr.entier.n = &_s_nbr;
	s_nbr.entier.bit = 8;
	s_vnbr.entier.n = &_s_vnbr;
	s_vnbr.entier.bit = 8;
	r[0].entier.n = &_r[0];
	r[1].entier.n = &_r[2];
	r[0].info = OUI;
	r[1].info = OUI;
	for(set = 0; set < 2; set++)
		memcpy(&rcpy[set], &r[set], sizeof(struct nombre));
	for(set = 0, i = 0; i < 4 && rcpy[set].entier.n[1] == 0; i++){
		if(blk[i] == 0)
			continue;
		temp.entier.n = &blk[i];
		if(i%2 == 0)
			for(temp.entier.bit = BITS; (*temp.entier.n&((unsigned long int)1<<(temp.entier.bit-1))) == 0; temp.entier.bit--);
		else
			temp.entier.bit = 1;
		n = &r[set];
		set = !set;
		*r[set].entier.n = 0;
		addition(&temp, n, &r[set], OUI);
	}
	if(r[set].entier.n[1] != 0)
		return 0;
	n = &r[set];
	r[!set].entier.n[0] = 0;
	multiplication(n, &s_ulint, &r[!set],OUI);
	if(r[set].entier.n[1] != 0)
		return 0;
	set = !set;
	n = &r[set];
	*r[!set].entier.n = 0;
	addition(n, &s_nbr, &r[!set], OUI);
	if(r[set].entier.n[1] != 0)
		return 0;
	set = !set;
	if(vblk || vbit){
		n = &r[set];
		addition(n, &s_vnbr, &r[!set], OUI);
		if(r[set].entier.n[1] != 0)
			return 0;
		set = !set;
	}
	return *r[set].entier.n;
}
void *vers_entier(void *n, int blk){
	struct nombre mdix = NBR, entier = NBR, *m, *a;
	if(!((struct nombre *)n)->virgule)
		return NULL;
	memcpy(&entier.entier, &((struct nombre *)n)->entier, sizeof(struct nbr));
	memcpy(&mdix.entier, &((struct nombre *)n)->virgule->mdix, sizeof(struct nbr));
	m = multiplication(&entier, &mdix, NULL, blk);
	memcpy(&entier.entier, &((struct nombre *)n)->virgule->n, sizeof(struct nbr));
	a = addition(m, &entier, NULL, blk);
	arrange(a);
	destroy_nbr(m);
	a->signe = ((struct nombre *)n)->signe;
	return a;
}
void vers_entier_aligne(void *num1, void *num2, void **r1, void **r2, int blk){
	struct nombre d1 = NBR, d2 = NBR, *d, *r;
	*r1 = vers_entier(num1, blk);
	*r2 = vers_entier(num2, blk);
	if(*r1)
		memcpy(&d1.entier, &((struct nombre *)num1)->virgule->mdix, sizeof(struct nbr));
	if(*r2)
		memcpy(&d2.entier, &((struct nombre *)num2)->virgule->mdix, sizeof(struct nbr));
	if(!*r1 || !*r2){
		if(!*r1 && !*r2)
			return;
		if(!*r1 && *r2){
			*r1 = multiplication(num1, &d2, NULL, blk);
			return;
		}
		if(*r1 && !*r2){
			*r2 = multiplication(num2, &d1, NULL, blk);
			return;
		}
	}
	switch(equal(&d1, &d2)){
		case 0:
			return;
		case 1:
			d = division(&d1, &d2, NULL, 0, blk, NULL, NULL);
			r = multiplication(*r2, d, NULL, blk);
			destroy_nbr(*r2);
			*r2 = r;
			break;
		case -1:
			d = division(&d2, &d1, NULL, 0, blk, NULL, NULL);
			r = multiplication(*r1, d, NULL, blk);
			destroy_nbr(*r1);
			*r1 = r;
			break;
	}
	destroy_nbr(d);
}
static void *lavirgule(void *num, void *mdix, int blk){
	struct nombre *m10, *n, *md, *_md, _mdix_ = NBR, dix = NBR, zero = NBR, un = NBR, *_m, *r, *d;
	unsigned long int _dix = 10, max = LIMAX,_zero = 0, _un = 1, *p, *d_, *s_, z, blk_ = 0;
	int bit_ = 1;
	dix.entier.n = &_dix;
	dix.entier.bit = 4;
	zero.entier.n = &_zero;
	zero.entier.bit = 1;
	un.entier.n = &_un;
	un.entier.bit = 1;
	memcpy(&_mdix_.entier, mdix, sizeof(struct nbr));
	m10 = duplic_nbr(&_mdix_, NULL, NON);
	n = modulo(num, m10, NULL, blk);
	arrange(n);
	if(equal(n, &zero) == 0){
		destroy_nbr(n);
		n = division(num, m10, NULL, 0, blk, NULL, NULL);
		destroy_nbr(m10);
		return n;
	}
	d = division(num, m10, NULL, 0, NON, NULL, NULL);
	r = duplic_nbr(n, NULL, NON);
	_md = duplic_nbr(m10, NULL, NON);
	dix.entier.n = &max;
	dix.entier.bit = BITS;
	while(equal(n, &zero) != 0 && equal(m10, &un) != 0){
		if(equal(m10, &dix) < 0){
			dix.entier.n = &_dix;
			dix.entier.bit = 4;
		}
		_m = division(n, &dix, NULL, 0, blk, NULL, NULL);
		destroy_nbr(n);
		arrange(_m);
		n = _m;
		md = division(m10, &dix, NULL, 0, blk, NULL, NULL);
		destroy_nbr(m10);
		m10 = md;
	}
	if((d->virgule = calloc(1, sizeof(struct vnbr))) == NULL){
		perror("calloc()");
		exit(EXIT_FAILURE);
	}
	memcpy(&d->virgule->n, &r->entier, sizeof(struct nbr));
	memcpy(&d->virgule->mdix, &_md->entier, sizeof(struct nbr));
	if(r->virgule){
		free(r->virgule->mdix.n);
		free(r->virgule->n.n);
		free(r->virgule);
	}
	free(r);
	free(_md);
	dix.entier.n = &max;
	dix.entier.bit = BITS;
	while(equal(m10, &dix) >= 0){
		d->virgule->n.bit += (CHAINES-1);
		if(d->virgule->n.bit > BITS){
			if(++d->virgule->n.blk == 0){
				fprintf(stderr, "Trop de chiffre apres la virgule\n");
				exit(EXIT_FAILURE);
			}
			d->virgule->n.bit -= BITS;
			if((p = calloc(d->virgule->n.blk, sizeof(unsigned long int))) == NULL){
				perror("calloc()");
				exit(EXIT_FAILURE);
			}
			for(z = 0, d_ = p, s_ = d->virgule->n.n; z < d->virgule->n.blk; z++, d_++, s_++)
				*d_ = *s_;
			free(d->virgule->n.n);
			d->virgule->n.n = p;
		}
		md = division(m10, &dix, NULL, 0, blk, NULL, NULL);
		destroy_nbr(m10);
		m10 = md;
	}
	dix.entier.n = &_dix;
	dix.entier.bit = 4;
	while(equal(m10, &un) == 1){
		if(++d->virgule->n.bit > BITS){
			if(++d->virgule->n.blk == 0){
				fprintf(stderr, "Trop de chiffre apres la virgule\n");
				exit(EXIT_FAILURE);
			}
			d->virgule->n.bit -= BITS;
			if((p = calloc(d->virgule->n.blk+1, sizeof(unsigned long int))) == NULL){
				perror("calloc()");
				exit(EXIT_FAILURE);
			}
			for(z = 0, d_ = p, s_ = d->virgule->n.n; z < d->virgule->n.blk; z++, d_++, s_++)
				*d_ = *s_;
			free(d->virgule->n.n);
			d->virgule->n.n = p;
		}
		md = division(m10, &dix, NULL, 0, blk, NULL, NULL);
		destroy_nbr(m10);
		m10 = md;
	}
	destroy_nbr(m10);
	destroy_nbr(n);
	return d;
}
void *puissance_scalaire(void *num, void *p, void *virgule, int arrondi, int blk){
	struct nombre *n = num, *_n, *_p, zero = NBR, *r, *pseudo, *_pseudo, un = NBR, mdix = NBR, *_mdix = NULL, *_pmdix, *_ppmdix,*pmdix;
	unsigned long int z = 0, _un = 1;
	int signe = '+';
	printf("START\n");
	zero.entier.n = &z;
	zero.entier.bit = 1;
	if(((struct nombre *)p)->virgule){
		fprintf(stderr, "puissance non scalaire\n");
		return NULL;
	}
	if(equal(p, &zero) == 0){
		r = duplic_nbr(&zero, NULL, blk);
		*r->entier.n = 1;
		return r;
	}
	if(n->virgule){
		memcpy(&mdix.entier, &n->virgule->mdix, sizeof(struct nbr));
		_mdix = &mdix;
		n = vers_entier(num, blk);
	}
	_p = duplic_nbr(p, NULL, blk);
	un.entier.n = &_un;
	un.entier.bit = 1;
	pseudo = &un;
	_ppmdix = &un;
	if(equal(_p, &zero) < 0){
		_p->signe = '+';
		signe = '-';
	}
	while(_p->entier.blk != 0 || _p->entier.bit != 1){
		if((*_p->entier.n&1) == 1){
			*_p->entier.n -= 1;
			_pseudo = multiplication(n, pseudo, NULL,blk);
			if(_mdix)
				_pmdix = multiplication(_mdix, _ppmdix, NULL, blk);
			if(pseudo != &un){
				destroy_nbr(pseudo);
				if(_mdix)
					destroy_nbr(_ppmdix);
			}
			pseudo = _pseudo;
			_ppmdix = _pmdix;
		}else{
			droite(_p, _p, 1, 0, blk);
			arrange(_p);
			_n = multiplication(n, n, NULL, blk);
			if(n != num)
				destroy_nbr(n);
			if(_mdix){
				pmdix = multiplication(_mdix, _mdix, NULL, blk);
				if(_mdix != &mdix)
					destroy_nbr(_mdix);
				_mdix = pmdix;
			}
			n = _n;
		}
	}
	r = multiplication(n, pseudo, NULL, blk);
	printf("MILLIEU\n");
	if(_mdix){
		pmdix = multiplication(_mdix, _ppmdix, NULL, blk);
		destroy_nbr(n);
		n = lavirgule(r, &pmdix->entier, blk);
		destroy_nbr(r);
		destroy_nbr(pmdix);
		r = n;
		if(_ppmdix != &un)
			destroy_nbr(_ppmdix);
		if(_mdix != &mdix)
			destroy_nbr(_mdix);
	}else
		if(n != num)
			destroy_nbr(n);
	if(pseudo != &un)
		destroy_nbr(pseudo);
	destroy_nbr(_p);
	if(signe == '-'){
		_n = division(&un, r, virgule, arrondi, blk, NULL, NULL);
		destroy_nbr(r);
		r = _n;
	}
	printf("FIN\n");
	return r;
}
/*void *encode_depuis(void *num, void *base){
	struct nombre *n = num, dix = NBR;
}*/
