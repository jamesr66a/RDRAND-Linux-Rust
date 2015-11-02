RUST_ROOT := /usr/local

-include ./config.mk

RC := $(RUST_ROOT)/bin/rustc
RCFLAGS := -O -C code-model=kernel -C relocation-model=static


KER = $(shell uname -r)
OBJ = rdrand
RMODS = macros.rs raw.rs

obj-m = ${OBJ}.o
rdrand-objs := rdrand-rust-c.o rdrand-rust.o

all: ${OBJ}.ko

${OBJ}.ko: rdrand-rust-c.c rdrand-rust.o ${RMODS}
	make -C /lib/modules/$(KER)/build M=$(PWD) modules

%.o: %.rs
	$(RC) $(RCFLAGS) --crate-type lib -o $@ --emit obj $<

insmod:
	sudo insmod ${OBJ}.ko
	dmesg | tail

rmmod:
	sudo rmmod ${OBJ}
	dmesg | tail

clean:
	make -C /lib/modules/$(KER)/build M=$(PWD) clean

test: ${OBJ}.ko
	sudo insmod ${OBJ}.ko
	sudo rmmod ${OBJ}
	dmesg | tail -3

.PHONY: all clean insmod rmmod test
