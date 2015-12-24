CC = gcc
CFLAGS = -std=c99 -lm -pedantic -g -Wall -Werror
OUTPUT = ./exec
EXEOBJ = ./WHT.o
default: $(OUTPUT)
all: default

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OUTPUT): $(EXEOBJ)
	$(CC) $(CFLAGS) $(EXEOBJ) timer.h -o $@

clean:
	rm -f *.o $(OUTPUT)
