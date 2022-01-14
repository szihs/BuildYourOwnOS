GCCPARAMS = -m32 -Iinclude -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -fcheck-new -Wall -Werror
LDPARAMS = -melf_i386
ASPARAMS = --32

objects = obj/loader.o \
		  obj/gdt.o \
		  obj/multitasking.o \
		  obj/memorymanagement.o \
		  obj/drivers/driver.o \
		  obj/hardwarecomm/port.o \
		  obj/hardwarecomm/interruptstubs.o \
		  obj/hardwarecomm/interrupts.o \
		  obj/hardwarecomm/pci.o \
		  obj/drivers/keyboard.o \
		  obj/drivers/vga.o \
		  obj/drivers/mouse.o \
		  obj/drivers/amd_am79c973.o \
		  obj/drivers/ata.o \
		  obj/gui/widget.o \
		  obj/gui/window.o \
		  obj/gui/desktop.o \
		  obj/kernel.o

obj/%.o: src/%.cpp
	mkdir -p $(@D)
	g++ $(GCCPARAMS) -o $@ -c $<

obj/%.o: src/%.s
	mkdir -p $(@D)
	as $(ASPARAMS) -o $@ $<

mykernel.bin: linker.ld $(objects)
	ld $(LDPARAMS) -T $< -o $@ $(objects)

install: mykernel.bin
	sudo cp $< /boot/mykernel.bin

mykernel.iso: mykernel.bin
	mkdir iso
	mkdir iso/boot
	mkdir iso/boot/grub
	cp $< iso/boot/
	echo 'set timeout=0' > iso/boot/grub/grub.cfg
	echo 'set default=0' >> iso/boot/grub/grub.cfg
	echo '' >> iso/boot/grub/grub.cfg
	echo 'menuentry "My Operating System" {' >> iso/boot/grub/grub.cfg
	echo '   multiboot /boot/mykernel.bin' >> iso/boot/grub/grub.cfg
	echo '   boot' >> iso/boot/grub/grub.cfg
	echo '}' >> iso/boot/grub/grub.cfg
	grub-mkrescue --output=$@ iso
	rm -rf iso

run: mykernel.iso
	(kill `pidof VirtualBox` && sleep 1) || true
	VirtualBox --startvm MyOS &

.PHONY: clean
clean:
	rm -rf obj mykernel.bin mykernel.iso
