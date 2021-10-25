CC = gcc
CFLAGS = -O2 -Wall
LFLAGS =


all:	display_temp test_temp test_display


###

test_display:	test_display.o write_display.o
	$(CC) -o test_display test_display.o write_display.o $(LFLAGS)

test_display.o:	test_display.c write_display.h
	$(CC) $(CFLAGS) -c test_display.c

###

test_temp:	test_temp.o read_temp.o
	$(CC) -o test_temp test_temp.o read_temp.o $(LFLAGS)

test_temp.o:	test_temp.c read_temp.h
	$(CC) $(CFLAGS) -c test_temp.c




###

read_temp.o:	read_temp.c read_temp.h
	$(CC) $(CFLAGS) -c read_temp.c

###

write_display.o:	write_display.c write_display.h
	$(CC) $(CFLAGS) -c write_display.c

###

display_temp:	display_temp.o read_temp.o write_display.o
	$(CC) -o display_temp display_temp.o read_temp.o write_display.o $(LFLAGS)

display_temp.o:	display_temp.c read_temp.h write_display.h
	$(CC) $(CFLAGS) -c display_temp.c


submit:	display_temp.c README Makefile
	tar -czvf hw9_submit.tar.gz display_temp.c read_temp.h read_temp.c test_display.c test_temp.c write_display.h write_display.c fakedate README Makefile

clean:	
	rm -f *~ *.o display_temp display_final display_cool test_temp test_display

