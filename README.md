# arithmetique  
  (ATTENTION: Aucune garanties) 
Librairie de fonction mathematique.  
les librairies sont libre d'usage, de modification et (re/)distribution  
libtoolize  
aclocal  
autoreconf --install  
./configure --exec-prefix=/usr --prefix=/usr  
make  
make install  
  
  
Fuite de memoire plus que probable dans arithmetique.c dans le cadre des erreurs
Nota:
mathinbin.c est ici dans un pur but de curiositée.
gcc mathinbin.c
./a.out num1 num2 virgule

Bon j'ai essayé d'optimiser operation.c et je suis au moins 2x plus lent que bc :/  
Voir opertion.c (pour tester sur la puissance il faut aller dans le code ...)  
./a.out num1 num2 virgule
