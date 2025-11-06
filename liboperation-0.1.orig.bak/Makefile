CC=gcc
ARG=-g -Wall -Wpadded -Wextra

all:liboperation.so libarithmetique.so
	 $(CC) $(ARG) main/main.c -o main-bin -ansi -pedantic-errors -L./operation -loperation -L./arithmetique -larithmetique

liboperation.so:
	$(CC) $(ARG) -fPIC -o ./operation/operation.o -ansi -pedantic-errors -c ./operation/operation.c
	$(CC) $(ARG) -o ./operation/liboperation.so -ansi -pedantic-errors -shared ./operation/operation.o

libarithmetique.so:
	$(CC) $(ARG) -fPIC -o ./arithmetique/arithmetique.o -ansi -pedantic-errors -c ./arithmetique/arithmetique.c
	$(CC) $(ARG) -o ./arithmetique/libarithmetique.so -ansi -pedantic-errors -shared ./arithmetique/arithmetique.o


clean:
	rm -fv ./operation/liboperation.so ./operation/operation.o ./arithmetique/libarithmetique.so ./arithmetique/arithmetique.o main-bin
