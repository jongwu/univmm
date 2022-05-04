ARCH=$(shell uname -m)
ifeq ($(ARCH), aarch64)
	ARCH=arm64
endif

CC = gcc
LD := gcc
LDFLAG := -o
OBJ := univmm.o arch/$(ARCH)/$(ARCH).o
INCLUDE := include/arch/$(ARCH)
CFLAG := -c -I include -o
ASFLAG := -32 -o
all: univmm

univmm: $(OBJ)
	$(LD) $(LDFLAG) $@ $(OBJ)

arch/$(ARCH)/$(ARCH).o: arch/$(ARCH)
	make -C $<

univmm.o: univmm.c
	$(CC) $(CFLAG) $@ $<

clean:
	rm $(OBJ)
