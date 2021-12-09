#include "types.h"
#include "gdt.h"
void printf(const char *str) {
    uint16_t *VideoMemory = (uint16_t*)0xb8000;
    uint8_t c;
    for (uint32_t i = 0; str[i] != '\0'; i++) {
        #if 1
           VideoMemory[i] = (VideoMemory[i] & 0xFF00)| str[i];
        #else
           c = (VideoMemory[i] & 0xFF00) >> 8;
           c = (c & 0xF << 4) | (c & 0xF0 >> 4);
           VideoMemory[i] = (c << 8)| str[i];
        #endif
    }
}

typedef void (*constructor) ();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;

extern "C" void callConstructors() {
	for (constructor *i = &start_ctors; i != &end_ctors; i++) 
		(*i)();
}

extern "C" void kernelMain(void *multiboot_structure, uint32_t magicnumber) {
    printf("Hello ~ From Custom Kernel");
    GlobalDescriptorTable gdt;
    
    while(1);
}
