CC=gcc
ARG=-g -Wall -Wpadded -Wextra

all:liboperation.so libarithmetique.so
	 $(CC) $(ARG) main/main.c -o main-bin -L./operation -loperation -L./arithmetique -larithmetique -lm

liboperation.so:
	$(CC) $(ARG) -fPIC -o ./operation/operation.o -c ./operation/operation.c
	$(CC) $(ARG) -o ./operation/liboperation.so -shared ./operation/operation.o

libarithmetique.so:
	$(CC) $(ARG) -fPIC -o ./arithmetique/arithmetique.o -c ./arithmetique/arithmetique.c -L../operation -loperation
	$(CC) $(ARG) -o ./arithmetique/libarithmetique.so -shared ./arithmetique/arithmetique.o

clean:
	rm -fv ./operation/liboperation.so ./arithmetique/libarithmetique.so ./arithmetique/arithmetique.o ./operation/operation.o main-bin
