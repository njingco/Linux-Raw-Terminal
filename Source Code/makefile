CC=gcc -Wall
#CLIB= -lpthread

terminal: main.o input.o output.o translate.o
	$(CC) -o terminal main.o input.o output.o translate.o $(CLIB)

clean:
	rm -f *.o core.* terminal

main.o:
	$(CC) -c main.c
input.o:
	$(CC) -c input.c
output.o:
	$(CC) -c output.c
translate.o:
	$(CC) -c translate.c
