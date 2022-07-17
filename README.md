# arithmetique  
Librairie de fonction mathematique.  
les librairies sont libre d'usage, de modification et (re/)distribution  
patch -i file.patch file.c pour ne pas utilser la librairie <math>  
Un script pour l'exemple est fournie  
libtoolize  
aclocal  
autoreconf --install  
./configure --exec-prefix=/usr --prefix=/usr  
make  
make install  
  
  
il y a un bug mais je ne sais pas ou :/
