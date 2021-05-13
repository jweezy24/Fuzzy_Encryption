RM := rm -rf


ODIR= bin
SDIR= src
CFLAGS= -I src/headers -lm


CC=gcc
LD=ld
SIZE=size


OBJS += \
		gauss.o\
		galois.o\
		main.o\
		


	

OBJ = $(patsubst %,$(ODIR)/%,$(OBJS))

$(ODIR)/%.o: $(SDIR)/%.c
		$(CC) $(CFLAGS) -c -g -o $@ $^
$(ODIR)/%.o: $(SDIR)/%.s
	nasm -f elf32 -g -o $@ $^

all: bin

bin: $(OBJ)
	$(CC) $(CFLAGS) $(ODIR)/*  -o Fuzzy.o

clean:
	rm bin/*


tx:
	gcc -I src/headers ./src/tx.c -o ./tx.o -lm bin/galois.o bin/gauss.o

rx:
	gcc -I src/headers ./src/rx.c -o ./rx.o -lm bin/galois.o bin/gauss.o 

gen:
	gcc -I src/headers ./src/gen.c -o ./gen.o -lm bin/galois.o bin/gauss.o 