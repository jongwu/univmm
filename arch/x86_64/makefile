CC := gcc
CFLAG := -c -o
AS := as
ASFLAG := -32
TOP_DIR := ../..

all: x86_64.o test.bin

x86_64.o: x86_64.c
	$(CC) $(CFLAG) $@ $<

test.bin: test.S
	$(AS) $(ASFLAG) -o test.o test.S
	objcopy -O binary test.o $@
	cp $@ $(TOP_DIR)
