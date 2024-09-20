CC=gcc
ARG=-g -Wall -Wpadded -Wextra

all:liboperation.so
	 $(CC) $(ARG) main/main.c -o main-bin -ansi -pedantic-errors -L./operation -loperation

liboperation.so:
	$(CC) $(ARG) -fPIC -o ./operation/operation.o -ansi -pedantic-errors -c ./operation/operation.c
	$(CC) $(ARG) -o ./operation/liboperation.so -ansi -pedantic-errors -shared ./operation/operation.o


clean:
	rm -fv ./operation/liboperation.so ./operation/operation.o main-bin
