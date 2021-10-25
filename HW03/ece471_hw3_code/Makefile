LD = ld
AS = as
ASFLAGS_THUMB = -mthumb-interwork
LDFLAGS_THUMB = --thumb-entry=_start

CC = gcc
CFLAGS = -O2 -Wall -g
CFLAGS_THUMB = -mthumb -march=armv7-a
CFLAGS_NOOPT = -Wall -g
LFLAGS =


all:	exit_asm integer_print integer_print.thumb2 integer_print.extra \
	integer_print_asm crash

####

exit_asm:	exit_asm.o
	$(LD) $(LDFLAGS) -o exit_asm exit_asm.o

exit_asm.o:	exit_asm.s
	$(AS) $(ASFLAGS) -o exit_asm.o exit_asm.s

####

integer_print_asm:	integer_print_asm.o
	$(LD) $(LDFLAGS) -o integer_print_asm integer_print_asm.o

integer_print_asm.o:	integer_print_asm.s
	$(AS) $(ASFLAGS) -o integer_print_asm.o integer_print_asm.s


#####

crash:	crash.o
	$(CC) $(LFLAGS) -o crash crash.o

crash.o:	crash.c
	$(CC) $(CFLAGS_NOOPT) -o crash.o -c crash.c

#####

integer_print:	integer_print.o
	$(CC) $(LFLAGS) -o integer_print integer_print.o

integer_print.o:	integer_print.c
	$(CC) $(CFLAGS) -o integer_print.o -c integer_print.c

#####

integer_print.extra:	integer_print.extra.o
	$(CC) $(LFLAGS) -o integer_print.extra integer_print.extra.o

integer_print.extra.o:	integer_print.extra.c
	$(CC) $(CFLAGS) -o integer_print.extra.o -c integer_print.extra.c


####

integer_print.thumb2:	integer_print.thumb2.o
	$(CC) $(LFLAGS) -o integer_print.thumb2 integer_print.thumb2.o

integer_print.thumb2.o:	integer_print.c
	$(CC) $(CFLAGS) $(CFLAGS_THUMB) -o integer_print.thumb2.o -c integer_print.c

####


submit:	exit_asm.s integer_print.c integer_print.extra.c integer_print_asm.s \
		Makefile README
	tar -czvf hw3_submit.tar.gz exit_asm.s integer_print.c integer_print.extra.c \
		integer_print_asm.s Makefile README

clean:	
	rm -f *~ *.o exit_asm integer_print integer_print.thumb \
			integer_print.extra integer_print.thumb2 \
			integer_print_asm crash
