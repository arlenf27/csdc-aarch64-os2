qemu_build:
	mkdir -p build
	aarch64-none-elf-gcc --version
	aarch64-none-elf-gcc -std=c11 -g -mgeneral-regs-only -ffreestanding -fno-builtin -fno-stack-protector -fno-pie -fno-pic -Isrc -Isrc/dtb -Isrc/uart -Isrc/gic -Isrc/timer -c src/boot.S src/vectors.S src/exception_handlers.S src/handlers.c src/kernel.c src/utils.c src/dtb/dtb.c src/uart/uart.c src/timer/timer.c src/gic/gic.c
	mv *.o ./build/
	aarch64-none-elf-ld -nostdlib -T linker/qemu_virt/linker.ld build/boot.o build/vectors.o build/exception_handlers.o build/handlers.o build/kernel.o build/utils.o build/dtb.o build/uart.o build/timer.o build/gic.o -o build/kernel.elf

qemu_run:
	qemu-system-aarch64 -M virt-8.0 -cpu cortex-a57 -m 1024 -nographic -serial mon:stdio -kernel build/kernel.elf

qemu_debug:
	qemu-system-aarch64 -M virt-8.0 -cpu cortex-a57 -m 1024 -nographic -serial mon:stdio -kernel build/kernel.elf -S -s

qemu_dtb_dump:
	qemu-system-aarch64 -M virt-8.0,dumpdtb=virt.dtb -cpu cortex-a57 -m 1024

qemu_dtb_decompile:
	dtc -I dtb -O dts -o virt.dts virt.dtb

qemu_device_help:
	qemu-system-aarch64 -M virt-8.0 -cpu cortex-a57 -m 1024 --device help

clean:
	rm -f build/*.o build/*.elf *.dtb *.dts compile_commands.json
	rm -rf build/ .cache/ .clangd/

qemu_alternate_run_0:
	qemu-system-aarch64 -M virt-8.0 -cpu cortex-a53 -m 512 -nographic -serial mon:stdio -kernel build/kernel.elf

qemu_alternate_run_0_dtb_dump:
	qemu-system-aarch64 -M virt-8.0,dumpdtb=virt.dtb -cpu cortex-a53 -m 512
