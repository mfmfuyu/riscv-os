QEMU=qemu-system-riscv32
CC=/opt/homebrew/opt/llvm/bin/clang
CFLAGS=-std=c11 -O2 -g3 -Wall -Wextra --target=riscv32-unknown-elf -fuse-ld=lld -fno-stack-protector -ffreestanding -nostdlib
SOURCES=$(wildcard kernel/*.c)

all: build

build:
	$(CC) $(CFLAGS) -Wl,-Tkernel/kernel.ld -Wl,-Map=kernel/kernel.map -o kernel.elf $(SOURCES)

run: build
	$(QEMU) -machine virt -bios default -nographic -serial mon:stdio --no-reboot \
		-d unimp,guest_errors,int,cpu_reset -D qemu.log \
		-drive id=drive0,file=lorem.txt,format=raw,if=none \
		-device virtio-blk-device,drive=drive0,bus=virtio-mmio-bus.0 \
		-kernel kernel.elf
