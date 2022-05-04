CC := gcc
CFLAG := -c -o
TOP_DIR := ../..

all: arm64.o test.bin

arm64.o: arm64.c
	$(CC) $(CFLAG) $@ $<

test.bin: test.S
	$(CC) $(CFLAG) test.o test.S
	objcopy -O binary test.o $@
	cp test.bin $(TOP_DIR)

clean:
	rm test.o arm64.o
