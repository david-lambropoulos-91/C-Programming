CC = gcc
CFLAGS = -fopenmp -lm -std=gnu99 -Wall -Werror -pedantic -g
OUTPUT = ./exec
EXEOBJ = ./gol.o
default: $(OUTPUT)
all: default

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OUTPUT): $(EXEOBJ)
	$(CC) $(CFLAGS) $(EXEOBJ) timer.h -o $@

clean:
	rm -f *.o $(OUTPUT)
