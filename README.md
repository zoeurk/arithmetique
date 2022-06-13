# arithmetique  
Librairie de fonction mathematique.  
les librairies sont libre d'usage, de modification et (re/)distribution  
patch -i file.patch file.c pour ne pas utilser la librairie <math>  
libtoolize  
aclocal  
autoreconf --install  
./configure --exec-prefix=/usr --prefix=/usr  
make  
make install  
