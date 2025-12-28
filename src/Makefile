CC=gcc
CFLAGS=-Wall -O2
LDFLAGS=-lcurl  # Add this line

all: sbpi

sbpi: main.o cli.o exec.o port.o
	$(CC) $(CFLAGS) -o sbpi main.o cli.o exec.o port.o $(LDFLAGS)  # Add $(LDFLAGS) here

main.o: main.c cli.h
	$(CC) $(CFLAGS) -c main.c

cli.o: cli.c cli.h port.h
	$(CC) $(CFLAGS) -c cli.c

exec.o: exec.c exec.h
	$(CC) $(CFLAGS) -c exec.c

port.o: port.c port.h exec.h
	$(CC) $(CFLAGS) -c port.c

clean:
	rm -f *.o sbpi
