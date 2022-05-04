CC := gcc
CFLAG := -c -o
TOP_DIR := ../..

all: arm64.o test.o

arm64.o: arm64.c
	$(CC) $(CFLAG) $@ $<

test.o: test.S
	$(CC) $(CFLAG) test.o test.S
	objcopy -O binary test.o test.bin
	cp test.bin $(TOP_DIR)

clean:
	rm test.o arm64.o test.bin
