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
