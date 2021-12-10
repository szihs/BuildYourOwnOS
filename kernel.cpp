#include "gdt.h"
#include "interrupts.h"
#include "keyboard.h"
#include "types.h"
#define SCREEN_W 80
#define SCREEN_H 25

/*
 #if 1
    VideoMemory[i] = (VideoMemory[i] & 0xFF00)| str[i];
#else
    c = (VideoMemory[i] & 0xFF00) >> 8;
    c = (c & 0xF << 4) | (c & 0xF0 >> 4);
    VideoMemory[i] = (c << 8)| str[i];
#endif
 */
void printf(const char *str) {
  static uint8_t x = 0, y = 0;
  uint16_t *VideoMemory = (uint16_t *)(0xb8000);
  uint8_t c;
  for (uint32_t i = 0; str[i] != '\0'; i++) {

    switch (str[i]) {
    case '\n':
      x = 0;
      y++;
      break;
    default:
      VideoMemory[SCREEN_W * y + x] =
          (VideoMemory[SCREEN_W * y + x] & 0xFF00) | str[i];
      x++;
      break;
    }

    if (x >= SCREEN_W) {
      x = 0;
      y++;
    }
    if (y >= SCREEN_H) {
      for (y = 0; y < SCREEN_H; y++)
        for (x = 0; x < SCREEN_W; x++)
          VideoMemory[SCREEN_W * y + x] =
              (VideoMemory[SCREEN_W * y + x] & 0xFF00) | ' ';

      y = x = 0;
    }
  }
}

typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;

extern "C" void callConstructors() {
  for (constructor *i = &start_ctors; i != &end_ctors; i++)
    (*i)();
}

extern "C" void kernelMain(void *multiboot_structure, uint32_t magicnumber) {
  for (int i = 0; i < 26; i++) {
    printf("Hello ~ From Custom Kernel\n");
    printf("Hello Again ~ From Custom Kernel");
  }
  GlobalDescriptorTable gdt;
  InterruptManager interrupts(&gdt);
  KeyboardDriver keyboard(&interrupts);

  interrupts.Activate();
  while (1)
    ;
}
