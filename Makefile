QEMU=qemu-system-riscv32
CC=/opt/homebrew/opt/llvm/bin/clang
CFLAGS=-std=c11 -O2 -g3 -Wall -Wextra --target=riscv32-unknown-elf -fuse-ld=lld -fno-stack-protector -ffreestanding -nostdlib -Iinclude -Ilib
OBJCOPY=/opt/homebrew/opt/llvm/bin/llvm-objcopy

KERNEL_SOURCES=$(wildcard kernel/*.c)
# USER_SOURCES=$(wildcard user/*.c)
LIB_SOURCES=$(wildcard lib/*.c)

all: build

build:
	$(CC) $(CFLAGS) -Wl,-Tuser/user.ld -Wl,-Map=user/shell.map -o shell.elf user/sh.c user/user.c
	$(CC) $(CFLAGS) -Wl,-Tuser/user.ld -Wl,-Map=user/shell2.map -o shell2.elf user/sh2.c user/user.c
	$(OBJCOPY) --set-section-flags .bss=alloc,contents -O binary shell.elf shell.bin
	$(OBJCOPY) --set-section-flags .bss=alloc,contents -O binary shell2.elf shell2.bin
	$(OBJCOPY) -Ibinary -Oelf32-littleriscv shell.bin shell.bin.o
	$(OBJCOPY) -Ibinary -Oelf32-littleriscv shell2.bin shell2.bin.o
	$(CC) $(CFLAGS) -Wl,-Tkernel/kernel.ld -Wl,-Map=kernel/kernel.map -o kernel.elf $(KERNEL_SOURCES) $(LIB_SOURCES) shell.bin.o shell2.bin.o

run: build
	$(QEMU) -machine virt -bios default -nographic -serial mon:stdio --no-reboot \
		-d unimp,guest_errors,int,cpu_reset -D qemu.log \
		-drive id=drive0,file=lorem.txt,format=raw,if=none \
		-device virtio-blk-device,drive=drive0,bus=virtio-mmio-bus.0 \
		-kernel kernel.elf
