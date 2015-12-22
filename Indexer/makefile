CC = gcc
CFLAGS = -Wall -Werror -std=c99 -pedantic -g
OUTPUT = ./index
EXEOBJ = ./main.o
LIBOBJ = ./tokenizer.o ./fileManager.h ./word-list.o ./word-list.h ./location-list.o ./location-list.h ./alerts.o ./alerts.h
LIBOBJ2 = ./fileManager.o
LIBTOKEN = ./libtoken.a
default: $(OUTPUT)
all: default

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBTOKEN): $(LIBOBJ)
	ar rc $(LIBTOKEN) $(LIBOBJ)


$(OUTPUT): $(EXEOBJ) $(LIBTOKEN)
	$(CC) $(CFLAGS) $(EXEOBJ) $(LIBTOKEN) tokenizer.h -o $@
	$(CC) $(CFLAGS) $(EXEOBJ) $(LIBTOKEN) fileManager.h -o $@

clean:
	rm-f *.o $(OUTPUT)
	rm -f $(LIBTOKEN)
