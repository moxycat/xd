CC = gcc
STRIP = strip

ifeq ($(OS), Windows_NT)
	OUTFILE = xd.exe
else
	OUTFILE = xd
endif

all:
	$(CC) -static-libgcc -O2 -Wall -o $(OUTFILE) *.c
	$(STRIP) -sv $(OUTFILE)
