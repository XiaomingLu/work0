CFLAGS =  -c -Wall

all:        work0.elf

work0.elf:  main.o
	gcc $< -o $@

%.o:        %.c
	gcc $(CFLAGS) $< -o $@

debug:		work0.dbg.elf

work0.dbg.elf:	main.dbg.o
	gcc -g $< -o $@

%.dbg.o:	%.c
	gcc $(CFLAGS) -g $< -o $@

clear:
	rm -rf *.o *.elf