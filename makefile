CFLAGS =  -c -Wall

all:        work0.elf

work0.elf:  main.o
	gcc $< -o $@

%.o:        %.c
	gcc $(CFLAGS) $< -o $@

clear:
	rm -rf *.o *.elf